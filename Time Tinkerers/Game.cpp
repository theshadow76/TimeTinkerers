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

Powerup::Powerup(int x, int y) : x(x), y(y), texture(nullptr) {}

Powerup::~Powerup() {}

GreenCircle::GreenCircle(int x, int y, int radius) : x(x), y(y), radius(radius), texture(nullptr) {}

GreenCircle::~GreenCircle() {}


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

void GreenCircle::init(SDL_Renderer* renderer) {
    SDL_Surface* surface = IMG_Load("green_circle.png");
    if (surface) {
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
    }
    else {
        printf("Error al cargar la imagen del círculo verde: %s\n", IMG_GetError());
    }
}

void GreenCircle::render(SDL_Renderer* renderer) {
    if (texture) {
        SDL_Rect rect = { x - radius, y - radius, radius * 2, radius * 2 };
        SDL_RenderCopy(renderer, texture, nullptr, &rect);
    }
}

void GreenCircle::cleanup() {
    if (texture) {
        SDL_DestroyTexture(texture);
    }
}

bool GreenCircle::isColliding(const SDL_Rect& rect) {
    int dx = x - rect.x;
    int dy = y - rect.y;
    int distanceSquared = dx * dx + dy * dy;
    int minDistanceSquared = radius * radius;

    return distanceSquared < minDistanceSquared;
}

void Game::spawnEnemies(SDL_Renderer* renderer) {
    for (int i = 0; i < 10; ++i) {
        int x, y;

        // Genera coordenadas aleatorias en el borde del mapa
        if (rand() % 2 == 0) { // Aparece en el borde izquierdo o derecho
            x = (rand() % 2 == 0) ? -50 : 800;
            y = rand() % 600;
        }
        else { // Aparece en el borde superior o inferior
            x = rand() % 800;
            y = (rand() % 2 == 0) ? -50 : 600;
        }

        Enemy enemy(x, y);
        enemy.init(renderer);
        enemies.push_back(enemy);
    }
}

Circle::Circle(int x, int y) : x(x), y(y), size(30), texture(nullptr) {}

void Circle::init(SDL_Renderer* renderer) {
    SDL_Surface* surface = IMG_Load("green_circle.png");
    if (surface) {
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
    }
    else {
        printf("Error al cargar la imagen del círculo: %s\n", IMG_GetError());
    }
}

void Circle::render(SDL_Renderer* renderer) {
    if (texture) {
        SDL_Rect rect = { x - size / 2, y - size / 2, size, size };
        SDL_RenderCopy(renderer, texture, nullptr, &rect);
    }
}

bool Circle::isColliding(SDL_Rect playerRect) {
    SDL_Rect circleRect = { x - size / 2, y - size / 2, size, size };
    return Game::rectIntersect(circleRect, playerRect);
}

void Game::spawnCircles(SDL_Renderer* renderer) {
    int numCircles = rand() % 3 + 3; // Crea entre 3 y 5 círculos

    for (int i = 0; i < numCircles; ++i) {
        // Crea una posición aleatoria para el círculo
        int x = rand() % 800;
        int y = rand() % 600;

        // Crea el círculo y lo agrega al vector de círculos
        Circle circle(x, y);
        circle.init(renderer);
        circles.push_back(circle);
    }
}


void Powerup::init(SDL_Renderer* renderer) {
    SDL_Surface* surface = IMG_Load("powerup.png");
    if (surface) {
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
    }
    else {
        printf("Error al cargar la imagen del power-up: %s\n", IMG_GetError());
    }
}

bool Powerup::isColliding(const SDL_Rect& rect) const {
    SDL_Rect powerupRect = { x, y, 20, 20 };
    return Game::rectIntersect(powerupRect, rect);
}


void Powerup::update() {
    // Aquí puedes agregar la lógica de actualización del power-up
}

void Powerup::render(SDL_Renderer* renderer) {
    if (texture) {
        SDL_Rect rect = { x, y, 20, 20 };
        SDL_RenderCopy(renderer, texture, nullptr, &rect);
    }
}

void Powerup::cleanup() {
    if (texture) {
        SDL_DestroyTexture(texture);
    }
}

int Powerup::getX() const {
    return x;
}

int Powerup::getY() const {
    return y;
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

    for (Powerup& powerup : powerups) {
        powerup.update();

        // Si el jugador colisiona con un power-up, lo recoge y cura su salud
        if (powerup.isColliding(player.getRect())) {
            player.setHealth(100);
        }
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
            player.decreaseHealth(1);
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

    for (Powerup& powerup : powerups) {
        powerup.render(renderer);
    }

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

