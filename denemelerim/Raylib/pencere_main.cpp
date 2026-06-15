#include "raylib.h"
#include <vector>
#include <cstdlib> // rand() için

struct Particle {
    Vector2 position;  // X ve Y koordinatı
    Vector2 speed;     // Her karede ne yöne ne kadar hızla gideceği
    Color color;       // Kıvılcımın rengi
    float alpha;       // Görünmezlik oranı (Zamanla sönecek)
    float lifeTime;    // Parçacığın ömrü (Sıfıra gelince yok olacak)
};
// 2000 parçacıklık bir bellek havuzu oluşturuyoruz
std::vector<Particle> particlePool;
int main() {
    // Raylib penceresini ve M1 işlemcinin grafik gücünü uyandırıyoruz
    InitWindow(800, 600, "C++ Gücü + Raylib Laboratuvarı");
    SetTargetFPS(60);

Rectangle enemy = { 350.0f, 250.0f, 100.0f, 100.0f };
Color enemyColor = RED;
float hitTimer = 0.0f; 
float critTimer = 0.0f; // 🌟 kritsayaç yerine bunu ekle
bool isCrit = false;

    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime(); // 60 FPS için yaklaşık delta zamanı
        Vector2 mousePos = GetMousePosition();
        // 🧠 SİSTEMCİ ZAMANLAYICISI: Kritik süresini her karede azaltıyoruz
        if (isCrit) {
            critTimer -= deltaTime; // Zaman geriye doğru akıyor
            if (critTimer <= 0.0f) {
                isCrit = false; // Süre bitti, kritiği kapat!
                enemyColor = RED; // Rengi eski haline döndür
            }
        } else {
        // Eğer kritik değilse, normal vuruşun rengini hitTimer yönetsin
        if (hitTimer > 0.0f) {
            hitTimer -= deltaTime;
            if (hitTimer <= 0.0f) enemyColor = RED;
        }
    }
// ⚔️ VURUŞ KONTROLÜ
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mousePos, enemy)) {
        
        // %20 Kritik Şansı Kontrolü
        if (rand() % 100 < 20) {
            isCrit = true;
            critTimer = 0.2f; // 🌟 KRİTİK HİSSİYATI: Ekran 0.2 saniye sarı kalsın (Gözün görebileceği bir süre)
            enemyColor = YELLOW; // Düşman SARI olsun

            // 🔥 KRİTİK PATLAMASI: 200 Parçacık, daha hızlı!
            for (int i = 0; i < 200; i++) {
                Particle p;
                p.position = mousePos;
                p.speed.x = ((float)rand() / RAND_MAX) * 16.0f - 8.0f; // 2 kat daha hızlı fırlama
                p.speed.y = ((float)rand() / RAND_MAX) * 16.0f - 12.0f;
                p.color = GOLD; // Altın/Sarı renk
                p.alpha = 1.0f;
                p.lifeTime = ((float)rand() / RAND_MAX) * 0.6f + 0.2f;
                particlePool.push_back(p);
            }
        } 
        else { // Normal Vuruş
            hitTimer = 0.1f;
            enemyColor = WHITE;

            // Normal Patlama: 50 Parçacık
            for (int i = 0; i < 50; i++) {
                Particle p;
                p.position = mousePos;
                p.speed.x = ((float)rand() / RAND_MAX) * 8.0f - 4.0f;
                p.speed.y = ((float)rand() / RAND_MAX) * 8.0f - 6.0f;
                p.color = RED;
                p.alpha = 1.0f;
                p.lifeTime = ((float)rand() / RAND_MAX) * 0.5f + 0.2f;
                particlePool.push_back(p);
            }
        }
    }
        
        // 🚀 C++ GÜCÜ: Farenin sol tıkına basıldığında RAM'de anında 100 parçacık üret!
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            Vector2 mousePos = GetMousePosition();
            
            for (int i = 0; i < 100; i++) {
                Particle p;
                p.position = mousePos;
                // Rastgele yönlere saçılma hızları (Low-level C/C++ matematiği)
                p.speed.x = ((float)rand() / RAND_MAX) * 6.0f - 3.0f;
                p.speed.y = ((float)rand() / RAND_MAX) * 6.0f - 3.0f;
                p.color = (rand() % 2 == 0) ? GOLD : ORANGE;
                p.alpha = 1.0f;
                p.lifeTime = 1.0f; // 1 saniye ömür
                
                particlePool.push_back(p);
            }
        }
        // Eğer düşman vurulduysa, zamanı azalt ve süre bitince rengi eski haline (RED) getir
        if (hitTimer > 0.0f) {
            hitTimer -= deltaTime; // 60 FPS için delta zamanı
            if (hitTimer <= 0.0f) {
                enemyColor = RED;
            }
        }

        // 🧠 C++ GÜCÜ: Fizik Güncellemesi (CPU kanadı)
        for (size_t i = 0; i < particlePool.size(); i++) {
            particlePool[i].position.x += particlePool[i].speed.x;
            particlePool[i].position.y += particlePool[i].speed.y;
            particlePool[i].speed.y += 0.05f; // Hafif yerçekimi ivmesi uyguluyoruz!
            particlePool[i].lifeTime -= deltaTime; // Her karede ömrünü azalt (60 FPS için ~16ms)
            particlePool[i].alpha -= deltaTime;

            // Ömrü biten parçacığı bellekten temizle
            if (particlePool[i].lifeTime <= 0) {
                particlePool.erase(particlePool.begin() + i);
                i--;
            }
        }

        // 🎨 RAYLIB GÜCÜ: Çizim Aşaması (GPU kanadı)
        BeginDrawing();
            DrawFPS(550, 10); // FPS'i sol üst köşeye yazdır
            ClearBackground(BLACK); // Ekranı her karede siyaha boya
            DrawRectangleRec(enemy, enemyColor);
            // C++ vektöründeki tüm aktif parçacıkları Raylib ile ekrana basıyoruz
            for (const auto& p : particlePool) {
                // Rengi görünmezlik oranına göre ayarlıyoruz
                Color c = ColorAlpha(p.color, p.alpha);
                // Sadece 1 piksel veya küçük bir çember çizdiriyoruz
                DrawCircleV(p.position, 3.0f, c);
            }

            DrawText("Ekrana Tıkla ve Havai Fişekleri Patlat!", 20, 20, 20, RAYWHITE);
            DrawText(TextFormat("Aktif Parçacık Sayısı: %i", particlePool.size()), 20, 50, 20, LIME);
            if(isCrit){
                DrawText("KRİTİK VURUŞ", 550, 40, 20, RED);
            }
            
        EndDrawing();
    }

    CloseWindow();
    return 0;
}