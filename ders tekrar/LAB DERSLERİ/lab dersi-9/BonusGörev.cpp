#include <iostream>
#include <string>
#include <cstdlib>
#include "colors.h"

using namespace std;

void ekranTemizle() {
    #ifdef _WIN32
        system("cls");
    #else
        cout<<"\n\033[H\033[J" << endl;
    #endif
}

class Misafirler {
private:
    string ad;
    int odaNo;
    double gecerliUcret;
    int konaklamaSuresi;

public:
    Misafirler(string ad, int odano, double gecerliUcret, int konaklamaSuresi) {
        this->ad = ad;
        this->odaNo = odano;
        this->gecerliUcret = gecerliUcret;
        setKonaklamaSuresi(konaklamaSuresi);
    }

    ~Misafirler() {
        cout << KIRMIZI << ad << " isimli misafir odadan ayrıldı." << RESET << endl;
    }

    double toplamUcret() {
        if (konaklamaSuresi > 7) {
            return (gecerliUcret * konaklamaSuresi) * (0.9);
        } else {
            return gecerliUcret * konaklamaSuresi;
        }
    }

    void setKonaklamaSuresi(int sure) {
        if (sure < 1) {
            cerr << KIRMIZI_ARKA << BEYAZ << BOLD << " HATA: Konaklama süresi 1'den küçük olamaz! " << RESET << endl;
            exit(1);
        }
        konaklamaSuresi = sure;
    }
};

int main() {
    ekranTemizle();

    cout << MAVI_ARKA << BEYAZ << BOLD << " OTEL YÖNETİM SİSTEMİ " << RESET << "\n\n";

    Misafirler misafir[3] = {
        Misafirler("Ali", 12, 1200, 8),
        Misafirler("Emre", 11, 1000, 3),
        Misafirler("Ayse", 5, 1500, 1)
    };

    double sum = 0;
    for (int i = 0; i < 3; i++) {
         sum += misafir[i].toplamUcret();
    }

    cout << BOLD << YESIL << "Güncel Toplam Gelir: " << sum << " TL'dir." << RESET << "\n\n";
    cout << SARI << "Misafirler otelden ayrılıyor..." << RESET << "\n";

    return 0; 
}