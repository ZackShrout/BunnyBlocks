#include "game.h"

void game::draw_scene()
{
    draw_board();
    draw_piece(_pos_x, _pos_y, _piece, _rotation);
    draw_piece(_next_pos_x, _next_pos_y, _next_piece, _next_rotation);
}

void game::create_new_piece()
{
    // Set current piece to previous next piece
    _piece = _next_piece;
    _rotation = _next_rotation;
    _pos_x = (board_width / 2) + _pieces->get_x_initial_position(_piece, _rotation);
    _pos_y = _pieces->get_y_initial_position(_piece, _rotation);

    // Get a new random next piece
    _next_piece = get_rand(0, 6);
    _next_rotation = get_rand(0, 3);
}

void game::init_game()
{
    srand((unsigned int)time(nullptr));

    _piece = get_rand(0, 6);
    _rotation = get_rand(0, 3);
    _pos_x = (board_width / 2) + _pieces->get_x_initial_position(_piece, _rotation);
    _pos_y = _pieces->get_y_initial_position(_piece, _rotation);

    _next_piece = get_rand(0, 6);
    _next_rotation = get_rand(0, 3);
    _next_pos_x = board_width + 5;
    _next_pos_y = 5;
}

// PRIVATE

void game::draw_piece(int x, int y, int piece, int rotation)
{
    color c; // Color of the block

    // Obtain the position in pixel in the screen of the block we want to draw
    int pixels_x = _board->get_x_pos_in_pixels(x);
    int pixels_y = _board->get_y_pos_in_pixels(y);

    // Travel the matrix of blocks of the piece and draw the blocks that are filled
    for (int i{ 0 }; i < static_cast<int>(piece_blocks); ++i)
        for (int j{ 0 }; j < static_cast<int>(piece_blocks); ++j)
        {
            // Get the type of the block and draw it with the correct color
            switch (_pieces->get_block_type(piece, rotation, j, i))
            {
                case 1: c = green; break; // For each block of the piece except the pivot
                case 2: c = blue; break; // For the pivot
            }

            if (_pieces->get_block_type(piece, rotation, j, i) != 0)
                _io->draw_rectangle(pixels_x + i * block_size, pixels_y + j * block_size,
                                    (pixels_x + i * block_size) + block_size - 1,
                                    (pixels_y + j * block_size) + block_size - 1, c);
        }
}

void game::draw_board()
{
    // Calculate the limits of the board in pixels
    int x1{ board_position - (block_size * (board_width / 2)) - 1 };
    int x2{ board_position + (block_size * (board_width / 2)) };
    int y{ _screen_height - static_cast<int>(block_size * board_height) };

    // Check that the vertical margin is not to small
    //assert (y > min_vertical_margin);

    // Rectangles that delimits the board
    _io->draw_rectangle(x1 - board_line_width, y, x1, _screen_height - 1, blue);
    _io->draw_rectangle(x2, y, x2 + board_line_width, _screen_height - 1, blue);

    // Check that the horizontal margin is not to small
    //assert (x1 > min_horizontal_margin);

    // Drawing the blocks that are already stored in the board
    x1 += 1;
    for (int i{ 0 }; i < static_cast<int>(board_width); ++i)
        for (int j{ 0 }; j < static_cast<int>(board_height); ++j)
            // Check if the block is filled, if so, draw it
            if (!_board->is_free_block(i, j))
                _io->draw_rectangle(x1 + i * block_size, y + j * block_size,
                                    (x1 + i * block_size) + block_size - 1,
                                    (y + j * block_size) + block_size - 1, red);
}
