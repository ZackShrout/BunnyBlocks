#pragma once

#include <time.h>
#include <random>
#include "board.h"
#include "pieces.h"
#include "SDL/io.h"

class game
{
public:

    game(board* board, pieces* pieces, io* io, int screen_height)
            : _board{ board }, _pieces{ pieces }, _io{ io }, _screen_height{ screen_height } { init_game(); }
    /// @brief Update the game
    void update();
    /// @brief Create a new random piece
    void new_piece();
    /// @brief Get current piece x
    /// @return Current piece x
    int pos_x() { return _pos_x; }
    /// @brief Get current piece y
    /// @return Current piece y
    int pos_y() { return _pos_y; }
    /// @brief Get current piece
    /// @return Current piece
    int piece() { return _piece; }
    /// @brief Get current piece rotation
    /// @return Current piece rotation
    int rotation() { return _rotation; }
    int wait_time() { return _wait_time; }
    /// @brief Set current piece x
    /// @param x Horizontal position to set
    void set_pos_x(int x) { _pos_x = x; }
    /// @brief Set current piece y
    /// @param y Vrtical position to set
    void set_pos_y(int y) { _pos_y = y; }
    /// @brief Set current piece
    /// @param piece Piece to set
    void set_piece(int piece) { _piece = piece; }
    /// @brief Set current piece rotation
    /// @param rotation Rotation to set
    void set_rotation(int rotation) { _rotation = rotation; }
    void toggle_pause() { _is_paused = !_is_paused; }
    bool is_paused() const { return _is_paused; }

private:
    /// @brief Get a random int between to integers
    /// @param a First number
    /// @param b Second number
    /// @return Random number
    int get_rand (int a, int b) { return rand() % (b - a + 1) + a; }
    /// @brief Initialize game
    void init_game();
    /// @brief Draw all the objects of the scene
    void draw_scene();
    /// @brief Draw one piece to the board
    /// @param x Horizontal position in blocks
    /// @param y Vertical position in blocks
    /// @param piece Piece to draw
    /// @param rotation Specify which of th 4 possible rotations
    void draw_piece (int x, int y, int piece, int rotation);
    /// @brief Draw the game board
    void draw_board ();

    int     _pos_x{ 0 };
    int     _pos_y{ 0 };
    int     _piece{ 0 };
    int     _rotation{ 0 };
    int     _next_pos_x{ 0 };
    int     _next_pos_y{ 0 };
    int     _next_piece{ 0 };
    int     _next_rotation{ 0 };
    int     _level{ 1 };
    int     _score{ 0 };
    int     _screen_height;
    int     _wait_time{ 700 };
    bool    _is_paused{ false };
    board*  _board;
    pieces* _pieces;
    io*     _io;
};