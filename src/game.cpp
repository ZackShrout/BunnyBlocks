#include "game.h"

#include <algorithm>
#include <chrono>
#include <random>
#include <vector>
#include "pieces.h"

namespace bblocks
{
    namespace
    {
        struct rect_info
        {
            io*     gfx;
            int     left;
            int     top;
            int     right;
            int     bottom;
        };

        constexpr int       board_offset{ static_cast<int>(block_size) };
        std::vector<int>    pieces_{ 0, 1, 2, 3, 4, 5, 6 };
        std::vector<int>    next_pieces_{ 0, 1, 2, 3, 4, 5, 6 };
        int                 piece_index_{ 0 };
        uint32_t            menu_selection_{ 0 };

        void
        draw_piece_block(const rect_info& info, int piece)
        {
            color c; // Color of the block
            color h; // Highlight color of the block
            color s; // Shadow color of the block

            switch (piece)
            {
                case 0: c = red; h = light_red; s = dark_red; break;
                case 1: c = green; h = light_green; s = dark_green; break;
                case 2: c = purple; h = light_purple; s = dark_purple; break;
                case 3: c = orange; h = light_orange; s = dark_orange; break;
                case 4: c = yellow; h = light_yellow; s = dark_yellow; break;
                case 5: c = pink; h = light_pink; s = dark_pink; break;
                case 6: c = blue; h = light_blue; s = dark_blue; break;
            }

            // Draw highlight color
            info.gfx->draw_rectangle(info.left, info.top, info.right, info.bottom, h);
            // Draw shadow color
            info.gfx->draw_rectangle(info.left + 2, info.top + 2, info.right, info.bottom, s);
            // Draw regular color
            info.gfx->draw_rectangle(info.left + 2, info.top + 2, info.right - 2, info.bottom - 2, c);
        }

        void
        draw_board_block(const rect_info& info)
        {
            // Draw highlight color
            info.gfx->draw_rectangle(info.left, info.top, info.right, info.bottom, light_brick);
            // Draw shadow color
            info.gfx->draw_rectangle(info.left + 2, info.top + 2, info.right, info.bottom, dark_brick);
            // Draw regular color
            info.gfx->draw_rectangle(info.left + 2, info.top + 2, info.right - 2, info.bottom - 2, brick);
        }

        void
        darken_screen(const rect_info& screen)
        {
            screen.gfx->draw_rectangle(screen.left, screen.top, screen.right, screen.bottom, fade);
        }

        void
        draw_pause_overlay(const rect_info& screen)
        {
            darken_screen(screen);
            screen.gfx->draw_pause();
        }

        void
        draw_game_over_overlay(const rect_info& screen)
        {
            darken_screen(screen);
            screen.gfx->draw_game_over(menu_selection_);
        }

        void
        shuffle_pieces_bag(std::vector<int>& bag)
        {
    	    const unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
            std::shuffle(bag.begin(), bag.end(), std::default_random_engine(seed));
        }
    } // anonymous

    void
    game::process_input()
    {
    	switch (int key{ _info.game_io.poll_key() })
    	{
    	case (SDLK_ESCAPE):
            _running = false;
    		break;

    	case (SDLK_RIGHT):
    		if (_info.game_board.is_possible_movement(_pos_x + 1, _pos_y, _piece, _rotation))
    			++_pos_x;
    		break;

    	case (SDLK_LEFT):
    		if (_info.game_board.is_possible_movement(_pos_x - 1, _pos_y, _piece, _rotation))
    			--_pos_x;
    		break;

    	// Immediately move piece as far down as it goes and freeze it
    	case (SDLK_z):
    	case (SDLK_SPACE):
    		// Check collision from up to down
    		while (_info.game_board.is_possible_movement(_pos_x, _pos_y, _piece, _rotation))
    			++_pos_y;

    		_info.game_board.store_piece(_pos_x, _pos_y - 1, _piece, _rotation);
    		_info.game_board.delete_possible_lines(_info.dt);

    		if (_info.game_board.is_game_over())
    		{
    			draw_game_over();
    		}

    		new_piece();

    		break;

    	// Rotate piece
    	case (SDLK_UP):
    		if (_info.game_board.is_possible_movement(_pos_x, _pos_y, _piece, (_rotation + 1) % 4))
    			_rotation = (_rotation + 1) % 4;
    		break;

    	// Speed up fall
    	case (SDLK_DOWN):
    		if (_info.game_board.is_possible_movement(_pos_x, _pos_y + 1, _piece, _rotation))
    			++_pos_y;
    		break;

    	case (SDLK_p):
        case (SDLK_RETURN):
    		_is_paused = !_is_paused;
    		break;

    	default:
    		break;
    	}
    }

