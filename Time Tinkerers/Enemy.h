#pragma once

#include <SDL.h>

class Enemy {
public:
    Enemy(int x, int y);
    ~Enemy();

    void init(SDL_Renderer* renderer);
    void update();
    void render(SDL_Renderer* renderer);
    void cleanup();

private:
    int x, y;
    SDL_Texture* texture;
};
