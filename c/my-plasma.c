#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <caca.h>

#define XSIZ 256
#define YSIZ 256
#define TABLEX (XSIZ * 2)
#define TABLEY (YSIZ * 2)

static int frame = 0;
static uint8_t table[TABLEX * TABLEY];

static void do_plasma(uint8_t *pixels, double x_1, double y_1,
                      double x_2, double y_2, double x_3, double y_3)
{
    unsigned int X1 = x_1 * (TABLEX / 2),
                 Y1 = y_1 * (TABLEY / 2),
                 X2 = x_2 * (TABLEX / 2),
                 Y2 = y_2 * (TABLEY / 2),
                 X3 = x_3 * (TABLEX / 2),
                 Y3 = y_3 * (TABLEY / 2);
    unsigned int y;
    uint8_t * t1 = table + X1 + Y1 * TABLEX,
            * t2 = table + X2 + Y2 * TABLEX,
            * t3 = table + X3 + Y3 * TABLEX;

    for(y = 0; y < YSIZ; y++)
    {
        unsigned int x;
        uint8_t * tmp = pixels + y * YSIZ;
        unsigned int ty = y * TABLEX, tmax = ty + XSIZ;
        for(x = 0; ty < tmax; ty++, tmp++)
            tmp[0] = t1[ty] + t2[ty] + t3[ty];
    }
}


int main() {
    caca_canvas_t *cv;
    caca_display_t *dp;
    caca_event_t ev;
    dp = caca_create_display(NULL);
    if (!dp) return 1;
    cv = caca_get_canvas(dp);
    int width = caca_get_canvas_width(cv);
    int height = caca_get_canvas_height(cv);

    static caca_dither_t *dither;
    static uint8_t *screen;
    static uint32_t red[256], green[256], blue[256], alpha[256];
    static double r[3], R[6];

    int i, x, y;

    /* Prepare  */
    /* Fill various tables */
    for(i = 0 ; i < 256; i++)
	red[i] = green[i] = blue[i] = alpha[i] = 0;
    
    for(i = 0; i < 3; i++)
	r[i] = (double)(caca_rand(1, 1000)) / 60000 * M_PI;

    for(i = 0; i < 6; i++)
	R[i] = (double)(caca_rand(1, 1000)) / 10000;

    for(y = 0 ; y < TABLEY ; y++)
	for(x = 0 ; x < TABLEX ; x++)
	    {
		double tmp = (((double)((x - (TABLEX / 2)) * (x - (TABLEX / 2))
					+ (y - (TABLEX / 2)) * (y - (TABLEX / 2))))
			      * (M_PI / (TABLEX * TABLEX + TABLEY * TABLEY)));

		table[x + y * TABLEX] = (1.0 + sin(12.0 * sqrt(tmp))) * 256 / 6;
	    }

    /* Init */
    screen = malloc(XSIZ * YSIZ * sizeof(uint8_t));
    dither = caca_create_dither(8, XSIZ, YSIZ, XSIZ, 0, 0, 0, 0);

    
    while (!caca_get_event(dp, CACA_EVENT_KEY_PRESS | CACA_EVENT_QUIT, &ev, 0)) {
	/* Update */
	for(i = 0 ; i < 256; i++)
        {
            double z = ((double)i) / 256 * 6 * M_PI;

            red[i] = (1.0 + sin(z + r[1] * frame)) / 2 * 0xfff;
            blue[i] = (1.0 + cos(z + r[0] * (frame + 100))) / 2 * 0xfff;
            green[i] = (1.0 + cos(z + r[2] * (frame + 200))) / 2 * 0xfff;
        }

	frame++;

        /* Set the palette */
        caca_set_dither_palette(dither, red, green, blue, alpha);

        do_plasma(screen,
                  (1.0 + sin(((double)frame) * R[0])) / 2,
                  (1.0 + sin(((double)frame) * R[1])) / 2,
                  (1.0 + sin(((double)frame) * R[2])) / 2,
                  (1.0 + sin(((double)frame) * R[3])) / 2,
                  (1.0 + sin(((double)frame) * R[4])) / 2,
                  (1.0 + sin(((double)frame) * R[5])) / 2);

	/* Render  */
	caca_dither_bitmap(cv, 0, 0,
                            caca_get_canvas_width(cv),
                            caca_get_canvas_height(cv),
                            dither, screen);


	caca_refresh_display(dp);
	usleep(20000);
        // Clear the screen (system-specific)
        caca_clear_canvas(cv);
    }
    free(screen);
    caca_free_dither(dither);
    caca_free_canvas(cv);
    caca_free_display(dp);
    return 0;
}
