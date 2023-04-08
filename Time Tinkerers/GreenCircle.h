#pragma once

#include <SDL.h>

class GreenCircle {
public:
    GreenCircle(int x, int y, int r);
    ~GreenCircle();

    void init(SDL_Renderer* renderer);
    void render(SDL_Renderer* renderer);
    void checkCollision(SDL_Rect playerRect);

    bool isActive() const;
    void activate();
    void deactivate();

private:
    int x, y;
    int r;
    SDL_Texture* texture;
    bool active;
};
