#include "vod_cout.hpp"

using namespace VOD;

int main() {
    // Tek satırlık kutu
    Cout << "Sistem Baslatildi!" << std::endl;

    // Çok satırlı bellek tamponlama testi
    Cout << "Modul 1: Bellek Tahsisi Tamam\n"
         << "Modul 2: VOD Engine Aktif\n"
         << "Durum: 0 Hata ile Calisiyor" << std::endl;

    return 0;
}