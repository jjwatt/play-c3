import std::io;
import std::thread;
import caca;

struct Square {
    // TODO: Can probably use floats for these
    // TODO: Make a position vector with x,y
    float width, height, x, y;
    float [<2>] velocity;
}

struct World {
    float gravity;
    /* energy loss on collision 1.0 = no loss, 0.0 = total loss */
    float damping;
    /* horizontal speed decay */
    float air_resistance;
}
fn int main() {
    caca::Display *dp;
    caca::Canvas *cv;
    caca::Event ev;
    dp = caca::create_display(null);
    if (!dp) return 1;
    cv = caca::get_canvas(dp);
    int width = caca::get_canvas_width(cv);
    int height = caca::get_canvas_height(cv);
    
    Square square = {
	.width = 20,
	.height = 10,
    };

    World world = {
	.gravity = 0.5f,
	.damping = 0.9f,
	.air_resistance = 0.995f
    };

    // set initial square velocity
    square.velocity = { 3.0f, 0.0f };

    // draw initial square
    caca::set_color_ansi(cv, caca::BLUE, caca::BLACK);
    caca::fill_box(cv, (int)square.x, (int)square.y,
		   (int)square.width, (int)square.height, '#');

    /* main loop */
    while(!caca::get_event(dp, caca::EVENT_KEY_PRESS, ev, 0)) {
	caca::clear_canvas(cv);

	// Apply gravity
	square.velocity.y += world.gravity;
	// Apply air resistance to horizontal movement.
	square.velocity.x *= world.air_resistance;

	// Update position
	square.x += square.velocity.x;
	square.y += square.velocity.y;

	// Handle collisions
	bool right_wall = (square.x >= width - square.width);
	bool left_wall = (square.x <= 0);
	bool wall = (right_wall || left_wall);
	bool floor = (square.y >= height - square.height);
	bool ceiling = (square.y <= 0);

	if (wall) {
	    // Reset x on boundries
	    if (left_wall) {
		square.x = 0;
	    }
	    if (right_wall) {
		square.x = width - square.width;
	    }
	    // Bounce off wall with some energy loss
	    square.velocity.x *= -world.damping;
	    // Change color
            caca::set_color_ansi(cv, caca::rand(1, 16), caca::BLACK);
	}
	if (floor || ceiling) {
	    if (floor) {
		square.y = height - square.height;
		// Only bounce if moving fast enough
		if (square.velocity.y > 0.5f) {
		    square.velocity.y *= -world.damping;
		} else {
		    square.velocity.y = 0;
		    // Ground friction
		    square.velocity.x *= 0.95f;
		}
	    }
	    if (ceiling) {
		// Bounce off the ceiling w/o loss
		square.y = 0;
		square.velocity.y *= -world.damping;
	    }
	    caca::set_color_ansi(cv, caca::rand(1, 16), caca::BLACK);
	}
	// Draw
	caca::fill_box(cv, (int)square.x, (int)square.y,
		      (int)square.width, (int)square.height, '#');
	caca::refresh_display(dp);
	thread::sleep(70000);
    }
    // TODO: use defer
    caca::free_canvas(cv);
    caca::free_display(dp);
    return 0;
}
