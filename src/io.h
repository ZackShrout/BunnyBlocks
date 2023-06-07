#pragma once

#include <SDL2/SDL.h>
//#include "SDL/SDL_GfxPrimitives/sdl_gfxprimitives.h"

enum color
{
    black,
    red,
    green,
    blue,
    cyan,
    magenta,
    yellow,
    white,
    
    count
};

class io
{
public:
    io() { init_graph(); }
    ~io();
    void draw_rectangle(int x1, int y1, int x2, int y2, color c);
    void clear_screen();
    int get_screen_height() { return _height; }
    bool init_graph();
    int poll_key();
    int get_key();
    int is_key_down(int key);
    void update_screen();

private:
    SDL_Surface*    _surface{};
    SDL_Window*     _window{};
    int             _width{ 0 };
    int             _height{ 0 };
    uint32_t        _colors[color::count]
                    {
                        0x0000'00ff,
                        0xff00'00ff,
                        0x00ff'00ff,
                        0x0000'ffff,
                        0x00ff'ffff, 
                        0xff00'ffff,
                        0xffff'00ff,
                        0xffff'ffff
                    };
};