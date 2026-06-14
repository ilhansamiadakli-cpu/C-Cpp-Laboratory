#include <iostream>
#include <string>
#include <cctype>

using namespace std;

int main() {
    string tamAd;
    cout << "Adinizi ve soyadinizi girin: ";
    
    getline(cin, tamAd);

    cout << "Karakter sayisi: " << tamAd.length() << endl;

    string buyukAd = tamAd;
    for (int i = 0; i < buyukAd.length(); i++) {
        buyukAd[i] = toupper(buyukAd[i]);
    }
    cout << "Buyuk harfli: " << buyukAd << endl;

    string kucukAd = tamAd;
    for (int i = 0; i < kucukAd.length(); i++) {
        kucukAd[i] = tolower(kucukAd[i]);
    }
    cout << "Kucuk harfli: " << kucukAd << endl;

    
    int boslukIndeks = tamAd.find(" ");
    string ad = tamAd.substr(0, boslukIndeks);
    string soyad = tamAd.substr(boslukIndeks + 1);

    
    cout << "Tersten soyad: ";
    for (int i = soyad.length() - 1; i >= 0; i--) {
        cout << soyad[i];
    }
    cout << endl;

    cout << "Merhaba " << ad << "! Soyadiniz [" << soyad << "] " 
         << soyad.length() << " karakterden olusuyor." << endl;

    return 0;
}