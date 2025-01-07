#include <caca.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define XSIZ 256
#define YSIZ 256

int main() {

    caca_canvas_t *cv;
    caca_display_t *dp;
    caca_event_t ev;
    dp = caca_create_display(NULL);
    if (!dp) return 1;
    cv = caca_get_canvas(dp);
    int width = caca_get_canvas_width(cv);
    int height = caca_get_canvas_height(cv);
    caca_dither_t *dither;
    
    static uint32_t red[256], green[256], blue[256], alpha[256];
    int frame = 0;

    // Seed the random number generator
    srand(time(NULL));

    uint8_t *pixels = malloc(XSIZ * YSIZ * 4); // 4 bytes per pixel (RGBA)
    dither = caca_create_dither(8, XSIZ, YSIZ, XSIZ, 0, 0, 0, 0);

    // Initialize rgba values with 0
    for(int i = 0 ; i < 256; i++)
	red[i] = green[i] = blue[i] = alpha[i] = 0;

    while (!caca_get_event(dp, CACA_EVENT_KEY_PRESS | CACA_EVENT_QUIT, &ev, 0)) {

	for(int i = 0 ; i < 256; i++)
        {
            double z = ((double)i) / 256 * 6 * M_PI;

            red[i] = (1.0 + sin(z + rand() * frame)) / 2 * 0xfff;
            blue[i] = (1.0 + cos(z * (frame + 100))) / 2 * 0xfff;
            green[i] = (1.0 + cos(z * (frame + 200))) / 2 * 0xfff;
	    /* fprintf(stderr, "red[%d]: %d\n", i, red[i]); */
        }

	frame++;

	caca_set_dither_palette(dither, red, green, blue, alpha);

        // Generate plasma effect
        for (int y = 0; y < YSIZ; y++) {
            for (int x = 0; x < XSIZ; x++) {
		int value = (int)(((128 + (128 * sin(x / 32.0)))
			     + (128 + (128 * cos(y / 32.0)))
			     + (128 + (128 * sin(sqrt((x * x + y * y)) / 32.0)))) / 4);
                // Map noise to color (simple grayscale)
                int gray = (int)(value * 255);
                /* pixels[y * XSIZ * 4 + x * 4 + 0] = gray; // Red */
                /* pixels[y * XSIZ * 4 + x * 4 + 1] = gray; // Green */
                /* pixels[y * XSIZ * 4 + x * 4 + 2] = gray; // Blue */
                /* pixels[y * XSIZ * 4 + x * 4 + 3] = 0; // Alpha */
		pixels[x + y * XSIZ + 0] = gray; // Red
                pixels[x + y * XSIZ + 1] = gray; // Green
                pixels[x + y * XSIZ * 2] = gray; // Blue
                pixels[x + y * XSIZ * 3] = 0; // Alpha

            }
        }

        // Render to canvas using dithering
        caca_dither_bitmap(cv, 0, 0, width, height, dither, pixels);

        // Display the canvas
        caca_refresh_display(dp);

        // Sleep for a short duration
        usleep(50000); // 50 milliseconds
	caca_clear_canvas(cv);
    }

    free(pixels);
    caca_free_dither(dither);
    caca_free_canvas(cv);
    caca_free_display(dp);
    return 0;
}

