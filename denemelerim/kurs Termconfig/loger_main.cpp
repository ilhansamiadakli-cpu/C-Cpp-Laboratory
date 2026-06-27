#include <iostream>
#include <vector>
#include <string>

class Logger {
private:
    // Bu bizim odamızdaki çöp kutumuz (RAM'deki Tampon/Buffer)
    std::vector<std::string> m_buffer; 

public:
    // Sadece hafızaya atar, ASLA o an ekrana bir şey basmaz.
    // DİKKAT: 'const std::string&' kullanarak kopyalama maliyetinden kurtuluyoruz.
    void addLog(const std::string& message) {
        m_buffer.push_back(message);
    }

    // Çöpü dışarı dökme (Flush) operasyonu
    void printLogs() {
        if (m_buffer.empty()) return; // Kutu boşsa yorulma

        std::cout << "\n--- KAYITLI LOGLAR BASTIRILIYOR ---\n";
        for (const auto& log : m_buffer) {
            std::cout << "[SİSTEM]: " << log << "\n";
        }
        
        // Bastıktan sonra hafızayı temizle ki RAM şişmesin
        m_buffer.clear(); 
    }

    // YIKICI (Destructor): Sigorta poliçemiz!
    // Ya kullanıcı printLogs() demeyi unutursa? Bu yıkıcı bizi kurtaracak.
    ~Logger() {
        printLogs();
    }
};

int main() {
    // Sadece basit bir blok (scope) oluşturuyoruz ki nesnenin ömrü erken bitsin
    {
        Logger sistemLog;
        
        sistemLog.addLog("Oyun motoru baslatildi.");
        sistemLog.addLog("Hafiza tahsisi (malloc) basarili.");
        sistemLog.addLog("Dokular (Textures) yuklendi.");
        
        std::cout << "Su an loglar eklendi ama ekranda hicbir sey yok!\n";
        std::cout << "Blok (scope) kapaninca yikici devreye girecek...\n";
    } // 'sistemLog' nesnesinin ömrü tam burada biter!

    return 0;
}