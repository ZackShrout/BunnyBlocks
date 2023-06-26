#pragma once

#include "sdl_common.h"

enum color
{
    black,
    red,
    dark_red,
    light_red,
    green,
    dark_green,
    light_green,
    blue,
    dark_blue,
    light_blue,
    purple,
    dark_purple,
    light_purple,
    orange,
    dark_orange,
    light_orange,
    yellow,
    dark_yellow,
    light_yellow,
    pink,
    dark_pink,
    light_pink,
    cyan,
    magenta,
    light_grey,
    brick,
    dark_brick,
    light_brick,
    white,
    fade,
    
    count
};

class io
{
public:
    io() { init_graph(); }
    ~io() { shutdown(); }
    void draw_filled_rectangle(int x1, int y1, int x2, int y2, color c);
    void draw_rectangle(int x1, int y1, int x2, int y2, color c);
    void clear_screen();
    int get_screen_height() { return _height; }
    int get_screen_width() {return _width; }
    bool init_graph();
    void shutdown();
    int poll_key();
    int get_key();
    int is_key_down(int key);
    void render();
    void draw_hud(int points, int level);
    void draw_pause();

private:
    SDL_Surface*    _surface{};
    SDL_Window*     _window{};
    SDL_Renderer*   _renderer{};
    int             _width{ 0 };
    int             _height{ 0 };
    int             _max_width{ 0 };
    int             _max_height{ 0 };
    uint32_t        _colors[color::count]
                    {
                        0x0000'00ff,    // black
                        0xff00'00ff,    // red
                        0x8000'00ff,    // dark red
                        0xff7f'7fff,    // light red
                        0x00ff'00ff,    // green
                        0x0230'20ff,    // dark green
                        0x98fb'98ff,    // light green
                        0x0000'ffff,    // blue
                        0x1919'70ff,    // dark blue
                        0x7393'b3ff,    // light blue
                        0xda70'd6ff,    // purple
                        0x7f00'ffff,    // dark purple
                        0xd8bf'd8ff,    // light purple
                        0xff75'18ff,    // orange
                        0xc040'00ff,    // dark orange
                        0xffd5'80ff,    // light orange
                        0xfafa'33ff,    // yellow
                        0xffc0'00ff,    // dark yellow
                        0xffff'8fff,    // light yellow
                        0xf883'79ff,    // pink
                        0xaa33'6aff,    // dark pink
                        0xfff5'6aff,    // light pink
                        0x00ff'ffff,    // cyan
                        0xff00'ffff,    // magenta
                        0x2020'20ff,    // light grey
                        0x5900'00ff,    // brick
                        0x3300'00ff,    // dark brick
                        0x8c19'19ff,    // light brick
                        0xffff'ffff,    // white
                        0x0000'007f     // fade
                    };
};