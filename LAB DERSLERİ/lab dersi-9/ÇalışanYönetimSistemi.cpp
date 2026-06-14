#include <iostream>
#include <string>
#include <cstdlib>
#include "colors.h"

using namespace std;

// Ekranı işletim sistemine göre temizleyen yardımcı fonksiyon
void ekranTemizle() {
    #ifdef _WIN32
        system("cls");
    #else
        cout<<"\n\033[H\033[J" << endl;
    #endif
}

class Calisan {
private:
    string isim;
    string departman;
    double maas;

public:
    // Default constructor: isim girilmesse hata vermemesi için girdim hocam
    Calisan() {
        isim = "Belirtilmedi";
        departman = "Belirtilmedi";
        maas = 0.0;
    }

    Calisan(string isim, string departman, float maas) {
        this->isim = isim;
        this->departman = departman;
        this->maas = maas;
    }

    double getMaas() {
        return maas;
    }
    string getAd() {
        return isim;
    }
    string getDepartman() {
        return departman;
    }

    void zamYap(double oran) {
        maas = maas + (maas * oran / 100);
    }

    void bilgiGoster() {
        cout << CYAN << isim << BEYAZ << " | " 
             << SARI << departman << BEYAZ << " | " 
             << YESIL << maas << " TL" << RESET << endl;
    }
};

int main() {
    ekranTemizle(); // Program başladığında terminali temizle

    Calisan calisanlar[3] = {
        Calisan("İlhan", "Yazilim", 45000),
        Calisan("Sami", "Tasarim", 38000),
        Calisan("Can", "Pazarlama", 32000)
    };
    cout << MAGENTA << BOLD << "\n-----[ TÜM ÇALIŞANLAR ]-----" << RESET << endl;
    for (int i = 0; i < 3; i++) {
        calisanlar[i].bilgiGoster();
    }
    cout << endl;
    cout << SARI << BOLD << "\nÇalışanlara %15 zam yapıldı..." << RESET << endl;
    for (int i = 0; i < 3; i++) {
        calisanlar[i].zamYap(15);
    }

    double maxmaas = 0;
    string maxmaasliadam;
    string maxmaaslidepartmani;

    for (int i = 0; i < 3; i++) {
        if (calisanlar[i].getMaas() > maxmaas) {
            maxmaas = calisanlar[i].getMaas();
            maxmaasliadam = calisanlar[i].getAd();
            maxmaaslidepartmani = calisanlar[i].getDepartman();
        }
    }

    cout << MAGENTA << BOLD << "\n-----[ EN YÜKSEK MAAŞLI ÇALIŞAN ]-----" << RESET << endl;
    cout << CYAN << maxmaasliadam << BEYAZ << " | " 
         << SARI << maxmaaslidepartmani << BEYAZ << " | " 
         << YESIL << maxmaas << " TL" << RESET << endl;
    
    cout << MAGENTA << BOLD << "\n-----[ YAZILIM DEPARTMANI LİSTESİ ]-----" << RESET << endl;
    for (int i = 0; i < 3; i++) {
        if (calisanlar[i].getDepartman() == "Yazilim") {
            calisanlar[i].bilgiGoster();
        }
    }

    cout << MAGENTA << BOLD << "\n-----[ TÜM ÇALIŞANLARIN GÜNCEL DURUMU ]-----" << RESET << endl;
    for (int i = 0; i < 3; i++) {
        calisanlar[i].bilgiGoster();
    }
    cout << "\n";

    return 0;
}