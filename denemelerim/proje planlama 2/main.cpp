#include <iostream>
#include <termios.h>
#include <unistd.h>

class TermConfig {
private:
    struct termios m_original_termios;
    bool m_isRaw = false;

public:
    TermConfig() {
        tcgetattr(STDIN_FILENO, &m_original_termios);
    }

    TermConfig& enableRawMode() {
        if (!m_isRaw) {
            struct termios raw = m_original_termios;
            raw.c_cc[VMIN] = 0;  // Okumak için minimum karakter sayısı: 0
            raw.c_cc[VTIME] = 0; // Zaman aşımı (Timeout): 0
            raw.c_lflag &= ~(ICANON | ECHO);
            tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
            m_isRaw = true;
        }
        return *this;
    }

    TermConfig& clearScreen() {
        std::cout << "\033[2J\033[H";
        return *this;
    }

    ~TermConfig() {
        if (m_isRaw) {
            tcsetattr(STDIN_FILENO, TCSAFLUSH, &m_original_termios);
        }
    }
};
int main() {
    struct Vector{
        int x;
        int y;
    };
    TermConfig terminal;
    terminal.enableRawMode().clearScreen();
    const int WIDTH = 20;// x
    const int HEIGHT = 10;// y
    
    char back_buffer[WIDTH * HEIGHT];

    char c;
    Vector player;
    player.x = 3;
    player.y = 5;
    while (true) {
        std::cout << "\033[2J\033[H";

        // 2. BUFFER'I SIFIRLA
        for(int i = 0; i < WIDTH * HEIGHT; i++){
            back_buffer[i] = '.';
        }

        // 3. GİRDİYİ OKU VE POSİZYONU GÜNCELLE
        int n = read(STDIN_FILENO, &c, 1);
        if(n > 0){
            if(c == 'q' || c == 'Q') break;
            if(c == 'w' || c == 'W') player.y--;
            if(c == 'a' || c == 'A') player.x--;
            if(c == 's' || c == 'S') player.y++;
            if(c == 'd' || c == 'D') player.x++;
            
            // Ekran sınırlarından dışarı çıkmama koruması (Sınır Testi)
            if (player.x < 0) player.x = 0;
            if (player.x >= WIDTH) player.x = WIDTH - 1;
            if (player.y < 0) player.y = 0;
            if (player.y >= HEIGHT) player.y = HEIGHT - 1;
        }

        // 4. OYUNCUYU DOĞRUDAN BUFFER'A YERLEŞTİR (Döngüsüz, tek hamlede ışık hızı)
        back_buffer[(player.y * WIDTH) + player.x] = '@';

        // 5. BUFFER'I EKRANA BAS (Her halükarda çizilmesi için else dışına çıkardık)
        for (int y = 0; y < HEIGHT; y++) {
            for (int x = 0; x < WIDTH; x++) {
                std::cout << back_buffer[(y * WIDTH) + x];
            }
            std::cout << "\r\n"; 
        }

        usleep(100000);
    }

    return 0;
}