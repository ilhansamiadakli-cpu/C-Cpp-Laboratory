#include <iostream>
#include "colors.h"
using namespace std;

const int ODA = 4; 
const int KAT = 5; 
const int SAAT = 8; 

double ortalama(const double veri[], int n); 
double ortalama(const int veri[], int n); 
int anormalSayisi(const double sicaklik[], int n, double altSinir, double ustSinir); 
void uyariYazdir(const double sicaklik[], int n, double alt, double ust); 
void katRaporu(const double sicakliklar[][ODA], int katSayisi); 
int maksIndeks(const double veri[], int n); 

int main() {
    double sicaklik[] = {21.5, 22.0, 25.5, 29.3, 30.1, 27.8, 23.0, 21.0}; 
    
    double katOda[KAT][ODA] = { 
        {22.1, 23.4, 21.8, 24.0}, 

        {20.5, 19.8, 21.2, 20.9}, 
        {25.6, 26.2, 24.8, 25.1}, 
        {18.2, 17.9, 18.5, 19.0}, 
        {23.0, 22.8, 23.5, 22.6}  
    }; 
    
    double co2[] = {412.5, 680.3, 1250.8, 520.1, 890.4};   
    
    double buHafta[] = {120.5, 118.3, 125.7, 122.1, 119.8, 131.2, 128.6}; 
    double gecenHafta[] = {115.2, 117.0, 119.8, 121.5, 118.4, 125.3, 124.1};

    cout << "\033[H\033[J"<< endl;

    
    cout << "Gunluk ortalama sicaklik: " << YESIL<< ortalama(sicaklik, SAAT) << " C" << BEYAZ << "\n";
    
    int anormalAdet = anormalSayisi(sicaklik, SAAT, 18.0, 28.0);
    cout << "Anormal olcum sayisi: " << KIRMIZI << anormalAdet << BEYAZ << "\n";
    
    if (anormalAdet > 0) {
        uyariYazdir(sicaklik, SAAT, 18.0, 28.0);
    }
    
    cout << SARI<<"\n--- Kat Sicaklik Raporu ---" << BEYAZ << "\n";
    katRaporu(katOda, KAT);
    
    
    int enYuksekCo2Odasi = maksIndeks(co2, 5);
    cout << "\nEn yuksek CO2 seviyesi " << enYuksekCo2Odasi + 1 
         << ". odada tespit edildi (" << co2[enYuksekCo2Odasi] << " ppm).\n";
         
    double ortBuHafta = ortalama(buHafta, 7);
    double ortGecenHafta = ortalama(gecenHafta, 7);
    double degisim = ((ortBuHafta - ortGecenHafta) / ortGecenHafta) * 100.0;
    
    cout << "Enerji degisimi: %"<<YESIL << degisim << BEYAZ << (degisim > 0 ? " artis" : " azalis") <<"\n";

    return 0; 
}


double ortalama(const double veri[], int n) {
    double toplam = 0;
    for (int i = 0; i < n; i++) {
        toplam += veri[i];
    }
    return (n > 0) ? (toplam / n) : 0;
}

double ortalama(const int veri[], int n) {
    double toplam = 0;
    for (int i = 0; i < n; i++) {
        toplam += veri[i];
    }
    return (n > 0) ? (toplam / n) : 0;
}


int anormalSayisi(const double sicaklik[], int n, double altSinir, double ustSinir) {
    int sayac = 0;
    for (int i = 0; i < n; i++) {
        if (sicaklik[i] < altSinir || sicaklik[i] > ustSinir) {
            sayac++;
        }
    }
    return sayac;
}

void uyariYazdir(const double sicaklik[], int n, double alt, double ust) {
    for (int i = 0; i < n; i++) {
        if (sicaklik[i] < alt || sicaklik[i] > ust) {
            cout << "  -> UYARI! Saat " << i << " olcumu anormal: " << KIRMIZI<<sicaklik[i] << " C"<< BEYAZ << "\n";
        }
    }
}

void katRaporu(const double sicakliklar[][ODA], int katSayisi) {
    for (int i = 0; i < katSayisi; i++) {
        double toplam = 0;
        for (int j = 0; j < ODA; j++) {
            toplam += sicakliklar[i][j];
        }
        cout << "Kat " << i + 1 << " ortalama sicaklik: " << YESIL<<(toplam / ODA) << " C" << BEYAZ << "\n";
    }
}

int maksIndeks(const double veri[], int n) {
    if (n <= 0) return -1;
    int maksIdx = 0;
    for (int i = 1; i < n; i++) {
        if (veri[i] > veri[maksIdx]) {
            maksIdx = i;
        }
    }
    return maksIdx;
}