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
std::vector<Particle> particlePool;
int main() {

    InitWindow(800, 600, "C++ Gücü + Raylib Laboratuvarı");
    SetTargetFPS(60);

Rectangle enemy = { 350.0f, 250.0f, 100.0f, 100.0f };
Color enemyColor = RED;
float hitTimer = 0.0f; 
float critTimer = 0.0f; 
bool isCrit = false;

    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime(); 
        Vector2 mousePos = GetMousePosition();
        if (isCrit) {
            critTimer -= deltaTime; 
            if (critTimer <= 0.0f) {
                isCrit = false; 
                enemyColor = RED;
            }
        } else {
        if (hitTimer > 0.0f) {
            hitTimer -= deltaTime;
            if (hitTimer <= 0.0f) enemyColor = RED;
        }
    }

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mousePos, enemy)) {
        
        if (rand() % 100 < 20) {
            isCrit = true;
            critTimer = 0.2f; 
            enemyColor = YELLOW;

            
            for (int i = 0; i < 200; i++) {
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
        else { 
            hitTimer = 0.1f;
            enemyColor = WHITE;

            
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
        
        
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            Vector2 mousePos = GetMousePosition();
            
            for (int i = 0; i < 100; i++) {
                Particle p;
                p.position = mousePos;
                
                p.speed.x = ((float)rand() / RAND_MAX) * 6.0f - 3.0f;
                p.speed.y = ((float)rand() / RAND_MAX) * 6.0f - 3.0f;
                p.color = (rand() % 2 == 0) ? GOLD : ORANGE;
                p.alpha = 1.0f;
                p.lifeTime = 1.0f; 
                
                particlePool.push_back(p);
            }
        }
        
        if (hitTimer > 0.0f) {
            hitTimer -= deltaTime; 
            if (hitTimer <= 0.0f) {
                enemyColor = RED;
            }
        }

        for (size_t i = 0; i < particlePool.size(); i++) {
            particlePool[i].position.x += particlePool[i].speed.x;
            particlePool[i].position.y += particlePool[i].speed.y;
            particlePool[i].speed.y += 0.05f; 
            particlePool[i].lifeTime -= deltaTime; 
            particlePool[i].alpha -= deltaTime;

            if (particlePool[i].lifeTime <= 0) {
                particlePool.erase(particlePool.begin() + i);
                i--;
            }
        }

        BeginDrawing();
            DrawFPS(550, 10);
            ClearBackground(BLACK); 
            DrawRectangleRec(enemy, enemyColor);
            for (const auto& p : particlePool) {
                Color c = ColorAlpha(p.color, p.alpha);
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