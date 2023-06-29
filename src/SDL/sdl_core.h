#pragma once

#include "sdl_common.h"

namespace bblocks::sdl::core
{
    bool init();
    void shutdown();
    void draw_rectangle(const glm::uvec2& top_left, const glm::uvec2& bottom_right, color c, bool filled = true);
    void clear_screen();
    [[nodiscard]] u32 display_width();
    [[nodiscard]] u32 display_height();
    s32 poll_key();
    s32 get_key();
    u8 is_key_down(u32 key);
    void render();
    void draw_hud(u32 points, u32 level);
    void draw_pause();
    void game_over(u32 arrow_pos);
}