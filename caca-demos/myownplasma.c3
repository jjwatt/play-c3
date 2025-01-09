module myownplasma;
import std::math;
import std::thread;
import std::io;
import caca;

const usz XSIZ = 256;
const usz YSIZ = 256;
const usz TABLEX = XSIZ * 2;
const usz TABLEY = YSIZ * 2;

fn int main() {
    caca::Canvas *cv;
    caca::Display *dp;
    caca::Event ev;
    static caca::Dither dither;

    dp = caca::create_display(null);
    if (!dp) return 1;
    cv = caca::get_canvas(dp);

    defer {
	caca::free_canvas(cv);
	caca::free_display(dp);
    }
    
    int width = caca::get_canvas_width(cv);
    int height = caca::get_canvas_height(cv);
    // io::eprintfn("width: %s", width);
    // io::eprintfn("height: %s", height);

    char *screen;
    int frame;
    uint[256] red;
    uint[256] green;
    uint[256] blue;
    uint[256] alpha;
    double[3] r;
    // double[6] big_r;

    /* Prepare Tables */
    // Not working
    for (int i = 0; i< 3; i++) {
	r[i] = (double)(caca::rand(1, 1000)) / 60_000 * math::PI;
    }

    // for (int i = 0; i < 6; i++) {
    // 	big_r[i] = (double)(caca::rand(1, 1000)) / 10_000;
    // }
    
    /* Init */
    screen = malloc(XSIZ * YSIZ * char.sizeof);
    dither = caca::create_dither(8, XSIZ, YSIZ, XSIZ, 0, 0, 0, 0);
    defer {
	free(screen);
	caca::free_dither(dither);
    }

    // Update
    while(!caca::get_event(dp, caca::EVENT_KEY_PRESS | caca::EVENT_QUIT, ev, 0)) {
	// for loop to setup rgb palette
	for (int i = 0 ; i < 256; i++) {
	    double z = ((double)i) / 256 * 6 * math::PI;
	    red[i] = (uint)((1.0 + math::sin(z + r[1] * frame)) / 2 * 0xfff);
	    blue[i] = (uint)((1.0 + math::cos(z + r[0] * (double)(frame + 100))) / 2 * 0xfff);
	    green[i] = (uint)((1.0 + math::cos(z + r[2] * (double)(frame + 200))) / 2 * 0xfff);
	    // io::eprintfn("z: %d", z);
	    // io::eprintfn("red[%d]: %d", i, red[i]);
	    // io::eprintfn("green[%d]: %d", i, green[i]);
	    // io::eprintfn("blue[%d]: %d", i, blue[i]);
        }

	// Increase frame count
	frame++;	
	// Set the palette
	caca::set_dither_palette(dither, &red, &green, &blue, &alpha);

	// Generate plasma effect
	for (int y = 0; y < YSIZ; y++) {
	    for (int x = 0; x < XSIZ; x++) {
		int value = (int)(((128 + (128 * math::sin(x / 19.0)))
				   + (128 + (128 * math::cos(y / 32.0)))
				   + (128 + (128 * math::sin(math::sqrt((x * x + y * y)) / 32.0))))
				  / 4);
		screen[x + y * XSIZ] = (char) value;
	    }
	}
        
	/* Render */
	caca::dither_bitmap(cv, 0, 0,
	                    caca::get_canvas_width(cv),
	                    caca::get_canvas_height(cv),
	                    dither, screen);
	caca::refresh_display(dp);
	thread::sleep(5_000);
	caca::clear_canvas(cv);
    }
    
    // free(screen);
    // caca::free_dither(dither);
    // caca::free_canvas(cv);
    // caca::free_display(dp);
    return 0;
}
