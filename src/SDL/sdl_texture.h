#pragma once

#include "sdl_common.h"

class sdl_texture
{
public:
    sdl_texture() : _renderer{ nullptr }, _font{ nullptr } {}
    sdl_texture(SDL_Renderer* renderer, TTF_Font* font) : _renderer{ renderer }, _font{ font } {}
    ~sdl_texture() { free(); }
    DISABLE_COPY_AND_MOVE(sdl_texture)
    /// @brief Loads an image at a specified file path
    /// @param path File path of image to load
    /// @return True on success, false on fail
    bool load_from_file(const char* path);
    /// @brief Creates in image from a font string
    /// @param texture_text Text string to render
    /// @param text_color Color of text to render
    /// @return True on success, false on fail
    bool load_from_rendered_text(const char* texture_text, SDL_Color text_color);
    /// @brief Deallocate texture
    void free();
    /// @brief Renders texture at a given x,y coordinate
    /// @param x Left coordinate for the upper-left corner of texture
    /// @param y Upper coordinate for the upper-left corner of texture
    /// @param clip If provided, defines the portion of the texture to be rendered
    /// @param angle If provided, defines the angle the texture will be rendered at
    /// @param center If provided, defines the center of the texture
    /// @param flip How SDL should flip the image... default is none
    void render(u32 x, u32 y, const SDL_Rect* clip = nullptr, double angle = 0.0,
                const SDL_Point* center = nullptr, SDL_RendererFlip flip = SDL_FLIP_NONE) const;
    /// @brief Sets the color modulation of a texture
    /// @param red Red aspect of mod
    /// @param green Green aspect of mod
    /// @param blue Blue aspect of mod
    void color_mod(const u8 red, const u8 green, const u8 blue) const { SDL_SetTextureColorMod(_texture, red, green, blue); }
    /// @brief Sets the blending mode of texture
    /// @param blending Blending mode to set
    void blend_mode(const SDL_BlendMode blending) const { SDL_SetTextureBlendMode(_texture, blending); }
    /// @brief Set the alpha modulation of a texture
    /// @param alpha Alpha index of mod
    void alpha_mod(const u8 alpha) const { SDL_SetTextureAlphaMod(_texture, alpha); }
    [[nodiscard]] u32 width() const { return _width; }
    [[nodiscard]] u32 height() const { return _height; }
    void renderer(SDL_Renderer* renderer) { _renderer = renderer; }
    void font(TTF_Font* font) { _font = font; }

private:
    SDL_Renderer*   _renderer;
    TTF_Font*       _font;
    SDL_Texture*    _texture{ nullptr };
    u32             _width{ 0 };
    u32             _height{ 0 };
};