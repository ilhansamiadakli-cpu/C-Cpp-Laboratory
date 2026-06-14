#include "TerminalEngine.hpp"
#include <chrono>
#include <thread>

int main() {
    // 80 genişlik, 24 yükseklikte dinamik motorumuzu başlatalım
    TerminalEngine engine(80, 24);

    float progress = 0.0f;
    int circleX = 15;

    // Ana Oyun / Uygulama Döngüsü (Game Loop)
    while (progress <= 1.0f) {
        engine.BeginDrawing();

        // 1. UI Penceresi Çiz (Mavi boş bir kutu)
        engine.DrawRectangle(2, 1, 76, 22, '#', "\033[34m", false);

        // 2. Başlık Metni Ekle
        engine.DrawText(30, 3, "MODERN C++ TERMINAL ENGINE", "\033[33m");

        // 3. Geometrik Şekiller Çiz
        engine.DrawRectangle(8, 6, 12, 6, '%', "\033[31m", true); // Kırmızı dolu kare
        engine.DrawCircle(circleX, 16, 4, '@', "\033[32m");       // Yeşil daire (Hareket edecek)

        // 4. Dinamik Yükleme Çubuğu Çiz
        engine.DrawText(8, 11, "Sistem Yukleniyor...", "\033[36m");
        engine.DrawProgressBar(8, 12, 60, progress, "\033[32m", "\033[37m");

        engine.EndDrawing();

        // Mantık Güncellemeleri (Update Logic)
        progress += 0.01f;   // Yükleme çubuğunu ilerlet
        circleX += 1;        // Daireyi sağa doğru yürüt

        // ~30 FPS simülasyonu için küçük bir gecikme
        std::this_thread::sleep_for(std::chrono::milliseconds(33));
    }

    return 0;
}