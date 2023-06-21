#pragma once

#include "sdl_common.h"

class sdl_texture
{
public:
    sdl_texture() : _renderer{ nullptr }, _font{ nullptr } {};
    sdl_texture(SDL_Renderer* renderer, TTF_Font* font) : _renderer{ renderer }, _font{ font } {}
    ~sdl_texture() { free(); }
    /// @brief Loads an image at a specified file path
    /// @param path File path of image to load
    /// @return True on success, false on fail
    bool load_from_file(const char* path);
    /// @brief Creates in image from a font string
    /// @param texture_text Text string to render
    /// @param text_color Color of text to render
    /// @return True on success, false on fail
    bool load_from_rendered_text(const char* texture_text, SDL_Color text_color);
    /// @brief Dealocates a texture
    void free();
    /// @brief Renders texture at a given x,y coordinate
    /// @param x Left coordinate for the upper-left corner of texture
    /// @param y Upper coordinate for the upper-left corner of textre
    /// @param clip If provided, defines the portion of the texture to be rendered
    /// @param angle If provided, defines the angle the texture will be rendered at
    /// @param center If provided, defines the center of the texture
    /// @param flip How SDL should flip the image... default is none
    void render(int x, int y, SDL_Rect* clip = nullptr, double angle = 0.0,
                SDL_Point* center = nullptr, SDL_RendererFlip flip = SDL_FLIP_NONE);
    /// @brief Sets the color modulation of a texture
    /// @param red Red aspect of mod
    /// @param green Green aspect of mod
    /// @param blue Blue aspect of mod
    void color_mod(uint8_t red, uint8_t green, uint8_t blue) { SDL_SetTextureColorMod(_texture, red, green, blue); }
    /// @brief Sets the blending mode of texture
    /// @param blending Blending mode to set
    void blend_mode(SDL_BlendMode blending) { SDL_SetTextureBlendMode(_texture, blending); }
    /// @brief Set the alpha modulation of a texture
    /// @param alpha Alpha index of mod
    void alpha_mod(uint8_t alpha) { SDL_SetTextureAlphaMod(_texture, alpha); }
    int width() { return _width; }
    int height() { return _height; }
    void renderer(SDL_Renderer* renderer) { _renderer = renderer; }
    void font(TTF_Font* font) { _font = font; }

private:
    SDL_Renderer*   _renderer;
    TTF_Font*       _font;
    SDL_Texture*    _texture{ nullptr };
    int             _width{ 0 };
    int             _height{ 0 };
};