#include <algorithm>
#include <vector>
#include <string>

std::string columnarEncrypt(std::string msg, std::string key) {
    int cols = key.length();
    int rows = (msg.length() + cols - 1) / cols;
    
    std::vector<std::vector<char>> grid(rows, std::vector<char>(cols, 'X'));
    int k = 0;
    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j)
            if (k < msg.length()) grid[i][j] = msg[k++];
    
    std::vector<int> order(cols);
    for (int i = 0; i < cols; ++i) order[i] = i;
    std::sort(order.begin(), order.end(), [&](int a, int b) { return key[a] < key[b]; });
    
    std::string cipher;
    for (int c : order)
        for (int r = 0; r < rows; ++r)
            cipher += grid[r][c];
    return cipher;
}

std::string columnarDecrypt(std::string cipher, std::string key) {
    int cols = key.length();
    int rows = (cipher.length() + cols - 1) / cols;
    
    std::vector<int> order(cols);
    for (int i = 0; i < cols; ++i) order[i] = i;
    std::sort(order.begin(), order.end(), [&](int a, int b) { return key[a] < key[b]; });
    
    std::vector<std::vector<char>> grid(rows, std::vector<char>(cols));
    int k = 0;
    for (int idx : order)
        for (int r = 0; r < rows; ++r) {
            grid[r][idx] = cipher[k++];
        }
    
    std::string msg;
    for (int r = 0; r < rows; ++r)
        for (int c = 0; c < cols; ++c)
            msg += grid[r][c];
    return msg.substr(0, msg.find('X'));
}
