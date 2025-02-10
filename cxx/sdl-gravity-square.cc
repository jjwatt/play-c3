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

class OldSquare
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

    OldSquare() = default;
    OldSquare(double w, double h, double x_pos, double y_pos, Vec2 v)
	: width {w}
	, height {h}
	, x {x_pos}
	, y {y_pos}
	, velocity {v} {}
    OldSquare(double w, double h)
	: width {w}
	, height {h} {}
};

class Square
{
private:
    Vec2 m_size {10.0, 10.0};
    Vec2 m_position {0.0, 0.0};
    Vec2 m_velocity {};
    Color m_color {};

public:
    Square() = default;
    Square(Vec2 size, Vec2 pos, Vec2 v)
	: m_size {size}
	, m_position {pos}
	, m_velocity {v} {}
    Square(Vec2 size)
	: m_size {size} {}
    Vec2 size() const { return m_size; }
    void setSize(Vec2 size) { m_size = size; }
    Vec2 position() const { return m_position; }
    void setPos(Vec2 pos) { m_position = pos; }
    void setPosX(double x) { m_position.x = x; }
    void setPosY(double y) { m_position.y = y; }
    Vec2 velocity() const { return m_velocity; }
    void setVelocity(Vec2 velocity) { m_velocity = velocity; }
    Color color() const { return m_color; }
    void setColor(Color color) { m_color = color; }

    void applyGravity(double gravity)
    {
	m_velocity.y += gravity;
    }

    void applyAirResistance(double air_resistance)
    {
	m_velocity.x *= air_resistance;
    }

    void dampX(double damping) {
	m_velocity.x *= -damping;
    }

    void dampY(double damping) {
	m_velocity.y *= -damping;
    }

    void updatePosition()
    {
	m_position.x += m_velocity.x;
	m_position.y += m_velocity.y;
    }
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

void set_color(SDL_Renderer* renderer, Color color)
{
    SDL_SetRenderDrawColor(renderer,
			   color.red,
			   color.green,
			   color.blue,
			   color.alpha);
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
    Square square({100.0, 100.0},
		  {screen_width / 2, screen_height / 2},
		  {3.0, 0.0});
    World world{};

    // Setup first square color
    square.setColor(get_random_color());

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
	set_color(renderer, bg_color);
	SDL_RenderClear(renderer);

	// Set renderer color for painting square
	set_color(renderer, square.color());

	// Apply gravity
	square.applyGravity(world.gravity);
	// Apply air resistance to horizontal movement
	square.applyAirResistance(world.air_resistance);

	// Update position
	square.updatePosition();

	// Handle collisions
	bool is_on_right_wall = (square.position().x >= screen_width - square.size().x);
	bool is_on_left_wall = (square.position().x <= 0);
	bool is_on_wall = (is_on_right_wall || is_on_left_wall);
	bool is_on_floor = (square.position().y >= screen_height - square.size().y);
	bool is_on_ceiling = (square.position().y <= 0);

	if (is_on_wall) {
	    // Reset x on boundries
	    if (is_on_left_wall) {
		square.setPosX(0);
	    }
	    if (is_on_right_wall) {
		square.setPosX(screen_width - square.size().x);
	    }
	    // Bounce off wall with some energy loss
	    square.dampX(world.damping);
	    // Change to random color
	    square.setColor(get_random_color());
	}

	if (is_on_floor) {
	    square.setPosY(screen_height - square.size().y);
	    // Only bounce if moving fast enough
	    if (square.velocity().y > 0.5) {
		square.dampY(world.damping);
		// Change to random color
		square.setColor(get_random_color());
	    } else {
		// Ground friction
		square.setVelocity({square.velocity().x * 0.95, 0});
	    }
	}
	if (is_on_ceiling) {
	    // Bounce off the ceiling w/o loss
	    square.setPosY(0);
	    square.dampY(world.damping);
	    // Change to random color
	    square.setColor(get_random_color());
	}
	// Draw
	SDL_Rect rect = { .x = static_cast<int>(square.position().x),
			  .y = static_cast<int>(square.position().y),
			  .w = static_cast<int>(square.size().x),
			  .h = static_cast<int>(square.size().y) };
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
