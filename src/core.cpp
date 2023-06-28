#include "core.h"

#include <algorithm>
#include <chrono>
#include <random>
#include <vector>
#include <iostream>
#include "board.h"
#include "pieces.h"
#include "SDL/sdl_core.h"

namespace bblocks::core
{
    namespace
    {
        // TODO: Simplify with top_left and bottom_right coordinates with GLM or DXM
        struct rect_info
        {
            u32     left;
            u32     top;
            u32     right;
            u32     bottom;
        };

        std::vector<u32>    pieces_{ 0, 1, 2, 3, 4, 5, 6 };
        std::vector<u32>    next_pieces_{ 0, 1, 2, 3, 4, 5, 6 };
        u32                 piece_index_{ 0 };

        // TODO: Simplify coordinate vars with math lib like GLM or DXM
        u32                 current_piece_{ 0 };
        u32                 current_rotation_{ 0 };
        s32                 current_pos_x_{ 0 };
        s32                 current_pos_y_{ 0 };
        u32                 next_piece_{ 0 };
        u32                 next_rotation_{ 0 };
        s32                 next_pos_x_{ 0 };
        s32                 next_pos_y_{ 0 };

        u32                 level_{ 1 };
        u32                 score_{ 0 };
        u32                 wait_time_{ 700 };
        u32                 time1_{ SDL_GetTicks() };
        u32                 ticks_last_frame_{ 0 };
        f32                 dt_{ 0 };

        u32                 menu_selection_{ 0 };
        bool                running_{ false };
        bool                paused_{ false };
        bool                gfx_initialized_{ false };

        void
        draw_block(const rect_info& info, const u32 piece = 7)
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
                case 7: c = brick; h = light_brick; s = dark_brick; break;
                default: c = magenta; h = magenta; s = magenta; break;
            }

