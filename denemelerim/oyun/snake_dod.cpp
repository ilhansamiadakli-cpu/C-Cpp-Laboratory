#include <iostream>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>

// --- TÜM TERMİNALLERE UYMA STRATEJİSİ (CROSS-PLATFORM LAYER) ---
#if defined(_WIN32) || defined(_WIN64)
    #include <windows.h>
    #include <conio.h>
    #define PLATFORM_WINDOWS
#else
    #include <ncurses.h> // Mac/Linux için efsanevi ncurses
    #define PLATFORM_POSIX
#endif

constexpr int MAX_SNAKE_LEN = 10000;
constexpr int SCREEN_WIDTH = 40;
constexpr int SCREEN_HEIGHT = 20;

struct GameState {
    int snake_x[MAX_SNAKE_LEN];
    int snake_y[MAX_SNAKE_LEN];
    int snake_len;
    int max_snake_len;
    int dir_x;
    int dir_y;
    int fruit_x[2];
    int fruit_y[2];
    int fruit_type[2]; 
    bool running;
};

// --- WINDOWS 8 VE ALTI İÇİN YEREL KONSOL MANİPÜLASYONU ---
#if defined(PLATFORM_WINDOWS)
void win_set_cursor(int x, int y) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(hConsole, pos);
}

void win_set_color(int color_pair) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (color_pair == 1)      SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY); // Kırmızı
    else if (color_pair == 2) SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_INTENSITY); // Mavi
    else if (color_pair == 3) SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY); // Yeşil
    else if (color_pair == 4) SetConsoleTextAttribute(hConsole, BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED); // UI Barı (Ters)
    else                      SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED); // Normal Beyaz
}
#endif

// --- SİSTEMLER ---
void spawn_fruit_system(GameState& state, int index) {
    state.fruit_x[index] = rand() % SCREEN_WIDTH;
    // Üst barı korumak için meyveleri y=1'den başlatıyoruz
    state.fruit_y[index] = (rand() % (SCREEN_HEIGHT - 1)) + 1;
}

void init_system(GameState& state) {
    state.snake_len = 5;
    state.max_snake_len = 5;
    state.dir_x = 1;
    state.dir_y = 0;
    state.running = true;

    for(int i = 0; i < state.snake_len; ++i) {
        state.snake_x[i] = (SCREEN_WIDTH / 2) - i;
        state.snake_y[i] = SCREEN_HEIGHT / 2;
    }
    state.fruit_type[0] = 0; state.fruit_type[1] = 1;
    spawn_fruit_system(state, 0);
    spawn_fruit_system(state, 1);
}

void input_system(GameState& state) {
#if defined(PLATFORM_WINDOWS)
    if (_kbhit()) {
        int ch = _getch();
        if (ch == 0 || ch == 224) ch = _getch(); // Yön tuşları için
        switch(ch) {
            case 'w': case 72: if (state.dir_y != 1)  { state.dir_x = 0; state.dir_y = -1; } break;
            case 's': case 80: if (state.dir_y != -1) { state.dir_x = 0; state.dir_y = 1; } break;
            case 'a': case 75: if (state.dir_x != 1)  { state.dir_x = -1; state.dir_y = 0; } break;
            case 'd': case 77: if (state.dir_x != -1) { state.dir_x = 1; state.dir_y = 0; } break;
            case 'q': state.running = false; break;
        }
    }
#else
    int ch = getch();
    switch(ch) {
        case KEY_UP:    if (state.dir_y != 1)  { state.dir_x = 0; state.dir_y = -1; } break;
        case KEY_DOWN:  if (state.dir_y != -1) { state.dir_x = 0; state.dir_y = 1; } break;
        case KEY_LEFT:  if (state.dir_x != 1)  { state.dir_x = -1; state.dir_y = 0; } break;
        case KEY_RIGHT: if (state.dir_x != -1) { state.dir_x = 1; state.dir_y = 0; } break;
        case 'q': state.running = false; break;
    }
#endif
}

