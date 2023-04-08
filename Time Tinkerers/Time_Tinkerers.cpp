// Time Tinkerers.cpp : This file contains the 'main' function. Program execution begins and ends there.
// Desarollado por Vigo Walker

#include "Game.h"

int main(int argc, char* argv[]) {
    Game game;

    if (!game.init()) {
        return 1;
    }

    game.run();

    game.cleanup();

    return 0;
}
