#include "raylib-cpp.hpp"
#include <vector>
#include <algorithm>
#include <cmath>

// -------------------------------------------------------------------------
// OYUN DURUMLARI (STATE MACHINE)
// -------------------------------------------------------------------------
enum class GameState {
    MENU,
    PLAYING,
    VICTORY,
    GAME_OVER
};

// -------------------------------------------------------------------------
// PROSEDÜREL (MATEMATİKSEL) SES ÜRETİCİLERİ
// -------------------------------------------------------------------------
Sound GenerateLaserSound(float startFreq, float endFreq, float duration) {
    const int sampleRate = 44100;
    int frameCount = static_cast<int>(sampleRate * duration);

    Wave wave = { 0 };
    wave.frameCount = frameCount;
    wave.sampleRate = sampleRate;
    wave.sampleSize = 16;
    wave.channels = 1;

    short* data = static_cast<short*>(MemAlloc(frameCount * sizeof(short)));
    float phase = 0.0f;

    for (int i = 0; i < frameCount; ++i) {
        float progress = static_cast<float>(i) / frameCount;
        float currentFreq = startFreq + (endFreq - startFreq) * progress;
        phase += currentFreq / sampleRate;

        float sample = std::sin(2.0f * PI * phase);
        if (sample > 0.0f) sample = 0.8f; else sample = -0.8f;

        float amplitude = (1.0f - progress) * 9000.0f;
        data[i] = static_cast<short>(sample * amplitude);
    }

    wave.data = data;
    Sound sound = LoadSoundFromWave(wave);
    UnloadWave(wave);
    return sound;
}

Sound GenerateExplosionSound(float duration, float rumbleFreq) {
    const int sampleRate = 44100;
    int frameCount = static_cast<int>(sampleRate * duration);

    Wave wave = { 0 };
    wave.frameCount = frameCount;
    wave.sampleRate = sampleRate;
    wave.sampleSize = 16;
    wave.channels = 1;

    short* data = static_cast<short*>(MemAlloc(frameCount * sizeof(short)));
    float phase = 0.0f;
    float lastNoise = 0.0f;

    for (int i = 0; i < frameCount; ++i) {
        float progress = static_cast<float>(i) / frameCount;
        float rawNoise = static_cast<float>(GetRandomValue(-1000, 1000)) / 1000.0f;
        lastNoise = lastNoise * 0.75f + rawNoise * 0.25f;

        phase += (rumbleFreq * (1.0f - progress * 0.5f)) / sampleRate;
        float rumble = std::sin(2.0f * PI * phase);

        float combined = (lastNoise * 0.7f) + (rumble * 0.3f);
        float envelope = std::pow(1.0f - progress, 2.0f);
        data[i] = static_cast<short>(combined * envelope * 22000.0f);
    }

    wave.data = data;
    Sound sound = LoadSoundFromWave(wave);
    UnloadWave(wave);
    return sound;
}

// -------------------------------------------------------------------------
// YARDIMCI FONKSİYONLAR
// -------------------------------------------------------------------------
void DrawHealthBar(float x, float y, float width, float height, int currentHP, int maxHP) {
    if (currentHP < 0) currentHP = 0;
    float healthPercentage = static_cast<float>(currentHP) / maxHP;

    DrawRectangle(x, y, width, height, DARKGRAY);

    raylib::Color barColor = GREEN;
    if (healthPercentage <= 0.25f) barColor = RED;
    else if (healthPercentage <= 0.5f) barColor = GOLD;

    DrawRectangle(x, y, width * healthPercentage, height, barColor);
    DrawRectangleLines(x, y, width, height, WHITE);
}

// Metni yatay olarak ekranın tam ortasına hizalayarak çizen yardımcı fonksiyon
void DrawCenteredText(const char* text, int y, int fontSize, Color color, int screenWidth) {
    int textWidth = MeasureText(text, fontSize);
    ::DrawText(text, screenWidth / 2 - textWidth / 2, y, fontSize, color);
}

// -------------------------------------------------------------------------
// 1. VERİ YAPILARI (STRUCTS)
// -------------------------------------------------------------------------

struct Star {
    raylib::Vector2 position;
    float speed;
    float size;
    raylib::Color color;
};

struct ScreenShake {
    float timer = 0.0f;
    float magnitude = 0.0f;

    void Trigger(float duration, float mag) {
        if (mag >= magnitude || timer <= 0.0f) {
            timer = duration;
            magnitude = mag;
        }
    }

