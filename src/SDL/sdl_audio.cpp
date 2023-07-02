#include "sdl_audio.h"

#include <iostream>
#include <vector>

namespace bblocks::sdl::audio
{
	namespace
	{
		Mix_Music*					game_music_{ nullptr };
		Mix_Music*					game_over_music_{ nullptr };
		Mix_Music*					title_screen_music_ { nullptr };
		Mix_Chunk*					rotate_sfx_{ nullptr };
		Mix_Chunk*					died_sfx_{ nullptr };
		Mix_Chunk*					select_sfx_{ nullptr };
		Mix_Chunk*					thunk_sfx_{ nullptr };
		Mix_Chunk*					level_change_sfx_{ nullptr };
		Mix_Chunk*					clear_line_sfx_{ nullptr };
		Mix_Chunk*					move_sfx_{ nullptr };

		std::vector<Mix_Music*>		music_;
		std::vector<Mix_Chunk*>		sfx_;
	} // anonymous

	bool
	init()
	{
		if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
        {
            std::cerr << "SDL_mixer could not initialize! SDL_mixer Error... " << Mix_GetError() << std::endl;
            return false;
        }

    	game_music_ = Mix_LoadMUS("");
    	if(!game_music_)
			std::cerr << "Failed to load game_music_! SDL_mixer Error... " << Mix_GetError() << std::endl;
		else
			music_.emplace_back(game_music_);

		game_over_music_ = Mix_LoadMUS("");
    	if(!game_over_music_)
			std::cerr << "Failed to load game_over_music_! SDL_mixer Error... " << Mix_GetError() << std::endl;
		else
			music_.emplace_back(game_over_music_);
		
		title_screen_music_ = Mix_LoadMUS("");
    	if(!title_screen_music_)
			std::cerr << "Failed to load title_screen_music_! SDL_mixer Error... " << Mix_GetError() << std::endl;
		else
			music_.emplace_back(title_screen_music_);

    	rotate_sfx_ = Mix_LoadWAV("sfx/rotate.wav");
    	if(!rotate_sfx_)
    	    std::cerr << "Failed to load rotate_sfx_! SDL_mixer Error... " << Mix_GetError() << std::endl;
		else
			sfx_.emplace_back(rotate_sfx_);
		
		died_sfx_ = Mix_LoadWAV("sfx/game_over.wav");
    	if(!died_sfx_)
    	    std::cerr << "Failed to load died_sfx_! SDL_mixer Error... " << Mix_GetError() << std::endl;
		else
			sfx_.emplace_back(died_sfx_);

		select_sfx_ = Mix_LoadWAV("sfx/select.wav");
    	if(!select_sfx_)
    	    std::cerr << "Failed to load select_sfx_! SDL_mixer Error... " << Mix_GetError() << std::endl;
		else
			sfx_.emplace_back(select_sfx_);

		thunk_sfx_ = Mix_LoadWAV("sfx/thunk.wav");
    	if(!thunk_sfx_)
    	    std::cerr << "Failed to load thunk_sfx_! SDL_mixer Error... " << Mix_GetError() << std::endl;
		else
			sfx_.emplace_back(thunk_sfx_);

		level_change_sfx_ = Mix_LoadWAV("sfx/new_level.wav");
    	if(!level_change_sfx_)
    	    std::cerr << "Failed to load level_change_sfx_! SDL_mixer Error... " << Mix_GetError() << std::endl;
		else
			sfx_.emplace_back(level_change_sfx_);

		clear_line_sfx_ = Mix_LoadWAV("sfx/lines_clear.wav");
    	if(!clear_line_sfx_)
    	    std::cerr << "Failed to load clear_line_sfx_! SDL_mixer Error... " << Mix_GetError() << std::endl;
		else
			sfx_.emplace_back(clear_line_sfx_);

		move_sfx_ = Mix_LoadWAV("sfx/move.wav");
		if (!move_sfx_)
			std::cerr << "Failed to load move_sfx_! SDL_mixer Error... " << Mix_GetError() << std::endl;
		else
			sfx_.emplace_back(move_sfx_);
		
		return true;
	}

	void
	shutdown()
	{
		Mix_FreeMusic(game_music_);
		Mix_FreeMusic(game_over_music_);
		Mix_FreeMusic(title_screen_music_);
		Mix_FreeChunk(rotate_sfx_);
		Mix_FreeChunk(died_sfx_);
		Mix_FreeChunk(select_sfx_);
		Mix_FreeChunk(thunk_sfx_);
		Mix_FreeChunk(level_change_sfx_);
		Mix_FreeChunk(clear_line_sfx_);
		Mix_FreeChunk(move_sfx_);

		game_music_ = nullptr;
		game_over_music_ = nullptr;
		title_screen_music_ = nullptr;
		rotate_sfx_ = nullptr;
		died_sfx_ = nullptr;
		select_sfx_ = nullptr;
		thunk_sfx_ = nullptr;
		level_change_sfx_ = nullptr;
		clear_line_sfx_ = nullptr;
		move_sfx_ = nullptr;

		sfx_.clear();
		music_.clear();
		
		Mix_Quit();
	}

	void
	play_sfx(const u32 index)
	{
		if (sfx_.size() > index)
			Mix_PlayChannel(-1, sfx_.at(index), 0);
	}

	void
	play_music(const u32 index)
	{
		if (music_.size() <= index) return;
		
		if (Mix_PlayingMusic())
			Mix_FadeOutMusic(250);
		
		Mix_PlayMusic(music_.at(index), -1);
	}

	void
	pause_music()
	{
		Mix_PauseMusic();
	}

	void
	resume_music()
	{
		Mix_ResumeMusic();
	}
}