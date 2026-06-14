#include <iostream>
#include "colors.h"
using namespace std;


int* hastaKayitAc(int n);
void skorGir(int* hastalar, int n);
double ortalamaHesapla(const int* hastalar, int n);
int maksimumSkor(const int* hastalar, int n);
int minimumSkor(const int* hastalar, int n);
int* hastaEkle(int* eskiListe, int eskiN, int yeniSkor);

int main() {
    int n;
    cout<<"\n\033[H\033[J" << endl;
    cout << "Kac hasta var? "; 
    cin >> n;

    int* hastalar = hastaKayitAc(n);

    skorGir(hastalar, n);
    
    cout << YESIL<< "\n--- ILK RAPOR ---" <<RESET <<endl;
    cout << "Ortalama: " << SARI<<ortalamaHesapla(hastalar, n) << RESET<< endl;
    cout << "En Yuksek Skor: " << MAVI<<maksimumSkor(hastalar, n) << RESET<< endl;
    cout << "En Dusuk Skor: " << KIRMIZI<<minimumSkor(hastalar, n) << RESET<< endl;

    int yeniSkor; 
    do {
        cout << "\nYeni hasta skoru: ";
        cin >> yeniSkor;
        if (yeniSkor < 1 || yeniSkor > 10) {
            cout << "HATA: Gecersiz giris! Lutfen 1-10 arasinda girin." << endl;
        }
    } while (yeniSkor < 1 || yeniSkor > 10);

    // ! DİKKAT: hastaEkle fonksiyonu eski diziyi siliyor ve yeni dizinin adresini döndürüyor.
    hastalar = hastaEkle(hastalar, n, yeniSkor);
    n++; // todo:Dizimiz artık 1 eleman daha büyük olduğu için 'n' değerini de güncelliyoruz.

    cout << "Guncel Liste: "<< YESIL;
    for (int i = 0; i < n; i++) {
        cout << hastalar[i] << " ";
    }
    cout << RESET << endl;


    delete[] hastalar;
    hastalar = nullptr; 

    return 0;
}



// n elemanlı heap dizisi açar, elemanları 0 yapar, pointer döndürür
int* hastaKayitAc(int n) {
    int* yeniDizi = new int[n]; // Heap'te 'n' adet integer'lık yer ayır
    for (int i = 0; i < n; i++) {
        yeniDizi[i] = 0; // Başlangıç değerlerini sıfırla
    }
    return yeniDizi; // Bellekteki bu alanın başlangıç adresini döndür
}

// Kullanıcıdan ağrı skorlarını (1-10) alır, geçersiz girişte tekrar sorar
void skorGir(int* hastalar, int n) {
    for (int i = 0; i < n; i++) {
        int skor;
        do {
            cout << i + 1 << ". hastanin agri skoru (1-10): ";
            cin >> skor;
            if (skor < 1 || skor > 10) {
                cout << "HATA: Gecersiz giris! Lutfen 1-10 arasinda girin." << endl;
            }
        } while (skor < 1 || skor > 10); // 1-10 aralığında değilse döngüden çıkamaz
        
        hastalar[i] = skor;
    }
}

// Ortalama ağrı skorunu hesaplar
double ortalamaHesapla(const int* hastalar, int n) {
    if (n == 0) return 0; 
    
    double toplam = 0;
    for (int i = 0; i < n; i++) {
        toplam += hastalar[i];
    }
    return toplam / n; // double bölmesi yapabilmek için toplamı double tanımladık
}

// En yüksek skoru bulur
int maksimumSkor(const int* hastalar, int n) {
    int max = hastalar[0]; // İlk elemanı şimdilik en büyük kabul et
    for (int i = 1; i < n; i++) {
        if (hastalar[i] > max) {
            max = hastalar[i]; // Daha büyüğünü bulursan onu kaydet
        }
    }
    return max;
}

// En düşük skoru bulur
int minimumSkor(const int* hastalar, int n) {
    int min = hastalar[0]; // İlk elemanı şimdilik en küçük kabul et
    for (int i = 1; i < n; i++) {
        if (hastalar[i] < min) {
            min = hastalar[i]; // Daha küçüğünü bulursan onu kaydet
        }
    }
    return min;
}

// Listeye yeni hasta ekler, ESKİ BELLEĞİ SİLER, yeni pointer döndürür
int* hastaEkle(int* eskiListe, int eskiN, int yeniSkor) {
    // Adım 1: Yeni boyutta (eskiN + 1) yeni bir heap alanı aç
    int* yeniListe = new int[eskiN + 1];

    // Adım 2: Eski elemanları yeni listeye birebir kopyala
    for (int i = 0; i < eskiN; i++) {
        yeniListe[i] = eskiListe[i];
    }

    // Adım 3: Yeni gelen hastanın skorunu dizinin sonuna ekle
    yeniListe[eskiN] = yeniSkor; 

    // Adım 4: Eski alanı bellekten temizle. 
    // Bunu yapmazsak kopyaladıktan sonra eski dizi uzay boşluğunda (RAM'de) kalır ve sızıntı yapar.
    delete[] eskiListe; 

    // Adım 5: Artık yeni listemizin adresini geri döndür
    return yeniListe;
}