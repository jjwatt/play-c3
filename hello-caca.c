#include <stdio.h>
#include <caca.h>

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
    int square_w = 20;
    int square_h = 10;
    int square_x = (w / 2) - 50;
    int square_y = (h / 2) - 10;
    int dx = 1;
    int dy = 1;
    caca_set_color_ansi(cv, CACA_BLUE, CACA_BLACK);
    caca_fill_box(cv, square_x, square_y, square_w, square_h, '#');

    while (!caca_get_event(dp, CACA_EVENT_KEY_PRESS, NULL, 0)) {
	caca_clear_canvas(cv);
	/* caca_set_color_ansi(cv, CACA_BLUE, CACA_BLACK); */

	square_x += dx;
	square_y += dy;

	if (square_x >= w - square_w || square_x <= 0) {
	    caca_set_color_ansi(cv, caca_rand(1, 16), CACA_BLACK);
	    dx *= -1;
	}
	if (square_y >= h - square_h || square_y <= 0) {
	    caca_set_color_ansi(cv, caca_rand(1, 16), CACA_BLACK);
	    dy *= -1;
	}
	caca_fill_box(cv, square_x, square_y, square_w, square_h, '#');

	caca_refresh_display(dp);
	usleep(80000);
    }
    caca_free_display(dp);
    return 0;
}
