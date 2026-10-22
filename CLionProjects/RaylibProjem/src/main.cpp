#include "raylib-cpp.hpp"

int main() {
    // ---------------------------------------------------------------------
    // ADIM 1: BAŞLATMA (INITIALIZATION)
    // Oyun dünyasının temel ayarlarını burada yapıyoruz.
    // ---------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 600;

    // raylib::Window nesnesi oluşturulduğunda pencere açılır.
    // Nesnenin yaşam süresi (scope) bittiğinde pencere otomatik kapanır (RAII mantığı).
    raylib::Window window(screenWidth, screenHeight, "Raylib'e Giris - Adim Adim Ogretici");

    // Oyunun saniyede kaç kare (FPS) hızında çalışacağını belirliyoruz.
    // Bunu yazmazsan bilgisayarın gücüne göre oyun aşırı hızlı (örn: 2000 FPS) çalışabilir.
    SetTargetFPS(60);

    // ---------------------------------------------------------------------
    // ADIM 2: OYUN DEĞİŞKENLERİ (GAME STATE)
    // Oyuncunun konumu, hızı, rengi gibi kalıcı bilgileri burada tanımlarız.
    // ---------------------------------------------------------------------
    raylib::Vector2 playerPos(screenWidth / 2.0f, screenHeight / 2.0f);
    float playerSpeed = 5.0f;
    float playerRadius = 25.0f;

    int score = 0; // Tıklama ile artıracağımız basit bir skor

    // ---------------------------------------------------------------------
    // ADIM 3: ANA OYUN DÖNGÜSÜ (GAME LOOP)
    // Pencere kapatma tuşuna (X) veya ESC'ye basılana kadar bu döngü saniyede 60 kez döner.
    // ---------------------------------------------------------------------
    while (!window.ShouldClose()) {

        // --- A) GİRDİ (INPUT) & GÜNCELLEME (UPDATE) KISMI ---
        // Klavyeden veya fareden gelen verileri okuyup değişkenleri güncelleriz.

        // IsKeyDown: Tuşa basılı tutulduğu sürece TRUE döner (Sürekli hareket için).
        if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) playerPos.x += playerSpeed;
        if (IsKeyDown(KEY_LEFT)  || IsKeyDown(KEY_A)) playerPos.x -= playerSpeed;
        if (IsKeyDown(KEY_UP)    || IsKeyDown(KEY_W)) playerPos.y -= playerSpeed;
        if (IsKeyDown(KEY_DOWN)  || IsKeyDown(KEY_S)) playerPos.y += playerSpeed;

        // IsKeyPressed: Tuşa sadece ilk basıldığı an TRUE döner (Ateş etme, zıplama için).
        if (IsKeyPressed(KEY_SPACE)) {
            playerRadius += 5.0f; // Boşluğa basınca karakteri büyüt
        }

        // Fare girdisi kontrolü
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            score++; // Sol tıklandığında skoru artır
        }

        // Ekran Sınırları Kontrolü (Karakter ekrandan çıkmasın)
        // Raylib'de (0,0) koordinatı ekranın SOL ÜST köşesidir.
        if (playerPos.x < playerRadius) playerPos.x = playerRadius;
        if (playerPos.x > screenWidth - playerRadius) playerPos.x = screenWidth - playerRadius;
        if (playerPos.y < playerRadius) playerPos.y = playerRadius;
        if (playerPos.y > screenHeight - playerRadius) playerPos.y = screenHeight - playerRadius;


        // --- B) ÇİZİM (DRAW) KISMI ---
        // Arka planda hesaplanan değerleri ekrana basarız.

        window.BeginDrawing(); // Çizim işlemlerini başlat (GPU'ya veri göndermeye başla)

            // Her karenin başında ekranı tek bir renkle temizlemek zorundayız.
            // Temizlemezsek önceki karenin izleri (Windows XP'deki pencereler gibi) ekranda kalır.
            window.ClearBackground(RAYWHITE);

            // Metin Çizimi: (Yazı, X, Y, Font Boyutu, Renk)
            DrawText("Hareket: WASD/YON TUSLARI", 10, 10, 20, DARKGRAY);
            DrawText("Buyumek icin SPACE'e bas", 10, 40, 20, DARKGRAY);
            DrawText(TextFormat("Sol Tik Skor: %i", score), 10, 70, 20, BLUE);

            // FPS değerini sağ üst köşeye otomatik çizdirir
            DrawFPS(screenWidth - 100, 10);

            // Sabit bir dikdörtgen çizimi: (X, Y, Genişlik, Yükseklik, Renk)
            DrawRectangle(screenWidth / 2 - 50, 150, 100, 50, LIGHTGRAY);
            DrawText("Sabit Nesne", screenWidth / 2 - 40, 165, 15, BLACK);

            // Oyuncu (Daire) Çizimi
            DrawCircleV(playerPos, playerRadius, MAROON);

        window.EndDrawing(); // Çizimi bitir ve ekrana (Monitöre) yansıt
    }

    // ---------------------------------------------------------------------
    // ADIM 4: KAPATMA (DE-INITIALIZATION)
    // while döngüsü bittiğinde program buraya gelir.
    // raylib::Window nesnesi yok edilirken pencereyi ve GPU kaynaklarını otomatik temizler.
    // ---------------------------------------------------------------------

    return 0;
}