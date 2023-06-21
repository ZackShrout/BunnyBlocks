#include "board.h"

bool
board::is_possible_movement(int x, int y, int piece, int rotation)
{
    // Checks collision with pieces already stored in the board or the board limits
    // This is just to check the 5×5 blocks of a piece with the appropriate area in the board
    for (int i1{ x }, i2{ 0 }; i1 < x + static_cast<int>(piece_blocks); ++i1, ++i2)
        for (int j1{ y }, j2{ 0 }; j1 < y + static_cast<int>(piece_blocks); ++j1, ++j2)
        {
            // Check if the piece is outside the limits of the board
            if (i1 < 0 || i1 > static_cast<int>(board_width) - 1 || j1 > static_cast<int>(board_height) - 1)
                if (_pieces->get_block_type(piece, rotation, j2, i2) != 0) return 0;

            // Check if the piece have collisioned with a block already stored in the map
            if (j1 >= 0)
                if ((_pieces->get_block_type(piece, rotation, j2, i2) != 0) && (!is_free_block(i1, j1))) return false;
        }

    // No collision
    return true;
}

void
board::store_piece(int x, int y, int piece, int rotation)
{
    // Store each block of the piece into the board
    for (int i1{ x }, i2{ 0 }; i1 < x + static_cast<int>(piece_blocks); ++i1, ++i2)
        for (int j1{ y }, j2{ 0 }; j1 < y + static_cast<int>(piece_blocks); ++j1, ++j2)
            // Store only the blocks of the piece that are not holes
            if (_pieces->get_block_type(piece, rotation, j2, i2) != 0)
                _board[i1][j1] = piece + 1;
}

void
board::delete_possible_lines()
{
    for (int j{ 0 }; j < static_cast<int>(board_height); ++j)
    {
        int i{ 0 };
        while (i < static_cast<int>(board_width))
        {
            if (!(_board[i][j] > 0)) break;
            ++i;
        }

        if (i == board_width)
            delete_line(j);
    }
}

bool
board::is_game_over()
{
    //If the first line has blocks, then, game over
    for (int i{ 0 }; i < static_cast<int>(board_width); ++i)
        if (_board[i][0] > 0) return true;

    return false;
}

void
board::init_board()
{
    _board_position = (_screen_width / 2);
    
    for (int i{ 0 }; i < static_cast<int>(board_width); ++i)
        for (int j{ 0 }; j < static_cast<int>(board_height); ++j)
            _board[i][j] = 0;
}

void
board::delete_line(int y)
{
    // Moves all the upper lines one row down
    for (int j{ y }; j > 0; --j)
        for (int i{ 0 }; i < static_cast<int>(board_width); ++i)
            _board[i][j] = _board[i][j-1];

    ++_lines_deleted;
}
