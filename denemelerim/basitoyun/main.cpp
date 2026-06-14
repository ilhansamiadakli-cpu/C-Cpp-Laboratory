#include <iostream>
#include <thread>
#include <chrono>

namespace VoideRPG {

    // 1. Düşmanın Olası Durumları
    enum ZekaDurumu {
        IDLE,       // Boşta
        TAUNT,      // Kışkırtma (Oyuncuya yaklaşır)
        ATTACK,     // Saldırı
        FLEE        // Kaçış
    };

    // 2. Düşmanın Saf Verisi
    struct Goblin {
        ZekaDurumu durum = IDLE;
        int can = 30;
        int oyuncuyaUzaklik = 5; // Terminalde mesafeler "Kare" veya "Adım" cinsindendir
    };

    // =================================================================
    // BEYİN (KURAL YÖNETİCİSİ) - Ekrana yazı YAZMAZ, sadece karar alır!
    // =================================================================
    void GoblinBeyni(Goblin* g) {
        // Öncelik 1: Hayatta Kal!
        if (g->can <= 10 && g->durum != FLEE) {
            g->durum = FLEE;
            return;
        }

        // Öncelik 2: Saldır! (Yeterince yakınsa)
        if (g->oyuncuyaUzaklik <= 1 && g->can > 10) {
            g->durum = ATTACK;
            return;
        }

        // Öncelik 3: Oyuncuyu Kışkırt ve Yaklaş
        if(g->durum != FLEE){
            if (g->oyuncuyaUzaklik > 1 && g->oyuncuyaUzaklik < 6) {
                g->durum = TAUNT;
                return;
            }
        }

        // Hiçbiri değilse boş boş bekle
        if (g->oyuncuyaUzaklik >= 6) {
            g->durum = IDLE;
        }
    }

    // =================================================================
    // KASLAR (EYLEMLER) - Karar ALMAZ, sadece yapar ve terminale yazar!
    // =================================================================
    void GoblinHareketi(Goblin* g) {
        switch (g->durum) {
            case IDLE:
                std::cout << "[ ] Goblin burnunu karistiriyor.\n";
                break;
            case TAUNT:
                std::cout << "[!] Goblin bagiriyor: 'Buraya gel pis insan!' (Bir adim yaklasti)\n";
                g->oyuncuyaUzaklik--; // KAS fiziksel dünyayı değiştirir
                break;
            case ATTACK:
                std::cout << "[X] Goblin pasli kiliciyla SALDIRDI! (Uzaklik: " << g->oyuncuyaUzaklik << ")\n";
                break;
            case FLEE:
                std::cout << "[~] Goblin 'Canim cok az!' diye aglayarak KACIYOR!\n";
                g->oyuncuyaUzaklik++;
                break;
        }
    }
}

// Oyun Döngüsü Testi
int main() {
    VoideRPG::Goblin düşman;

    std::cout << "--- TERMINAL RPG SİMULASYONU BASLADI ---\n\n";

    // 10 turluk bir RPG savaş simülasyonu
    for (int tur = 1; tur <= 10; tur++) {
        std::cout << "Tur " << tur << " | ";
        
        // Önce beyin düşünür ve karar alır...
        VoideRPG::GoblinBeyni(&düşman);
        
        // Sonra kaslar o kararı uygular!
        VoideRPG::GoblinHareketi(&düşman);

        // 5. Turda senaryo gereği oyuncu Goblini ağır yaralasın
        if (tur == 5) {
            std::cout << "\n>>> OYUNCU GOBLINE BUYU YAPTI! (Goblin Cani 30 -> 8)\n\n";
            düşman.can = 8;
        }

        // Gerçekçilik için biraz beklet (1 saniye)
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

    std::cout << "\n--- SİMULASYON BITTI ---\n";
    return 0;
}