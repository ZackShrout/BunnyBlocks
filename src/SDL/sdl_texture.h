#pragma once

#include "sdl_common.h"

class sdl_texture
{
public:
    sdl_texture() {}
    ~sdl_texture() {}
    /// @brief Loads an image at a specified file path
    /// @param path File path of image to load
    /// @return True on success, false on fail
    bool load_from_file(const char* path) { return false; }
    /// @brief Creates in image from a font string
    /// @param texture_text Text string to render
    /// @param text_color Color of text to render
    /// @return True on success, false on fail
    bool load_from_rendered_text(const char* texture_text, SDL_Color text_color) { return false; }
    /// @brief Dealocates a texture
    void free() {}
    /// @brief Sets the color modulation of a texture
    /// @param red Red aspect of mod
    /// @param green Green aspect of mod
    /// @param blue Blue aspect of mod
    void set_color_mod(uint8_t red, uint8_t green, uint8_t blue) {}
    /// @brief Sets the blending mode of texture
    /// @param blending Blending mode to set
    void set_blend_mode(SDL_BlendMode blending) {}
    /// @brief Set the alpha modulation of a texture
    /// @param alpha Alpha index of mod
    void set_alpha_mod(uint8_t alpha) {}
    /// @brief Renders texture at a given x,y coordinate
    /// @param x Left coordinate for the upper-left corner of texture
    /// @param y Upper coordinate for the upper-left corner of textre
    /// @param clip If provided, defines the portion of the texture to be rendered
    /// @param angle If provided, defines the angle the texture will be rendered at
    /// @param center If provided, defines the center of the texture
    /// @param flip How SDL should flip the image... default is none
    void render(int x, int y, SDL_Rect* clip = nullptr, double angle = 0.0,
                SDL_Point* center = nullptr, SDL_RendererFlip flip = SDL_FLIP_NONE) {}
    int width() { return _width; }
    int height() { return _height; }

private:
    SDL_Texture*    _texture;
    int             _width{ 0 };
    int             _height{ 0 };
};