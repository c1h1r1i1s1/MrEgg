#include "Game.h"

int main()
{
    Game game;
    game.loadMenu();

    while (game.running())
    {
        game.update();
        game.render();
    }

    return 0;
}