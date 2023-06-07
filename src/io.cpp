#include <stdlib.h>
#include "io.h"

io::~io()
{
    SDL_DestroyWindow( _window );
    SDL_Quit();
}

void io::draw_rectangle(int x1, int y1, int x2, int y2, color c)
{
    SDL_Rect rect;
    rect.x = x1;
    rect.y = y1;
    rect.w = x2 - x1;
    rect.h = y2 - y1;

    uint8_t r = (_colors[c] >> 24) & 0xff;
    uint8_t g = (_colors[c] >> 16) & 0xff;
    uint8_t b = (_colors[c] >> 8) & 0xff;
    
    SDL_FillRect(_surface, &rect, SDL_MapRGB(_surface->format, r, g, b));
}

void io::clear_screen()
{
    SDL_FillRect(_surface, NULL, SDL_MapRGB(_surface->format, 0x00, 0x00, 0x00));
}

bool io::init_graph()
{
    SDL_DisplayMode     displayMode;
	uint8_t             video_bpp;
	uint32_t            video_flags;
        
	// Initialize SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0 )
    {
		fprintf(stderr, "Couldn't initialize SDL: %s\n", SDL_GetError());
		return false;
	}

	atexit(SDL_Quit);

    SDL_GetCurrentDisplayMode(0, &displayMode);
    _width = displayMode.w - 100;
    _height = displayMode.h - 100;

    _window = SDL_CreateWindow(
        "Bunny Blocks",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        _width,
        _height,
        /*SDL_WINDOW_BORDERLESS | SDL_WINDOW_FULLSCREEN*/ SDL_WINDOW_SHOWN);

    if (!_window)
    {
		fprintf(stderr, "Error creating SDL window: %s\n", SDL_GetError());
        return false;
    }

    //Get window surface
    _surface = SDL_GetWindowSurface(_window);

    //Fill the surface white
    SDL_FillRect(_surface, NULL, SDL_MapRGB(_surface->format, 0xFF, 0xFF, 0xFF));
    
    //Update the surface
    SDL_UpdateWindowSurface(_window);

    return true;
}

int io::poll_key()
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

int io::get_key()
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

int io::is_key_down(int key)
{
	int num_keys;

	SDL_PumpEvents();
	const uint8_t* key_table{ SDL_GetKeyboardState(&num_keys) };
	
    return key_table[key];
    return 0;
}


void io::update_screen()
{
    SDL_UpdateWindowSurface(_window);
}
