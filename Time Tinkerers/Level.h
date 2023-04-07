#pragma once

#include <SDL.h>

class Level {
public:
    Level();
    ~Level();

    void load();
    void update();
    void render(SDL_Renderer* renderer);

private:
    // Agrega las estructuras de datos necesarias para almacenar y gestionar los objetos del nivel
};
