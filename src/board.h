#pragma once

#include "common.h"

namespace bblocks::board
{
	/// @brief Initialize board
	/// @param width Width of board
	/// @param height Height of board
	void init(u32 width, u32 height);
    /// @brief Get the horizontal position in pixels of block in given position
    /// @param pos Horizontal position of the block in the board
    /// @return Position in pixels of given block
    u32 get_x_pos_in_pixels(u32 pos);
    /// @brief Get the vertical position in pixels of block in given position
    /// @param pos Vertical position of the block in the board
    /// @return Position in pixels of given block
    u32 get_y_pos_in_pixels(u32 pos);
    /// @brief Get the center position of the board from the left of the screen
    /// @return Center position of the board
    u32 get_board_position();
    /// @brief Check to see if a block on the board is empty or filled
    /// @param x Horizontal position in blocks
    /// @param y Vertical position in blocks
    /// @return True - empty, false - filled
    bool is_free_block(u32 x, u32 y);
    /// @brief Check the type of piece that is stored in the board at a given coordinate
    /// @param x x position of piece to check
    /// @param y y position of piece to check
    /// @return Piece type
    u32 get_piece_type(u32 x, u32 y);
    /// @brief Check if the piece can be stored at this position without any collision
    /// @param x Horizontal position in blocks
    /// @param y Vertical position in blocks
    /// @param piece Piece to draw
    /// @param rotation Specify which of th 4 possible rotations
    /// @return True - movement is possible, false - movement is not possible
    bool is_possible_movement(s32 x, s32 y, u32 piece, u32 rotation);
    /// @brief Store a piece in the board by filling the blocks
    /// @param x Horizontal position in blocks
    /// @param y Vertical position in blocks
    /// @param piece Piece to draw
    /// @param rotation Specify which of th 4 possible rotations
    void store_piece(s32 x, s32 y, u32 piece, u32 rotation);
    /// @brief Delete all the lines that should be removed
    /// @param dt Timestep
    void delete_possible_lines(f32 dt);
    /// @brief Check if the game is over because a piece has achieved the upper position
    /// @return True - game over, false - game not over
    bool is_game_over();
    u32 lines_deleted();
    u32 get_board_line_width();
    u32 get_block_size();
    u32 get_board_width();
    u32 get_board_height();
    u32 get_min_vertical_margin();
    u32 get_min_horizontal_margin();
    u32 get_piece_blocks();
}
