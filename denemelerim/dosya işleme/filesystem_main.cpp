#include <iostream>
#include <filesystem>
#include <string>

namespace fs = std::filesystem;

void kaynak_dosyalari_otomatik_bul(const std::string& baslangic_klasoru) {
    for (const auto& nesne : fs::recursive_directory_iterator(baslangic_klasoru)) {
        if (nesne.is_regular_file()) {
            if (nesne.path().extension() == ".cpp" || nesne.path().extension() == ".h" || nesne.path().extension() == ".txt") {
                std::cout << "Bulunan Dosya Yolu: " << nesne.path().string() << std::endl;
            }
        }
    }
}

// EKSİK OLAN VE LİNKERYİ ÇILDIRTAN GİRİŞ KAPISI:
int main() {
    std::cout << "=== VMAKE OTOMATIK DOSYA TARAMA MOTORU TESTI ===\n" << std::endl;

    // "." işareti "şu an içinde bulunduğum klasör" demektir.
    // İstersen buraya "/Users/mac/Visual Stdio C++/denemelerim/" gibi tam yol da yazabilirsin.
    std::string test_klasoru = "."; 

    kaynak_dosyalari_otomatik_bul(test_klasoru);

    std::cout << "\n=== TARAMA BASARIYLA TAMAMLANDI ===" << std::endl;
    return 0;
}