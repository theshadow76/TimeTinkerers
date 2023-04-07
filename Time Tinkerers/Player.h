#pragma once

#include <SDL.h>
#include <vector>

class Player {
public:
    Player();
    ~Player();

    void init(SDL_Renderer* renderer);
    void handleEvents(SDL_Event& event);
    void update();
    void render(SDL_Renderer* renderer);
    void cleanup();
    void initWeapon(SDL_Renderer* renderer);
    void renderWeapon(SDL_Renderer* renderer);
    void shoot(int mouseX, int mouseY);
    void updateWeaponAngle(int mouseX, int mouseY);

    double weaponAngle;

    SDL_Texture* weaponTexture;
    struct Laser {
        int x, y;
        double angle;
    };
    std::vector<Laser> lasers;

private:
    int x, y;
    SDL_Texture* texture;
};
