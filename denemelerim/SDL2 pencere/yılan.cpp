#include <SDL2/SDL.h>
#include <vector>
#include <cstdlib>
#include <ctime>

const int WIDTH = 600;
const int HEIGHT = 600;
const int CELL = 20;

struct Point {
    int x, y;
};

enum Dir { UP, DOWN, LEFT, RIGHT };

Dir ai_direction(Point head, Point food) {
    if (food.x > head.x) return RIGHT;
    if (food.x < head.x) return LEFT;
    if (food.y > head.y) return DOWN;
    if (food.y < head.y) return UP;
    return RIGHT;
}

bool collision(Point p, std::vector<Point>& snake) {
    if (p.x < 0 || p.x >= WIDTH || p.y < 0 || p.y >= HEIGHT)
        return true;

    for (auto& s : snake)
        if (s.x == p.x && s.y == p.y)
            return true;

    return false;
}

Point spawn_food() {
    return {
        (rand() % (WIDTH / CELL)) * CELL,
        (rand() % (HEIGHT / CELL)) * CELL
    };
}

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    srand(time(0));

    SDL_Window* window = SDL_CreateWindow(
        "AI Snake SDL2",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WIDTH,
        HEIGHT,
        0
    );

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

    std::vector<Point> snake = {{100,100}, {80,100}, {60,100}};
    Point food = spawn_food();

    bool running = true;
    SDL_Event e;

    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)
                running = false;
        }

        Point head = snake[0];
        Dir d = ai_direction(head, food);

        Point new_head = head;

        switch (d) {
            case UP: new_head.y -= CELL; break;
            case DOWN: new_head.y += CELL; break;
            case LEFT: new_head.x -= CELL; break;
            case RIGHT: new_head.x += CELL; break;
        }

        if (collision(new_head, snake)) {
            running = false;
        }

        snake.insert(snake.begin(), new_head);

        if (new_head.x == food.x && new_head.y == food.y) {
            food = spawn_food();
        } else {
            snake.pop_back();
        }

        // render
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // food
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_Rect f = {food.x, food.y, CELL, CELL};
        SDL_RenderFillRect(renderer, &f);

        // snake
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        for (auto& s : snake) {
            SDL_Rect r = {s.x, s.y, CELL, CELL};
            SDL_RenderFillRect(renderer, &r);
        }

        SDL_RenderPresent(renderer);

        SDL_Delay(120);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}