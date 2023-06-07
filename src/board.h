#pragma once

#include <stdint.h>
#include "pieces.h"

static constexpr uint32_t board_line_width{ 6 }; // Width of each of the two lines that delimit the board
static constexpr uint32_t block_size{ 16 }; // Width and Height of each block of a piece
static constexpr uint32_t board_position{ 320 }; // Center position of the board from the left of the screen
static constexpr uint32_t board_width{ 10 }; // Board width in blocks
static constexpr uint32_t board_height{ 20 }; // Board height in blocks
static constexpr uint32_t min_vertical_margin{ 20 }; // Minimum vertical margin for the board limit
static constexpr uint32_t min_horizontal_margin{ 20 }; // Minimum horizontal margin for the board limit
static constexpr uint32_t piece_blocks{ 5 }; // Number of horizontal and vertical blocks of a matrix piece

class board
{
public:
    board(pieces* pieces, int screen_height) : _pieces{ pieces }, _screen_height{ screen_height } { init_board(); }
    /// @brief Get the horizontal position in pixels of block in given position
    /// @param pos Horizontal position of the block in the board
    /// @return Position in pixels of given block
    int get_x_pos_in_pixels (int pos) { return ((board_position - (block_size * (board_width / 2))) + (pos * block_size)); }
    /// @brief Get the vertical position in pixels of block in given position
    /// @param pos Vertical position of the block in the board
    /// @return Position in pixels of given block
    int get_y_pos_in_pixels (int pos) { return ((_screen_height - (block_size * board_height)) + (pos * block_size)); }
    /// @brief Check to see if a block on the board is empty or filled
    /// @param x Horizontal position in blocks
    /// @param y Vertical position in blocks
    /// @return True - empty, false - filled
    bool is_free_block (int x, int y) { return _board[x][y] == pos_free; }
    /// @brief Check if the piece can be stored at this position without any collision
    /// @param x Horizontal position in blocks
    /// @param y Vertical position in blocks
    /// @param piece Piece to draw
    /// @param rotation Specify which of th 4 possible rotations
    /// @return True - movement is possible, false - movement is not possible
    bool is_possible_movement(int x, int y, int piece, int rotation);
    /// @brief Store a piece in the board by filling the blocks
    /// @param x Horizontal position in blocks
    /// @param y Vertical position in blocks
    /// @param piece Piece to draw
    /// @param rotation Specify which of th 4 possible rotations
    void store_piece(int x, int y, int piece, int rotation);
    /// @brief Delete all the lines that should be removed
    void delete_possible_lines();
    /// @brief Check if the game is over becase a piece have achived the upper position
    /// @return True - game over, false - game not over
    bool is_game_over();

private:
    /// @brief Init the board blocks with free positions
    void init_board();
    /// @brief Delete a line of the board by moving all above lines down
    /// @param y Vertical position in blocks of the line to delete
    void delete_line (int y);
    
    int     _board[board_width][board_height]; // Board that contains the pieces
    pieces* _pieces;
    int     _screen_height;
    enum    { pos_free, pos_filled }; // pos_free = free position of the board; pos_filled = filled position of the board
};
