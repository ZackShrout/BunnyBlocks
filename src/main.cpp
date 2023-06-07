#include "game.h"

int main()
{
    io              io_;
    pieces          pieces_;
    int             screen_height_{ io_.get_screen_height() };
    board           board_{ &pieces_, screen_height_ };
    game            game_{ &board_, &pieces_, &io_, screen_height_ };
    unsigned long   time1{ SDL_GetTicks() };

    while (!io_.is_key_down(SDLK_ESCAPE))
    {
        io_.clear_screen(); // Clear screen
        game_.draw_scene(); // Draw staff
        io_.update_screen(); // Put the graphic context in the screen

        int key{ io_.poll_key() };

        switch (key)
        {
            case (SDLK_RIGHT):
            {
                if (board_.is_possible_movement(game_.pos_x() + 1, game_.pos_y(), game_.piece(), game_.rotation()))
                    game_.set_pos_x(game_.pos_x() + 1);
                break;
            }

            case (SDLK_LEFT):
            {
                if (board_.is_possible_movement(game_.pos_x() - 1, game_.pos_y(), game_.piece(), game_.rotation()))
                    game_.set_pos_x(game_.pos_x() - 1);
                break;
            }

            case (SDLK_z):
            {
                if (board_.is_possible_movement(game_.pos_x(), game_.pos_y() + 1, game_.piece(), game_.rotation()))
                    game_.set_pos_y(game_.pos_y() + 1);
                break;
            }
        
            case (SDLK_SPACE):
            {
                // Check collision from up to down
                while (board_.is_possible_movement(game_.pos_x(), game_.pos_y(), game_.piece(), game_.rotation()))
                    game_.set_pos_y(game_.pos_y() + 1);

                board_.store_piece(game_.pos_x(), game_.pos_y() - 1, game_.piece(), game_.rotation());
                board_.delete_possible_lines();

                if (board_.is_game_over())
                {
                    io_.get_key();
                    exit(0);
                }

                game_.create_new_piece();

                break;
            }

            case (SDLK_DOWN):
            {
                if (board_.is_possible_movement(game_.pos_x(), game_.pos_y(), game_.piece(), (game_.rotation() + 1) % 4))
                    game_.set_rotation((game_.rotation() + 1) % 4);

                break;
            }
        }

        unsigned long time2{ SDL_GetTicks() };

        if ((time2 - time1) > wait_time)
        {
            if (board_.is_possible_movement(game_.pos_x(), game_.pos_y() + 1, game_.piece(), game_.rotation()))
                game_.set_pos_y(game_.pos_y() + 1);
            else
            {
                board_.store_piece(game_.pos_x(), game_.pos_y(), game_.piece(), game_.rotation());
                board_.delete_possible_lines();

                if (board_.is_game_over())
                {
                    io_.get_key();
                    exit(0);
                }

                game_.create_new_piece();
            }

            time1 = SDL_GetTicks();
        }
    }

    return 0;
}