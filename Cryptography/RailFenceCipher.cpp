#include <iostream>
#include <string>
#include <vector>
using namespace std;

string railFenceEncrypt(string msg, int key) {
    int msgLen = msg.length();
    vector<vector<char>> rail(key, vector<char>(msgLen, '\n'));
    
    int row = 0, col = 0, k = -1;
    for (int i = 0; i < msgLen; ++i) {
        rail[row][col++] = msg[i];
        if (row == 0 || row == key - 1) k = -k;
        row += k;
    }
    
    string cipher;
    for (int i = 0; i < key; ++i)
        for (int j = 0; j < msgLen; ++j)
            if (rail[i][j] != '\n') cipher += rail[i][j];
    return cipher;
}

string railFenceDecrypt(string cipher, int key) {
    int msgLen = cipher.length();
    vector<vector<char>> rail(key, vector<char>(msgLen, '\n'));
    
    int row = 0, col = 0, dir = 1;
    int index = 0;
    for (int i = 0; i < key; ++i)
        for (int j = 0; j < msgLen; ++j)
            if (rail[i][j] == '\n') {
                rail[i][j] = '*';
                index++;
                if (index == msgLen) break;
            }
    
    index = 0;
    for (int i = 0; i < msgLen; ++i) {
        rail[row][col++] = cipher[index++];
        if (row == 0) dir = 1;
        else if (row == key - 1) dir = -1;
        row += dir;
    }
    
    string msg;
    for (int i = 0; i < key; ++i)
        for (int j = 0; j < msgLen; ++j)
            if (rail[i][j] != '*') msg += rail[i][j];
    return msg;
}
