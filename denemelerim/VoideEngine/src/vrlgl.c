#include "../include/vrlgl.h" // şimdilik böyle
#include <_stdio.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>

static struct termios orig_termios;
static int g_width = 0;
static int g_height = 0;

void vrlgl_Init(int width, int height) {
    g_width = width;
    g_height = height;
    
    tcgetattr(STDIN_FILENO, &orig_termios);
    struct termios raw = orig_termios;
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 0;
    raw.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void vrlgl_Close(void) {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

void vrlgl_Clear(void) {
    printf("\033[2J\033[H");
    fflush(stdout);
}

void vrlgl_PresentFrame(const vrlgl_Cell* back_buffer) {
    // Önceki karedeki renkleri takip eden değişkenler (Başlangıçta imkansız bir değer)
    int last_r = -1, last_g = -1, last_b = -1;

    for (int y = 0; y < g_height; y++) {
        for (int x = 0; x < g_width; x++) {
            vrlgl_Cell cell = back_buffer[y * g_width + x];

            // OPTİMİZASYON: Renk bir önceki hücreyle aynı mı?
            if (cell.r != last_r || cell.g != last_g || cell.b != last_b) {
                // Hayır, renk değişti! Terminale 24-bit True Color kodunu fırlat
                printf("\033[38;2;%d;%d;%dm", cell.r, cell.g, cell.b);
                
                // Durumu güncelle
                last_r = cell.r;
                last_g = cell.g;
                last_b = cell.b;
            }

            // Karakteri bas (Işık hızında)
            putchar(cell.ch);
        }
        // Satır sonu pürüzsüzlüğü
        putchar('\r');
        putchar('\n');
    }
    
    // İşimiz bittiğinde terminal rengini sıfırla ki terminalimiz bozuk kalmasın
    printf("\033[0m");
    fflush(stdout);
}
char vrlgl_getRawKey(){
    char c;
    int n = read(STDIN_FILENO, &c, 1);
    if(n>0) return c;
    return 0;
}