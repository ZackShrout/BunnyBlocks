#pragma once

#ifdef _WIN64

#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#else

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#endif

#include "../common.h"

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