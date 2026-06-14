#include "TerminalEngine.hpp"

// Constructor gövdesi (Eğer .hpp içinde inline yapmadıysan)
TerminalEngine::TerminalEngine(int w, int h) : width(w), height(h) {
    screen_buffer.resize(width * height);
    render_cache.reserve(width * height * 15);
}

void TerminalEngine::BeginDrawing() {
    for (auto& pixel : screen_buffer) {
        pixel.character = ' ';
        pixel.color = "\033[0m";
    }
}

void TerminalEngine::DrawPixel(int x, int y, char c, const std::string& color) {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        int idx = get_index(x, y);
        screen_buffer[idx].character = c;
        screen_buffer[idx].color = color;
    }
}

void TerminalEngine::DrawText(int x, int y, const std::string& text, const std::string& color) {
    for (size_t i = 0; i < text.length(); ++i) {
        DrawPixel(x + i, y, text[i], color);
    }
}

void TerminalEngine::DrawRectangle(int startX, int startY, int rectWidth, int rectHeight, char c, const std::string& color, bool fill) {
    for (int y = 0; y < rectHeight; ++y) {
        for (int x = 0; x < rectWidth; ++x) {
            if (!fill && x > 0 && x < rectWidth - 1 && y > 0 && y < rectHeight - 1) {
                continue; 
            }
            DrawPixel(startX + x, startY + y, c, color);
        }
    }
}

void TerminalEngine::DrawProgressBar(int x, int y, int barWidth, float percentage, const std::string& barColor, const std::string& bgColor) {
    if (percentage < 0.0f) percentage = 0.0f;
    if (percentage > 1.0f) percentage = 1.0f;

    int filledWidth = (int)(barWidth * percentage);
    DrawRectangle(x, y, barWidth, 1, '.', bgColor);
    DrawRectangle(x, y, filledWidth, 1, '#', barColor);
}

void TerminalEngine::DrawCircle(int centerX, int centerY, int radius, char c, const std::string& color) {
    float aspect_ratio = 2.0f; 
    for (int y = -radius; y <= radius; ++y) {
        for (int x = (int)(-radius * aspect_ratio); x <= (int)(radius * aspect_ratio); ++x) {
            if ((x * x) / (aspect_ratio * aspect_ratio) + y * y <= radius * radius) {
                DrawPixel(centerX + x, centerY + y, c, color);
            }
        }
    }
}

void TerminalEngine::EndDrawing() {
    render_cache = "\033[H"; 
    std::string last_color = "";
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            const auto& pixel = screen_buffer[get_index(x, y)];
            if (pixel.color != last_color) {
                render_cache += pixel.color;
                last_color = pixel.color;
            }
            render_cache += pixel.character;
        }
        if (y < height - 1) render_cache += "\n";
    }
    render_cache += "\033[0m"; 
    std::cout << render_cache << std::flush;
}