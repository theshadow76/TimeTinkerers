#include "Game.h"
#include "Enemy.h"
#include <SDL.h>
#include <vector>
#include <SDL_image.h>
#include <cstdlib>
#include <ctime>

std::vector<Enemy> enemies;

Game::Game() : window(nullptr), renderer(nullptr), running(false) {}

Game::~Game() {}

bool Game::init() {
    srand(static_cast<unsigned>(time(0))); // Inicializa la función rand()

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        return false;
    }

    window = SDL_CreateWindow("Time Tinkerers", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);
    if (!window) {
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        return false;
    }

    running = true;
    player.init(renderer);

    return true;
}

void Game::run() {
    while (running) {
        handleEvents();
        update();
        render();
        SDL_Delay(16);
    }
}

void Game::spawnEnemies(SDL_Renderer* renderer) {
    for (int i = 0; i < 10; ++i) {
        int x = rand() % (800 - 50); // Asume que screenWidth es el ancho de la ventana del juego
        int y = rand() % (600 - 50); // Asume que screenHeight es el alto de la ventana del juego
        Enemy enemy(x, y);
        enemy.init(renderer);
        enemies.push_back(enemy);
    }
}

void Game::cleanup() {
    player.cleanup();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Game::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = false;
        }
        player.handleEvents(event);
        if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
            case SDLK_SPACE: // Cambia SDLK_SPACE al botón que prefieras para agregar enemigos
                spawnEnemies(renderer);
                break;
            }
        }
    }
}

void Game::update() {
    player.update();
    for (Enemy& enemy : enemies) {
        enemy.update();
    }
}

void Game::render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    player.render(renderer);

    SDL_RenderPresent(renderer);

    for (Enemy& enemy : enemies) {
        enemy.render(renderer);
    }

    SDL_RenderPresent(renderer);
}
