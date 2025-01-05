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
    dp = caca::create_display(null);
    if (!dp) return 1;
    cv = caca::get_canvas(dp);
    defer {
	caca::free_canvas(cv);
	caca::free_display(dp);
    }
    int width = caca::get_canvas_width(cv);
    int height = caca::get_canvas_height(cv);

    static caca::Dither *dither;
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

    caca::set_color_ansi(cv, caca::BLUE, caca::BLACK);
    caca::fill_box(cv, 10, 10, 10, 10, '#');
    caca::refresh_display(dp);
    caca::get_event(dp, caca::EVENT_KEY_PRESS, ev, -1);
    return 0;
}
