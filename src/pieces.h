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
    uint32_t get_block_type (uint32_t piece, uint32_t rotation, uint32_t x, uint32_t y);
    /// @brief Returns the horizontal displacement of the piece that has to be applied in order to create it in the correct position.
    /// @param piece Piece to draw
    /// @param rotation Specify which of th 4 possible rotations
    /// @return Horizontal displacement
    int32_t get_x_initial_position (uint32_t piece, uint32_t rotation);
    /// @brief Returns the vertical displacement of the piece that has to be applied in order to create it in the correct position.
    /// @param piece Piece to draw
    /// @param rotation Specify which of th 4 possible rotations
    /// @return Vertical displacement
    int32_t get_y_initial_position (uint32_t piece, uint32_t rotation);
}