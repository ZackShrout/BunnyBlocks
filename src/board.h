#pragma once

#include "common.h"

namespace bblocks::board
{
	/// @brief Initialize board
	/// @param width Width of board
	/// @param height Height of board
	void init(uint32_t width, uint32_t height);
    /// @brief Get the horizontal position in pixels of block in given position
    /// @param pos Horizontal position of the block in the board
    /// @return Position in pixels of given block
    uint32_t get_x_pos_in_pixels(uint32_t pos);
    /// @brief Get the vertical position in pixels of block in given position
    /// @param pos Vertical position of the block in the board
    /// @return Position in pixels of given block
    uint32_t get_y_pos_in_pixels(uint32_t pos);
    /// @brief Get the center position of the board from the left of the screen
    /// @return Center position of the board
    uint32_t get_board_position();
    /// @brief Check to see if a block on the board is empty or filled
    /// @param x Horizontal position in blocks
    /// @param y Vertical position in blocks
    /// @return True - empty, false - filled
    bool is_free_block(uint32_t x, uint32_t y);
    /// @brief Check the type of piece that is stored in the board at a given coordinate
    /// @param x x position of piece to check
    /// @param y y position of piece to check
    /// @return Piece type
    uint32_t get_piece_type(uint32_t x, uint32_t y);
    /// @brief Check if the piece can be stored at this position without any collision
    /// @param x Horizontal position in blocks
    /// @param y Vertical position in blocks
    /// @param piece Piece to draw
    /// @param rotation Specify which of th 4 possible rotations
    /// @return True - movement is possible, false - movement is not possible
    bool is_possible_movement(int32_t x, int32_t y, uint32_t piece, uint32_t rotation);
    /// @brief Store a piece in the board by filling the blocks
    /// @param x Horizontal position in blocks
    /// @param y Vertical position in blocks
    /// @param piece Piece to draw
    /// @param rotation Specify which of th 4 possible rotations
    void store_piece(int32_t x, int32_t y, uint32_t piece, uint32_t rotation);
    /// @brief Delete all the lines that should be removed
    /// @param dt Timestep
    void delete_possible_lines(float dt);
    /// @brief Check if the game is over because a piece has achieved the upper position
    /// @return True - game over, false - game not over
    bool is_game_over();
    uint32_t lines_deleted();
    uint32_t get_board_line_width();
    uint32_t get_block_size();
    uint32_t get_board_width();
    uint32_t get_board_height();
    uint32_t get_min_vertical_margin();
    uint32_t get_min_horizontal_margin();
    uint32_t get_piece_blocks();
}
