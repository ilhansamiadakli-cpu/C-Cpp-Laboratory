#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

// Fonksiyon prototipleri
void stokuListele(const string& dosyaAdi);
void urunEkle(const string& dosyaAdi, const string& urun, double fiyat, int adet);
void urunSil(const string& dosyaAdi, const string& hedef);
void fiyatGuncelle(const string& dosyaAdi, const string& hedef, double yeniFiyat);

int main() {
    string dosya = "urun_stok.txt";
    ofstream baslangic(dosya);
    if (baslangic) {
        baslangic << "Laptop 15000 5\n";
        baslangic << "Mouse 250 30\n";
        baslangic << "Klavye 450 20\n";
        baslangic.close();
    }

    int secim = -1;
    while (secim != 0) {
       // cout<<"\n\033[H\033[J" << endl;
        cout << "\n--- STOK YONETIM MODULU ---\n";
        cout << "1) Tum stoku listele\n";
        cout << "2) Yeni urun ekle\n";
        cout << "3) Urun sil\n";
        cout << "4) Fiyat guncelle\n";
        cout << "0) Cikis\n";
        cout << "Seciminiz: ";
        cin >> secim;

        if (secim == 1) {
            stokuListele(dosya);
        } 
        else if (secim == 2) {
            string ad;
            double fiyat;
            int adet;
            cout << "Urun Adi: "; cin >> ad;
            cout << "Fiyat: "; cin >> fiyat;
            cout << "Adet: "; cin >> adet;
            urunEkle(dosya, ad, fiyat, adet);
        } 
        else if (secim == 3) {
            string hedef;
            cout << "Silinecek urun adi: "; cin >> hedef;
            urunSil(dosya, hedef);
        } 
        else if (secim == 4) {
            string hedef;
            double yeniFiyat;
            cout << "Fiyati guncellenecek urun adi: "; cin >> hedef;
            cout << "Yeni Fiyat: "; cin >> yeniFiyat;
            fiyatGuncelle(dosya, hedef, yeniFiyat);
        } 
        else if (secim != 0) {
            cout << "Gecersiz secim! Lutfen tekrar deneyin.\n";
        }
    }

    cout << "Programdan cikiliyor. Iyi gunler!\n";
    return 0;
}


void stokuListele(const string& dosyaAdi) {
    ifstream oku;
    oku.open(dosyaAdi);
    if (!oku) { cout << "Dosya acilamadi!\n"; return; }

    string urun;
    double fiyat;
    int adet;
    int i = 1;
    
    cout << "\n--- STOK LISTESI ---\n";
    while (oku >> urun >> fiyat >> adet) {
        cout << i++ << ". " << urun << " | " << fiyat << " TL | " << adet << " Adet\n";
    }
    oku.close();
}

void urunEkle(const string& dosyaAdi, const string& urun, double fiyat, int adet) {
    ofstream yaz;
    yaz.open(dosyaAdi, ios::app);
    if (!yaz) { cout << "Dosya acilamadi!\n"; return; }

    yaz << urun << " " << fiyat << " " << adet << endl;
    yaz.close();
    cout << "-> " << urun << " basariyla eklendi.\n";
}

void urunSil(const string& dosyaAdi, const string& hedef) {
    ifstream oku;
    oku.open(dosyaAdi);
    if (!oku) { cout << "Dosya bulunamadi!\n"; return; }

    vector<string> urunler;
    vector<double> fiyatlar;
    vector<int> adetler;
    
    string u; double f; int a;
    bool bulundu = false;

    while (oku >> u >> f >> a) {
        if (u == hedef) {
            bulundu = true; 
        } else {
            urunler.push_back(u);
            fiyatlar.push_back(f);
            adetler.push_back(a);
        }
    }
    oku.close();

    if (!bulundu) {
        cout << "-> Hata: '" << hedef << "' adinda bir urun bulunamadi!\n";
        return;
    }

    ofstream yaz;
    yaz.open(dosyaAdi);
    for (size_t i = 0; i < urunler.size(); i++) {
        yaz << urunler[i] << " " << fiyatlar[i] << " " << adetler[i] << endl;
    }
    yaz.close();
    
    cout << "-> " << hedef << " basariyla silindi.\n";
}

void fiyatGuncelle(const string& dosyaAdi, const string& hedef, double yeniFiyat) {
    ifstream oku;
    oku.open(dosyaAdi);
    if (!oku) { cout << "Dosya bulunamadi!\n"; return; }

    vector<string> urunler;
    vector<double> fiyatlar;
    vector<int> adetler;
    
    string u; double f; int a;
    bool bulundu = false;

    while (oku >> u >> f >> a) {
        if (u == hedef) {
            f = yeniFiyat; 
            bulundu = true;
        }
        urunler.push_back(u);
        fiyatlar.push_back(f);
        adetler.push_back(a);
    }
    oku.close();

    if (!bulundu) {
        cout << "-> Hata: '" << hedef << "' adinda bir urun bulunamadi!\n";
        return;
    }

    ofstream yaz;
    yaz.open(dosyaAdi);
    for (size_t i = 0; i < urunler.size(); i++) {
        yaz << urunler[i] << " " << fiyatlar[i] << " " << adetler[i] << endl;
    }
    yaz.close();
    
    cout << "-> " << hedef << " fiyati basariyla " << yeniFiyat << " olarak guncellendi.\n";
}