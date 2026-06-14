#include <iostream>
#include <fstream>
#include <string>
#include <cstdint>

// 1. WAD Başlığı (Önceki yazdığımız 12 baytlık yapı)
struct WadHeader {
    char identification[4];
    int32_t numlumps;
    int32_t infotableofs;
};

// 2. YENİ: Dizin (Directory) içindeki her bir elemanın yapısı (Tam 16 bayt)
struct DirectoryEntry {
    int32_t filepos; // Verinin dosyadaki konumu (4 bayt)
    int32_t size;    // Verinin boyutu (4 bayt)
    char name[8];    // Verinin adı (8 bayt - C dilinde sınır budur!)
};

int main() {
    std::ifstream file("doom2.wad", std::ios::binary);
    
    if (!file.is_open()) {
        std::cout << "HATA: doom2.wad dosyasi bulunamadi!" << std::endl;
        return 1;
    }

    // Başlığı okuyoruz
    WadHeader header;
    file.read(reinterpret_cast<char*>(&header), sizeof(WadHeader));

    std::cout << "--- HEDEF KOORDINATA ZIPLANIYOR: " << header.infotableofs << " ---" << std::endl;

    // 3. İŞTE SİHİR BURADA: Dosyanın okuma imlecini (plak iğnesi gibi düşün) 
    // doğrudan infotableofs adresine (14.6 MB ilerisine) kaydırıyoruz!
    file.seekg(header.infotableofs, std::ios::beg);

    std::cout << "Ilk 15 Dosyanin (Lump) Listesi:\n" << std::endl;

    // Bütün listeyi (2920 tane) yazdırırsak terminal kitlenir. 
    // O yüzden şimdilik sadece ilk 15 tanesini okuyan bir döngü kuruyoruz.
    for (int i = 0; i < 15; i++) {
        DirectoryEntry entry;
        
        // 16 baytlık yapımızı okuyoruz
        file.read(reinterpret_cast<char*>(&entry), sizeof(DirectoryEntry));

        // DOOM isimleri her zaman 8 karakterdir ve sonunda \0 (bitiş) garantisi yoktur.
        // O yüzden güvenli bir şekilde C++ string'ine çeviriyoruz.
        std::string lumpName(entry.name, 8);

        std::cout << i + 1 << ". Isim: [" << lumpName << "] "
                  << "| Boyut: " << entry.size << " bayt "
                  << "| Konum: " << entry.filepos << std::endl;
    }

    file.close();
    return 0;
}