#include <iostream>
#include <string>
#include <algorithm>
using namespace std;

// longest palindromic string
// string checkLongestPalindromeString(string input) {
//     string longestPalindromicString(1, input[0]);
//     int i = 0;

//     if (input.length() == 1) {
//         return string(1, input[0]);
//     }

//     while (i < (int)input.length()) {
//         if (input.length() % 2 == 0) { // fixed even check
//             // even
//             string substring1 = input.substr(0, input.length() / 2);
//             string substring2 = input.substr(input.length() / 2, input.length() / 2);

//             if (substring1 == substring2) {
//                 longestPalindromicString = substring1;
//             } else {
//                 return longestPalindromicString;
//             }
//         }
//         ++i; // fixed: must be outside the if, otherwise infinite loop when odd length
//     }

//     return longestPalindromicString;

int main() {
    string inputString;
    cout << "input something" << endl;   // show prompt
    cin >> inputString;                  // note: reads only up to whitespace [web:67]
    cout << inputString;
    return 0;
}

//I CANT ASYUDGYUJASDGBHJASBDHJAKSB