    void
    game::update()
    {
        _info.dt = static_cast<float>(SDL_GetTicks() - _info.ticks_last_frame) / 1000.f;
        _info.dt = (_info.dt > 0.05f) ? 0.05f : _info.dt;
        _info.ticks_last_frame = SDL_GetTicks();

        _info.game_board.delete_possible_lines(_info.dt);

        _info.game_io.clear_screen();

        if (_score != _info.game_board.lines_deleted())
        {
            _score = _info.game_board.lines_deleted();

            if (_score != 0 && (_score - _score % 15) / 15 > _level - 1)
            {
                ++_level;
                _wait_time -= static_cast<int>(static_cast<float>(_wait_time) / 4.f);
            }
        }

        if ((SDL_GetTicks() - _info.time1) > static_cast<uint32_t>(_wait_time) && !_is_paused)
        {
            if (_info.game_board.is_possible_movement(_pos_x, _pos_y + 1, _piece, _rotation))
                ++_pos_y;
            else
            {
                _info.game_board.store_piece(_pos_x, _pos_y, _piece, _rotation);

                if (_info.game_board.is_game_over())
                {
                    draw_game_over();
                }

                new_piece();
            }

            _info.time1 = SDL_GetTicks();
        }

        if (!_info.game_board.is_game_over())
            draw_scene();
        else
            draw_game_over();
    }

    void
    game::render()
    {
        _info.game_io.render();
    }

    // PRIVATE

    void
    game::new_piece()
    {
        ++piece_index_;

        if (piece_index_ == 7)
        {
            pieces_ = next_pieces_;
            piece_index_ = 0;     
        }

        _piece = _next_piece;
        _rotation = _next_rotation;
        _pos_x = (board_width / 2) + bblocks::piece::get_x_initial_position(_piece, _rotation);
        _pos_y = bblocks::piece::get_y_initial_position(_piece, _rotation);

        if (piece_index_ == 6)
        {
            shuffle_pieces_bag(next_pieces_);
            _next_piece = next_pieces_.at(0);
        }
        else
            _next_piece = pieces_.at(piece_index_ + 1);

        _next_rotation = get_rand(0, 3);
    }

    void
    game::init_game()
    {
        _screen_height = _info.game_io.get_screen_height();

        shuffle_pieces_bag(pieces_);
        _piece = pieces_.at(0);
        _rotation = get_rand(0, 3);
        _pos_x = (board_width / 2) + bblocks::piece::get_x_initial_position(_piece, _rotation);
        _pos_y = bblocks::piece::get_y_initial_position(_piece, _rotation);

        _next_piece = pieces_.at(1);
        _next_rotation = get_rand(0, 3);
        _next_pos_x = board_width + 3;
        _next_pos_y = 5;

        _running = true;
    }

    void
    game::draw_scene()
    {
        _info.game_io.clear_screen();

        draw_board();
        draw_piece(_pos_x, _pos_y, _piece, _rotation);
        draw_piece(_next_pos_x, _next_pos_y, _next_piece, _next_rotation);

        if (_is_paused)
        {
    	    const rect_info info{ &_info.game_io, 0, 0, _info.game_io.get_screen_width(), _info.game_io.get_screen_height() };

            draw_pause_overlay(info);
        }
    }