            // Draw highlight color
            sdl::core::draw_rectangle(info.left, info.top, info.right, info.bottom, h);
            // Draw shadow color
            sdl::core::draw_rectangle(info.left + 2, info.top + 2, info.right, info.bottom, s);
            // Draw regular color
            sdl::core::draw_rectangle(info.left + 2, info.top + 2, info.right - 2, info.bottom - 2, c);
        }

        void
        darken_screen(const rect_info& screen)
        {
            sdl::core::draw_rectangle(screen.left, screen.top, screen.right, screen.bottom, fade);
        }

        void
        draw_pause_overlay(const rect_info& screen)
        {
            darken_screen(screen);
            sdl::core::draw_pause();
        }

        void
        draw_game_over_overlay(const rect_info& screen)
        {
            darken_screen(screen);
            sdl::core::game_over(menu_selection_);
        }

        void
        shuffle_pieces_bag(std::vector<u32>& bag)
        {
    	    const unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
            std::shuffle(bag.begin(), bag.end(), std::default_random_engine(seed));
        }

        u32
        get_rand(u32 a, u32 b)
        { 
            return rand() % (b - a + 1) + a;
        }

	    void
        new_piece()
        {
            ++piece_index_;

            if (piece_index_ == 7)
            {
                pieces_ = next_pieces_;
                piece_index_ = 0;     
            }

            current_piece_ = next_piece_;
            current_rotation_ = next_rotation_;
            current_pos_x_ = (board::get_board_width() / 2) + piece::get_x_initial_position(current_piece_, current_rotation_);
            current_pos_y_ = piece::get_y_initial_position(current_piece_, current_rotation_);

            if (piece_index_ == 6)
            {
                shuffle_pieces_bag(next_pieces_);
                next_piece_ = next_pieces_.at(0);
            }
            else
                next_piece_ = pieces_.at(piece_index_ + 1);

            next_rotation_ = get_rand(0, 3);
        }

	    void
        draw_piece(s32 x, s32 y, u32 piece, u32 rotation)
        {
            rect_info info{};

            // Obtain the position in pixel in the screen of the block we want to draw
            const u32 pixels_x = board::get_x_pos_in_pixels(x);
            const u32 pixels_y = board::get_y_pos_in_pixels(y) - board::get_block_size();

            // Travel the matrix of blocks of the piece and draw the blocks that are filled
            for (u32 i{ 0 }; i < board::get_piece_blocks(); ++i)
                for (u32 j{ 0 }; j < board::get_piece_blocks(); ++j)
                {
                    if (piece::get_block_type(piece, rotation, (s32)j, (s32)i) != 0)
                    {
                        info.left = pixels_x + i * board::get_block_size();
                        info.top = pixels_y + j * board::get_block_size();
                        info.right = (pixels_x + i * board::get_block_size()) + board::get_block_size ()- 1;
                        info.bottom = (pixels_y + j * board::get_block_size()) + board::get_block_size ()- 1;

                        draw_block(info, piece);
                    }
                }
        }

	    void
        draw_board()
        {
            rect_info info{};

            // Calculate the limits of the board in pixels
            u32 x1{ board::get_board_position() - (board::get_block_size() * (board::get_board_width() / 2)) - 1 };
            const u32 x2{ board::get_board_position() + (board::get_block_size() * (board::get_board_width() / 2)) };
            const u32 y{ sdl::core::display_height() - ((board::get_block_size() * board::get_board_height()) + board::get_block_size()) };
    
            // Draw the vertical board boundaries
            for (u32 i{ 0 }; i <= board::get_block_size() * board::get_board_height(); i += board::get_block_size())
            {
                info.left = x1 - board::get_block_size();
                info.top = y + i;
                info.right = x1;
                info.bottom = y + i + board::get_block_size() - 1;
    
                draw_block(info);
    
                info.left = x2;
                info.right = x2 + board::get_block_size();
    
                draw_block(info);
            }
    
            // Draw the horizontal board boundaries
            u32 x_offset{ 0 };
            for (u32 i{ 1 }; i <= board::get_board_width(); ++i)
            {
                x_offset = i * board::get_block_size();
                info.left = (x1 - board::get_block_size()) + x_offset + 1;
                info.top = sdl::core::display_height() - board::get_block_size();
                info.right = x1 + x_offset;
                info.bottom = sdl::core::display_height() - 1;
    
                draw_block(info);
            }
    
            // Drawing the blocks that are already stored in the board
            x1 += 1;
            for (u32 i{ 0 }; i < board::get_board_width(); ++i)
                for (u32 j{ 0 }; j < board::get_board_height(); ++j)
                    // Check if the block is filled, if so, draw it
                    if (!board::is_free_block(i, j))
                    {
                        info.left = x1 + i * board::get_block_size();
                        info.top = y + j * board::get_block_size();
                        info.right = (x1 + i * board::get_block_size()) + board::get_block_size() - 1;
                        info.bottom = (y + j * board::get_block_size()) + board::get_block_size() - 1;
    
                        draw_block(info, board::get_piece_type(i, j));
                    }
    
            sdl::core::draw_hud(score_, level_);
        }

        void
        draw_scene()
        {
            sdl::core::clear_screen();

            draw_board();
            draw_piece(current_pos_x_, current_pos_y_, current_piece_, current_rotation_);
            draw_piece(next_pos_x_, next_pos_y_, next_piece_, next_rotation_);

            if (paused_)
            {
    	        const rect_info info{ 0, 0, sdl::core::display_width(), sdl::core::display_height() };

                draw_pause_overlay(info);
            }
        }

        void
        draw_game_over()
        {
            bool new_game{ false };

            while (running_)
            {
                draw_scene();

                rect_info info{ 0, 0, sdl::core::display_width(), sdl::core::display_height() };
                draw_game_over_overlay(info);

                sdl::core::render();

                switch (int key{ sdl::core::poll_key() })
                {
                case (SDLK_x):
                case (SDLK_ESCAPE):
    	            running_ = false;
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
                    menu_selection_ == 0 ? new_game = true : running_ = false;
                    break; 

                default: break;
                }

                if (new_game)
                {
                    current_pos_x_ = 0;
                    current_pos_y_ = 0;
                    current_piece_ = 0;
                    current_rotation_ = 0;
                    next_pos_x_ = 0;
                    next_pos_y_ = 0;
                    next_piece_ = 0;
                    next_rotation_ = 0;
                    level_ = 1;
                    score_ = 0;
                    wait_time_ = 700;
                    paused_ = false;
                    running_ = false;
                    board::init(sdl::core::display_height(), sdl::core::display_width());
                    time1_ = SDL_GetTicks();
                    ticks_last_frame_ = 0;
                    dt_ = 0.f;

                    init();

                    return;
                }
            }
        }
    } // anonymous

    bool
    init()
    {
        if (!gfx_initialized_)
            gfx_initialized_ = sdl::core::init();

        if (!gfx_initialized_) return false;
        board::init(sdl::core::display_width(), sdl::core::display_height());
        

        shuffle_pieces_bag(pieces_);
        current_piece_ = pieces_.at(0);
        current_rotation_ = get_rand(0, 3);
        current_pos_x_ = (board::get_board_width() / 2) + piece::get_x_initial_position(current_piece_, current_rotation_);
        current_pos_y_ = piece::get_y_initial_position(current_piece_, current_rotation_);

        next_piece_ = pieces_.at(1);
        next_rotation_ = get_rand(0, 3);
        next_pos_x_ = board::get_board_width() + 3;
        next_pos_y_ = 5;

        running_ = true;
        
        return running_;
    }

    void
    shutdown()
    {
        sdl::core::shutdown();
    }

    void
    process_input()
    {
        switch (int key{ sdl::core::poll_key() })
    	{
    	case (SDLK_ESCAPE):
            running_ = false;
    		break;

    	case (SDLK_RIGHT):
    		if (board::is_possible_movement(current_pos_x_ + 1, current_pos_y_, current_piece_, current_rotation_))
    			++current_pos_x_;
    		break;

    	case (SDLK_LEFT):
    		if (board::is_possible_movement(current_pos_x_ - 1, current_pos_y_, current_piece_, current_rotation_))
    			--current_pos_x_;
    		break;

    	// Immediately move piece as far down as it goes and freeze it
    	case (SDLK_z):
    	case (SDLK_SPACE):
    		// Check collision from up to down
    		while (board::is_possible_movement(current_pos_x_, current_pos_y_, current_piece_, current_rotation_))
    			++current_pos_y_;

            board::store_piece(current_pos_x_, current_pos_y_ - 1, current_piece_, current_rotation_);
            board::delete_possible_lines(dt_);

    		if (board::is_game_over())
    		{
    			draw_game_over();
    		}

    		new_piece();

    		break;

    	// Rotate piece
    	case (SDLK_UP):
    		if (board::is_possible_movement(current_pos_x_, current_pos_y_, current_piece_, (current_rotation_ + 1) % 4))
    			current_rotation_ = (current_rotation_ + 1) % 4;
    		break;

    	// Speed up fall
    	case (SDLK_DOWN):
    		if (board::is_possible_movement(current_pos_x_, current_pos_y_ + 1, current_piece_, current_rotation_))
    			++current_pos_y_;
    		break;

    	case (SDLK_p):
        case (SDLK_RETURN):
    		paused_ = !paused_;
    		break;

    	default:
    		break;
    	}
    }

    void
    update()
    {
        dt_ = static_cast<f32>(SDL_GetTicks() - ticks_last_frame_) / 1000.f;
        dt_ = (dt_ > 0.05f) ? 0.05f : dt_;
        ticks_last_frame_ = SDL_GetTicks();

        board::delete_possible_lines(dt_);

        sdl::core::clear_screen();

        if (score_ != board::lines_deleted())
        {
            score_ = board::lines_deleted();

            if (score_ != 0 && (score_ - score_ % 15) / 15 > level_ - 1)
            {
                ++level_;
                wait_time_ -= static_cast<u32>(static_cast<f32>(wait_time_) / 4.f);
            }
        }

        if ((SDL_GetTicks() - time1_) > static_cast<u32>(wait_time_) && !paused_)
        {
            if (board::is_possible_movement(current_pos_x_, current_pos_y_ + 1, current_piece_, current_rotation_))
                ++current_pos_y_;
            else
            {
                board::store_piece(current_pos_x_, current_pos_y_, current_piece_, current_rotation_);

                if (board::is_game_over())
                {
                    draw_game_over();
                }

                new_piece();
            }

            time1_ = SDL_GetTicks();
        }

        if (!board::is_game_over())
            draw_scene();
        else
            draw_game_over();
    }

    void
    render()
    {
        sdl::core::render();
    }

    bool
    is_running()
    {
        return running_;
    } 
}