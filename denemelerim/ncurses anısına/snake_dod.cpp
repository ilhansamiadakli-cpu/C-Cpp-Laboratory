// --- PLATFORM BAZLI KÜTÜPHANE SEÇİMİ ---
#if defined(_WIN32) || defined(_WIN64)
    #include <curses.h>   // Windows'ta pdcurses için
#else
    #include <ncurses.h>  // macOS/Linux için
#endif

#include <cstdlib>
#include <ctime>
#include <thread>         // usleep yerine platform bağımsız bekleme için
#include <chrono>         // zaman birimleri için

constexpr int MAX_SNAKE_LEN = 10000; 

// --- 1. VERİ MİMARİSİ (DOD - SoA) ---
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

    int width;
    int height;
    bool running;
};

// --- 2. SİSTEMLER ---

void spawn_fruit_system(GameState& state, int index) {
    state.fruit_x[index] = rand() % state.width;
    // Meyveler 0. satırda (UI barı) çıkamaz. 1 ile height-1 arası.
    state.fruit_y[index] = (rand() % (state.height - 1)) + 1; 
}

void init_system(GameState& state) {
    getmaxyx(stdscr, state.height, state.width);
    state.snake_len = 5;
    state.max_snake_len = 5;
    state.dir_x = 1;
    state.dir_y = 0;
    state.running = true;

    for(int i = 0; i < state.snake_len; ++i) {
        state.snake_x[i] = (state.width / 2) - i;
        state.snake_y[i] = state.height / 2;
    }

    state.fruit_type[0] = 0; // Kırmızı
    state.fruit_type[1] = 1; // Mavi
    spawn_fruit_system(state, 0);
    spawn_fruit_system(state, 1);
}

void input_system(GameState& state) {
    int ch = getch();
    switch(ch) {
        case KEY_UP:    if (state.dir_y != 1)  { state.dir_x = 0; state.dir_y = -1; } break;
        case KEY_DOWN:  if (state.dir_y != -1) { state.dir_x = 0; state.dir_y = 1; } break;
        case KEY_LEFT:  if (state.dir_x != 1)  { state.dir_x = -1; state.dir_y = 0; } break;
        case KEY_RIGHT: if (state.dir_x != -1) { state.dir_x = 1; state.dir_y = 0; } break;
        case 'q': state.running = false; break;
    }
}

void movement_and_collision_system(GameState& state) {
    int next_x = state.snake_x[0] + state.dir_x;
    int next_y = state.snake_y[0] + state.dir_y;

    // Duvar Çarpışması - X Ekseni
    next_x = (next_x + state.width) % state.width;
    if (next_x < 0) next_x += state.width; // Negatif modülüs koruması
    
    // Duvar Çarpışması - Y Ekseni (0. satır UI)
    int game_height = state.height - 1;
    next_y = next_y - 1; 
    if (next_y < 0) next_y += game_height;
    next_y = (next_y % game_height) + 1;

    // Kendine Çarpma Kontrolü
    for(int i = 0; i < state.snake_len; ++i) {
        if(state.snake_x[i] == next_x && state.snake_y[i] == next_y) {
            state.running = false;
            return;
        }
    }

    // Shift işlemi
    for(int i = state.snake_len - 1; i > 0; --i) {
        state.snake_x[i] = state.snake_x[i - 1];
        state.snake_y[i] = state.snake_y[i - 1];
    }
    state.snake_x[0] = next_x;
    state.snake_y[0] = next_y;

    // Meyve Çarpışması
    for(int i = 0; i < 2; ++i) {
        if(state.snake_x[0] == state.fruit_x[i] && state.snake_y[0] == state.fruit_y[i]) {
            if(state.fruit_type[i] == 0) {
                state.snake_len++;
                if(state.snake_len > state.max_snake_len) {
                    state.max_snake_len = state.snake_len;
                }
            } else {
                state.snake_len--;
                if(state.snake_len <= 0) {
                    state.running = false;
                    return;
                }
            }
            spawn_fruit_system(state, i);
        }
    }
}

void render_system(const GameState& state) {
    clear();

    // 1. UI: Üst Bar Çizimi
    attron(A_REVERSE);
    for(int x = 0; x < state.width; ++x) {
        mvaddch(0, x, ' '); 
    }
    mvprintw(0, 2, " OYUN: CANAVAR MAGARASI | MEVCUT BOYUT: %d | REKOR BOYUT: %d ", state.snake_len, state.max_snake_len);
    attroff(A_REVERSE);

    // 2. Meyveleri Çiz
    for(int i = 0; i < 2; ++i) {
        if(state.fruit_type[i] == 0) {
            attron(COLOR_PAIR(1));
            mvaddch(state.fruit_y[i], state.fruit_x[i], '@');
            attroff(COLOR_PAIR(1));
        } else {
            attron(COLOR_PAIR(2));
            mvaddch(state.fruit_y[i], state.fruit_x[i], '@');
            attroff(COLOR_PAIR(2));
        }
    }

    // 3. Yılanı Çiz
    attron(COLOR_PAIR(3));
    for(int i = 0; i < state.snake_len; ++i) {
        mvaddch(state.snake_y[i], state.snake_x[i], i == 0 ? 'O' : 'o');
    }
    attroff(COLOR_PAIR(3));

    refresh();
}

// --- 3. ANA DÖNGÜ ---
int main() {
    initscr();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    start_color();
    
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_BLUE, COLOR_BLACK);
    init_pair(3, COLOR_GREEN, COLOR_BLACK);

    srand(time(NULL));

    GameState* state = new GameState();
    init_system(*state);

    while(state->running) {
        input_system(*state);
        movement_and_collision_system(*state);
        render_system(*state);
        
        // Platform bağımsız uyku (C++11 ve sonrası)
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); 
    }

    nodelay(stdscr, FALSE);
    clear();
    
    int cy = state->height / 2;
    int cx = state->width / 2;
    
    mvprintw(cy - 3, cx - 12, "========================");
    mvprintw(cy - 2, cx - 6,  " OYUN BITTI! ");
    mvprintw(cy - 1, cx - 12, "========================");
    mvprintw(cy + 1, cx - 10, "Ulasilan Skor: %d", state->max_snake_len);
    mvprintw(cy + 2, cx - 10, "Bitis Boyutu : %d", state->snake_len);
    
    if (state->snake_len <= 0) {
        attron(COLOR_PAIR(1));
        mvprintw(cy + 4, cx - 15, "(Mavi meyveden dolayi yok oldun)");
        attroff(COLOR_PAIR(1));
    }
    
    mvprintw(cy + 6, cx - 14, "Cikmak icin bir tusa basin...");
    refresh();
    
    getch();

    delete state;
    endwin();
    return 0;
}