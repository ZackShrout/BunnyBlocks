#include "io.h"

#include <cstdlib>
#include <iostream>
#include <string>

#include "sdl_texture.h"

namespace
{
    constexpr int   desired_screen_w = 580;
    constexpr int   desired_screen_h = 420;

    int             current_points_{ -1 };
    int             current_level_{ -1 };

    TTF_Font*       hud_font_{ nullptr };
    TTF_Font*       heading_font_{ nullptr };
    sdl_texture     level_text_;
    sdl_texture     score_text_;
    sdl_texture     paused_text_;
    sdl_texture     game_over_text_;
    sdl_texture     new_game_text_;
    sdl_texture     exit_text_;
    sdl_texture     select_arrow_;
} // anonymous

void
io::draw_rectangle(int x1, int y1, int x2, int y2, color c, bool filled/* = true*/)
{
    SDL_Rect rect;
    rect.x = x1;
    rect.y = y1;
    rect.w = x2 - x1;
    rect.h = y2 - y1;

    uint8_t r = (_colors[c] >> 24) & 0xff;
    uint8_t g = (_colors[c] >> 16) & 0xff;
    uint8_t b = (_colors[c] >> 8) & 0xff;
    uint8_t a = _colors[c] & 0xff;
    
    SDL_SetRenderDrawColor(_renderer, r, g, b, a);

    if (filled)
        SDL_RenderFillRect(_renderer, &rect);
    else
        SDL_RenderDrawRect(_renderer, &rect);
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
    SDL_DisplayMode     display_mode;
	uint8_t             video_bpp;
	uint32_t            video_flags;
        
	// Initialize SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0 )
    {
        std::cerr << "Couldn't initialize SDL... " << SDL_GetError() << std::endl;
		return false;
	}

	atexit(SDL_Quit);

    SDL_GetCurrentDisplayMode(0, &display_mode);
    _max_width = display_mode.w;
    _max_height = display_mode.h;
    _width = (_max_width >= desired_screen_w ? desired_screen_w : _max_width);
    _height = (_max_height >= desired_screen_h ? desired_screen_h : _max_height);

    _window = SDL_CreateWindow(
        "Bunny Blocks",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        _width,
        _height,
        SDL_WINDOW_SHOWN);

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

    SDL_SetRenderDrawBlendMode(_renderer, SDL_BLENDMODE_BLEND);

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

    heading_font_ = TTF_OpenFont("fonts/Monoisome-Regular.ttf", 28);

    if(!hud_font_)
    {   
        std::cerr << "Failed to load heading font! SDL_ttf Error... " << TTF_GetError() << std::endl;
        return false;
    }

    level_text_.renderer(_renderer);
    score_text_.renderer(_renderer);
    paused_text_.renderer(_renderer);
    game_over_text_.renderer(_renderer);
    new_game_text_.renderer(_renderer);
    exit_text_.renderer(_renderer);
    level_text_.font(hud_font_);
    score_text_.font(hud_font_);
    paused_text_.font(heading_font_);
    game_over_text_.font(heading_font_);
    new_game_text_.font(hud_font_);
    exit_text_.font(hud_font_);
    select_arrow_.renderer(_renderer);

    SDL_Color text_color = { 0xff, 0xff, 0xff, 0xff };

    if(!paused_text_.load_from_rendered_text("Paused", text_color) ||
       !game_over_text_.load_from_rendered_text("Game Over...", text_color) ||
       !new_game_text_.load_from_rendered_text("New Game", text_color) ||
       !exit_text_.load_from_rendered_text("Exit", text_color))
    {
        std::cerr << "Failed to render text texture..." << std::endl;
        return false;
    }

    select_arrow_.blend_mode(SDL_BLENDMODE_BLEND);

    if (!select_arrow_.load_from_file("images/arrow.png"))
    {
        std::cerr << "Failed to render image texture..." << std::endl;
        return false;
    }

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

void
io::render()
{
    SDL_RenderPresent(_renderer);
}

void
io::draw_hud(int points, int level)
{
    if (current_points_ == -1 || current_points_ != points)
    {
        SDL_Color text_color = { 0xff, 0xff, 0xff, 0xff };
        std::string label{ std::to_string(points) };
        int digits{ static_cast<int>(label.size()) };

        if (digits != 9)
        {
            std::string prepend{ "0" };
            
            // NOTE: prepend already has one 0 in it, so we only need to fill (8 - digits)
            for (int i{ 0 }; i < 8 - digits; ++i)
            {
                prepend.append("0");
            }

            label = prepend + label;
        }

        if(!score_text_.load_from_rendered_text(label.c_str(), text_color))
        {
            std::cerr << "Failed to render text texture..." << std::endl;
            return;
        }

        current_points_ = points;
    }

    if (current_level_ == -1 || current_level_ != level)
    {   
        SDL_Color text_color = { 0xff, 0xff, 0xff, 0xff };
        std::string label{ "Level: "};
        label = label + std::to_string(level);

        if(!level_text_.load_from_rendered_text(label.c_str(), text_color))
        {
            std::cerr << "Failed to render text texture..." << std::endl;
            return;
        }

        current_level_ = level;
    }

    level_text_.render(60, 100);
    score_text_.render(60, 200);
}

void
io::draw_pause()
{
    int x{ static_cast<int>(_width / 2.f) - static_cast<int>(paused_text_.width() / 2.f) };
    int y{ static_cast<int>(_height / 2.f) - static_cast<int>(paused_text_.height() / 2.f) };

    draw_rectangle(x - 30, y - 20, x + paused_text_.width() + 30, y + paused_text_.height() + 20, white, false);
    paused_text_.render(x, y);
}

void
io::draw_game_over(uint32_t arrow_pos)
{
    int x{ static_cast<int>(_width / 2.f) - static_cast<int>(game_over_text_.width() / 2.f) };
    int game_over_y{ static_cast<int>(_height / 3.f) - static_cast<int>(game_over_text_.height() / 3.f) };
    int new_game_y{ game_over_y + game_over_text_.height() + 10 };
    int exit_y{ new_game_y + new_game_text_.height() + 10 };
    int arrow_offset_y{ static_cast<int>((new_game_text_.height() - select_arrow_.height()) / 2.f) };

    draw_rectangle(x - 30, game_over_y - 20,
                   x + game_over_text_.width() + 30, exit_y + exit_text_.height() + 20,
                   black);
    draw_rectangle(x - 30, game_over_y - 20,
                   x + game_over_text_.width() + 30, exit_y + exit_text_.height() + 20,
                   white, false);
    game_over_text_.render(x, game_over_y);

    switch (arrow_pos)
    {
        case 0:
            select_arrow_.render(x + 30, new_game_y + arrow_offset_y);
            break;

        case 1:
            select_arrow_.render(x + 30, exit_y + arrow_offset_y);
            break;
            
        default:
            break;
    }

    x = static_cast<int>(_width / 2.f) - static_cast<int>(new_game_text_.width() / 2.f);
    new_game_text_.render(x, new_game_y);

    x = static_cast<int>(_width / 2.f) - static_cast<int>(exit_text_.width() / 2.f);
    exit_text_.render(x, exit_y);
}
