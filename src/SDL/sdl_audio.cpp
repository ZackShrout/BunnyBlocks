#include "sdl_audio.h"

namespace bblocks::sdl::audio
{
	namespace
	{
		Mix_Music* music_{ nullptr };
		Mix_Chunk* rotate_sfx_{ nullptr };
		Mix_Chunk* died_sfx_{ nullptr };
		Mix_Chunk* select_sfx_{ nullptr };
		Mix_Chunk* thunk_sfx_{ nullptr };
		Mix_Chunk* level_change_sfx_{ nullptr };
		Mix_Chunk* clear_line_sfx_{ nullptr };

	} // anonymous


}