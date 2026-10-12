#include "Lexer.hpp"
#include "Parser.hpp"
#include "Evaluator.hpp"
#include "vod_cout.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <chrono>

void print_usage(const char* prog_name) {
    std::cout << "VodScript CLI v0.1.0 - Sifir Bagimlilikli Veri Donusturucu\n"
              << "Kullanim:\n"
              << "  " << prog_name << " <girdi.vod> [secenekler]\n\n"
              << "Secenekler:\n"
              << "  -o, --output <dosya.json>   Ciktinin yazilacagi dosya yolu (Varsayilan: stdout)\n"
              << "  -t, --time                  Calisma suresini mikrosaniye olarak goster\n"
              << "  -h, --help                  Bu yardim iletisini goster\n\n"
              << "Ornek:\n"
              << "  " << prog_name << " config.vod -o config.json -t\n";
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        print_usage(argv[0]);
        return 0;
    }

    std::string input_path;
    std::string output_path;
    bool measure_time = false;

    // 1. Terminal Argümanlarını Ayrıştır (Parsing CLI Flags)
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "-h" || arg == "--help") {
            print_usage(argv[0]);
            return 0;
        } else if (arg == "-t" || arg == "--time") {
            measure_time = true;
        } else if (arg == "-o" || arg == "--output") {
            if (i + 1 < argc) {
                output_path = argv[++i];
            } else {
                std::cerr << "Hata: -o bayragindan sonra cikti dosya adi belirtilmelidir.\n";
                return 1;
            }
        } else if (arg[0] == '-') {
            std::cerr << "Bilinmeyen secenek: " << arg << "\n";
            print_usage(argv[0]);
            return 1;
        } else {
            if (input_path.empty()) {
                input_path = arg;
            } else {
                std::cerr << "Hata: Birden fazla girdi dosyasi belirtilemez: " << arg << "\n";
                return 1;
            }
        }
    }

    if (input_path.empty()) {
        std::cerr << "Hata: Bir girdi .vod dosyasi belirtilmelidir.\n";
        return 1;
    }

    // 2. Girdi Dosyasını Oku
    std::ifstream file(input_path);
    if (!file.is_open()) {
        std::cerr << "Hata: '" << input_path << "' dosyasi acilamadi!\n";
        return 1;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string source = buffer.str();
    file.close();

    try {
        auto start_time = std::chrono::high_resolution_clock::now();

        // 3. Boru Hattını Çalıştır (Lexer -> Parser -> Evaluator)
        Lexer lexer(source);
        auto tokens = lexer.tokenize();

        Parser parser(std::move(tokens));
        auto ast = parser.parse_program();

        Evaluator evaluator;
        std::string json_output = evaluator.evaluate(ast.get());

        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();

        // 4. Çıktıyı Dosyaya veya Terminale Bas
        if (!output_path.empty()) {
            std::ofstream out_file(output_path);
            if (!out_file.is_open()) {
                std::cerr << "Hata: Cikti dosyasi olusturulamadi: " << output_path << "\n";
                return 1;
            }
            out_file << json_output << "\n";
            out_file.close();
            std::cout << "[+] Basariyla derlendi -> " << output_path << "\n";
        } else {
            VOD::Cout << json_output << std::endl;
        }

        if (measure_time) {
            std::cerr << "[!] Toplam islem suresi: " << duration << " mikrosaniye (" 
                      << duration / 1000.0 << " ms)\n";
        }

    } catch (const std::exception& e) {
        std::cerr << "Derleme Hatasi: " << e.what() << "\n";
        return 1;
    }

    return 0;
}