    void Update(float dt, raylib::Camera2D& camera) {
        if (timer > 0.0f) {
            timer -= dt;
            float offsetX = static_cast<float>(GetRandomValue(-100, 100)) / 100.0f * magnitude;
            float offsetY = static_cast<float>(GetRandomValue(-100, 100)) / 100.0f * magnitude;
            camera.offset = raylib::Vector2(offsetX, offsetY);
            magnitude *= 0.92f;
        } else {
            timer = 0.0f;
            magnitude = 0.0f;
            camera.offset = raylib::Vector2(0.0f, 0.0f);
        }
    }
};

struct Particle {
    raylib::Vector2 position;
    raylib::Vector2 velocity;
    raylib::Color color;
    float size;
    float life;
    float maxLife;
    bool active;

    Particle(raylib::Vector2 pos, raylib::Vector2 vel, raylib::Color col, float s, float l)
        : position(pos), velocity(vel), color(col), size(s), life(l), maxLife(l), active(true) {}

    void Update(float dt) {
        position.x += velocity.x * dt;
        position.y += velocity.y * dt;
        velocity.x *= 0.95f;
        velocity.y *= 0.95f;

        life -= dt;
        if (life <= 0.0f) active = false;
    }

    void Draw() const {
        if (!active) return;
        float alpha = life / maxLife;
        DrawCircleV(position, size * alpha, Fade(color, alpha));
    }
};

void SpawnExplosion(std::vector<Particle>& particles, raylib::Vector2 center, raylib::Color baseColor, int count) {
    for (int i = 0; i < count; ++i) {
        float angle = GetRandomValue(0, 360) * DEG2RAD;
        float speed = static_cast<float>(GetRandomValue(50, 260));
        raylib::Vector2 velocity(std::cos(angle) * speed, std::sin(angle) * speed);

        float size = GetRandomValue(25, 65) / 10.0f;
        float life = GetRandomValue(30, 80) / 100.0f;

        raylib::Color pColor = baseColor;
        int colorRoll = GetRandomValue(0, 2);
        if (colorRoll == 1) pColor = ORANGE;
        else if (colorRoll == 2) pColor = YELLOW;

        particles.emplace_back(center, velocity, pColor, size, life);
    }
}

struct Laser {
    raylib::Vector2 position;
    float speed;
    float radius;
    int damage;
    int directionY;
    raylib::Color color;
    bool active;

    Laser(raylib::Vector2 pos, int dmg, int dirY, raylib::Color col)
        : position(pos), speed(420.0f), radius(4.5f), damage(dmg),
          directionY(dirY), color(col), active(true) {}

    void Update(float dt, int screenHeight) {
        position.y += speed * directionY * dt;
        if (position.y < -radius || position.y > screenHeight + radius) {
            active = false;
        }
    }

    void Draw() const {
        DrawCircleV(position, radius, color);
    }
};

struct TargetDummy {
    raylib::Rectangle rect;
    int maxHealth;
    int currentHealth;
    int scoreValue; // Yok edildiğinde vereceği puan
    bool alive;

    TargetDummy(float x, float y, float w, float h, int hp)
        : rect(x, y, w, h), maxHealth(hp), currentHealth(hp), scoreValue(hp), alive(true) {}

    void TakeDamage(int amount) {
        currentHealth -= amount;
        if (currentHealth <= 0) {
            currentHealth = 0;
            alive = false;
        }
    }

    raylib::Vector2 GetGunPosition() const {
        return raylib::Vector2(rect.x + rect.width / 2.0f, rect.y + rect.height + 5.0f);
    }

    raylib::Vector2 GetCenter() const {
        return raylib::Vector2(rect.x + rect.width / 2.0f, rect.y + rect.height / 2.0f);
    }

    void Draw() const {
        if (!alive) return;
        rect.Draw(MAROON);
        DrawRectangleLines(rect.x, rect.y, rect.width, rect.height, RED);
        DrawHealthBar(rect.x, rect.y - 10.0f, rect.width, 5.0f, currentHealth, maxHealth);
    }
};

struct Spaceship {
    raylib::Vector2 position;
    float speed;
    float width;
    float height;
    int maxHealth;
    int currentHealth;

    Spaceship(float x, float y)
        : position(x, y), speed(350.0f), width(40.0f), height(20.0f),
          maxHealth(100), currentHealth(100) {}

    void Reset(float x, float y) {
        position = raylib::Vector2(x, y);
        currentHealth = maxHealth;
    }

