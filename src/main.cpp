#include "game.h"

int main()
{
    io              io;
    pieces          pieces;
    board           board{ &pieces, io.get_screen_height(), io.get_screen_width() };
    game            game{ &board, &pieces, &io, io.get_screen_height() };
    uint32_t        time1{ SDL_GetTicks() };
    uint32_t        ticks_last_frame;
    float           dt{ 0 };

    bool running{ true };
    while (running)
    {
        dt = (SDL_GetTicks() - ticks_last_frame) / 1000.f;
        dt = (dt > 0.05f) ? 0.05f : dt;
        ticks_last_frame = SDL_GetTicks();

        board.delete_possible_lines(dt);

        io.clear_screen();
        game.update();
        io.render();

        int key{ io.poll_key() };

        switch (key)
        {
            case (SDLK_ESCAPE): running = false; break;
            
            case (SDLK_RIGHT):
            {
                if (board.is_possible_movement(game.pos_x() + 1, game.pos_y(), game.piece(), game.rotation()))
                    game.set_pos_x(game.pos_x() + 1);
                break;
            }

            case (SDLK_LEFT):
            {
                if (board.is_possible_movement(game.pos_x() - 1, game.pos_y(), game.piece(), game.rotation()))
                    game.set_pos_x(game.pos_x() - 1);
                break;
            }

            // Immediately move piece as far down as it goes and freeze it
            case (SDLK_z):
            case (SDLK_SPACE):
            {
                // Check collision from up to down
                while (board.is_possible_movement(game.pos_x(), game.pos_y(), game.piece(), game.rotation()))
                    game.set_pos_y(game.pos_y() + 1);

                board.store_piece(game.pos_x(), game.pos_y() - 1, game.piece(), game.rotation());
                board.delete_possible_lines(dt);

                if (board.is_game_over())
                {
                    io.get_key();
                    exit(0);
                }

                game.new_piece();
                
                break;
            }

            // Rotate piece
            case (SDLK_UP):
            {
                if (board.is_possible_movement(game.pos_x(), game.pos_y(), game.piece(), (game.rotation() + 1) % 4))
                    game.set_rotation((game.rotation() + 1) % 4);
                
                break;
            }

            // Speed up fall
            case (SDLK_DOWN):
            {
                if (board.is_possible_movement(game.pos_x(), game.pos_y() + 1, game.piece(), game.rotation()))
                    game.set_pos_y(game.pos_y() + 1);
            
                break;
            }

            case (SDLK_p):
            {
                game.toggle_pause();
            }
        }

        uint32_t time2{ SDL_GetTicks() };

        if ((time2 - time1) > static_cast<uint32_t>(game.wait_time()) && !game.is_paused())
        {
            if (board.is_possible_movement(game.pos_x(), game.pos_y() + 1, game.piece(), game.rotation()))
                game.set_pos_y(game.pos_y() + 1);
            else
            {
                board.store_piece(game.pos_x(), game.pos_y(), game.piece(), game.rotation());

                if (board.is_game_over())
                {
                    io.get_key();
                    exit(0);
                }

                game.new_piece();
            }

            time1 = SDL_GetTicks();
        }
    }

    return 0;
}