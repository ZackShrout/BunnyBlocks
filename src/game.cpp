#include "game.h"
#include <iostream>
#include <chrono>
#include <random>
#include <algorithm>
#include <vector>

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
        info.gfx->draw_filled_rectangle(info.left, info.top, info.right, info.bottom, h);
        // Draw shadow color
        info.gfx->draw_filled_rectangle(info.left + 2, info.top + 2, info.right, info.bottom, s);
        // Draw regular color
        info.gfx->draw_filled_rectangle(info.left + 2, info.top + 2, info.right - 2, info.bottom - 2, c);
    }

    void
    draw_board_block(const rect_info& info)
    {
        // Draw highlight color
        info.gfx->draw_filled_rectangle(info.left, info.top, info.right, info.bottom, light_brick);
        // Draw shadow color
        info.gfx->draw_filled_rectangle(info.left + 2, info.top + 2, info.right, info.bottom, dark_brick);
        // Draw regular color
        info.gfx->draw_filled_rectangle(info.left + 2, info.top + 2, info.right - 2, info.bottom - 2, brick);
    }

    void
    darken_screen(const rect_info& screen)
    {
        screen.gfx->draw_filled_rectangle(screen.left, screen.top, screen.right, screen.bottom, fade);
    }

    void
    draw_pause_overlay(const rect_info& screen)
    {
        darken_screen(screen);
        screen.gfx->draw_pause();
    }

    void
    shuffle_pieces_bag(std::vector<int>& bag)
    {
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::shuffle(bag.begin(), bag.end(), std::default_random_engine(seed));
    }

    constexpr int       board_offset{ static_cast<int>(block_size) };
    std::vector<int>    pieces_{ 0, 1, 2, 3, 4, 5, 6 };
    std::vector<int>    next_pieces_{ 0, 1, 2, 3, 4, 5, 6 };
    int                 piece_index_{ 0 };
} // anonymous

void game::update()
{
    if (_score != _board->lines_deleted())
    {
        _score = _board->lines_deleted();

        if (_score != 0 && ((_score - (_score % 15)) / 15) > _level - 1)
        {
            ++_level;
            _wait_time -= static_cast<int>(_wait_time / 4.f);
            std::cerr << "Wait time: " << _wait_time << std::endl;
        }
    }

    draw_scene();
}

void game::new_piece()
{
    ++piece_index_;

    if (piece_index_ == 7)
    {
        pieces_ = next_pieces_;
        piece_index_ = 0;     
    }

    _piece = _next_piece;
    _rotation = _next_rotation;
    _pos_x = (board_width / 2) + _pieces->get_x_initial_position(_piece, _rotation);
    _pos_y = _pieces->get_y_initial_position(_piece, _rotation);

    if (piece_index_ == 6)
    {
        shuffle_pieces_bag(next_pieces_);
        _next_piece = next_pieces_.at(0);
    }
    else
        _next_piece = pieces_.at(piece_index_ + 1);

    _next_rotation = get_rand(0, 3);
}

// PRIVATE

void game::init_game()
{
    shuffle_pieces_bag(pieces_);
    _piece = pieces_.at(0);
    _rotation = get_rand(0, 3);
    _pos_x = (board_width / 2) + _pieces->get_x_initial_position(_piece, _rotation);
    _pos_y = _pieces->get_y_initial_position(_piece, _rotation);

    _next_piece = pieces_.at(1);
    _next_rotation = get_rand(0, 3);
    _next_pos_x = board_width + 3;
    _next_pos_y = 5;
}

void game::draw_scene()
{
    _io->clear_screen();
    
    draw_board();
    draw_piece(_pos_x, _pos_y, _piece, _rotation);
    draw_piece(_next_pos_x, _next_pos_y, _next_piece, _next_rotation);

    if (_is_paused)
    {
        rect_info info{ _io, 0, 0, _io->get_screen_width(), _io->get_screen_height() };

        draw_pause_overlay(info);
    }
}

void game::draw_piece(int x, int y, int piece, int rotation)
{
    rect_info info{};
    info.gfx = _io;
    
    // Obtain the position in pixel in the screen of the block we want to draw
    int pixels_x = _board->get_x_pos_in_pixels(x);
    int pixels_y = _board->get_y_pos_in_pixels(y) - board_offset;

    // Travel the matrix of blocks of the piece and draw the blocks that are filled
    for (int i{ 0 }; i < static_cast<int>(piece_blocks); ++i)
        for (int j{ 0 }; j < static_cast<int>(piece_blocks); ++j)
        {
            if (_pieces->get_block_type(piece, rotation, j, i) != 0)
            {
                info.left = pixels_x + i * block_size;
                info.top = pixels_y + j * block_size;
                info.right = (pixels_x + i * block_size) + block_size - 1;
                info.bottom = (pixels_y + j * block_size) + block_size - 1;

                draw_piece_block(info, piece);
            }
        }
}

void game::draw_board()
{
    rect_info info{};
    info.gfx = _io;

    // Calculate the limits of the board in pixels
    int x1{ _board->get_board_position() - static_cast<int>(block_size * (board_width / 2)) - 1 };
    int x2{ _board->get_board_position() + static_cast<int>(block_size * (board_width / 2)) };
    int y{ _screen_height - static_cast<int>((block_size * board_height) + board_offset) };

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
        info.top = _screen_height - (int)block_size;
        info.right = x1 + x_offset;
        info.bottom = _screen_height - 1;

        draw_board_block(info);
    }

    // Drawing the blocks that are already stored in the board
    x1 += 1;
    for (int i{ 0 }; i < static_cast<int>(board_width); ++i)
        for (int j{ 0 }; j < static_cast<int>(board_height); ++j)
            // Check if the block is filled, if so, draw it
            if (!_board->is_free_block(i, j))
            {
                info.left = x1 + i * block_size;
                info.top = y + j * block_size;
                info.right = (x1 + i * block_size) + block_size - 1;
                info.bottom = (y + j * block_size) + block_size - 1;

                draw_piece_block(info, _board->get_piece_type(i, j));
            }

    _io->draw_hud(_score, _level);
}
