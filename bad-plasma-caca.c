#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <caca.h>

// Simple noise function (you can use more sophisticated ones like Perlin noise)
double noise(double x, double y) {
    return sin(x) + cos(y); 
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
    while (1) { // Infinite loop for continuous animation
        double x_offset = sin(clock() / 1000.0); // Simple animation for x-offset
        double y_offset = cos(clock() / 1500.0); // Simple animation for y-offset

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                double n = noise(x + x_offset, y + y_offset); 
                n = (n + 1.0) * 0.5; // Normalize to 0.0 - 1.0
                int intensity = n * 7; // Map to character intensity (0-7)
                frame[y][x] = " .:-=+*#%@"[intensity]; 
            }
        }

        // Clear the screen (system-specific)
        caca_clear_canvas(cv);

        // Print the frame
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
		caca_put_char(cv, x, y, fame[y][x])
                /* printf("%c", frame[y][x]); */
            }
        }
    }
    return 0;
}
