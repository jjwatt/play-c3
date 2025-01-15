#include <SDL2/SDL.h>
#include <stdbool.h>

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480


int main(int argc, char* argv[])
{
    // SDL Initialization
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
	SDL_Log("SDL_Init Error: %s\n", SDL_GetError());
	return 1;
    }

    // Window creation and position in the center of the screen
    SDL_Window* window = SDL_CreateWindow("Hello World SDL", SDL_WINDOWPOS_UNDEFINED,
					  SDL_WINDOWPOS_UNDEFINED,
					  WINDOW_WIDTH,
					  WINDOW_HEIGHT,
					  SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    /* SDL_Surface* screenSurface = SDL_GetWindowSurface(window); */
    SDL_Event e;

    bool quit = false;

    SDL_Rect fillRect = { WINDOW_WIDTH / 4, WINDOW_HEIGHT / 4, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2};

    while (!quit) {
	while (SDL_PollEvent(&e) != 0) {
	    if (e.type == SDL_QUIT) {
		quit = true;
	    }
	}
	// Clear screen
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);  // White
	SDL_RenderClear(renderer);

	// Draw red filled square
	SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);  // Red
	SDL_RenderFillRect(renderer, &fillRect);

	// Update screen
	SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
