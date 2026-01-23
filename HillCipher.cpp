// square
// invertible: inverse should exist and
//           : det cannot be multiple of 2 or 13
// assume that input string is 3 so key will be 3x3

#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <cctype>
using namespace std;

static const int MOD = 26;

// ---------- helpers ----------
int mod26(int x) {
    x %= MOD;
    if (x < 0) x += MOD;
    return x;
}

int charToVal(char c) { // a->0 ... z->25
    c = (char)tolower((unsigned char)c);
    return (c - 'a');
}

char valToChar(int v) {
    return (char)('a' + mod26(v));
}

// Extended Euclid to find modular inverse (a^-1 mod m), if gcd(a,m)==1
int egcd(int a, int b, int &x, int &y) {
    if (b == 0) { x = 1; y = 0; return a; }
    int x1, y1;
    int g = egcd(b, a % b, x1, y1);
    x = y1;
    y = x1 - (a / b) * y1;
    return g;
}

bool modInverse(int a, int m, int &inv) {
    int x, y;
    int g = egcd(a, m, x, y);
    if (g != 1) return false;              // inverse doesn't exist if not coprime [web:8]
    inv = mod26(x);
    return true;
}

// ---------- determinant (your vector-based idea, fixed) ----------
int getDet(vector<vector<int>>& mat, int n = 3) {
    if (n == 1) return mat[0][0];
    if (n == 2) return mat[0][0] * mat[1][1] - mat[0][1] * mat[1][0];

    int res = 0;
    for (int col = 0; col < n; ++col) {
        vector<vector<int>> sub(n - 1, vector<int>(n - 1));
        for (int i = 1; i < n; ++i) {
            int subcol = 0;
            for (int j = 0; j < n; ++j) {
                if (j == col) continue;
                sub[i - 1][subcol++] = mat[i][j];
            }
        }
        int sign = (col % 2 == 0) ? 1 : -1;
        res += sign * mat[0][col] * getDet(sub, n - 1);
    }
    return res;
}

vector<vector<int>> toVec(int keyMatrix[3][3]) {
    vector<vector<int>> v(3, vector<int>(3));
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            v[i][j] = keyMatrix[i][j];
    return v;
}

// ---------- matrix inverse mod 26 (adjugate method) ----------
int det2(int a, int b, int c, int d) { return a * d - b * c; }

void adjugate3x3(int A[3][3], int adj[3][3]) {
    // Cofactor matrix (then transpose)
    int c00 =  det2(A[1][1], A[1][2], A[2][1], A[2][2]);
    int c01 = -det2(A[1][0], A[1][2], A[2][0], A[2][2]);
    int c02 =  det2(A[1][0], A[1][1], A[2][0], A[2][1]);

    int c10 = -det2(A[0][1], A[0][2], A[2][1], A[2][2]);
    int c11 =  det2(A[0][0], A[0][2], A[2][0], A[2][2]);
    int c12 = -det2(A[0][0], A[0][1], A[2][0], A[2][1]);

    int c20 =  det2(A[0][1], A[0][2], A[1][1], A[1][2]);
    int c21 = -det2(A[0][0], A[0][2], A[1][0], A[1][2]);
    int c22 =  det2(A[0][0], A[0][1], A[1][0], A[1][1]);

    // adj(A) = cofactor(A)^T
    adj[0][0] = c00; adj[0][1] = c10; adj[0][2] = c20;
    adj[1][0] = c01; adj[1][1] = c11; adj[1][2] = c21;
    adj[2][0] = c02; adj[2][1] = c12; adj[2][2] = c22;
}

bool invertMatrixMod26(int keyMatrix[3][3], int invKey[3][3]) {
    vector<vector<int>> v = toVec(keyMatrix);
    int det = getDet(v, 3);
    det = mod26(det);

    int detInv;
    if (!modInverse(det, MOD, detInv)) return false; // needs gcd(det,26)==1 [web:6][web:8]

    int adj[3][3];
    adjugate3x3(keyMatrix, adj);

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            invKey[i][j] = mod26(adj[i][j] * detInv);
        }
    }
    return true;
}

// ---------- your validateKey (fixed signature & logic) ----------
bool validateKey(int keyMatrix[3][3]) {
    vector<vector<int>> v = toVec(keyMatrix);
    int det = getDet(v, 3);
    det = mod26(det);

    // check1: det should not be a multiple of 2 or 13 (equivalently gcd(det,26)=1) [web:6]
    if (det % 2 != 0 && det % 13 != 0) {
        int invKey[3][3];
        if (invertMatrixMod26(keyMatrix, invKey)) return true;
    }
    return false;
}

// ---------- keyStringToMatrix (kept, fixed mapping) ----------
bool keyStringToMatrix(string key, int keyMatrix[][3]) {
    if ((int)key.size() != 9) return false;

    int k = 0;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            keyMatrix[i][j] = charToVal(key[k]); // a->0..z->25
            ++k;
        }
    }

    if (validateKey(keyMatrix)) {
        cout << "valid keyMatix found! performing Hill cipher..." << endl;
        return true;
    } else {
        cout << "invalid matrix, try another key." << endl;
        return false;
    }
}

// ---------- encryption/decryption ----------
void encryptBlock3(const int key[3][3], const int p[3], int c[3]) {
    for (int i = 0; i < 3; ++i) {
        int sum = 0;
        for (int k = 0; k < 3; ++k) sum += key[i][k] * p[k];
        c[i] = mod26(sum);
    }
}

void decryptBlock3(const int invKey[3][3], const int c[3], int p[3]) {
    for (int i = 0; i < 3; ++i) {
        int sum = 0;
        for (int k = 0; k < 3; ++k) sum += invKey[i][k] * c[k];
        p[i] = mod26(sum);
    }
}

int HillCipher(string inputString) {
    if (inputString.length() != 3) {
        cout << "invalid input!" << endl;
        return EXIT_FAILURE;
    }

    // Keep it “the way it is”: you asked input length=3, so use a 3-vector.
    int p[3] = { charToVal(inputString[0]), charToVal(inputString[1]), charToVal(inputString[2]) };

    // For a deterministic demo, use a known valid 3x3 key (classic example used in many notes).
    // Key string "gybnqkurp" => matrix:
    // [6 24 1; 13 16 10; 20 17 15], which is invertible mod 26 (det coprime with 26) [web:6][web:7]
    string key = "gybnqkurp";
    int keyMatrix[3][3];
    if (!keyStringToMatrix(key, keyMatrix)) return EXIT_FAILURE;

    int invKey[3][3];
    if (!invertMatrixMod26(keyMatrix, invKey)) {
        cout << "Key is not invertible mod 26." << endl;
        return EXIT_FAILURE;
    }

    int c[3];
    encryptBlock3(keyMatrix, p, c);

    cout << "Plaintext:  " << inputString << endl;
    cout << "Key:        " << key << endl;

    string cipher;
    for (int i = 0; i < 3; ++i) cipher.push_back(valToChar(c[i]));
    cout << "Ciphertext: " << cipher << endl;

    int p2[3];
    decryptBlock3(invKey, c, p2);
    string recovered;
    for (int i = 0; i < 3; ++i) recovered.push_back(valToChar(p2[i]));
    cout << "Decrypted:  " << recovered << endl;

    return EXIT_SUCCESS;
}

int main() {
    string inputString;
    cout << "Enter the input string: " << endl;
    cin >> inputString;

    return HillCipher(inputString);
}
