#include "pieces.h"

namespace bblocks::piece
{
    namespace
    {
        // NOTE: layout is pieces[kind][rotation][horizontal blocks][vertical blocks]
        char pieces [7][4][5][5]
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

        // NOTE: layout is pieces_initial_position[kind][rotation][position]
        int32_t pieces_initial_position[7][4][2]
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
    } // anonymous namespace    

    uint32_t
    get_block_type (uint32_t piece, uint32_t rotation, uint32_t x, uint32_t y)
    { 
        return pieces[piece][rotation][x][y];
    }

    int32_t
    get_x_initial_position (uint32_t piece, uint32_t rotation)
    { 
        return pieces_initial_position[piece][rotation][0]; 
    }

    int32_t
    get_y_initial_position (uint32_t piece, uint32_t rotation)
    { 
        return pieces_initial_position[piece][rotation][1];
    }
}