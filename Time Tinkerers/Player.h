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

    void decreaseHealth(int amount);

    int getHealth() const;

    SDL_Texture* weaponTexture;
    struct Laser {
        int x, y;
        double angle;
    };
    std::vector<Laser> lasers;

    int getX() const;
    int getY() const;

    void setHealth(int health);
    SDL_Rect getRect() const;

private:
    int x, y;
    int health;
    int width;
    int height;
    SDL_Texture* texture;

};
