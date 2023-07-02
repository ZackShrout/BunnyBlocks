#include "sdl_core.h"

#include <iostream>
#include <string>

#include "sdl_audio.h"
#include "sdl_texture.h"

namespace bblocks::sdl::core
{
    namespace
    {
        constexpr s32       joystick_dead_zone{ 8000 };
    	constexpr u32       desired_screen_w = 580;
        constexpr u32       desired_screen_h = 420;
        
        SDL_Window*         window_{};
        SDL_Renderer*       renderer_{};
        SDL_GameController* controller_{};

        u32                 display_width_{ 0 };
        u32                 display_height_{ 0 };
        u32                 max_display_width_{ 0 };
        u32                 max_display_height_{ 0 };

        s32                 current_points_{ -1 };
        s32                 current_level_{ -1 };

        TTF_Font*           hud_font_{ nullptr };
        TTF_Font*           heading_font_{ nullptr };
        sdl_texture         level_text_;
        sdl_texture         score_text_;
        sdl_texture         paused_text_;
        sdl_texture         game_over_text_;
        sdl_texture         new_game_text_;
        sdl_texture         exit_text_;
        sdl_texture         select_arrow_;

        u32                 colors_[count]
                            {
                                0x0000'00ff,    // black
                                0xff00'00ff,    // red
                                0x8000'00ff,    // dark red
                                0xff7f'7fff,    // light red
                                0x00ff'00ff,    // green
                                0x0230'20ff,    // dark green
                                0x98fb'98ff,    // light green
                                0x0000'ffff,    // blue
                                0x1919'70ff,    // dark blue
                                0x7393'b3ff,    // light blue
                                0xda70'd6ff,    // purple
                                0x7f00'ffff,    // dark purple
                                0xd8bf'd8ff,    // light purple
                                0xff75'18ff,    // orange
                                0xc040'00ff,    // dark orange
                                0xffd5'80ff,    // light orange
                                0xfafa'33ff,    // yellow
                                0xffc0'00ff,    // dark yellow
                                0xffff'8fff,    // light yellow
                                0xf883'79ff,    // pink
                                0xaa33'6aff,    // dark pink
                                0xfff5'6aff,    // light pink
                                0x00ff'ffff,    // cyan
                                0xff00'ffff,    // magenta
                                0x2020'20ff,    // light grey
                                0x5900'00ff,    // brick
                                0x3300'00ff,    // dark brick
                                0x8c19'19ff,    // light brick
                                0xffff'ffff,    // white
                                0x0000'007f     // fade
                            };
    } // anonymous

