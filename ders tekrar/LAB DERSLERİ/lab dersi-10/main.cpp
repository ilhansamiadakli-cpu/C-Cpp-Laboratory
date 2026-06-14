#include <iostream>
#include <string>

using namespace std;


template <typename T>
class SensorDepo {
private:
    string ad;     
    T sonDeger;     
    T minDeger;    
    T maxDeger; 

public:
 
    SensorDepo(string ad, T baslangic) {
        this->ad = ad;
        this->sonDeger = baslangic;
        this->minDeger = baslangic;
        this->maxDeger = baslangic;
    }

 
    void guncelle(T yeniDeger) {
        sonDeger = yeniDeger; 

        if (yeniDeger < minDeger) {
            minDeger = yeniDeger;
        }
        if (yeniDeger > maxDeger) {
            maxDeger = yeniDeger;
        }
    }


    void raporla() {
        cout << ad << " | Son: " << sonDeger 
             << " | Min: " << minDeger 
             << " | Max: " << maxDeger << endl;
    }
};

int main() {
  

    SensorDepo<double> sicaklik("Sicaklik-1", 22.5); 
    sicaklik.guncelle(25.3);
    sicaklik.guncelle(19.8); 
    sicaklik.guncelle(31.0); 
    sicaklik.raporla(); 

    SensorDepo<int> titresim("Titresim-A", 100);
    titresim.guncelle(450); 
    titresim.guncelle(80);
    titresim.raporla(); 

    SensorDepo<string> durum("Durum-X", "normal"); 
    durum.guncelle("alarm"); 
    durum.guncelle("normal"); 
    durum.raporla(); 

    return 0; 
}