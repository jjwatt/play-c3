#include <iostream>
#include <random>

#include "SDL2/SDL.h"

struct Vec2 {
    double x = 0.0;
    double y = 0.0;

    Vec2() = default;
    Vec2(double x_val, double y_val)
	: x(x_val)
	, y(y_val) {}
    Vec2 operator+(const Vec2& other) const {
	return Vec2(x + other.x, y + other.y);
    }
    // TODO: Add more methods
};

struct Square {
    double width = 10.0;
    double height = 10.0;
    double x = 0.0;
    double y = 0.0;
    Vec2 velocity;

    Square(double w, double h, double x_pos, double y_pos,
	   Vec2 vel = Vec2())
	: width(w)
	, height(h)
	, x(x_pos)
	, y(y_pos)
	, velocity(vel) {}
    Square() = default;
    Square(double w, double h)
	: width(w)
	, height(h) {}
};

struct World {
    double gravity = 0.5;
    double damping = 0.9;
    double air_resistance = 0.995;
    World(double g, double d, double ar)
	: gravity(g), damping(d), air_resistance(ar) {}
    World() = default;
};

int main(void)
{
    constexpr int screen_width {640};
    constexpr int screen_height {480};

    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
	SDL_Log("SDL_Init Error: %s\n", SDL_GetError());
	return 1;
    }

    window = SDL_CreateWindow("Gravity Square SDL C++",
			      SDL_WINDOWPOS_UNDEFINED,
			      SDL_WINDOWPOS_UNDEFINED,
			      screen_width,
			      screen_height,
			      SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Create an event handler and a quit flag
    SDL_Event e{};
    bool quit {false};

    double start_x = screen_width / 2;
    double start_y = screen_height / 2;

    // TODO: Array of squares
    Square square(100.0, 100.0);

    // Initial square velocity
    square.velocity = Vec2(3.0, 0.0);

    // Initial square position
    // TODO: randomize position
    square.x = start_x;
    square.y = start_y;

    World world{};

    // Setup random number generator for colors
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, 255);

    // Setup first square color
    Uint8 Red = distrib(gen);
    Uint8 Green = distrib(gen);
    Uint8 Blue = distrib(gen);

    while (!quit) {
	while (SDL_PollEvent(&e) != 0) {
	    if (e.type == SDL_QUIT) {
		quit = true;
	    }
	}
	// Set background color to white
	SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
	SDL_RenderClear(renderer);

	// Set square color
	SDL_SetRenderDrawColor(renderer, Red, Green, Blue, 0xff);

	// Apply gravity
	square.velocity.y += world.gravity;
	// Apply air resistance to horizontal movement
	square.velocity.x *= world.air_resistance;

	// Update position
	square.x += square.velocity.x;
	square.y += square.velocity.y;

	// Handle collisions
	bool on_right_wall = (square.x >= screen_width - square.width);
	bool on_left_wall = (square.x <= 0);
	bool on_wall = (on_right_wall || on_left_wall);
	bool on_floor = (square.y >= screen_height - square.height);
	bool on_ceiling = (square.y <= 0);
	
	if (on_wall) {
	    // Reset x on boundries
	    if (on_left_wall) {
		square.x = 0;
	    }
	    if (on_right_wall) {
		square.x = screen_width - square.width;
	    }
	    // Bounce off wall with some energy loss
	    square.velocity.x *= -world.damping;
	    // Change to random color
	    Red = distrib(gen);
	    Green = distrib(gen);
	    Blue = distrib(gen);
	}
	
	if (on_floor) {
	    square.y = screen_height - square.height;
	    // Only bounce if moving fast enough
	    if (square.velocity.y > 0.5) {
		square.velocity.y *= -world.damping;
		// Change to random color
		Red = distrib(gen);
		Green = distrib(gen);
		Blue = distrib(gen);

	    } else {
		square.velocity.y = 0;
		// Ground friction
		square.velocity.x *= 0.95;
	    }
	}
	if (on_ceiling) {
	    // Bounce off the ceiling w/o loss
	    square.y = 0;
	    square.velocity.y *= -world.damping;
	    SDL_SetRenderDrawColor(renderer,
				   distrib(gen),
				   distrib(gen),
				   distrib(gen),
				   255);
	}
	// Draw
	SDL_Rect rect = { .x = static_cast<int>(square.x),
			  .y = static_cast<int>(square.y),
			  .w = static_cast<int>(square.width),
			  .h = static_cast<int>(square.height) };
	SDL_RenderFillRect(renderer, &rect);

	// Update the screen
	SDL_RenderPresent(renderer);
	SDL_Delay(20);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