    void Update(float dt, int screenWidth) {
        if (currentHealth <= 0) return;

        if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) position.x -= speed * dt;
        if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) position.x += speed * dt;

        if (position.x < 0) position.x = 0;
        if (position.x > screenWidth - width) position.x = screenWidth - width;
    }

    void TakeDamage(int amount) {
        currentHealth -= amount;
        if (currentHealth < 0) currentHealth = 0;
    }

    void Draw() const {
        if (currentHealth <= 0) return;

        DrawRectangle(position.x, position.y, width, height, LIME);
        DrawRectangle(position.x + width / 2.0f - 4.0f, position.y - 8.0f, 8.0f, 8.0f, GREEN);
        DrawHealthBar(position.x, position.y + height + 6.0f, width, 5.0f, currentHealth, maxHealth);
    }

    raylib::Vector2 GetGunPosition() const {
        return raylib::Vector2(position.x + width / 2.0f, position.y - 8.0f);
    }

    raylib::Vector2 GetCenter() const {
        return raylib::Vector2(position.x + width / 2.0f, position.y + height / 2.0f);
    }

    raylib::Rectangle GetRect() const {
        return raylib::Rectangle(position.x, position.y, width, height);
    }
};

void SpawnEnemyGrid(std::vector<TargetDummy>& enemies) {
    enemies.clear();
    const int rows = 3;
    const int cols = 6;
    const float enemyW = 50.0f;
    const float enemyH = 25.0f;
    const float paddingX = 25.0f;
    const float paddingY = 30.0f;
    const float startX = 65.0f;
    const float startY = 80.0f;

    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            float x = startX + c * (enemyW + paddingX);
            float y = startY + r * (enemyH + paddingY);
            int hp = (rows - r) * 25 + 25; // 100, 75, 50 HP (Aynı zamanda puan değeri)
            enemies.emplace_back(x, y, enemyW, enemyH, hp);
        }
    }
}

// -------------------------------------------------------------------------
// 2. ANA DÖNGÜ (MAIN)
// -------------------------------------------------------------------------

