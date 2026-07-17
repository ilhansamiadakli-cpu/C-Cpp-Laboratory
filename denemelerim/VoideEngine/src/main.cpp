#include <unistd.h>
#include "../include/vrlgl.h"

struct Vector2D{
    int x;
    int y;
};
const int WIDTH = 20;// x
const int HEIGHT = 10;// y

int main() {
    vrlgl_Init(WIDTH, HEIGHT);
    vrlgl_Clear();
    vrlgl_Cell back_buffer[WIDTH * HEIGHT];
    Vector2D player;
    player.x = 5;
    player.y = 5;
    char tus;
    while (true) {
        vrlgl_Clear();
        for(int i = 0; i < WIDTH * HEIGHT; i++){
            back_buffer[i].ch = '.';
            back_buffer[i].r = 20;
            back_buffer[i].g = 30;
            back_buffer[i].b = 50;
        }
        tus = vrlgl_getRawKey();
        if(tus == 'q' || tus == 'Q') break;
        if(tus == 'w' || tus == 'W') player.y--;
        if(tus == 'a' || tus == 'A') player.x--;
        if(tus == 's' || tus == 'S') player.y++;
        if(tus == 'd' || tus == 'D') player.x++;

        if (player.x < 0) player.x = 0;
        if (player.x >= WIDTH) player.x = WIDTH - 1;
        if (player.y < 0) player.y = 0;
        if (player.y >= HEIGHT) player.y = HEIGHT - 1;

        back_buffer[(player.y * WIDTH) + player.x].ch = '@';
        back_buffer[(player.y * WIDTH) + player.x].r = 255;
        back_buffer[(player.y * WIDTH) + player.x].g = 60;
        back_buffer[(player.y * WIDTH) + player.x].b = 0;
        vrlgl_PresentFrame(back_buffer);

        usleep(100000);
    }


    vrlgl_Close();
    return 0;
}