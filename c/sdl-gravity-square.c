#include <SDL2/SDL.h>
#include <stdbool.h>
#include <unistd.h>

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

struct Square {
    int width;
    int height;
    int x;
    int y;
};

int main(int argc, char* argv[])
{
    // SDL Initialization
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
	SDL_Log("SDL_Init Error: %s\n", SDL_GetError());
	return 1;
    }

    struct Square square = { 100, 100, (WINDOW_WIDTH / 2) - 50, (WINDOW_HEIGHT / 2) - 10 };

    // Velocity
    float dx = 4.0f;
    float dy = 0.0f;

    // Physics
    const float gravity = 0.5f;

    // energy loss on collision; 1.0 = no loss, 0.0 = total loss
    const float damping = 0.8f;
    // horizontal speed decay
    const float air_resistance = 0.995f;

    // Window creation and position in the center of the screen
    SDL_Window* window = SDL_CreateWindow("Gravity Square SDL", SDL_WINDOWPOS_UNDEFINED,
					  SDL_WINDOWPOS_UNDEFINED,
					  WINDOW_WIDTH,
					  WINDOW_HEIGHT,
					  SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    /* SDL_Surface* screenSurface = SDL_GetWindowSurface(window); */
    SDL_Event e;

    bool quit = false;

    while (!quit) {
	while (SDL_PollEvent(&e) != 0) {
	    if (e.type == SDL_QUIT) {
		quit = true;
	    }
	}
	// Clear screen
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);  // White
	SDL_RenderClear(renderer);

	// Apply gravity
	dy += gravity;

	// Apply air resistance to horizontal movement.
	dx *= air_resistance;

	// Update position.
	square.x += dx;
	square.y += dy;

	// Handle collisions with bounds
	if (square.x >= WINDOW_WIDTH - square.width || square.x <= 0) {
	    // Bounce off wall with some energy loss.
	    if (square.x >= WINDOW_WIDTH - square.width) {
		square.x = WINDOW_WIDTH - square.width;
	    }
	    if (square.x <= 0) {
		square.x = 0;
	    }
	    dx *= -damping;
	    // TODO: change to random color here
	}

	if (square.y >= WINDOW_HEIGHT - square.height || square.y <= 0) {
	    // Bounce off floor/ceiling with some energy loss.
	    if (square.y >= WINDOW_HEIGHT - square.height) {
		square.y = WINDOW_HEIGHT - square.height;
		// Only bounce if moving fast enough.
		if (dy >= 0.5f) {
		    dy *= -damping;
		} else {
		    // Stop bouncing when too slow.
		    dy = 0;
		    // Additional friction on the ground.
		    dx *= 0.95f;
		}
	    }
	    if (square.y <= 0) {
	      square.y = 0;
	      dy *= -damping;
	    }
	    // TODO: set to random color here
	}

	/* fillRect = { WINDOW_WIDTH / 4, */
	/* 	     WINDOW_HEIGHT / 4, */
	/* 	     WINDOW_WIDTH / 2, */
	/* 	     WINDOW_HEIGHT / 2}; */
	SDL_Rect fillRect = { square.x, square.y, square.width, square.height };

	// Draw red filled square
	SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);  // Red
	SDL_RenderFillRect(renderer, &fillRect);

	// Update screen
	SDL_RenderPresent(renderer);
	usleep(50000);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
