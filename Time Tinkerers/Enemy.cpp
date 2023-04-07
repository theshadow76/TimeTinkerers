#include "Enemy.h"
#include <SDL_image.h>
#include <iostream>

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

void Enemy::update() {
    // Aquí puedes agregar la lógica de actualización del enemigo, como la IA y la detección de colisiones.
}

void Enemy::render(SDL_Renderer* renderer) {
    if (texture) {
        SDL_Rect rect = { x, y, 50, 50 };
        SDL_RenderCopy(renderer, texture, nullptr, &rect);
    }
}

void Enemy::cleanup() {
    if (texture) {
        SDL_DestroyTexture(texture);
    }
}
