#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <caca.h>

// Simple noise function (you can use more sophisticated ones like Perlin noise)
double noise(double x, double y) {
    return sin(y) + cos(x); 
}

int main() {
    caca_canvas_t *cv;
    caca_display_t *dp;
    dp = caca_create_display(NULL);
    if (!dp) return 1;
    cv = caca_get_canvas(dp);
    int width = caca_get_canvas_width(cv);
    int height = caca_get_canvas_height(cv);
    char frame[height][width];
    while (!caca_get_event(dp, CACA_EVENT_KEY_PRESS, NULL, 0)) {
        double x_offset = sin(clock() / 1000.0); // Simple animation for x-offset
        double y_offset = cos(clock() / 1500.0); // Simple animation for y-offset

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                double n = noise(x + x_offset, y + y_offset); 
                n = (n + 1.0) * 0.5; // Normalize to 0.0 - 1.0
		/* Map noise to color */
		int red = (int)(n * 16);
		int green = (int)((1 - n) * 16);
		int blue = (int)((n * 0.5) * 16);

		caca_set_color_ansi(cv, red, CACA_BLACK);
		caca_put_char(cv, x, y, '#');
            }
        }
	caca_refresh_display(dp);
	usleep(10000);
        // Clear the screen (system-specific)
        caca_clear_canvas(cv);
    }
    caca_free_canvas(cv);
    caca_free_display(dp);
    return 0;
}
