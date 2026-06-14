#include <iostream>
#include <chrono>
#include <thread>
#include <vector>

// Simüle edilmiş donanım süreleri (Milisaniye)
const int CPU_YAZMA_SURESI = 2;  // CPU veriyi RAM'e çok hızlı yazar
const int DISK_BASMA_SURESI = 8; // Disk (Ring 0) hantaldır, yavaş yazar

// --- 1. YÖNTEM: ESKİ HANTAL SİSTEM (Single Buffer / Blocking) ---
void eski_hantal_sistem_testi(int vagon_sayisi) {
    auto baslangic = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < vagon_sayisi; ++i) {
        // CPU yazar (Disk bekler)
        std::this_thread::sleep_for(std::chrono::milliseconds(CPU_YAZMA_SURESI));
        
        // Disk basar (CPU bekler) - İŞTE BEKLENEN YER BURASI!
        std::this_thread::sleep_for(std::chrono::milliseconds(DISK_BASMA_SURESI));
    }

    auto bitis = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> sure = bitis - baslangic;
    std::cout << "[TEK TAMPON]: " << vagon_sayisi << " vagonu toplam " << sure.count() << " ms sürede işledi.\n";
}

// --- 2. YÖNTEM: SENİN GELİŞTİRDİĞİN TREN SİSTEMİ (Double Buffer / Non-blocking) ---
// Disk arkada bağımsız çalışırken, CPU yeni vagonu hazırlayacak
void disk_isci_ipligi(int vagon_sayisi) {
    for (int i = 0; i < vagon_sayisi; ++i) {
        // Disk Front Buffer'daki veriyi basıyor (Arka planda bağımsız)
        std::this_thread::sleep_for(std::chrono::milliseconds(DISK_BASMA_SURESI));
    }
}

void yeni_tren_sistemi_testi(int vagon_sayisi) {
    auto baslangic = std::chrono::high_resolution_clock::now();

    // 1. ADIM: Disk işçisini (Ring 0 / Thread) arkada bağımsız olarak başlatıyoruz!
    // Bu işçi kendi çekirdeğinde diske yazma işlemlerini yapacak.
    std::thread disk_isçisi(disk_isci_ipligi, vagon_sayisi);

    // 2. ADIM: Aynı anda ana çekirdekte (CPU / Ring 3) verileri hazırlıyoruz
    for (int i = 0; i < vagon_sayisi; ++i) {
        // CPU Back Buffer'ı dolduruyor. Diskin bitmesini BEKLEMİYOR!
        std::this_thread::sleep_for(std::chrono::milliseconds(CPU_YAZMA_SURESI));
    }

    // İki işçinin de tamamen bitmesini bekleyip süreyi ölçüyoruz
    disk_isçisi.join();

    auto bitis = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> sure = bitis - baslangic;
    std::cout << "[ÇİFT TAMPON]: " << vagon_sayisi << " vagonu toplam " << sure.count() << " ms sürede işledi.\n";
}

int main() {
    int test_vagon_sayisi = 50;
    
    std::cout << "--- HIZ TESTI BASLADI (50 Vagon Veri Gönderiliyor) ---\n\n";
    
    eski_hantal_sistem_testi(test_vagon_sayisi);
    yeni_tren_sistemi_testi(test_vagon_sayisi);
    
    return 0;
}