#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
using namespace std;

// ---------- helpers ----------
static string normalizeKeepSpaces(const string& s) {
    // Keep as-is (including spaces) so encryption/decryption are symmetric.
    return s;
}

static vector<int> keywordOrder(const string& key) {
    // Returns a permutation "order" of size n:
    // order[i] = rank of column i in sorted(key) with stable tie-break.
    int n = (int)key.size();
    vector<pair<char,int>> v;
    v.reserve(n);
    for (int i = 0; i < n; i++) v.push_back({(char)tolower(key[i]), i});
    stable_sort(v.begin(), v.end(), [](auto &a, auto &b){
        if (a.first != b.first) return a.first < b.first;
        return a.second < b.second;
    });

    vector<int> order(n);
    for (int rank = 0; rank < n; rank++) order[v[rank].second] = rank;
    return order;
}

static vector<int> rankToColIndex(const vector<int>& order) {
    // Given order[col] = rank, create inv[rank] = col.
    int n = (int)order.size();
    vector<int> inv(n);
    for (int col = 0; col < n; col++) inv[order[col]] = col;
    return inv;
}

// ---------- Rail Fence ----------
string railEncrypt(const string& text, int rails) {
    string s = normalizeKeepSpaces(text);
    if (rails <= 1 || (int)s.size() <= 1) return s;

    vector<string> row(rails);
    int r = 0;
    int dir = 1; // +1 down, -1 up

    for (char c : s) {
        row[r].push_back(c);
        if (r == 0) dir = 1;
        else if (r == rails - 1) dir = -1;
        r += dir;
    }
    string out;
    for (auto &t : row) out += t;
    return out;
}

string railDecrypt(const string& cipher, int rails) {
    string c = normalizeKeepSpaces(cipher);
    if (rails <= 1 || (int)c.size() <= 1) return c;

    int n = (int)c.size();
    vector<int> railOfPos(n);

    int r = 0, dir = 1;
    for (int i = 0; i < n; i++) {
        railOfPos[i] = r;
        if (r == 0) dir = 1;
        else if (r == rails - 1) dir = -1;
        r += dir;
    }

    vector<int> cnt(rails, 0);
    for (int x : railOfPos) cnt[x]++;

    vector<string> rows(rails);
    int idx = 0;
    for (int rr = 0; rr < rails; rr++) {
        rows[rr] = c.substr(idx, cnt[rr]);
        idx += cnt[rr];
    }

    vector<int> used(rails, 0);
    string plain;
    plain.reserve(n);
    for (int i = 0; i < n; i++) {
        int rr = railOfPos[i];
        plain.push_back(rows[rr][used[rr]++]);
    }
    return plain;
}

// ---------- Single Columnar ----------
string columnarEncrypt(const string& text, const string& key, char pad = 'X') {
    string s = normalizeKeepSpaces(text);
    int cols = (int)key.size();
    if (cols <= 0) return s;

    int n = (int)s.size();
    int rows = (n + cols - 1) / cols;
    int total = rows * cols;

    s.resize(total, pad); // pad to full rectangle

    vector<int> order = keywordOrder(key);
    vector<int> inv = rankToColIndex(order);

    // read columns by increasing rank
    string out;
    out.reserve(total);
    for (int rank = 0; rank < cols; rank++) {
        int col = inv[rank];
        for (int r = 0; r < rows; r++) {
            out.push_back(s[r * cols + col]);
        }
    }
    return out;
}

string columnarDecrypt(const string& cipher, const string& key) {
    string c = normalizeKeepSpaces(cipher);
    int cols = (int)key.size();
    if (cols <= 0) return c;

    int total = (int)c.size();
    int rows = (total + cols - 1) / cols; // should be exact if padded similarly

    // If not exact, still handle by padding cipher to rectangle for reconstruction.
    int rect = rows * cols;
    c.resize(rect, 'X');

    vector<int> order = keywordOrder(key);
    vector<int> inv = rankToColIndex(order);

    // Fill grid column-by-column in the same order used in encryption.
    vector<string> grid(rows, string(cols, '\0'));
    int idx = 0;
    for (int rank = 0; rank < cols; rank++) {
        int col = inv[rank];
        for (int r = 0; r < rows; r++) {
            grid[r][col] = c[idx++];
        }
    }

    // Read row-wise
    string plain;
    plain.reserve(rect);
    for (int r = 0; r < rows; r++)
        for (int col = 0; col < cols; col++)
            plain.push_back(grid[r][col]);

    return plain;
}

// ---------- Double Columnar ----------
string doubleColumnarEncrypt(const string& text, const string& key1, const string& key2, char pad='X') {
    return columnarEncrypt(columnarEncrypt(text, key1, pad), key2, pad);
}

string doubleColumnarDecrypt(const string& cipher, const string& key1, const string& key2) {
    // reverse order when decrypting
    return columnarDecrypt(columnarDecrypt(cipher, key2), key1);
}

// ---------- demo main ----------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // Example usage (replace with your inputs):
    string plaintext = "attack at once";
    int rails = 3;

    string key1 = "ZEBRA";
    string key2 = "CARGO";

    cout << "Plaintext: " << plaintext << "\n\n";

    // Rail Fence
    string rf = railEncrypt(plaintext, rails);
    cout << "[Rail Fence] Cipher: " << rf << "\n";
    cout << "[Rail Fence] Decipher: " << railDecrypt(rf, rails) << "\n\n";

    // Single Columnar
    string col = columnarEncrypt(plaintext, key1, 'X');
    cout << "[Columnar] Cipher: " << col << "\n";
    cout << "[Columnar] Decipher: " << columnarDecrypt(col, key1) << "\n\n";

    // Double Columnar
    string dcol = doubleColumnarEncrypt(plaintext, key1, key2, 'X');
    cout << "[Double Columnar] Cipher: " << dcol << "\n";
    cout << "[Double Columnar] Decipher: " << doubleColumnarDecrypt(dcol, key1, key2) << "\n";

    return 0;
}
