#pragma once

#include "Player.h"
#include "Enemy.h" // Asegate de incluir este archivo
#include "C:/src/SDL2-2.26.4/i686-w64-mingw32/include/SDL2/SDL.h"
#include <vector>
#include "C:\\src\\SDL2_image\\i686-w64-mingw32\\include\\SDL2\\SDL_image.h"
#include <cstdlib>
#include <ctime>
#include <iostream>

using namespace std;

class Game {
public:
    Game();
    ~Game();

    bool init();
    void run();
    void cleanup();
    void spawnEnemies(SDL_Renderer* renderer);

    void drawMap();
    void drawTile(SDL_Rect rect, SDL_Color color);

    void spawnCircles(SDL_Renderer* renderer);

    static bool rectIntersect(const SDL_Rect& a, const SDL_Rect& b);

private:
    void handleEvents();
    void update();
    void render();

    void checkCollisions();
    void drawRect(SDL_Rect rect, SDL_Color color);

    SDL_Window* window;
    SDL_Renderer* renderer;
    bool running;

    Player player;
    vector<Enemy> enemies;
    vector<Powerup> powerups;
    vector<Circle> circles;
};

class Powerup {
public:
    Powerup(int x, int y);
    ~Powerup();

    void init(SDL_Renderer* renderer);
    void update();
    void render(SDL_Renderer* renderer);
    void cleanup();

    int getX() const;
    int getY() const;

    bool isColliding(const SDL_Rect& rect) const;
    static bool rectIntersect(const SDL_Rect& a, const SDL_Rect& b);

private:
    int x, y;
    SDL_Texture* texture;
    int rotation;
};

class Circle {
public:
    Circle(int x, int y);
    void init(SDL_Renderer* renderer);
    void render(SDL_Renderer* renderer);
    bool isColliding(SDL_Rect playerRect);

private:
    int x, y;
    int size;
    SDL_Texture* texture;
};

class GreenCircle {
public:
    GreenCircle(int x, int y, int radius);
    ~GreenCircle();

    void init(SDL_Renderer* renderer);
    void render(SDL_Renderer* renderer);
    void cleanup();

    bool isColliding(const SDL_Rect& rect);
private:
    int x, y;
    int radius;
    SDL_Texture* texture;
};