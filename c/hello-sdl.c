#include <SDL2/SDL.h>

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

#define IMAGE_WIDTH 256
#define IMAGE_HEIGHT 256

int main(int argc, char* argv[])
{
    // SDL Initialization
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
	SDL_Log("SDL_Init Error: %s\n", SDL_GetError());
	return 1;
    }
    
}
