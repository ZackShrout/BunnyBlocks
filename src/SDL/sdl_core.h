#pragma once

#include "sdl_common.h"

namespace bblocks::sdl::core
{
    enum : s32
    {
        key_n = SDLK_n,
        key_p = SDLK_p,
        key_x = SDLK_x,
        key_z = SDLK_z,
        key_escape = SDLK_ESCAPE,
        key_enter = SDLK_RETURN,
        key_space = SDLK_SPACE,
        key_up = SDLK_UP,
        key_down = SDLK_DOWN,
        key_left = SDLK_LEFT,
        key_right = SDLK_RIGHT,
    };
    
    bool init();
    void shutdown();
    void draw_rectangle(const glm::uvec2& top_left, const glm::uvec2& bottom_right, color c, bool filled = true);
    void clear_screen();
    [[nodiscard]] u32 display_width();
    [[nodiscard]] u32 display_height();
    s32 poll_key();
    void render();
    void draw_hud(u32 points, u32 level);
    void draw_pause();
    void game_over(u32 arrow_pos);
}