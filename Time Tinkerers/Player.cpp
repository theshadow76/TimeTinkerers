#include "Player.h"
#include <SDL_image.h>
#include <iostream>
#include <cmath>
#include "Enemy.h"
#include "Game.h"

Player::Player() : x(100), y(100), texture(nullptr) {}

Player::~Player() {}

void Player::init(SDL_Renderer* renderer) {
    SDL_Surface* surface = IMG_Load("player.png");
    if (surface) {
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
    }
    else {
        printf("Error al cargar la imagen: %s\n", IMG_GetError());
    }
    initWeapon(renderer);
}

void Player::initWeapon(SDL_Renderer* renderer) {
    SDL_Surface* surface = IMG_Load("weapon.png");
    if (surface) {
        weaponTexture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
    }
    else {
        printf("Error al cargar la imagen del arma: %s\n", IMG_GetError());
    }
}

void Player::handleEvents(SDL_Event& event) {
    if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
        case SDLK_LEFT:
            x -= 5;
            break;
        case SDLK_RIGHT:
            x += 5;
            break;
        case SDLK_UP:
            y -= 5;
            break;
        case SDLK_DOWN:
            y += 5;
            break;
        default:
            break;
        }
    }
    if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
        int mouseX = event.button.x;
        int mouseY = event.button.y;
        shoot(mouseX, mouseY);
    }
    if (event.type == SDL_MOUSEMOTION) {
        int mouseX = event.motion.x;
        int mouseY = event.motion.y;
        updateWeaponAngle(mouseX, mouseY);
    }
}

void Player::updateWeaponAngle(int mouseX, int mouseY) {
    int weaponCenterX = x + 25;
    int weaponCenterY = y + 25;

    double dx = mouseX - weaponCenterX;
    double dy = mouseY - weaponCenterY;
    weaponAngle = atan2(dy, dx);
}

void Player::shoot(int mouseX, int mouseY) {
    Laser laser;
    laser.x = x + 25; // Centra el láser en el personaje
    laser.y = y + 25;

    double dx = mouseX - laser.x;
    double dy = mouseY - laser.y;
    laser.angle = atan2(dy, dx);

    lasers.push_back(laser);
}

void Player::update() {
    // Puedes agregar aquí la lógica de actualización del personaje, como la detección de colisiones o las mecánicas de manipulación del tiempo.
    for (Laser& laser : lasers) {
        int laserSpeed = 10;
        laser.x += laserSpeed * cos(laser.angle);
        laser.y += laserSpeed * sin(laser.angle);
    }
}

void Player::render(SDL_Renderer* renderer) {
    if (texture) {
        SDL_Rect rect = { x, y, 50, 50 };
        SDL_RenderCopy(renderer, texture, nullptr, &rect);
    }
    // Renderiza los láseres
    for (Laser& laser : lasers) {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

        SDL_Texture* laserTexture = nullptr;
        SDL_Surface* surface = SDL_CreateRGBSurface(0, 10, 2, 32, 0, 0, 0, 0);
        if (surface) {
            SDL_FillRect(surface, nullptr, SDL_MapRGB(surface->format, 255, 0, 0));
            laserTexture = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_FreeSurface(surface);
        }

        if (laserTexture) {
            SDL_Rect srcRect = { 0, 0, 10, 2 };
            SDL_Rect dstRect = { laser.x, laser.y, 10, 2 };
            double angleInDegrees = laser.angle * 180 / M_PI;
            SDL_Point pivot = { 5, 1 }; // Centro del láser (la mitad de su ancho y alto)

            SDL_RenderCopyEx(renderer, laserTexture, &srcRect, &dstRect, angleInDegrees, &pivot, SDL_FLIP_NONE);
            SDL_DestroyTexture(laserTexture);
        }
    }
    renderWeapon(renderer);
}


void Player::renderWeapon(SDL_Renderer* renderer) {
    if (weaponTexture) {
        SDL_Rect srcRect = { 0, 0, 30, 30 };
        SDL_Rect dstRect = { x + 20, y + 20, 30, 30 };
        double angleInDegrees = weaponAngle * 180 / M_PI;
        SDL_Point pivot = { 15, 15 }; // Centro del arma (la mitad de su ancho y alto)

        SDL_RenderCopyEx(renderer, weaponTexture, &srcRect, &dstRect, angleInDegrees, &pivot, SDL_FLIP_NONE);
    }
}

void Player::cleanup() {
    if (texture) {
        SDL_DestroyTexture(texture);
    }
    if (weaponTexture) {
        SDL_DestroyTexture(weaponTexture);
    }
}
