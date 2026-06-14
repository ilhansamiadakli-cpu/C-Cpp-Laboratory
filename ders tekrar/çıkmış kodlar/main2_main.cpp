#include <iostream>
using namespace std;

class SensorTracker {
private:
    double* veriler; // Dinamik dizi için pointer
    int kapasite;    // Dizinin toplam ayrılan boyutu
    int adet;        // Mevcut eleman sayısı

    // Diziyi dinamik olarak büyüten iç fonksiyon
    void kapasiteArttir() {
        kapasite *= 2; // Kapasiteyi iki katına çıkar
        double* yeniDizi = new double[kapasite]; // Heap'te yeni yer aç
        
        for (int i = 0; i < adet; i++) {
            yeniDizi[i] = veriler[i]; // Eski verileri kopyala
        }
        
        delete[] veriler; // Eski hafızayı temizle (Bellek sızıntısını önle!)
        veriler = yeniDizi; // Pointer'ı yeni diziye yönlendir
        cout << "--> Log: Kapasite dinamik olarak " << kapasite << " yapildi.\n";
    }

public:
    // Constructor (Yapıcı Fonksiyon)
    SensorTracker(int baslangicKapasitesi = 2) {
        kapasite = baslangicKapasitesi;
        adet = 0;
        veriler = new double[kapasite]; // Heap'te ilk yer tahsisi
    }

    // Destructor (Yıkıcı Fonksiyon)
    ~SensorTracker() {
        delete[] veriler; // Heap'teki hafızayı iade et
        cout << "--> Log: Dinamik hafıza temizlendi ve nesne yok edildi.\n";
    }

    void ekle(double deger) {
        if (adet == kapasite) {
            kapasiteArttir(); // Dizi dolduysa büyüt
        }
        veriler[adet] = deger;
        adet++;
    }

    void listele() const {
        for (int i = 0; i < adet; i++) {
            cout << veriler[i] << " ";
        }
        cout << endl;
    }
};

int main() {
    SensorTracker tracker(2); // Başlangıç kapasitesi 2
    tracker.ekle(22.5);
    tracker.ekle(24.1);
    tracker.ekle(19.8); // Burada dinamik büyüme tetiklenecek
    
    cout << "Sensor Verileri: ";
    tracker.listele();
    return 0;
}