    void
    game::draw_game_over()
    {
        bool new_game{ false };

        while (_running)
        {
            draw_scene();

            rect_info info{ &_info.game_io, 0, 0, _info.game_io.get_screen_width(), _info.game_io.get_screen_height() };
            draw_game_over_overlay(info);

            _info.game_io.render();

            switch (int key{ _info.game_io.poll_key() })
            {
            case (SDLK_x):
            case (SDLK_ESCAPE):
    	        _running = false;
    	        break;

            case (SDLK_n):
    	        new_game = true;
    	        break;

            case (SDLK_UP):
                menu_selection_ = menu_selection_ == 0 ? 1 : 0;
                break;

            case (SDLK_DOWN):
                menu_selection_ = menu_selection_ == 0 ? 1 : 0;
                break;

            case (SDLK_SPACE):
            case (SDLK_RETURN):
                menu_selection_ == 0 ? new_game = true : _running = false;
                break; 

            default: break;
            }

            if (new_game)
            {
                _pos_x = 0;
                _pos_y = 0;
                _piece = 0;
                _rotation = 0;
                _next_pos_x = 0;
                _next_pos_y = 0;
                _next_piece = 0;
                _next_rotation = 0;
                _level = 1;
                _score = 0;
                _screen_height = 0;
                _wait_time = 700;
                _is_paused = false;
                _running = false;
                _info.game_board = { _info.game_io.get_screen_height(), _info.game_io.get_screen_width() };
                _info.time1 = SDL_GetTicks();
                _info.ticks_last_frame = 0;
                _info.dt = 0;

                init_game();

                return;
            }
        }
    }

    void
    game::draw_piece(int x, int y, int piece, int rotation)
    {
        rect_info info{};
        info.gfx = &_info.game_io;

        // Obtain the position in pixel in the screen of the block we want to draw
        const int pixels_x = _info.game_board.get_x_pos_in_pixels(x);
        const int pixels_y = _info.game_board.get_y_pos_in_pixels(y) - board_offset;

        // Travel the matrix of blocks of the piece and draw the blocks that are filled
        for (int i{ 0 }; i < static_cast<int>(piece_blocks); ++i)
            for (int j{ 0 }; j < static_cast<int>(piece_blocks); ++j)
            {
                if (bblocks::piece::get_block_type(piece, rotation, j, i) != 0)
                {
                    info.left = pixels_x + i * block_size;
                    info.top = pixels_y + j * block_size;
                    info.right = (pixels_x + i * block_size) + block_size - 1;
                    info.bottom = (pixels_y + j * block_size) + block_size - 1;

                    draw_piece_block(info, piece);
                }
            }
    }

    void
    game::draw_board()
    {
        rect_info info{};
        info.gfx = &_info.game_io;

        // Calculate the limits of the board in pixels
        int x1{ _info.game_board.get_board_position() - static_cast<int>(block_size * (board_width / 2)) - 1 };
        const int x2{ _info.game_board.get_board_position() + static_cast<int>(block_size * (board_width / 2)) };
        const int y{ _screen_height - static_cast<int>((block_size * board_height) + board_offset) };

        // Draw the vertical board boundaries
        for (int i{ 0 }; i <= static_cast<int>(block_size * board_height); i += block_size)
        {
            info.left = x1 - block_size;
            info.top = y + i;
            info.right = x1;
            info.bottom = y + i + block_size - 1;

            draw_board_block(info);

            info.left = x2;
            info.right = x2 + block_size;

            draw_board_block(info);
        }

        // Draw the horizontal board boundaries
        int x_offset{ 0 };
        for (int i{ 1 }; i <= static_cast<int>(board_width); ++i)
        {
            x_offset = i * static_cast<int>(block_size);
            info.left = (x1 - block_size) + x_offset + 1;
            info.top = _screen_height - static_cast<int>(block_size);
            info.right = x1 + x_offset;
            info.bottom = _screen_height - 1;

            draw_board_block(info);
        }

        // Drawing the blocks that are already stored in the board
        x1 += 1;
        for (int i{ 0 }; i < static_cast<int>(board_width); ++i)
            for (int j{ 0 }; j < static_cast<int>(board_height); ++j)
                // Check if the block is filled, if so, draw it
                if (!_info.game_board.is_free_block(i, j))
                {
                    info.left = x1 + i * block_size;
                    info.top = y + j * block_size;
                    info.right = (x1 + i * block_size) + block_size - 1;
                    info.bottom = (y + j * block_size) + block_size - 1;

                    draw_piece_block(info, _info.game_board.get_piece_type(i, j));
                }

        _info.game_io.draw_hud(_score, _level);
    }
}
