module plasma_caca;
import std::math;

import caca;

const int XSIZ = 256;
const int YSIZ = 256;
const int TABLEX = XSIZ * 2;
const int TABLEY = YSIZ * 2;

int frame;
uint[TABLEX * TABLEY] table;

fn int main() {
    caca::Display *dp;
    caca::Canvas *cv;
    caca::Event ev;
    // static?
    caca::Dither *dither;

    dp = caca::create_display(null);
    if (!dp) return 1;
    cv = caca::get_canvas(dp);
    defer {
	caca::free_canvas(cv);
	caca::free_display(dp);
    }
    int width = caca::get_canvas_width(cv);
    int height = caca::get_canvas_height(cv);

    static uint *screen;
    static uint[256] red;
    static uint[256] green;
    static uint[256] blue;
    static uint[256] alpha;
    double[3] r;
    double[6] big_r;

    int i, x, y;

    /* Prepare */
    // Fill various tables
    // They're zeroed by default in C3
    for (i = 0; i < 3; i++) {
	r[i] = (double)(caca::rand(1, 1000)) / 60_000 * math::PI;
    }

    for (i = 0; i < 6; i++) {
	big_r[i] = (double)(caca::rand(1, 1000)) / 10_000;
    }

    for (y = 0; y < TABLEY; y++) {
	for(x = 0; x < TABLEX; x++) {
	    double tmp = (((double)((x - (TABLEX / 2)) * (x - (TABLEX / 2))
	                            + (y - (TABLEX / 2)) * (y - (TABLEX / 2))))
	                  * (math::PI / (TABLEX * TABLEX + TABLEY * TABLEY)));
	    table[x + y * TABLEX] = (uint)(1.0 + math::sin(12.0 * math::sqrt((uint)tmp))) * 256 / 6;
	}
    }

    /* Init */
    screen = malloc(XSIZ * YSIZ * uint.sizeof);
    dither = caca::create_dither(8, XSIZ, YSIZ, XSIZ, 0, 0, 0, 0);

    /* Update */
    while(!caca::get_event(dp, caca::EVENT_KEY_PRESS | caca::EVENT_QUIT, ev, 0)) {
	for(i = 0; i < 256; i++) {
	    double z = ((double)i) / 256 * 6 * math::PI;

	    red[i] = (uint)(1.0 + math::sin(z + r[1] * frame)) / 2 * 0xfff;
	    blue[i] = (uint)(1.0 + math::cos(z + r[0] * (double)(frame + 100))) / 2 * 0xfff;
	    green[i] = (uint)(1.0 + math::cos(z + r[2] * (double)(frame + 200))) / 2 * 0xfff;
	}

	// Increase frame count
	frame++;

	// Set the palette
	caca::set_dither_palette(dither, &red, &green, &blue, &alpha);

	caca::set_color_ansi(cv, caca::BLUE, caca::BLACK);
	caca::fill_box(cv, 10, 10, 10, 10, '#');
	caca::refresh_display(dp);
    }
    
    free(screen);
    caca::free_dither(dither);
    return 0;
}
