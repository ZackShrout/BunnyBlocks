#include "game.h"

int main()
{
    io              io_;
    pieces          pieces_;
    board           board_{ &pieces_, io_.get_screen_height(), io_.get_screen_width() };
    game            game_{ &board_, &pieces_, &io_, io_.get_screen_height() };
    uint32_t        time1_{ SDL_GetTicks() };
    uint32_t        ticks_last_frame_;
    float           dt_{ 0 };


    bool running{ true };
    while (running)
    {
        dt_ = (SDL_GetTicks() - ticks_last_frame_) / 1000.f;
        dt_ = (dt_ > 0.05f) ? 0.05f : dt_;
        ticks_last_frame_ = SDL_GetTicks();

        board_.delete_possible_lines(dt_);

        io_.clear_screen();
        game_.update();
        io_.render();

        int key{ io_.poll_key() };

        switch (key)
        {
            case (SDLK_ESCAPE): running = false; break;
            
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

            // Immediately move piece as far down as it goes and freeze it
            case (SDLK_z):
            case (SDLK_SPACE):
            {
                // Check collision from up to down
                while (board_.is_possible_movement(game_.pos_x(), game_.pos_y(), game_.piece(), game_.rotation()))
                    game_.set_pos_y(game_.pos_y() + 1);

                board_.store_piece(game_.pos_x(), game_.pos_y() - 1, game_.piece(), game_.rotation());
                board_.delete_possible_lines(dt_);

                if (board_.is_game_over())
                {
                    io_.get_key();
                    exit(0);
                }

                game_.new_piece();
                
                break;
            }

            // Rotate piece
            case (SDLK_UP):
            {
                if (board_.is_possible_movement(game_.pos_x(), game_.pos_y(), game_.piece(), (game_.rotation() + 1) % 4))
                    game_.set_rotation((game_.rotation() + 1) % 4);
                
                break;
            }

            // Speed up fall
            case (SDLK_DOWN):
            {
                if (board_.is_possible_movement(game_.pos_x(), game_.pos_y() + 1, game_.piece(), game_.rotation()))
                    game_.set_pos_y(game_.pos_y() + 1);
            
                break;
            }
        }

        uint32_t time2{ SDL_GetTicks() };

        if ((time2 - time1_) > static_cast<uint32_t>(game_.wait_time()))
        {
            if (board_.is_possible_movement(game_.pos_x(), game_.pos_y() + 1, game_.piece(), game_.rotation()))
                game_.set_pos_y(game_.pos_y() + 1);
            else
            {
                board_.store_piece(game_.pos_x(), game_.pos_y(), game_.piece(), game_.rotation());
                // board_.delete_possible_lines(dt_);

                if (board_.is_game_over())
                {
                    io_.get_key();
                    exit(0);
                }

                game_.new_piece();
            }

            time1_ = SDL_GetTicks();
        }
    }

    return 0;
}