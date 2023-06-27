#include "board.h"

#include <algorithm>
#include <vector>

namespace
{
    std::vector<int>    lines_to_delete_;
    bool                lines_for_deletion_{ false };
} // anonymous

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
board::delete_possible_lines(float dt)
{
    static float delay{ 0.f };

    for (int j{ 0 }; j < static_cast<int>(board_height); ++j)
    {
        int i{ 0 };
        while (i < static_cast<int>(board_width))
        {
            if (!(_board[i][j] > 0)) break;
            ++i;
        }

    // std::vector<int> v = { 4, 7, 5, 2, 6, 9 };
    // int key = 6;
 
    // if (std::find(v.begin(), v.end(), key) != v.end()) {
        // std::cout << "Element found";
    // }

        if (i == board_width)
            if (!lines_to_delete_.size() ||
                std::find(lines_to_delete_.begin(), lines_to_delete_.end(), j) == lines_to_delete_.end())
            {
                
                lines_to_delete_.emplace_back(j);
                lines_for_deletion_ = true;
            }
    }

    if (lines_for_deletion_ && delay < .25f)
    {
        delay += dt;
        return;
    }

    if (lines_for_deletion_)
    {
        for (uint32_t i{ 0 }; i < lines_to_delete_.size(); ++i)
        {
            delete_line(lines_to_delete_[i], dt);
        }
        
        lines_to_delete_.clear();
        lines_for_deletion_ = false;
        delay = 0.f;
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
board::delete_line(int y, float dt)
{   
    // Moves all the upper lines one row down
    for (int j{ y }; j > 0; --j)
        for (int i{ 0 }; i < static_cast<int>(board_width); ++i)
            _board[i][j] = _board[i][j-1];
    
    ++_lines_deleted;
}
