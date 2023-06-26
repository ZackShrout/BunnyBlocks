If you do not have SDL on your system, or are missing some of the SDL2 headers:

For Red Hat users, use:
 - sudo dnf install SDL2-devel SDL2_image-devel SDL2_ttf-devel SDL2_mixer-devel

For Debian users, use:
 - sudo apt-get install libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev libsdl2-mixer-dev

For MacOS users, use:
 - brew install sdl2 sdl2_image sdl2_ttf sdl2_mixer

For Windows user (assuming Visual Studio):
 - Download SDL2 headers and binaries (SDL2-devel-x.y.z-VC.zip) from https://github.com/libsdl-org/SDL/releases
 - Download SDL2 image extension (SDL2_image-devel-x.y.z-VC.zip) from https://github.com/libsdl-org/SDL_image/releases
 - Download SDL2 TTF extension (SDL2_ttf-devel-x.y.x-VC.zip) from https://github.com/libsdl-org/SDL_ttf/releases
 - Make sure to put the "include" folder for each of the three downloaded folders in additional includes
 - Make sure to put the "lib\x64" folder for each of the three downloaded folders in Additional Library Directories
 - Make sure to put SDL2.lib, SDL2main.lib, SDL_image.lib, and SDL_ttf.lib in Additional Dependencies
 - Make sure to set environment variable path to "lib/x64" for each of the three downloaded folders
 - Once environment variables are set, restart and open solution back up