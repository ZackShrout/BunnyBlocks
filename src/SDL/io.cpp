#include <stdlib.h>
#include <iostream>
#include <string>
#include "sdl_texture.h"
#include "io.h"

namespace
{
    constexpr int   desired_screen_w = 580;
    constexpr int   desired_screen_h = 420;

    int             current_points_{ -1 };
    int             current_level_{ -1 };

    TTF_Font*       hud_font_{ nullptr };
    sdl_texture     level_text_;
    sdl_texture     score_text_;
} // anonymous

void
io::draw_rectangle(int x1, int y1, int x2, int y2, color c)
{
    SDL_Rect rect;
    rect.x = x1;
    rect.y = y1;
    rect.w = x2 - x1;
    rect.h = y2 - y1;

    uint8_t r = (_colors[c] >> 24) & 0xff;
    uint8_t g = (_colors[c] >> 16) & 0xff;
    uint8_t b = (_colors[c] >> 8) & 0xff;
    
    SDL_SetRenderDrawColor(_renderer, r, g, b, 0xff);
    SDL_RenderFillRect(_renderer, &rect);
}

void
io::clear_screen()
{
    SDL_SetRenderDrawColor(_renderer, 0x00, 0x00, 0x00, 0xff);
    SDL_RenderClear(_renderer);
}

bool
io::init_graph()
{
    SDL_DisplayMode     displayMode;
	uint8_t             video_bpp;
	uint32_t            video_flags;
        
	// Initialize SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0 )
    {
        std::cerr << "Couldn't initialize SDL... " << SDL_GetError() << std::endl;
		return false;
	}

	atexit(SDL_Quit);

    SDL_GetCurrentDisplayMode(0, &displayMode);
    _max_width = displayMode.w;
    _max_height = displayMode.h;
    _width = (_max_width >= desired_screen_w ? desired_screen_w : _max_width);
    _height = (_max_height >= desired_screen_h ? desired_screen_h : _max_height);

    _window = SDL_CreateWindow(
        "Bunny Blocks",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        _width,
        _height,
        /*SDL_WINDOW_BORDERLESS | SDL_WINDOW_FULLSCREEN*/ SDL_WINDOW_SHOWN);

    if (!_window)
    {
        std::cerr << "Error creating SDL window... " << SDL_GetError() << std::endl;
        return false;
    }

    _renderer = SDL_CreateRenderer(_window, -1, 0);
    if (!_renderer)
    {
        std::cerr << "Error creating SDL renderer... " << SDL_GetError() << std::endl;
        return false;
    }

    int img_flags = IMG_INIT_PNG;
    if(!(IMG_Init(img_flags) & img_flags))
    {
        std::cerr << "SDL_image could not initialize! SDL_image Error... " << IMG_GetError() << std::endl;
        return false;
    }

    if(TTF_Init() == -1)
    {
        std::cerr << "SDL_ttf could not initialize! SDL_ttf Error... " << TTF_GetError() << std::endl;
        return false;
    }

    hud_font_ = TTF_OpenFont("fonts/Monoisome-Regular.ttf", 16);

    if(!hud_font_)
    {   
        std::cerr << "Failed to load HUD font! SDL_ttf Error... " << TTF_GetError() << std::endl;
        return false;
    }

    level_text_.renderer(_renderer);
    score_text_.renderer(_renderer);
    level_text_.font(hud_font_);
    score_text_.font(hud_font_);

    return true;
}

void
io::shutdown()
{
	level_text_.free();
	score_text_.free();

	TTF_CloseFont(hud_font_);
	hud_font_ = nullptr;

	SDL_DestroyRenderer(_renderer);
	SDL_DestroyWindow(_window);
	_window = nullptr;
	_renderer = nullptr;

	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

int
io::poll_key()
{
    SDL_Event event;

	while (SDL_PollEvent(&event)) 
	{
		switch (event.type)
        {
			case SDL_KEYDOWN:
				return event.key.keysym.sym;
			case SDL_QUIT:
				exit(3);
		}
	}
	return -1;
}

int
io::get_key()
{
    SDL_Event event;

	while (true)
	{
	  SDL_WaitEvent(&event);
	  if (event.type == SDL_KEYDOWN)
		  break;
      if (event.type == SDL_QUIT)
		  exit(3);
	};

	return event.key.keysym.sym;
}

int
io::is_key_down(int key)
{
	int num_keys;

	SDL_PumpEvents();
	const uint8_t* key_table{ SDL_GetKeyboardState(&num_keys) };
	
    return key_table[key];
    return 0;
}

void io::render()
{
    SDL_RenderPresent(_renderer);
}

void
io::draw_hud(int points, int level)
{
    if (current_points_ == -1 || current_points_ != points)
    {
        SDL_Color text_color = { 0xff, 0xff, 0xff, 0xff };
        std::string lable{ std::to_string(points) };
        int digits{ static_cast<int>(lable.size()) };

        if (digits != 9)
        {
            std::string prepend{ "0" };
            
            // NOTE: prepend already has one 0 in it, so we only need to fill (8 - digits)
            for (int i{ 0 }; i < 8 - digits; ++i)
            {
                prepend = prepend + "0";
            }

            lable = prepend + lable;
        }

        if(!score_text_.load_from_rendered_text(lable.c_str(), text_color))
        {
            std::cerr << "Failed to render text texture..." << std::endl;
            return;
        }

        current_points_ = points;
    }

    if (current_level_ == -1 || current_level_ != level)
    {   
        SDL_Color text_color = { 0xff, 0xff, 0xff, 0xff };
        std::string lable{ "Level: "};
        lable = lable + std::to_string(level);

        if(!level_text_.load_from_rendered_text(lable.c_str(), text_color))
        {
            std::cerr << "Failed to render text texture..." << std::endl;
            return;
        }

        current_level_ = level;
    }
    
    level_text_.render(60, 100);
    score_text_.render(60, 200);
}