int main() {
    const int screenWidth = 600;
    const int screenHeight = 700;

    SetTraceLogLevel(LOG_NONE);

    raylib::Window window(screenWidth, screenHeight, "Raylib - Space Invaders Arcade");
    SetTargetFPS(60);

    InitAudioDevice();

    // Prosedürel Ses Efektleri
    Sound sfxPlayerLaser = GenerateLaserSound(880.0f, 180.0f, 0.12f);
    Sound sfxEnemyLaser  = GenerateLaserSound(420.0f, 110.0f, 0.18f);
    Sound sfxHit         = GenerateExplosionSound(0.08f, 150.0f);
    Sound sfxExplosion   = GenerateExplosionSound(0.40f, 65.0f);
    Sound sfxWin         = GenerateLaserSound(300.0f, 1200.0f, 0.45f); // Yükselen zafer sesi
    Sound sfxSelect      = GenerateLaserSound(500.0f, 900.0f, 0.10f);  // Menü geçiş sesi

    raylib::Camera2D camera(raylib::Vector2(0.0f, 0.0f), raylib::Vector2(0.0f, 0.0f), 0.0f, 1.0f);
    ScreenShake shaker;

    // Arka plan yıldızlarını oluştur
    std::vector<Star> stars;
    for (int i = 0; i < 60; ++i) {
        stars.push_back({
            raylib::Vector2(GetRandomValue(0, screenWidth), GetRandomValue(0, screenHeight)),
            static_cast<float>(GetRandomValue(20, 100)),
            static_cast<float>(GetRandomValue(10, 25)) / 10.0f,
            Fade(RAYWHITE, GetRandomValue(30, 90) / 100.0f)
        });
    }

    // Oyun Durumu ve İstatistik Değişkenleri
    GameState currentState = GameState::MENU;
    int score = 0;
    int highScore = 0;
    int timeBonus = 0;
    int healthBonus = 0;
    float playTime = 0.0f;

    Spaceship ship(screenWidth / 2.0f - 20.0f, screenHeight - 70.0f);

    std::vector<Laser> playerLasers;
    std::vector<Laser> enemyLasers;
    std::vector<TargetDummy> enemies;
    std::vector<Particle> particles;

    float swarmSpeed = 90.0f;
    int swarmDirection = 1;
    float swarmDropAmount = 20.0f;

    const float playerFireCooldown = 0.2f;
    float timeSinceLastPlayerShot = 0.0f;

    float enemyFireCooldown = 0.8f;
    float timeSinceLastEnemyShot = 0.0f;

    // Oyunu Sıfırlama ve Başlatma Lambda Fonksiyonu
    auto StartNewGame = [&]() {
        ship.Reset(screenWidth / 2.0f - 20.0f, screenHeight - 70.0f);
        playerLasers.clear();
        enemyLasers.clear();
        particles.clear();
        SpawnEnemyGrid(enemies);
        swarmDirection = 1;
        timeSinceLastPlayerShot = 0.0f;
        timeSinceLastEnemyShot = 0.0f;
        score = 0;
        timeBonus = 0;
        healthBonus = 0;
        playTime = 0.0f;
        currentState = GameState::PLAYING;
        PlaySound(sfxSelect);
    };

    while (!window.ShouldClose()) {
        float dt = GetFrameTime();

        // Arka plan yıldızlarını her durumda akıtmaya devam et
        for (auto& star : stars) {
            star.position.y += star.speed * dt;
            if (star.position.y > screenHeight) {
                star.position.y = 0.0f;
                star.position.x = static_cast<float>(GetRandomValue(0, screenWidth));
            }
        }

        // =====================================================================
        // DURUM MAKİNESİ: GÜNCELLEME (UPDATE)
        // =====================================================================
        switch (currentState) {
            case GameState::MENU: {
                if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
                    StartNewGame();
                }
                break;
            }

            case GameState::PLAYING: {
                playTime += dt; // Süre sayacını ilerlet
                timeSinceLastPlayerShot += dt;
                timeSinceLastEnemyShot += dt;

                ship.Update(dt, screenWidth);

                // Oyuncu Ateşi
                if (IsKeyDown(KEY_SPACE) && timeSinceLastPlayerShot >= playerFireCooldown) {
                    playerLasers.emplace_back(ship.GetGunPosition(), 25, -1, YELLOW);
                    timeSinceLastPlayerShot = 0.0f;
                    SetSoundPitch(sfxPlayerLaser, GetRandomValue(95, 108) / 100.0f);
                    PlaySound(sfxPlayerLaser);
                }

                // Düşman Sürüsü Hareketi
                bool hitEdge = false;
                for (auto& enemy : enemies) {
                    enemy.rect.x += swarmSpeed * swarmDirection * dt;
                    if (enemy.rect.x + enemy.rect.width >= screenWidth - 15.0f && swarmDirection == 1) hitEdge = true;
                    else if (enemy.rect.x <= 15.0f && swarmDirection == -1) hitEdge = true;
                }

                if (hitEdge) {
                    swarmDirection *= -1;
                    for (auto& enemy : enemies) enemy.rect.y += swarmDropAmount;
                }

                // Düşman Ateşi
                if (timeSinceLastEnemyShot >= enemyFireCooldown && !enemies.empty()) {
                    int randomIndex = GetRandomValue(0, static_cast<int>(enemies.size()) - 1);
                    enemyLasers.emplace_back(enemies[randomIndex].GetGunPosition(), 15, 1, ORANGE);
                    timeSinceLastEnemyShot = 0.0f;
                    enemyFireCooldown = GetRandomValue(5, 11) / 10.0f;
                    PlaySound(sfxEnemyLaser);
                }

                // Çarpışmalar: Oyuncu Mermileri -> Düşmanlar
                for (auto& laser : playerLasers) {
                    laser.Update(dt, screenHeight);
                    for (auto& enemy : enemies) {
                        if (enemy.alive && laser.active && CheckCollisionCircleRec(laser.position, laser.radius, enemy.rect)) {
                            enemy.TakeDamage(laser.damage);
                            laser.active = false;
                            score += 10; // İsabet puanı

                            SpawnExplosion(particles, laser.position, YELLOW, 8);

                            if (!enemy.alive) {
                                score += enemy.scoreValue; // Düşman yok etme puanı (+50, +75, +100)
                                SpawnExplosion(particles, enemy.GetCenter(), RED, 35);
                                shaker.Trigger(0.20f, 6.0f);
                                SetSoundPitch(sfxExplosion, GetRandomValue(90, 115) / 100.0f);
                                PlaySound(sfxExplosion);
                            } else {
                                PlaySound(sfxHit);
                            }
                            break;
                        }
                    }
                }

                // Çarpışmalar: Düşman Mermileri -> Oyuncu
                for (auto& laser : enemyLasers) {
                    laser.Update(dt, screenHeight);
                    if (ship.currentHealth > 0 && laser.active && CheckCollisionCircleRec(laser.position, laser.radius, ship.GetRect())) {
                        ship.TakeDamage(laser.damage);
                        laser.active = false;
                        SpawnExplosion(particles, laser.position, LIME, 12);

                        if (ship.currentHealth <= 0) {
                            SpawnExplosion(particles, ship.GetCenter(), GREEN, 50);
                            shaker.Trigger(0.50f, 16.0f);
                            SetSoundPitch(sfxExplosion, 0.7f);
                            PlaySound(sfxExplosion);
                        } else {
                            shaker.Trigger(0.25f, 10.0f);
                            PlaySound(sfxHit);
                        }
                    }
                }

                // Düşmanların Gemiye Çarpması
                for (auto& enemy : enemies) {
                    if (enemy.alive && ship.currentHealth > 0) {
                        if (CheckCollisionRecs(enemy.rect, ship.GetRect()) || enemy.rect.y + enemy.rect.height >= ship.position.y) {
                            ship.TakeDamage(100);
                            SpawnExplosion(particles, ship.GetCenter(), GREEN, 50);
                            shaker.Trigger(0.50f, 16.0f);
                            SetSoundPitch(sfxExplosion, 0.7f);
                            PlaySound(sfxExplosion);
                        }
                    }
                }

                // Bellek Temizliği
                playerLasers.erase(std::remove_if(playerLasers.begin(), playerLasers.end(), [](const Laser& l) { return !l.active; }), playerLasers.end());
                enemyLasers.erase(std::remove_if(enemyLasers.begin(), enemyLasers.end(), [](const Laser& l) { return !l.active; }), enemyLasers.end());
                enemies.erase(std::remove_if(enemies.begin(), enemies.end(), [](const TargetDummy& e) { return !e.alive; }), enemies.end());

                // Zafer veya Yenilgi Durumuna Geçiş Kontrolü
                if (ship.currentHealth <= 0) {
                    if (score > highScore) highScore = score;
                    currentState = GameState::GAME_OVER;
                } else if (enemies.empty()) {
                    // Zafer Bonuslarını Hesapla
                    timeBonus = std::max(0, static_cast<int>((60.0f - playTime) * 10.0f));
                    healthBonus = ship.currentHealth * 5;
                    score += timeBonus + healthBonus;
                    if (score > highScore) highScore = score;
                    PlaySound(sfxWin);
                    currentState = GameState::VICTORY;
                }
                break;
            }

            case GameState::VICTORY:
            case GameState::GAME_OVER: {
                // Bitiş ekranlarında 'R' veya 'ENTER' ile yeniden başlat, 'M' ile ana menüye dön
                if (IsKeyPressed(KEY_R) || IsKeyPressed(KEY_ENTER)) {
                    StartNewGame();
                } else if (IsKeyPressed(KEY_M)) {
                    currentState = GameState::MENU;
                    PlaySound(sfxSelect);
                }
                break;
            }
        }

        // Parçacıklar ve sarsıntı bitiş ekranına geçilse bile sönümlenene kadar devam etsin
        for (auto& p : particles) p.Update(dt);
        particles.erase(std::remove_if(particles.begin(), particles.end(), [](const Particle& p) { return !p.active; }), particles.end());
        shaker.Update(dt, camera);

        // =====================================================================
        // DURUM MAKİNESİ: ÇİZİM (DRAW)
        // =====================================================================
        window.BeginDrawing();
            window.ClearBackground(BLACK);

            // Arka plan yıldızlarını çiz
            for (const auto& star : stars) {
                DrawCircleV(star.position, star.size, star.color);
            }

        if (currentState == GameState::MENU) {
            // --- ANA MENÜ EKRANI ---
            DrawCenteredText("SPACE INVADERS", 150, 48, GREEN, screenWidth);
            DrawCenteredText("RAYLIB C++ ARCADE EDITION", 210, 18, LIME, screenWidth);

            // --- SÜRÜM VE YAPIMCI BİLGİSİ ---
            DrawCenteredText("v1.0.0", 240, 15, DARKGRAY, screenWidth);
            DrawCenteredText("Gelistirici: Ilhan Sami Adakli", 260, 15, GRAY, screenWidth);

            // Dekoratif Çerçeve
            DrawRectangleLines(100, 310, 400, 160, DARKGRAY);
            DrawCenteredText("KONTROLLER", 330, 20, GOLD, screenWidth);
            DrawCenteredText("Hareket :  A / D  veya  Sol / Sag Ok", 370, 18, RAYWHITE, screenWidth);
            DrawCenteredText("Ates Etme :  SPACE (Bosluk)", 405, 18, RAYWHITE, screenWidth);

            // Yanıp sönen başlama yazısı
            float blinkAlpha = (std::sin(GetTime() * 5.0f) + 1.0f) / 2.0f;
            DrawCenteredText("BASLAMAK ICIN [ENTER] VEYA [SPACE] BASIN", 520, 20, Fade(YELLOW, 0.3f + blinkAlpha * 0.7f), screenWidth);

            if (highScore > 0) {
                DrawCenteredText(TextFormat("EN YUKSEK SKOR: %i", highScore), 620, 20, GOLD, screenWidth);
            }
            } else {
                // --- OYUN DÜNYASI (PLAYING, VICTORY, GAME_OVER) ---
                camera.BeginMode();
                    for (const auto& enemy : enemies) enemy.Draw();
                    for (const auto& laser : playerLasers) laser.Draw();
                    for (const auto& laser : enemyLasers)  laser.Draw();
                    for (const auto& p : particles) p.Draw();
                    ship.Draw();
                camera.EndMode();

                // --- ÜST GÖSTERGE PANELİ (HUD) ---
                DrawRectangle(0, 0, screenWidth, 42, Fade(DARKGRAY, 0.4f));
                DrawLine(0, 42, screenWidth, 42, GRAY);

                DrawText("CAN:", 15, 13, 16, RAYWHITE);
                DrawHealthBar(60, 13, 120, 16, ship.currentHealth, ship.maxHealth);

                DrawText(TextFormat("SKOR: %i", score), 220, 13, 18, GOLD);
                DrawText(TextFormat("SURE: %.1f sn", playTime), 430, 13, 18, SKYBLUE);

                // --- ZAFER VEYA YENİLGİ BİTİŞ PANELİ (OVERLAY) ---
                if (currentState == GameState::VICTORY || currentState == GameState::GAME_OVER) {
                    // Arka planı hafif karart ve ortada şık bir özet kutusu aç
                    DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, 0.75f));

                    int boxX = 80, boxY = 170, boxW = 440, boxH = 360;
                    DrawRectangle(boxX, boxY, boxW, boxH, Fade(DARKGRAY, 0.3f));
                    raylib::Color borderColor = (currentState == GameState::VICTORY) ? GREEN : RED;
                    DrawRectangleLines(boxX, boxY, boxW, boxH, borderColor);

                    if (currentState == GameState::VICTORY) {
                        DrawCenteredText("ZAFER! DALGA TEMIZLENDI!", boxY + 30, 26, GREEN, screenWidth);
                    } else {
                        DrawCenteredText("GEMIN YOK EDILDI!", boxY + 30, 28, RED, screenWidth);
                    }

                    // İstatistik ve Skor Özeti
                    DrawLine(boxX + 40, boxY + 75, boxX + boxW - 40, boxY + 75, GRAY);

                    DrawCenteredText(TextFormat("Bitis Suresi :  %.2f Saniye", playTime), boxY + 100, 20, SKYBLUE, screenWidth);
                    DrawCenteredText(TextFormat("Kalan Düsman :  %zu", enemies.size()), boxY + 135, 18, LIGHTGRAY, screenWidth);

                    if (currentState == GameState::VICTORY) {
                        DrawCenteredText(TextFormat("Hiz ve Can Bonusu :  +%i Puan", timeBonus + healthBonus), boxY + 170, 18, LIME, screenWidth);
                    }

                    DrawCenteredText(TextFormat("TOPLAM SKOR :  %i", score), boxY + 215, 28, GOLD, screenWidth);
                    DrawCenteredText(TextFormat("En Yuksek Rekor :  %i", highScore), boxY + 255, 18, ORANGE, screenWidth);

                    DrawLine(boxX + 40, boxY + 290, boxX + boxW - 40, boxY + 290, GRAY);
                    DrawCenteredText("[R / ENTER] Yeniden Oyna    |    [M] Ana Menu", boxY + 312, 17, RAYWHITE, screenWidth);
                }
            }

        window.EndDrawing();
    }

    UnloadSound(sfxPlayerLaser);
    UnloadSound(sfxEnemyLaser);
    UnloadSound(sfxHit);
    UnloadSound(sfxExplosion);
    UnloadSound(sfxWin);
    UnloadSound(sfxSelect);
    CloseAudioDevice();

    return 0;
}