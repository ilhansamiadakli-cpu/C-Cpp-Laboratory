#include<iostream>
#include<string>

using namespace std;
void odemeYap(string &ad, double &bakiye, double odemeMiktari);
void iadeal(string &ad, double &bakiye, double iadeMiktari);
void transferyap(string &gonderenAd, string &aliciAd, double &gonderenBakiye, double &aliciBakiye, double transferMiktari);

int main(){
double bakiye1 = 1000, bakiye2 = 500;
string ad1 = "Ahmet", ad2 = "Mehmet";
cout << "=== Bakiye Kayit ve Guncelleme Uygulamasi ===" << endl;
cout << "Baslangic Bakiyeleri:" << endl;
cout << ad1 << ": " << bakiye1 << " TL" << endl;
cout << ad2 << ": " << bakiye2 << " TL" << endl;

cout << "\n[Odeme Islemi]" << endl;
odemeYap(ad1, bakiye1, 100);
cout << "\n[Iade Islemi]" << endl;
iadeal(ad2, bakiye2, 150);
cout << "\n[Transfer Islemi]" << endl;
transferyap(ad1, ad2, bakiye1, bakiye2, 300);

cout << "\nGuncel Bakiyeler:" << endl;
cout << ad1 << ": " << bakiye1 << " TL" << endl;
cout << ad2 << ": " << bakiye2 << " TL" << endl;
return 0;

}
void odemeYap(string &ad, double &bakiye, double odemeMiktari) {
    if (odemeMiktari > bakiye) {
        cout << "Yetersiz bakiye! Odeme gerceklestirilemedi." << endl;
    } else {
        bakiye -= odemeMiktari;
        cout << ad << ", odemeniz basariyla gerceklestirildi. Kalan bakiyeniz: " << bakiye << " TL" << endl;
    }
}
void iadeal(string &ad, double &bakiye, double iadeMiktari) {
    bakiye += iadeMiktari;
    cout << ad << ", iadeleriniz basariyla gerceklestirildi. Guncel bakiyeniz: " << bakiye << " TL" << endl;
}
void transferyap(string &gonderenAd, string &aliciAd, double &gonderenBakiye, double &aliciBakiye, double transferMiktari) {
    if (transferMiktari > gonderenBakiye) {
        cout << "Yetersiz bakiye! Transfer gerceklestirilemedi." << std::endl;
    } else {
        gonderenBakiye -= transferMiktari;
        aliciBakiye += transferMiktari;
        cout << gonderenAd << ", " << aliciAd << " isimli kullaniciya " << transferMiktari << " TL transfer etti. Kalan bakiyeniz: " << gonderenBakiye << " TL" << std::endl;
        cout << aliciAd << ", " << gonderenAd << " isimli kullanicidan " << transferMiktari << " TL transfer aldi. Guncel bakiyeniz: " << aliciBakiye << " TL" << std::endl;
    }
}