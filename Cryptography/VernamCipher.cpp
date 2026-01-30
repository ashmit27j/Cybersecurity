// #include<iostream>
// #include<string>
// #include<algorithm>
// #include<vector>
// using namespace std;
// int keygen(int a, int c, int m, string seed, int size){
//     int key[size];
//     for(int i = 0; i < size; i++){
//         key[i] = (( a * seed[i] ) + c ) % m;
//     }
//     return *key;
// }
// int encrypt(int *PT, int key, int length){
//     int CT[length];
//     for(int i = 0; i < length; i ++){
//         CT[i] = ( PT[i] + key ) % 26;
//         ++key;
//     }
//     return *CT;
// }
// // string decrypt(){

// // }

// int main(){
//     string plainText, seed;
//     int plainTextASCII[];
//     int a, c, m;
//     cout << "input plain text: ";
//     cin >> plainText;
//     cout << "enter a c m: ";
//     cin >> a;
//     cin >> c;
//     cin >> m;
//     cout << "enter seed: ";
//     cin >> seed;
//     //convert each char of plain text to ascii
//     // 65 - 90 upper case 97 - 122 lowercase
//     for(int i =0; i < plainText.length(); i++){
//         plainTextASCII[i] = (int(plainText[i]));
//     }

//     int key = keygen(a, c, m, seed, plainText.size());
//     for(int i = 0; i < plainText.length(); i ++){
//         cout << key;
//         ++key;
//     }
//     int CipherText = encrypt(plainTextASCII, key, plainText.size());
//     cout << "encrypted cipher: " + CipherText;
//     // cout << "decrypted cipher: " + decrypt(plainTextASCII, key, plainText.size());
// }

#include<iostream>
#include<string>
#include<algorithm>
#include<vector>
using namespace std;

vector<int> keygen(int a, int c, int m, string seed, int size){
    vector<int> key(size);
    int current = (int)seed[0]; // Initialize with first char of seed
    
    for(int i = 0; i < size; i++){
        current = ((a * current) + c) % m;
        key[i] = current % 26; // Keep key in range 0-25
    }
    return key;
}

vector<int> encrypt(vector<int> PT, vector<int> key, int length){
    vector<int> CT(length);
    for(int i = 0; i < length; i++){
        CT[i] = (PT[i] + key[i]) % 26;
    }
    return CT;
}

string decrypt(vector<int> CT, vector<int> key, int length){
    string plainText = "";
    for(int i = 0; i < length; i++){
        int decrypted = (CT[i] - key[i] + 26) % 26; // +26 to handle negative
        plainText += char(decrypted + 65); // Convert back to uppercase letter
    }
    return plainText;
}

int main(){
    string plainText, seed;
    int a, c, m;
    
    cout << "input plain text (uppercase): ";
    cin >> plainText;
    cout << "enter a c m: ";
    cin >> a >> c >> m;
    cout << "enter seed: ";
    cin >> seed;
    
    // Convert each char to 0-25 range 0 1 2 3 4 5 ...
    vector<int> plainTextNumeric(plainText.length());
    for(int i = 0; i < plainText.length(); i++){
        if(plainText[i] >= 'A' && plainText[i] <= 'Z')
            plainTextNumeric[i] = plainText[i] - 65;
        else if(plainText[i] >= 'a' && plainText[i] <= 'z')
            plainTextNumeric[i] = plainText[i] - 97;
    }
    
    // Generate keystream
    vector<int> keystream = keygen(a, c, m, seed, plainText.size());
    
    cout << "Key stream: ";
    for(int i = 0; i < plainText.length(); i++){
        cout << keystream[i] << " ";
    }
    cout << endl;
    
    vector<int> cipherTextNumeric = encrypt(plainTextNumeric, keystream, plainText.size());
    
    cout << "Encrypted cipher (numeric): ";
    for(int i = 0; i < cipherTextNumeric.size(); i++){
        cout << cipherTextNumeric[i] << " ";
    }
    cout << endl;
    
    cout << "Encrypted cipher (letters): ";
    for(int i = 0; i < cipherTextNumeric.size(); i++){
        cout << char(cipherTextNumeric[i] + 65);
    }
    cout << endl;

    string decryptedText = decrypt(cipherTextNumeric, keystream, plainText.size());
    cout << "Decrypted cipher: " << decryptedText << endl;
    
    return 0;
}
