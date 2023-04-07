#include "Enemy.h"
#include <SDL_image.h>
#include <iostream>
#include "Game.h"

Enemy::Enemy(int x, int y) : x(x), y(y), texture(nullptr) {}

Enemy::~Enemy() {}

void Enemy::init(SDL_Renderer* renderer) {
    SDL_Surface* surface = IMG_Load("enemy.png");
    if (surface) {
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
    }
    else {
        printf("Error al cargar la imagen del enemigo: %s\n", IMG_GetError());
    }
}

void Enemy::die(std::vector<Enemy>& enemies) {
    addParticles();
    // Elimina el enemigo del vector de enemigos
    std::vector<Enemy>::iterator enemyIt = std::find(enemies.begin(), enemies.end(), *this);
    if (enemyIt != enemies.end()) {
        enemies.erase(enemyIt);
    }
}

void Enemy::addParticles() {
    int width, height;
    SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);

    for (int i = 0; i < 10; ++i) {
        int x = getX() + width / 2;
        int y = getY() + height / 2;
        int vx = rand() % 5 - 2; // Velocidad horizontal aleatoria
        int vy = rand() % 5 - 2; // Velocidad vertical aleatoria
        int life = rand() % 20 + 20; // Tiempo de vida aleatorio
        SDL_Color color = { 0, 255, 0, 255 }; // Partículas verdes
        Particle particle(x, y, vx, vy, life, color);
        particles.push_back(particle);
    }
}


void Enemy::update(int playerX, int playerY) {
    // Aquí puedes agregar la lógica de actualización del enemigo, como la IA y la detección de colisiones.
    int dx = playerX - x;
    int dy = playerY - y;
    double distance = sqrt(dx * dx + dy * dy);

    if (distance > 0) {
        double speed = 2.0;
        x += static_cast<int>(dx * speed / distance);
        y += static_cast<int>(dy * speed / distance);
    }
    // Actualiza las partículas
    for (auto it = particles.begin(); it != particles.end();) {
        it->update();
        if (it->isDead()) {
            it = particles.erase(it);
        }
        else {
            ++it;
        }
    }
}

void Enemy::render(SDL_Renderer* renderer) {
    if (texture) {
        SDL_Rect rect = { x, y, 50, 50 };
        SDL_RenderCopy(renderer, texture, nullptr, &rect);
    }
    // Renderiza las partículas
    for (Particle& particle : particles) {
        particle.render(renderer);
    }
}

void Enemy::cleanup() {
    if (texture) {
        SDL_DestroyTexture(texture);
    }
}

int Enemy::getX() const {
    return x;
}

int Enemy::getY() const {
    return y;
}