void movement_and_collision_system(GameState& state) {
    int next_x = state.snake_x[0] + state.dir_x;
    int next_y = state.snake_y[0] + state.dir_y;

    next_x = (next_x + SCREEN_WIDTH) % SCREEN_WIDTH;
    
    // Üst UI barı korumalı Pac-man geçişi
    int game_height = SCREEN_HEIGHT - 1;
    next_y = next_y - 1;
    if (next_y < 0) next_y += game_height;
    next_y = (next_y % game_height) + 1;

    for(int i = 0; i < state.snake_len; ++i) {
        if(state.snake_x[i] == next_x && state.snake_y[i] == next_y) {
            state.running = false;
            return;
        }
    }

    for(int i = state.snake_len - 1; i > 0; --i) {
        state.snake_x[i] = state.snake_x[i - 1];
        state.snake_y[i] = state.snake_y[i - 1];
    }
    state.snake_x[0] = next_x;
    state.snake_y[0] = next_y;

    for(int i = 0; i < 2; ++i) {
        if(state.snake_x[0] == state.fruit_x[i] && state.snake_y[0] == state.fruit_y[i]) {
            if(state.fruit_type[i] == 0) {
                state.snake_len++;
                if(state.snake_len > state.max_snake_len) state.max_snake_len = state.snake_len;
            } else {
                state.snake_len--;
                if(state.snake_len <= 0) { state.running = false; return; }
            }
            spawn_fruit_system(state, i);
        }
    }
}

void render_system(const GameState& state) {
#if defined(PLATFORM_WINDOWS)
    win_set_cursor(0, 0);
    win_set_color(4); // Ters renkli bar
    std::cout << " OYUN: CANAVAR MAGARASI | BOYUT: " << state.snake_len << " | REKOR: " << state.max_snake_len << " ";
    // Barın geri kalanını doldur
    for(int i = 0; i < SCREEN_WIDTH - 46; ++i) std::cout << " ";
    std::cout << "\n";
    win_set_color(0);

    for (int y = 1; y < SCREEN_HEIGHT; ++y) {
        for (int x = 0; x < SCREEN_WIDTH; ++x) {
            bool printed = false;
            for (int i = 0; i < state.snake_len; ++i) {
                if (state.snake_x[i] == x && state.snake_y[i] == y) {
                    win_set_color(3); // Yeşil
                    std::cout << (i == 0 ? 'O' : 'o');
                    printed = true; break;
                }
            }
            if (!printed) {
                for (int i = 0; i < 2; ++i) {
                    if (state.fruit_x[i] == x && state.fruit_y[i] == y) {
                        win_set_color(state.fruit_type[i] == 0 ? 1 : 2); // Kırmızı veya Mavi
                        std::cout << '@';
                        printed = true; break;
                    }
                }
            }
            if (!printed) { win_set_color(0); std::cout << ' '; }
        }
        std::cout << '\n';
    }
#else
    // MAC İÇİN NCURSES RENDER
    clear();
    attron(A_REVERSE);
    mvprintw(0, 0, " OYUN: CANAVAR MAGARASI | BOYUT: %d | REKOR: %d ", state.snake_len, state.max_snake_len);
    attroff(A_REVERSE);

    for(int i = 0; i < 2; ++i) {
        attron(COLOR_PAIR(state.fruit_type[i] == 0 ? 1 : 2));
        mvaddch(state.fruit_y[i], state.fruit_x[i], '@');
    }
    attron(COLOR_PAIR(3));
    for(int i = 0; i < state.snake_len; ++i) {
        mvaddch(state.snake_y[i], state.snake_x[i], i == 0 ? 'O' : 'o');
    }
    attroff(COLOR_PAIR(1) | COLOR_PAIR(2) | COLOR_PAIR(3));
    refresh();
#endif
}

int main() {
    srand(time(NULL));

#if defined(PLATFORM_POSIX)
    // Mac için ncurses kurulumu
    initscr(); noecho(); curs_set(0);
    keypad(stdscr, TRUE); nodelay(stdscr, TRUE); start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_BLUE, COLOR_BLACK);
    init_pair(3, COLOR_GREEN, COLOR_BLACK);
#else
    // Windows 8 için ekran temizleme
    system("cls");
#endif

    GameState* state = new GameState();
    init_system(*state);

    while(state->running) {
        input_system(*state);
        movement_and_collision_system(*state);
        render_system(*state);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

#if defined(PLATFORM_POSIX)
    nodelay(stdscr, FALSE); clear();
    mvprintw(SCREEN_HEIGHT/2, SCREEN_WIDTH/2 - 6, "OYUN BITTI!");
    mvprintw(SCREEN_HEIGHT/2 + 2, SCREEN_WIDTH/2 - 8, "Maks Skor: %d", state->max_snake_len);
    refresh(); getch(); endwin();
#else
    system("cls");
    win_set_color(1);
    std::cout << "========================\n";
    std::cout << "       OYUN BITTI!      \n";
    std::cout << "========================\n";
    win_set_color(0);
    std::cout << "Ulasilan Skor: " << state->max_snake_len << "\n";
    std::cout << "Bitis Boyutu : " << state->snake_len << "\n";
    std::cout << "\nCikmak icin Enter'a basin...";
    std::cin.get();
#endif

    delete state;
    return 0;
}