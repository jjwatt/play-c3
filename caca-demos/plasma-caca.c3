module plasma_caca;
import std::math;
import std::thread;
import std::io;
import caca;

const CUInt XSIZ = 256;
const CUInt YSIZ = 256;
const CUInt TABLEX = XSIZ * 2;
const CUInt TABLEY = YSIZ * 2;

int frame;
uint[TABLEX * TABLEY] table;

fn void do_plasma(CUInt *pixels, double x_1, double y_1, double x_2, double y_2,
                  double x_3, double y_3) {
    CUInt x1 = (CUInt)(x_1 * (TABLEX / 2));
    CUInt y1 = (CUInt)(y_1 * (TABLEY / 2));
    CUInt x2 = (CUInt)(x_2 * (TABLEX / 2));
    CUInt y2 = (CUInt)(y_2 * (TABLEY / 2));
    CUInt x3 = (CUInt)(x_3 * (TABLEX / 2));
    CUInt y3 = (CUInt)(y_3 * (TABLEY / 2));
    CUInt *t1 = &table[x1 + y1 * TABLEX];
    CUInt *t2 = &table[x2 + y2 * TABLEX];
    CUInt *t3 = &table[x3 + y3 * TABLEX];
    // Print initial coordinates
    // io::eprintfn("Coordinates: (%d,%d) (%d,%d) (%d,%d)", 
    // 		 x1, y1, x2, y2, x3, y3);
    for(uint y = 0; y < YSIZ; y++) {
	CUInt *tmp = pixels + y * YSIZ;
	CUInt ty = y * TABLEX;
	CUInt tmax = ty + XSIZ;
	for(uint x = 0; ty < tmax; ty++, tmp++) {
	    // io::eprintfn("t1[ty]: %s", t1[ty]);
	    tmp[0] = (t1[ty] + t2[ty] + t3[ty]);
	    // io::eprintfn("tmp[0]: %s", tmp[0]);
	    // io::eprintfn("pixels: %s", pixels[ty]);
	}
    }
}

fn int main() {
    caca::Canvas *cv;
    caca::Display *dp;
    caca::Event ev;
    static caca::Dither dither;

    dp = caca::create_display(null);
    if (!dp) return 1;
    cv = caca::get_canvas(dp);

    int width = caca::get_canvas_width(cv);
    int height = caca::get_canvas_height(cv);
    io::eprintfn("width: %s", width);
    io::eprintfn("height: %s", height);

    static CUInt *screen;
    static CUInt[256] red;
    static CUInt[256] green;
    static CUInt[256] blue;
    static CUInt[256] alpha;
    static double[3] r;
    static double[6] big_r;

    int i, x, y;

    /* Prepare */
    // Fill various tables
    // They're zeroed by default in C3
    // for(i = 0; i < 256; i++) {
    // 	red[i] = green[i] = blue[i] = alpha[i] = 0;
    // }
    
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
	    table[x + y * TABLEX] = (uint)((1.0 + math::sin(12.0 * math::sqrt(tmp))) * 256 / 6);
	    // io::eprintfn("table[x + y * TABLEX]: %s", table[x + y * TABLEX]);
	}
    }

    /* Init */
    screen = malloc(XSIZ * YSIZ * uint.sizeof);
    dither = caca::create_dither(8, XSIZ, YSIZ, XSIZ, 0, 0, 0, 0);

    /* Update */
    while(!caca::get_event(dp, caca::EVENT_KEY_PRESS | caca::EVENT_QUIT, ev, 0)) {
	for(i = 0; i < 256; i++) {
	    double z = ((double)i) / 256 * 6 * math::PI;
	    // io::eprintfn("z: %s", z);
	    red[i] = (CUInt)((1.0 + math::sin(z + r[1] * frame)) / 2 * 0xfff);
	    blue[i] = (CUInt)((1.0 + math::cos(z + r[0] * (double)(frame + 100))) / 2 * 0xfff);
	    green[i] = (CUInt)((1.0 + math::cos(z + r[2] * (double)(frame + 200))) / 2 * 0xfff);
	    // io::eprintfn("r[1]: %s", r[1]);
	    // io::eprintfn("r[0]: %s", r[0]);
	    // io::eprintfn("r[2]: %s", r[2]);
	    // io::eprintfn("1.0 + math::sin(z + r[1] * frame) / 2 * 0xfff: %s", (CUInt)((1.0 + math::sin(z + r[1] * frame)) / 2 * 0xfff) % 256);   
	    // io::eprintfn("red[%d]: %s", i, red[i]);
	    // io::eprintfn("green[%d]: %s", i, green[i]);
	    // io::eprintfn("blue[%d]: %s", i, blue[i]);
	}

	// Increase frame count
	frame++;

	// Set the palette
	caca::set_dither_palette(dither, &red, &green, &blue, &alpha);

	// do plasma effect
	do_plasma(screen,
                  (1.0 + math::sin(((double)frame) * big_r[0])) / 2,
                  (1.0 + math::sin(((double)frame) * big_r[1])) / 2,
                  (1.0 + math::sin(((double)frame) * big_r[2])) / 2,
                  (1.0 + math::sin(((double)frame) * big_r[3])) / 2,
                  (1.0 + math::sin(((double)frame) * big_r[4])) / 2,
                  (1.0 + math::sin(((double)frame) * big_r[5])) / 2);

	/* Render */
	caca::dither_bitmap(cv, 0, 0,
	                    caca::get_canvas_width(cv),
	                    caca::get_canvas_height(cv),
	                    dither, screen);
	caca::refresh_display(dp);
	thread::sleep(20000);
	caca::clear_canvas(cv);
    }
    
    free(screen);
    caca::free_dither(dither);
    caca::free_canvas(cv);
    caca::free_display(dp);
    return 0;
}
