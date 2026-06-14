#ifndef COLORS_H
#define COLORS_H

#ifdef __cplusplus
extern "C" {
#endif

//----- Windows için Gerekli Ayarlar -----
#ifdef _WIN32
    #include <windows.h>
    #include <stdio.h>

    #ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
    #define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
    #endif
#endif

//----- Yazı Renkleri -----
#define YESIL      "\033[1;32m"
#define BEYAZ      "\033[0m"
#define KIRMIZI    "\033[1;31m"
#define SARI       "\033[1;33m"
#define MAVI       "\033[1;34m"
#define ACIK_MAVI  "\033[1;36m"
#define MOR        "\033[1;35m"
#define SIYAH      "\033[1;30m"
#define CYAN       "\033[1;36m"
#define MAGENTA    "\033[1;35m"

//----- Arka Plan Renkleri -----
#define SARI_ARKA       "\033[43m"
#define MAVI_ARKA       "\033[44m"
#define MOR_ARKA        "\033[45m"
#define ACIK_MAVI_ARKA  "\033[46m"
#define KIRMIZI_ARKA    "\033[41m"
#define YESIL_ARKA      "\033[42m"
#define BEYAZ_ARKA      "\033[47m"
#define SIYAH_ARKA      "\033[40m" 

//----- Stil Tanımları -----
#define BOLD       "\033[1m"          // Kalın yazı
#define DIM        "\033[2m"          // Soluk yazı
#define UNDERLINE  "\033[4m"          // Altı çizili
#define REVERSE    "\033[7m"          // Ters renk
#define RESET      "\033[0m"          // Sıfırla

#ifdef __cplusplus
}
#endif

#endif // COLORS_H