#include "game.h"

int main()
{
    bblocks::game game;

    while (game.is_running())
    {
        game.process_input();
        game.update();
        game.render();
    }

    // TODO: would like a cleanup function here

    return 0;
}