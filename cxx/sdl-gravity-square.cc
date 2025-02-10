#include <iostream>
#include <random>

#include "SDL2/SDL.h"

struct Vec2 {
    double x {0.0};
    double y {0.0};

    Vec2() = default;
    Vec2(double x_val, double y_val)
	: x {x_val}
	, y {y_val} {}
    Vec2 operator+(const Vec2& other) const {
	return Vec2(x + other.x, y + other.y);
    }
    // TODO: Add more methods
};

struct Color {
    // Default to white
    Uint8 red {0xff};
    Uint8 green {0xff};
    Uint8 blue {0xff};
    Uint8 alpha {0xff};
    Color() = default;
    Color(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
	: red {r}
	, green {g}
	, blue {b}
	, alpha(a) {}
    Color(Uint8 r, Uint8 g, Uint8 b)
	: Color(r, g, b, 0xff) {}

};

// TODO: Add set_color function that takes a Color
// TODO: Add set_random_color function to this class
class Square
{
public:
    // TODO: Make width and height a Vec2, dimensions
    double width {10.0};
    double height {10.0};
    // TODO: Make x and y a Vec2, position
    double x {0.0};
    double y {0.0};
    Vec2 position {};
    Vec2 velocity {};
    Color color {};

    Square() = default;
    Square(double w, double h, double x_pos, double y_pos, Vec2 v)
	: width {w}
	, height {h}
	, x {x_pos}
	, y {y_pos}
	, velocity {v} {}
    Square(double w, double h)
	: width {w}
	, height {h} {}
};

class Square2
{
private:
    Vec2 m_size {10.0, 10.0};
    Vec2 m_position {0.0, 0.0};
    Vec2 m_velocity {};
    Color m_color {};

public:
    Square2() = default;
    Square2(Vec2 size, Vec2 pos, Vec2 v)
	: m_size {size}
	, m_position {pos}
	, m_velocity {v} {}
    Square2(Vec2 size)
	: m_size {size} {}
    Vec2 size() const { return m_size; }
    void setSize(Vec2 size) { m_size = size; }
    Vec2 position() const { return m_position; }
    void setPos(Vec2 pos) { m_position = pos; }
    Vec2 velocity() const { return m_velocity; }
    void setVelocity(Vec2 velocity) { m_velocity = velocity; }
    Color color() const { return m_color; }
    void setColor(Color color) { m_color = color; }
};

struct World {
    double gravity {0.5};
    double damping {0.9};
    double air_resistance {0.995};
    World() = default;
    World(double g, double d, double ar)
	: gravity {g}, damping {d}, air_resistance {ar} {}
};

void set_random_color(Color& color)
{
    // Setup random number generator for colors
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> distrib(0, 255);

    color.red = distrib(gen);
    color.green = distrib(gen);
    color.blue = distrib(gen);
}

Color get_random_color(void)
{
    // Setup random number generator for colors
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> distrib(0, 255);

    Color color {};
    color.red = distrib(gen);
    color.green = distrib(gen);
    color.blue = distrib(gen);
    return color;
}

int main(void)
{
    constexpr int screen_width {640};
    constexpr int screen_height {480};

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
	SDL_Log("SDL_Init Error: %s\n", SDL_GetError());
	return 1;
    }

    // SDL_Window *window = nullptr;
    // SDL_Renderer *renderer = nullptr;
    auto window = SDL_CreateWindow("Gravity Square SDL C++",
			      SDL_WINDOWPOS_UNDEFINED,
			      SDL_WINDOWPOS_UNDEFINED,
			      screen_width,
			      screen_height,
			      SDL_WINDOW_SHOWN);
    auto renderer = SDL_CreateRenderer(window,
				       -1,
				       SDL_RENDERER_ACCELERATED);

    // Create an event handler and a quit flag
    SDL_Event e{};
    bool quit {false};

    // TODO: Array of squares
    // TODO: Random velocity for each square
    Square square(100.0,
		  100.0,
		  screen_width / 2,
		  screen_height / 2,
		  Vec2 {3.0, 0.0});

    World world{};

    // TODO: Put color in the square
    // Setup first square color
    Color square_color {};
    set_random_color(square_color);

    // Set background color
    Color bg_color {};

    // Main loop
    while (!quit) {
	while (SDL_PollEvent(&e) != 0) {
	    if (e.type == SDL_QUIT) {
		quit = true;
	    }
	}
	// Draw background
	SDL_SetRenderDrawColor(renderer,
			       bg_color.red,
			       bg_color.green,
			       bg_color.blue,
			       bg_color.alpha);
	SDL_RenderClear(renderer);

	// Set square color
	SDL_SetRenderDrawColor(renderer,
			       square_color.red,
			       square_color.green,
			       square_color.blue,
			       square_color.alpha);

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
	    set_random_color(square_color);
	}
	
	if (on_floor) {
	    square.y = screen_height - square.height;
	    // Only bounce if moving fast enough
	    if (square.velocity.y > 0.5) {
		square.velocity.y *= -world.damping;
		// Change to random color
		set_random_color(square_color);
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
	    // Change to random color
	    set_random_color(square_color);
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
