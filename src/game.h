#pragma once

#include "board.h"
#include "SDL/io.h"

namespace bblocks
{
    struct game_info
    {
        io              game_io;
        board           game_board{ game_io.get_screen_height(), game_io.get_screen_width() };
        uint32_t        time1{ SDL_GetTicks() };
        uint32_t        ticks_last_frame;
        float           dt{ 0 };
    };

    class game
    {
    public:
        game() { init_game(); };
        void process_input();
        /// @brief Update the game
        void update();
        void render();
        bool is_running() const { return _running; }

    private:
        /// @brief Get a random int between to integers
        /// @param a First number
        /// @param b Second number
        /// @return Random number
        int get_rand (int a, int b) { return rand() % (b - a + 1) + a; }
        /// @brief Create a new random piece
        void new_piece();
        /// @brief Initialize game
        void init_game();
        /// @brief Draw all the objects of the scene
        void draw_scene();
        void draw_game_over();
        /// @brief Draw one piece to the board
        /// @param x Horizontal position in blocks
        /// @param y Vertical position in blocks
        /// @param piece Piece to draw
        /// @param rotation Specify which of th 4 possible rotations
        void draw_piece (int x, int y, int piece, int rotation);
        /// @brief Draw the game board
        void draw_board ();

        int         _pos_x{ 0 };
        int         _pos_y{ 0 };
        int         _piece{ 0 };
        int         _rotation{ 0 };
        int         _next_pos_x{ 0 };
        int         _next_pos_y{ 0 };
        int         _next_piece{ 0 };
        int         _next_rotation{ 0 };
        int         _level{ 1 };
        int         _score{ 0 };
        int         _screen_height{ 0 };
        int         _wait_time{ 700 };
        bool        _is_paused{ false };
        bool        _running{ false };
        game_info   _info{};
    };
}