    bool
    init()
    {
        SDL_DisplayMode display_mode;

	    // Initialize SDL
	    if (SDL_Init(SDL_INIT_EVERYTHING) != 0 )
        {
            std::cerr << "Couldn't initialize SDL... " << SDL_GetError() << std::endl;
	    	return false;
	    }

        SDL_GetCurrentDisplayMode(0, &display_mode);
        max_display_width_ = display_mode.w;
        max_display_height_ = display_mode.h;
        display_width_ = (max_display_width_ >= desired_screen_w ? desired_screen_w : max_display_width_);
        display_height_ = (max_display_height_ >= desired_screen_h ? desired_screen_h : max_display_height_);

        window_ = SDL_CreateWindow(
	        "Bunny Blocks",
	        SDL_WINDOWPOS_CENTERED,
	        SDL_WINDOWPOS_CENTERED,
	        static_cast<int>(display_width_),
	        static_cast<int>(display_height_),
	        SDL_WINDOW_SHOWN);

        if (!window_)
        {
            std::cerr << "Error creating SDL window... " << SDL_GetError() << std::endl;
            return false;
        }

        renderer_ = SDL_CreateRenderer(window_, -1, 0);
        if (!renderer_)
        {
            std::cerr << "Error creating SDL renderer... " << SDL_GetError() << std::endl;
            return false;
        }

        if (SDL_NumJoysticks() > 0)
        {
            controller_ = SDL_GameControllerOpen(0);
            // NOTE: Even if the game controller can't be opened, we still allow for keyboard input,
            //       so this is not a fatal error.
            if (!controller_)
                std::cerr << "Error opening game controller... " << SDL_GetError() << std::endl;
        }

        SDL_SetRenderDrawBlendMode(renderer_, SDL_BLENDMODE_BLEND);

        if(constexpr u32 img_flags = IMG_INIT_PNG; !(IMG_Init(img_flags) & img_flags))
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

        level_text_.renderer(renderer_);
        score_text_.renderer(renderer_);
        paused_text_.renderer(renderer_);
        game_over_text_.renderer(renderer_);
        new_game_text_.renderer(renderer_);
        exit_text_.renderer(renderer_);
        level_text_.font(hud_font_);
        score_text_.font(hud_font_);
        paused_text_.font(heading_font_);
        game_over_text_.font(heading_font_);
        new_game_text_.font(hud_font_);
        exit_text_.font(hud_font_);
        select_arrow_.renderer(renderer_);

        if(constexpr SDL_Color text_color = { 0xff, 0xff, 0xff, 0xff }; !paused_text_.load_from_rendered_text("Paused", text_color) ||
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

        if (!audio::init()) return false;

        return true;
    }

    void
    shutdown()
    {
        audio::shutdown();
        
        select_arrow_.free();
        level_text_.free();
	    score_text_.free();
        paused_text_.free();
        game_over_text_.free();
        new_game_text_.free();
        exit_text_.free();

	    TTF_CloseFont(hud_font_);
        TTF_CloseFont(heading_font_);
        hud_font_ = nullptr;
	    heading_font_ = nullptr;

        SDL_GameControllerClose(controller_);
	    SDL_DestroyRenderer(renderer_);
	    SDL_DestroyWindow(window_);
        controller_ = nullptr;
	    window_ = nullptr;
	    renderer_ = nullptr;

	    IMG_Quit();
	    TTF_Quit();
	    SDL_Quit();
    }

    void
    draw_rectangle(const glm::uvec2& top_left, const glm::uvec2& bottom_right, const color c, const bool filled/* = true*/)
    {
        SDL_Rect rect;
        rect.x = static_cast<int>(top_left.x);
        rect.y = static_cast<int>(top_left.y);
        rect.w = static_cast<int>(bottom_right.x - top_left.x);
        rect.h = static_cast<int>(bottom_right.y - top_left.y);

        const u8 r = (colors_[c] >> 24) & 0xff;
        const u8 g = (colors_[c] >> 16) & 0xff;
        const u8 b = (colors_[c] >> 8) & 0xff;
        const u8 a = colors_[c] & 0xff;

        SDL_SetRenderDrawColor(renderer_, r, g, b, a);

        if (filled)
            SDL_RenderFillRect(renderer_, &rect);
        else
            SDL_RenderDrawRect(renderer_, &rect);
    }

    void
    clear_screen()
    {
        SDL_SetRenderDrawColor(renderer_, 0x00, 0x00, 0x00, 0xff);
        SDL_RenderClear(renderer_);
    }
    
    u32
    display_width()
    {
        return display_width_;
    }

    u32
    display_height()
    { 
        return display_height_;
    }

    s32
    poll_key()
    {
        SDL_Event event;

        while (SDL_PollEvent(&event))
        {
        	switch (event.type)
	        {
	        case SDL_KEYDOWN:
		        return event.key.keysym.sym;
	        case SDL_QUIT:
		        return SDLK_ESCAPE;
	        case SDL_CONTROLLERBUTTONDOWN:
                if (event.cbutton.which == 0) // Is this coming from controller_?
                {
                    if (event.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_LEFT)
                        return key_left;
                    if (event.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_RIGHT)
                        return key_right;
                    if (event.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_UP)
                        return key_up;
                    if (event.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_DOWN)
                        return key_down;
                    if (event.cbutton.button == SDL_CONTROLLER_BUTTON_START)
                        return key_enter;
                    if (event.cbutton.button == SDL_CONTROLLER_BUTTON_B)
                        return key_space;
                    if (event.cbutton.button == SDL_CONTROLLER_BUTTON_A)
                        return key_down;
                    if (event.cbutton.button == SDL_CONTROLLER_BUTTON_X)
                        return key_up;
                    if (event.cbutton.button == SDL_CONTROLLER_BUTTON_Y)
                        std::cerr << "Stop pressing me, you are wasting your time!" << std::endl;
                }
                break;

	        case SDL_JOYAXISMOTION:
		        if (event.jaxis.which == 0) // Do we have motion on game_controller_?
		        {
			        if (event.jaxis.axis == 0) // X axis motion?
			        {
				        if (event.jaxis.value < -joystick_dead_zone)
					        return key_left;
				        if (event.jaxis.value > joystick_dead_zone)
					        return key_right;
			        }
			        else if (event.jaxis.axis == 1) // Y axis motion?
			        {
				        if (event.jaxis.value < -joystick_dead_zone)
					        return key_up;
				        if (event.jaxis.value > joystick_dead_zone)
					        return key_down;
			        }
		        }
		        break;
            default: break;
	        }
        }

	    return -1;
    }

    void
    render()
    {
        SDL_RenderPresent(renderer_);
    }

    void
    draw_hud(const u32 points, const u32 level)
    {
        if (current_points_ == -1 || current_points_ != static_cast<s32>(points))
        {
	        constexpr SDL_Color text_color = { 0xff, 0xff, 0xff, 0xff };
            std::string label{ std::to_string(points) };

	        if (const u32 digits{ static_cast<u32>(label.size()) }; digits != 9)
            {
                std::string prepend{ "0" };
                
                // NOTE: prepend already has one 0 in it, so we only need to fill (8 - digits)
                for (u32 i{ 0 }; i < 8 - digits; ++i)
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
    
            current_points_ = static_cast<s32>(points);
        }
    
        if (current_level_ == -1 || current_level_ != static_cast<s32>(level))
        {
	        constexpr SDL_Color text_color = { 0xff, 0xff, 0xff, 0xff };
            std::string label{ "Level: "};
            label = label + std::to_string(level);
    
            if(!level_text_.load_from_rendered_text(label.c_str(), text_color))
            {
                std::cerr << "Failed to render text texture..." << std::endl;
                return;
            }
    
            current_level_ = static_cast<s32>(level);
        }
    
        level_text_.render(60, 100);
        score_text_.render(60, 200);
    }

    void
    draw_pause()
    {
	    const u32 x{ display_width_ / 2 - paused_text_.width() / 2 };
	    const u32 y{ display_height_ / 2 - paused_text_.height() / 2 };

	    draw_rectangle({ x - 30, y - 20 }, { x + paused_text_.width() + 30, y + paused_text_.height() + 20 }, white,
	                   false);
        paused_text_.render(x, y);
    }

    void
    game_over(const u32 arrow_pos)
    {
	    u32 x{ display_width_ / 2 - game_over_text_.width() / 2 };
	    const u32 game_over_y{
		    static_cast<u32>(static_cast<f32>(display_height_) / 3.f) - static_cast<u32>(static_cast<f32>(
			    game_over_text_.height()) / 3.f)
	    };
	    const u32 new_game_y{ game_over_y + game_over_text_.height() + 10 };
	    const u32 exit_y{ new_game_y + new_game_text_.height() + 10 };
	    const u32 arrow_offset_y{ ((new_game_text_.height() - select_arrow_.height()) / 2) };

	    draw_rectangle({ x - 30, game_over_y - 20 },
	                   { x + game_over_text_.width() + 30, exit_y + exit_text_.height() + 20 },
	                   black);
	    draw_rectangle({ x - 30, game_over_y - 20 },
	                   { x + game_over_text_.width() + 30, exit_y + exit_text_.height() + 20 },
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

	    x = display_width_ / 2 - new_game_text_.width() / 2;
	    new_game_text_.render(x, new_game_y);

	    x = display_width_ / 2 - exit_text_.width() / 2;
	    exit_text_.render(x, exit_y);
    }
}
