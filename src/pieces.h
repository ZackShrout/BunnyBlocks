#pragma once

#include "common.h"

namespace bblocks::piece
{
    /// @brief Get the type of any given block inside of a shape
    /// @param piece Piece to draw
    /// @param rotation Specify which of 4 possible rotations
    /// @param x Horizontal position in block
    /// @param y Vertical position in block
    /// @return 0 if no block, 1 if normal block, 2 if rotation block
    u32 get_block_type (u32 piece, u32 rotation, s32 x, s32 y);
    /// @brief Returns the horizontal displacement of the piece that has to be applied in order to create it in the correct position.
    /// @param piece Piece to draw
    /// @param rotation Specify which of th 4 possible rotations
    /// @return Horizontal displacement
    s32 get_x_initial_position (u32 piece, u32 rotation);
    /// @brief Returns the vertical displacement of the piece that has to be applied in order to create it in the correct position.
    /// @param piece Piece to draw
    /// @param rotation Specify which of th 4 possible rotations
    /// @return Vertical displacement
    s32 get_y_initial_position (u32 piece, u32 rotation);
}