#pragma once
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>

namespace VOD {

class VodStreamBuffer {
private:
    std::ostringstream buffer;

    void draw_box_and_flush() {
        std::string raw = buffer.str();
        buffer.str("");
        buffer.clear();

        if (raw.empty()) return;

        // Metni satırlara böl
        std::vector<std::string> lines;
        std::string line;
        std::istringstream stream(raw);
        size_t max_len = 0;

        while (std::getline(stream, line)) {
            // \r karakterlerini temizle
            if (!line.empty() && line.back() == '\r') line.pop_back();
            lines.push_back(line);
            if (line.length() > max_len) {
                max_len = line.length();
            }
        }

        if (lines.empty()) return;

        // ANSI Renk Kodları & Unicode Çerçeve Elemanları
        const std::string CYAN  = "\033[1;36m";
        const std::string GREEN = "\033[32m";
        const std::string RESET = "\033[0m";

        // 1. Üst Kenarlık ┌───┐
        std::cout << CYAN << "┌─";
        for (size_t i = 0; i < max_len + 2; ++i) std::cout << "─";
        std::cout << "┐\n" << RESET;

        // 2. İçerik Satırları │ Metin │
        for (const auto& l : lines) {
            std::cout << CYAN << "│ " << GREEN << l;
            // Sağ boşlukları doldur
            for (size_t i = 0; i < (max_len - l.length()); ++i) std::cout << " ";
            std::cout << CYAN << " │\n" << RESET;
        }

        // 3. Alt Kenarlık └───┘
        std::cout << CYAN << "└─";
        for (size_t i = 0; i < max_len + 2; ++i) std::cout << "─";
        std::cout << "┘\n" << RESET;
        std::cout.flush();
    }

public:
    // Genel tipler için operator<< (int, std::string, double vb.)
    template<typename T>
    VodStreamBuffer& operator<<(const T& val) {
        buffer << val;
        return *this;
    }

    // std::endl ve std::flush gibi manipülatörleri yakalama
    typedef std::ostream& (*StreamManipulator)(std::ostream&);
    VodStreamBuffer& operator<<(StreamManipulator manip) {
        // Eğer giden şey std::endl veya std::flush ise kutuyu çiz ve tamponu boşalt
        if (manip == static_cast<StreamManipulator>(std::endl) ||
            manip == static_cast<StreamManipulator>(std::flush)) {
            draw_box_and_flush();
        } else {
            buffer << manip;
        }
        return *this;
    }
};

inline VodStreamBuffer Cout;

} // namespace VOD