#pragma once

#include "Player.h"
#include "Enemy.h" // Asegúrate de incluir este archivo
#include <SDL.h>
#include <vector>
#include <SDL_image.h>
#include <cstdlib>
#include <ctime>

class Game {
public:
    Game();
    ~Game();

    bool init();
    void run();
    void cleanup();
    void spawnEnemies(SDL_Renderer* renderer);

    void drawMap();
    void drawTile(SDL_Rect rect, SDL_Color color);

private:
    void handleEvents();
    void update();
    void render();

    void checkCollisions();
    void drawRect(SDL_Rect rect, SDL_Color color);

    bool rectIntersect(const SDL_Rect& a, const SDL_Rect& b);

    SDL_Window* window;
    SDL_Renderer* renderer;
    bool running;

    Player player;

    std::vector<Enemy> enemies;
};