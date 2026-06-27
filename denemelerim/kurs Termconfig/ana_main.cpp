#include <iostream>
#include <vector>
#include <string>
#include <termios.h>
#include <unistd.h>

// ========================================================
// 1. MODÜL: TERMİNAL AYARLARI (Fluent Interface / Akıcı Arayüz)
// ========================================================
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

// ========================================================
// 2. MODÜL: LOG YÖNETİMİ (Singleton & Buffer)
// ========================================================
class Logger {
private:
    std::vector<std::string> m_buffer;

    // KURAL 1: Kurucu private! Dışarıdan nesne üretmek yasak!
    Logger() = default;

public:
    // KURAL 2: Sistemin yegane nesnesini (Instance) dönen statik fonksiyon
    static Logger& getInstance() {
        static Logger instance; // Sadece ilk çağrıldığında RAM'de oluşur
        return instance;
    }

    // KURAL 3: Nesnenin klonlanmasını (kopyalanmasını) kesin olarak yasakla
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    void addLog(const std::string& message) {
        m_buffer.push_back(message);
    }

    void printLogs() {
        if (m_buffer.empty()) return;
        
        std::cout << "\r\n--- SISTEM LOGLARI ---\r\n";
        for (const auto& log : m_buffer) {
            std::cout << "[KAYIT]: " << log << "\r\n";
        }
        m_buffer.clear();
    }

    ~Logger() {
        printLogs();
    }
};

// ========================================================
// 3. INLINE KISA YOLLAR (Kullanıcının Hayatını Kurtaran Bölüm)
// ========================================================
// Kullanıcı artık getInstance() vs. yazmayacak. Sadece bu fonksiyonları çağıracak.
inline void Log(const std::string& msg) { Logger::getInstance().addLog(msg); }
inline void LogPrint()                  { Logger::getInstance().printLogs(); }

// ========================================================
// ANA PROGRAM (Kütüphanemizi Kullanan Kişinin Ekranı)
// ========================================================
int main() {
    // Terminali tek satırda şekillendir
    TermConfig terminal;
    terminal.enableRawMode().clearScreen();

    std::cout << "Sistem baslatildi. 'q' ile cikis yapabilirsiniz.\r\n";

    // Sıfır nesne üretimi! Her yerden sadece Log() yazarak kayıt ekliyoruz
    Log("Terminal Raw Moda alindi.");
    Log("Ekran temizleme (ClearScreen) calisti.");
    
    char c;
    while (read(STDIN_FILENO, &c, 1) == 1 && c != 'q') {
        std::cout << "Girdi: " << c << "\r\n";
        
        // Eğer kullanıcı boşluk (space) tuşuna basarsa özel bir log ekle
        if (c == ' ') {
            Log("Kullanici bosluk tusuna basti!");
        }
    }

    Log("Program q tusuna basilarak normal sekilde kapatiliyor.");

    // Program biterken sırasıyla:
    // 1. main fonksiyonu kapanır.
    // 2. Global Logger'ın yıkıcısı devreye girip tüm biriken logları ekrana basar.
    // 3. 'terminal' nesnesinin yıkıcısı devreye girip terminali eski haline döndürür.
    return 0;
}