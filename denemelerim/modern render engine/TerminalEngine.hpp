#pragma once
#include "Pixel.hpp"
#include <vector>
#include <string>
#include <iostream>

class TerminalEngine {
private:
    int width;
    int height;
    std::vector<Pixel> screen_buffer;  // 2D yerine tek boyutlu vektör (Önbellek dostu!)
    std::string render_cache;          // Ekran kartına tek seferde basılacak string

    // 2D koordinatı 1D vektör indeksine çeviren inline yardımcı metot
    inline int get_index(int x, int y) const { return (y * width) + x; }

public:
    // Constructor
    TerminalEngine(int w, int h);

    // Temel çizim fonksiyonları
    void BeginDrawing();
    void EndDrawing();
    
    // Pixel ve şekil çizme
    void DrawPixel(int x, int y, char c, const std::string& color);
    void DrawText(int x, int y, const std::string& text, const std::string& color);
    
    // Şekil çizme fonksiyonları
    void DrawRectangle(int startX, int startY, int rectWidth, int rectHeight, 
                      char c, const std::string& color, bool fill = true);
    void DrawCircle(int centerX, int centerY, int radius, char c, const std::string& color);
    void DrawProgressBar(int x, int y, int barWidth, float percentage, 
                        const std::string& barColor, const std::string& bgColor);
};
