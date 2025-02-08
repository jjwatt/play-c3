#include <iostream>

#include "SDL2/SDL.h"

struct Vec2 {
    double x = 0.0;
    double y = 0.0;

    Vec2() = default;
    Vec2(double x_val, double y_val) : x(x_val), y(y_val) {}
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
	   Vec2 vel = Vec2()) : width(w), height(h), x(x_pos),
				y(y_pos), velocity(vel) {}
    Square(double w, double h) : width(w), height(h) {}
    Square() = default;
};

struct World {
    double gravity = 0.5;
    double damping = 0.9;
    double air_resistance = 0.995;
    World(double g, double d, double ar) : gravity(g),
					   damping(d),
					   air_resistance(ar) {}
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
    
    Square square1(20.0, 20.0);

    // Initial square velocity
    square1.velocity = Vec2(3.0, 0.0);

    // Initial square position
    square1.x = start_x;
    square1.y = start_y;

    World world{};

    while (!quit) {
	while (SDL_PollEvent(&e) != 0) {
	    if (e.type == SDL_QUIT) {
		quit = true;
	    }
	}
	// Set background color to white
	SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
	SDL_RenderClear(renderer);

	// Apply gravity
	square1.velocity.y += world.gravity;
	// Apply air resistance to horizontal movement
	square1.velocity.x *= world.air_resistance;

	// Update position
	square.x += square.velocity.x;
	square.y += square.velocity.y;
    }
}
