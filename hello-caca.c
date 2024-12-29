#include <caca.h>

struct Square {
    int w;
    int h;
    int x;
    int y;
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
    int dx = 1;
    int dy = 1;
    caca_set_color_ansi(cv, CACA_BLUE, CACA_BLACK);
    caca_fill_box(cv, square.x, square.y, square.w, square.h, '#');
    while (!caca_get_event(dp, CACA_EVENT_KEY_PRESS, &ev, 0)) {
	caca_clear_canvas(cv);
	square.x += dx;
	square.y += dy;
	if (square.x >= w - square.w || square.x <= 0) {
	    caca_set_color_ansi(cv, caca_rand(1, 16), CACA_BLACK);
	    dx *= -1;
	}
	if (square.y >= h - square.h || square.y <= 0) {
	    caca_set_color_ansi(cv, caca_rand(1, 16), CACA_BLACK);
	    dy *= -1;
	}
	caca_fill_box(cv, square.x, square.y, square.w, square.h, '#');
	caca_refresh_display(dp);
	usleep(80000);
    }
    caca_free_display(dp);
    return 0;
}
