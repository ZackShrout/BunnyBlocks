#pragma once

class pieces
{
public:
    /// @brief Get the type of any given block inside of a shape
    /// @param piece Piece to draw
    /// @param rotation Specify which of 4 possible rotations
    /// @param x Horizontal position in block
    /// @param y Vertical position in block
    /// @return 0 if no block, 1 if normal block, 2 if rotation block
    int get_block_type (int piece, int rotation, int x, int y) { return _pieces[piece][rotation][x][y]; }
    /// @brief Returns the horizontal displacement of the piece that has to be applied in order to create it in the correct position.
    /// @param piece Piece to draw
    /// @param rotation Specify which of th 4 possible rotations
    /// @return Horizontal displacement
    int get_x_initial_position (int piece, int rotation) { return _pieces_initial_position[piece][rotation][0]; }
    /// @brief Returns the vertical displacement of the piece that has to be applied in order to create it in the correct position.
    /// @param piece Piece to draw
    /// @param rotation Specify which of th 4 possible rotations
    /// @return Vertical displacement
    int get_y_initial_position (int piece, int rotation) { return _pieces_initial_position[piece][rotation][1]; }

private:
    char _pieces [7 /*kind */ ][4 /* rotation */ ][5 /* horizontal blocks */ ][5 /* vertical blocks */ ]
    {
        // Square
        {
            {{0, 0, 0, 0, 0},
             {0, 0, 0, 0, 0},
             {0, 0, 2, 1, 0},
             {0, 0, 1, 1, 0},
             {0, 0, 0, 0, 0}},
            {{0, 0, 0, 0, 0},
             {0, 0, 0, 0, 0},
             {0, 0, 2, 1, 0},
             {0, 0, 1, 1, 0},
             {0, 0, 0, 0, 0}},
            {{0, 0, 0, 0, 0},
             {0, 0, 0, 0, 0},
             {0, 0, 2, 1, 0},
             {0, 0, 1, 1, 0},
             {0, 0, 0, 0, 0}},
            {{0, 0, 0, 0, 0},
             {0, 0, 0, 0, 0},
             {0, 0, 2, 1, 0},
             {0, 0, 1, 1, 0},
             {0, 0, 0, 0, 0}}
        },
        // I
        {
            {{0, 0, 0, 0, 0},
             {0, 0, 0, 0, 0},
             {0, 1, 2, 1, 1},
             {0, 0, 0, 0, 0},
             {0, 0, 0, 0, 0}},
            {{0, 0, 0, 0, 0},
             {0, 0, 1, 0, 0},
             {0, 0, 2, 0, 0},
             {0, 0, 1, 0, 0},
             {0, 0, 1, 0, 0}},
            {{0, 0, 0, 0, 0},
             {0, 0, 0, 0, 0},
             {1, 1, 2, 1, 0},
             {0, 0, 0, 0, 0},
             {0, 0, 0, 0, 0}},
            {{0, 0, 1, 0, 0},
             {0, 0, 1, 0, 0},
             {0, 0, 2, 0, 0},
             {0, 0, 1, 0, 0},
             {0, 0, 0, 0, 0}}
        },
        // L
        {
            {{0, 0, 0, 0, 0},
             {0, 0, 1, 0, 0},
             {0, 0, 2, 0, 0},
             {0, 0, 1, 1, 0},
             {0, 0, 0, 0, 0}},
            {{0, 0, 0, 0, 0},
             {0, 0, 0, 0, 0},
             {0, 1, 2, 1, 0},
             {0, 1, 0, 0, 0},
             {0, 0, 0, 0, 0}},
            {{0, 0, 0, 0, 0},
             {0, 1, 1, 0, 0},
             {0, 0, 2, 0, 0},
             {0, 0, 1, 0, 0},
             {0, 0, 0, 0, 0}},
            {{0, 0, 0, 0, 0},
             {0, 0, 0, 1, 0},
             {0, 1, 2, 1, 0},
             {0, 0, 0, 0, 0},
             {0, 0, 0, 0, 0}}
        },
        // L mirrored
        {
            {{0, 0, 0, 0, 0},
             {0, 0, 1, 0, 0},
             {0, 0, 2, 0, 0},
             {0, 1, 1, 0, 0},
             {0, 0, 0, 0, 0}},
            {{0, 0, 0, 0, 0},
             {0, 1, 0, 0, 0},
             {0, 1, 2, 1, 0},
             {0, 0, 0, 0, 0},
             {0, 0, 0, 0, 0}},
            {{0, 0, 0, 0, 0},
             {0, 0, 1, 1, 0},
             {0, 0, 2, 0, 0},
             {0, 0, 1, 0, 0},
             {0, 0, 0, 0, 0}},
            {{0, 0, 0, 0, 0},
             {0, 0, 0, 0, 0},
             {0, 1, 2, 1, 0},
             {0, 0, 0, 1, 0},
             {0, 0, 0, 0, 0}}
        },
        // N
        {
            {{0, 0, 0, 0, 0},
             {0, 0, 0, 1, 0},
             {0, 0, 2, 1, 0},
             {0, 0, 1, 0, 0},
             {0, 0, 0, 0, 0}},
            {{0, 0, 0, 0, 0},
             {0, 0, 0, 0, 0},
             {0, 1, 2, 0, 0},
             {0, 0, 1, 1, 0},
             {0, 0, 0, 0, 0}},
            {{0, 0, 0, 0, 0},
             {0, 0, 1, 0, 0},
             {0, 1, 2, 0, 0},
             {0, 1, 0, 0, 0},
             {0, 0, 0, 0, 0}},
            {{0, 0, 0, 0, 0},
             {0, 1, 1, 0, 0},
             {0, 0, 2, 1, 0},
             {0, 0, 0, 0, 0},
             {0, 0, 0, 0, 0}}
        },
        // N mirrored
        {
            {{0, 0, 0, 0, 0},
             {0, 0, 1, 0, 0},
             {0, 0, 2, 1, 0},
             {0, 0, 0, 1, 0},
             {0, 0, 0, 0, 0}},
            {{0, 0, 0, 0, 0},
             {0, 0, 0, 0, 0},
             {0, 0, 2, 1, 0},
             {0, 1, 1, 0, 0},
             {0, 0, 0, 0, 0}},
            {{0, 0, 0, 0, 0},
             {0, 1, 0, 0, 0},
             {0, 1, 2, 0, 0},
             {0, 0, 1, 0, 0},
             {0, 0, 0, 0, 0}},
            {{0, 0, 0, 0, 0},
             {0, 0, 1, 1, 0},
             {0, 1, 2, 0, 0},
             {0, 0, 0, 0, 0},
             {0, 0, 0, 0, 0}}
        },
        // T
        {
            {{0, 0, 0, 0, 0},
             {0, 0, 1, 0, 0},
             {0, 0, 2, 1, 0},
             {0, 0, 1, 0, 0},
             {0, 0, 0, 0, 0}},
            {{0, 0, 0, 0, 0},
             {0, 0, 0, 0, 0},
             {0, 1, 2, 1, 0},
             {0, 0, 1, 0, 0},
             {0, 0, 0, 0, 0}},
            {{0, 0, 0, 0, 0},
             {0, 0, 1, 0, 0},
             {0, 1, 2, 0, 0},
             {0, 0, 1, 0, 0},
             {0, 0, 0, 0, 0}},
            {{0, 0, 0, 0, 0},
             {0, 0, 1, 0, 0},
             {0, 1, 2, 1, 0},
             {0, 0, 0, 0, 0},
             {0, 0, 0, 0, 0}}
        }
    };

    int _pieces_initial_position [7 /*kind */ ][4 /* r2otation */ ][2 /* position */]
    {
        // Square
        {{-2, -3},
         {-2, -3},
         {-2, -3},
         {-2, -3}},
        // I
        {{-2, -2},
         {-2, -3},
         {-2, -2},
         {-2, -3}},
        // L
        {{-2, -3},
         {-2, -3},
         {-2, -3},
         {-2, -2}},
        // L mirrored
        {{-2, -3},
         {-2, -2},
         {-2, -3},
         {-2, -3}},
        // N
        {{-2, -3},
         {-2, -3},
         {-2, -3},
         {-2, -2}},
        // N mirrored
        {{-2, -3},
         {-2, -3},
         {-2, -3},
         {-2, -2}},
        // T
        {{-2, -3},
         {-2, -3},
         {-2, -3},
         {-2, -2}},
    };
};