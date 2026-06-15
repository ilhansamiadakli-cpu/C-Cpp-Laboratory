#include "raylib.h"
#include <vector>
#include <cstdlib>

struct Particle {
    Vector2 position;  // X ve Y koordinatı
    Vector2 speed;     // Her karede ne yöne ne kadar hızla gideceği
    Color color;       // Kıvılcımın rengi
    float alpha;       // Görünmezlik oranı (Zamanla sönecek)
    float lifeTime;    // Parçacığın ömrü (Sıfıra gelince yok olacak)
};
// 2000 parçacıklık bir bellek havuzu oluşturuyoruz
std::vector<Particle> particlePool;
class Enemy {
private:
    // 🔒 Sadece bu sınıfın içinden erişilebilen Low-Level Veriler (State)
    Rectangle bounds;
    Color currentColor;
    float maxHealth;
    float currentHealth;
    Vector2 speed;
    
    // Zamanlayıcılar (Timer)
    float hitTimer;
    float critTimer;
    bool isCrit;

public:
    // 🛠️ Yapıcı Metot (Constructor): Düşman doğarken özelliklerini belirliyoruz
    Enemy(float x, float y, float width, float height, float health) {
        bounds = { x, y, width, height };
        maxHealth = health;
        currentHealth = health;
        currentColor = RED;
        
        // Düşmana hafif bir hareket hızı verelim (Sağa sola gitsin)
        speed = { 2.0f, 0.0f }; 
        
        hitTimer = 0.0f;
        critTimer = 0.0f;
        isCrit = false;
    }

    // 🧠 FİZİK VE MANTIK GÜNCELLEME (Her karede çalışır)
    void Update(float deltaTime) {
        // 1. Hareket Mantığı (Ekranın kenarlarından seken bir yapay zeka)
        bounds.x += speed.x;
        if (bounds.x <= 0 || bounds.x + bounds.width >= 800) {
            speed.x *= -1.0f; // Duvara çarpınca ters yöne dön
        }

        // 2. Zamanlayıcıların Geri Sayım Mantığı
        if (isCrit) {
            critTimer -= deltaTime;
            if (critTimer <= 0.0f) {
                isCrit = false;
                currentColor = RED;
            }
        } else if (hitTimer > 0.0f) {
            hitTimer -= deltaTime;
            if (hitTimer <= 0.0f) {
                currentColor = RED;
            }
        }
    }

    // ⚔️ VURULMA KONTROLÜ VE HASAR ALMA (Tıklama anında çağrılır)
    // Dışardan parçacık havuzunun referansını (&) alıyor ki içine kıvılcım ekleyebilsin!
    void CheckHit(Vector2 mousePos, std::vector<Particle>& particlePool) {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mousePos, bounds) && currentHealth > 0) {

            // %20 Şansla Kritik Vuruş
            if (rand() < 20) {
                isCrit = true;
                critTimer = 0.2f;
                currentColor = YELLOW;
                currentHealth -= 25.0f; // Büyük hasar!

                // Kritik Parçacıkları (Hızlı ve Altın Renkli)
                for (int i = 0; i < 150; i++) {
                    Particle p;
                    p.position = mousePos;
                    p.speed.x = ((float)rand() / RAND_MAX) * 16.0f - 8.0f;
                    p.speed.y = ((float)rand() / RAND_MAX) * 16.0f - 12.0f;
                    p.color = GOLD;
                    p.alpha = 1.0f;
                    p.lifeTime = ((float)rand() / RAND_MAX) * 0.6f + 0.2f;
                    particlePool.push_back(p);
                }
            } 
            else { // Normal Vuruş
                hitTimer = 0.1f;
                currentColor = WHITE;
                currentHealth -= 10.0f; // Normal hasar

                // Normal Parçacıklar (Kırmızı)
                for (int i = 0; i < 40; i++) {
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
    }

    // 🎨 ÇİZİM MOTORU (Her karede tuval açıkken çağrılır)
    void Draw() {
        if (currentHealth > 0) {
            // 1. Düşmanın Kendisini Çiz
            DrawRectangleRec(bounds, currentColor);

            // 2. Tost Makinesi Optimizasyonlu Şık Can Barı (Health Bar)
            // Düşmanın hemen kafasının üstüne yeşil bir bar çiziyoruz
            float healthBarWidth = (currentHealth / maxHealth) * bounds.width;
            DrawRectangle(bounds.x, bounds.y - 15, bounds.width, 8, GRAY); // Arka plan (Boş bar)
            DrawRectangle(bounds.x, bounds.y - 15, healthBarWidth, 8, GREEN); // Ön plan (Dolu bar)
        } else {
            // Öldüyse ekrana yazı yazdırabilirsin
            DrawText("DÜŞMAN ELENDİ!", bounds.x - 10, bounds.y + bounds.height/2, 16, LIME);
        }
    }
};

int main() {
    InitWindow(800, 600, "C++ Sınıf Yapısı + Raylib");
    SetTargetFPS(60);

    // 🚀 DÜŞMAN NESNEMİZİ OLUŞTURUYORUZ
    // X=350, Y=250, Genişlik=100, Yükseklik=100, Can=100
    Enemy boss(350.0f, 250.0f, 100.0f, 100.0f, 100.0f);

    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime();
        Vector2 mousePos = GetMousePosition();

        // 🧠 1. GÜNCELLEME (Update)
        boss.Update(deltaTime);
        boss.CheckHit(mousePos, particlePool); // Vuruldu mu kontrol et

    // Parçacıkların kendi fizik güncellemeleri
    for (size_t i = 0; i < particlePool.size(); i++) {
    
    // 🚀 İŞTE EKSİK OLAN HAREKET MOTORU BURASI!
    // Her karede, parçacığın hızını (speed), konumuna (position) ekliyoruz.
    particlePool[i].position.x += particlePool[i].speed.x;
    particlePool[i].position.y += particlePool[i].speed.y;
    
    // Yerçekimi ivmesi (Aşağı doğru düşmelerini sağlar)
    particlePool[i].speed.y += 0.05f; 

    // Ömür ve Görünmezlik değerini azalt
    particlePool[i].lifeTime -= deltaTime; 
    particlePool[i].alpha -= deltaTime * 1.5f; // Sönme hızı

    // Ömrü biten parçacığı bellekten (RAM'den) temizle
    if (particlePool[i].lifeTime <= 0) {
        particlePool.erase(particlePool.begin() + i);
        i--;
    }
}

        // 🎨 2. ÇİZİM (Render)
        BeginDrawing();
            ClearBackground(BLACK);

            // Sadece tek bir satırla düşmanı ve can barını çizdiriyoruz!
            boss.Draw();

            // Parçacıkları çizdirme döngüsü...
            for (const auto& p : particlePool) {
                DrawCircleV(p.position, 3.0f, ColorAlpha(p.color, p.alpha));
            }
            
        EndDrawing();
    }

    CloseWindow();
    return 0;
}