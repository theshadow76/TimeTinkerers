#include "GreenCircle.h"
#include <SDL_image.h>
#include <iostream>

GreenCircle::GreenCircle(int x, int y, int r) : x(x), y(y), r(r), texture(nullptr), active(false) {}

GreenCircle::~GreenCircle() {}

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
    if (active && texture) {
        SDL_Rect rect = { x - r, y - r, r * 2, r * 2 };
        SDL_RenderCopy(renderer, texture, nullptr, &rect);
    }
}

void GreenCircle::checkCollision(SDL_Rect playerRect) {
    if (active) {
        int dx = x - playerRect.x;
        int dy = y - playerRect.y;
        int distanceSquared = dx * dx + dy * dy;
        int minDistanceSquared = (r + 20) * (r + 20); // Radio del círculo + radio del jugador

        if (distanceSquared < minDistanceSquared) {
            active = false;
        }
    }
}

bool GreenCircle::isActive() const {
    return active;
}

void GreenCircle::activate() {
    active = true;
}

void GreenCircle::deactivate() {
    active = false;
}
