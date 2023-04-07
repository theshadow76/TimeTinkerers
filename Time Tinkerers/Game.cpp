#include "Game.h"
#include "Enemy.h"
#include <SDL.h>
#include <vector>
#include <SDL_image.h>
#include <cstdlib>
#include <ctime>
#include "Player.h"

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
        checkCollisions(); // Agrega esta línea
        render();
        SDL_Delay(16);
    }
}

void Game::drawRect(SDL_Rect rect, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, &rect);
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
        enemy.update(player.getX(), player.getY());
    }

    for (auto enemyIt = enemies.begin(); enemyIt != enemies.end();) {
        bool enemyHit = false;
        enemyIt->update(player.getX(), player.getY());

        for (auto laserIt = player.lasers.begin(); laserIt != player.lasers.end();) {
            SDL_Rect laserRect = { laserIt->x, laserIt->y, 10, 2 };
            SDL_Rect enemyRect = { enemyIt->getX(), enemyIt->getY(), 50, 50 };

            if (rectIntersect(laserRect, enemyRect)) {
                enemyHit = true;
                laserIt = player.lasers.erase(laserIt);
            }
            else {
                ++laserIt;
            }
        }

        if (enemyHit) {
            enemyIt = enemies.erase(enemyIt);
        }
        else {
            ++enemyIt;
        }
    }
    checkCollisions();
}

void Game::checkCollisions() {
    for (Enemy& enemy : enemies) {
        int dx = player.getX() - enemy.getX();
        int dy = player.getY() - enemy.getY();
        int distanceSquared = dx * dx + dy * dy;
        int minDistanceSquared = 50 * 50; // Radio del jugador + radio del enemigo

        if (distanceSquared < minDistanceSquared) {
            player.decreaseHealth(3);
        }
    }
}


void Game::render() {
    // Renderiza el mapa
    SDL_SetRenderDrawColor(renderer, 34, 34, 34, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 51, 153, 255, 255);
    SDL_Rect wall = { 0, 0, 50, 600 };
    SDL_RenderFillRect(renderer, &wall);
    SDL_Rect floor = { 50, 0, 750, 600 };
    SDL_RenderFillRect(renderer, &floor);

    // Renderiza los enemigos
    for (Enemy& enemy : enemies) {
        enemy.render(renderer);
    }

    // Renderiza el jugador
    player.render(renderer);

    // Renderiza los láseres

    // Renderiza la barra de salud del jugador
    SDL_Rect healthBarOutline = { 20, 20, 104, 24 };
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &healthBarOutline);
    SDL_Rect healthBarFill = { 22, 22, static_cast<int>(player.getHealth()), 20 };
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &healthBarFill);

    // Renderiza todo
    SDL_RenderPresent(renderer);
}

void Game::drawMap() {
    // Colores
    SDL_Color darkBlue = { 20, 50, 100, 255 }; // Un azul oscuro más apagado
    SDL_Color lightBlue = { 40, 80, 130, 255 }; // Un azul medio más apagado


    // Dibuja un cuadrado azul oscuro que cubre toda la pantalla
    drawTile({ 0, 0, 800, 600 }, darkBlue);

    // Dibuja una "muralla" azul claro al final de la pantalla
    int wallWidth = 20;
    drawTile({ 800 - wallWidth, 0, wallWidth, 600 }, lightBlue);
}

void Game::drawTile(SDL_Rect rect, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, &rect);
}


bool Game::rectIntersect(const SDL_Rect& a, const SDL_Rect& b) {
    return a.x < b.x + b.w &&
        a.x + a.w > b.x &&
        a.y < b.y + b.h &&
        a.y + a.h > b.y;
}
