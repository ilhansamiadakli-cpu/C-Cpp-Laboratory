#include <iostream>
#include <termios.h>
#include <unistd.h>

class TermConfig {
private:
    struct termios m_original_termios;
    bool m_isRaw;

public:
    TermConfig() : m_isRaw(false) {
        tcgetattr(STDIN_FILENO, &m_original_termios);
    }

    // DİKKAT 1: Geri dönüş tipi artık 'void' değil, 'TermConfig&' (Sınıfın referansı)
    TermConfig& enableRawMode() {
        if (!m_isRaw) {
            struct termios raw = m_original_termios;
            raw.c_lflag &= ~(ICANON | ECHO);
            tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
            m_isRaw = true;
        }
        // DİKKAT 2: Sınıfın kendisini referans olarak geri fırlatıyoruz!
        return *this; 
    }

    // Yeni bir özellik: Ekranı temizler (ANSI Escape Code ile)
    TermConfig& clearScreen() {
        // \033[2J ekranı temizler, \033[H imleci sol üst köşeye (0,0) alır
        std::cout << "\033[2J\033[H"; 
        return *this;
    }

    // Yeni bir özellik: İmleci gizler
    TermConfig& hideCursor() {
        std::cout << "\033[?25l";
        return *this;
    }

    TermConfig& disableRawMode() {
        if (m_isRaw) {
            tcsetattr(STDIN_FILENO, TCSAFLUSH, &m_original_termios);
            m_isRaw = false;
        }
        return *this;
    }

    // Yıkıcı (Destructor) - İmleci geri getirir ve raw modu kapatır
    ~TermConfig() {
        std::cout << "\033[?25h"; // İmleci tekrar görünür yap
        disableRawMode();
    }
};

int main() {
    TermConfig terminal;
    
    // İŞTE MÜHENDİSLİK ZARAFETİ BURADA BAŞLIYOR!
    // Tek satırda: Raw moda geç, ekranı temizle ve imleci gizle.
    terminal.enableRawMode().clearScreen().hideCursor();
    
    std::cout << "Akici Arayuz (Fluent Interface) devrede! (Cikmak icin 'q'): " << std::flush;
    
    char c;
    while (read(STDIN_FILENO, &c, 1) == 1 && c != 'q') {
        std::cout << "\nBasilan: " << c << "\r\n";
    }
    
    return 0;
}