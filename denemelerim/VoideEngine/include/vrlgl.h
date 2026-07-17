#ifndef VRLGL_H
#define VRLGL_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    char ch;              // 1 byte (Karakter)
    unsigned char r;      // 1 byte (Kırmızı: 0-255)
    unsigned char g;      // 1 byte (Yeşil: 0-255)
    unsigned char b;      // 1 byte (Mavi: 0-255)
} vrlgl_Cell;
void vrlgl_Init(int width, int height);

void vrlgl_Close(void);

void vrlgl_Clear(void);

void vrlgl_PresentFrame(const vrlgl_Cell* back_buffer);

char vrlgl_getRawKey();

#ifdef __cplusplus
}
#endif

#endif // VRLGL_H