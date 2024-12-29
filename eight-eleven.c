#include <caca.h>
#include <math.h>

/* TODO: Put these in a header file */
double norm(double value, double low, double high) {
    return (value - low) / (high - low);
}

double lerp(double low, double high, double amount) {
    return low + amount * (high - low);
}

double mapvalue(double value, double low1, double high1, double low2, double high2) {
    double n = norm(value, low1, high1);
    return lerp(low2, high2, n);
}

void draw_eight_eleven(double width, double height, double power, caca_canvas_t *cv) {
    for (int x = 2; x < width; x += 2) {
	double n = mapvalue((double)x, 2, width, -1, 1);
	double p = pow(n, power);
	double ypos = lerp(10, height, p);
	caca_draw_line(cv, x, 0, x, ypos, '#');
    }    
}

int main(void)
{
    caca_canvas_t *cv;
    caca_display_t *dp;
    caca_event_t ev;
    dp = caca_create_display(NULL);
    if (!dp) return 1;
    cv = caca_get_canvas(dp);
    int width = caca_get_canvas_width(cv);
    int height = caca_get_canvas_height(cv);
    caca_set_color_ansi(cv, CACA_BLUE, CACA_BLACK);
    draw_eight_eleven(width, height, 4, cv);
    caca_refresh_display(dp);
    caca_get_event(dp, CACA_EVENT_KEY_PRESS, &ev, -1);
    caca_free_display(dp);
    return 0;
}
