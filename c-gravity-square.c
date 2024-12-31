#include <caca.h>
#include <unistd.h>

struct Square {
  int w, h, x, y;
};

int main(void)
{
    caca_canvas_t *cv;
    caca_display_t *dp;
    caca_event_t ev;
    dp = caca_create_display(NULL);
    if (!dp) return 1;
    cv = caca_get_canvas(dp);
    int w = caca_get_canvas_width(cv);
    int h = caca_get_canvas_height(cv);

    struct Square square = { 20, 10, (w / 2) - 50, (h / 2) - 10 };

    /* Velocity */
    float dx = 3.0f;
    float dy = 0.0f;

    /* Physics */
    const float gravity = 0.3f;
    /* energy loss on collision 1.0 = 0 loss, 0.0 = total loss */
    const float damping = 0.6f;
    /* horizonatal speed decay */
    const float air_resistance = 0.99f;

    caca_set_color_ansi(cv, CACA_BLUE, CACA_BLACK);
    caca_fill_box(cv, square.x, square.y, square.w, square.h, '#');
    while (!caca_get_event(dp, CACA_EVENT_KEY_PRESS, NULL, 0)) {
	caca_clear_canvas(cv);
	// Apply gravity.
	dy += gravity;

	// Apply air resistance to horizontal movement.
	dx *= air_resistance;

	// Update position.
	square.x += dx;
	square.y += dy;

	// Handle collisions with bounds
	if (square.x >= w - square.w || square.x <= 0) {
	    // Bounce off wall with some energy loss.
	    if (square.x >= w - square.w) {
		square.x = w - square.w;
	    }
	    if (square.x <= 0) {
		square.x = 0;
	    }
	    dx += -damping;
	    caca_set_color_ansi(cv, caca_rand(1, 16), CACA_BLACK);
	}
	if (square.y >= h - square.h || square.y <= 0) {
	    // Bounce off floor/ceiling with some energy loss.
	    if (square.y >= h - square.h) {
		square.y = h - square.h;
		// Only bounce if moving fast enough.
		if (dy > 0.5f) {
		    dy *= -damping;
		} else {
		    // Stop bouncing when too slow.
		    dy = 0;
		    // Additional friction on the ground.
		    dx *= 0.95f;
		}
		if (square.y <= 0) {
		    square.y = 0;
		    dy *= -damping;
		}
	    }
	    caca_set_color_ansi(cv, caca_rand(1, 16), CACA_BLACK);
	}
	caca_fill_box(cv, (int)square.x, (int)square.y,
		      (int)square.w, (int)square.h, '#');
	caca_refresh_display(dp);
	usleep(30000);
    }
    caca_free_canvas(cv);
    caca_free_display(dp);
    return 0;
}
