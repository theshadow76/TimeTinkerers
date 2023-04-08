#pragma once
#include <iostream>
#include "C:/src/SDL2-2.26.4/i686-w64-mingw32/include/SDL2/SDL.h"
#include <vector>
#include "Game.h"

class Particle {
public:
    Particle(); // Constructor predeterminado
    Particle(int x, int y, int vx, int vy, int life, SDL_Color color);
    void update();
    void render(SDL_Renderer* renderer);

    bool isDead() const; // Declaracin de la funcin

private:
    int x, y;
    int vx, vy;
    int life;
    SDL_Color color;
};
// Implementación del constructor predeterminado
Particle::Particle() {
    // Inicializa las variables miembro con valores predeterminados, por ejemplo:
    x = 0;
    y = 0;
    vx = 0;
    vy = 0;
    life = 0;
    color = {0, 0, 0, 255}; // Color negro con opacidad total
}
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

bool Particle::isDead() const { // Definici�n de la funci�n
    return life <= 0;
}
