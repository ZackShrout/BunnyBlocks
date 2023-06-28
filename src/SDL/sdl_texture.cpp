#include "sdl_texture.h"

#include <iostream>

bool
sdl_texture::load_from_file(const char *path)
{
	free();

	SDL_Texture* new_texture{ nullptr };

	if(SDL_Surface* loaded_surface{ IMG_Load(path) }; !loaded_surface)
        std::cerr << "Unable to load image " << path << "! SDL_image Error... " << IMG_GetError() << std::endl;
	else
	{
		SDL_SetColorKey(loaded_surface, SDL_TRUE, SDL_MapRGB(loaded_surface->format, 0, 0xff, 0xff));

        new_texture = SDL_CreateTextureFromSurface( _renderer, loaded_surface );
		if(!new_texture)
            std::cerr << "Unable to create texture from " << path << "! SDL Error... " << SDL_GetError() << std::endl;
		else
		{
			_width = loaded_surface->w;
			_height = loaded_surface->h;
		}

		SDL_FreeSurface(loaded_surface);
	}

	_texture = new_texture;
	return _texture != nullptr;
}

bool
sdl_texture::load_from_rendered_text(const char *texture_text, SDL_Color text_color)
{
    free();

    SDL_Texture* new_texture{ nullptr };

    SDL_Surface* text_surface = TTF_RenderText_Solid(_font, texture_text, text_color);
    if(!text_surface)
        std::cerr << "Unable to render text surface! SDL_ttf Error... " << TTF_GetError() << std::endl;
    else
    {
        new_texture = SDL_CreateTextureFromSurface(_renderer, text_surface);
        if(!new_texture)
            std::cerr << "Unable to create texture from rendered text! SDL Error... " << SDL_GetError() << std::endl;
        else
        {
            _width = text_surface->w;
            _height = text_surface->h;
        }

        SDL_FreeSurface(text_surface);
    }
    
    _texture = new_texture;
    return _texture != nullptr;
}

void
sdl_texture::free()
{
	if(_texture)
	{
		SDL_DestroyTexture(_texture);
		_texture = nullptr;
		_width = 0;
		_height = 0;
	}
}

void
sdl_texture::render(const int x, const int y, const SDL_Rect* clip/* = nullptr*/, const double angle/* = 0.0*/,
                    const SDL_Point* center/* = nullptr*/, const SDL_RendererFlip flip/* = SDL_FLIP_NONE*/) const
{
	SDL_Rect render_quad{ x, y, _width, _height };

	if (clip)
	{
		render_quad.w = clip->w;
		render_quad.h = clip->h;
	}

	if (_renderer)
		SDL_RenderCopyEx(_renderer, _texture, clip, &render_quad, angle, center, flip);
}
