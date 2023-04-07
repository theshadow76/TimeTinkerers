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

private:
    void handleEvents();
    void update();
    void render();

    SDL_Window* window;
    SDL_Renderer* renderer;
    bool running;

    Player player;

    std::vector<Enemy> enemies;
};