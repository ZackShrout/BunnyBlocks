#pragma once

#include "sdl_common.h"

using u32 = uint32_t;

namespace bblocks::sdl::audio
{
	bool init();
    void shutdown();
    void play_sfx(u32 index);
    void play_music(u32 index);
    void pause_music();
    void resume_music();
}