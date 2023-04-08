#pragma once
#include <iostream>
#include <SDL.h>
#include <vector>
#include "Game.h"

class Enemy {
public:
    Enemy(int x, int y);
    ~Enemy();

    void init(SDL_Renderer* renderer);
    void update(int playerX, int playerY);
    void render(SDL_Renderer* renderer);
    void cleanup();

    int getX() const;
    int getY() const;

    void die();
    void addParticles();

    void die(std::vector<Enemy>& enemies);

    void getWidth();

private:
    int x, y;
    SDL_Texture* texture;

    std::vector<Particle> particles;
};

class Particle {
public:
    Particle(int x, int y, int vx, int vy, int life, SDL_Color color);
    void update();
    void render(SDL_Renderer* renderer);

    bool isDead() const; // Declaración de la función

private:
    int x, y;
    int vx, vy;
    int life;
    SDL_Color color;
};

bool Particle::isDead() const { // Definición de la función
    return life <= 0;
}