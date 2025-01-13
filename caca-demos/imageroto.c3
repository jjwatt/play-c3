module imageroto;
import std::io;
import std::math;
import std::thread;
import caca;

const int TABLE_SIZE = 65536;
const int PRECISION = 8;

macro @tofix(#a) {
    return ((int)( (#a)*(double)(1<<PRECISION)));    
}

macro @fmul(#a, #b) {
    return (#a * #b) >> PRECISION;
}

macro @toint(#a) {
    return (#a>>PRECISION);
}

fn int main() {
    caca::Canvas *cv;
    caca::Display *dp;
    caca::Event ev;

    caca::Canvas *imported_cv;

    dp = caca::create_display(null);
    if (!dp) return 1;
    cv = caca::get_canvas(dp);

    defer caca::free_display(dp);
    defer caca::free_canvas(cv);
    
    int width = caca::get_canvas_width(cv);
    int height = caca::get_canvas_height(cv);

    imported_cv = caca::create_canvas(0, 0);
    usz size = caca::import_canvas_from_file(imported_cv,
					     "reply.caca",
					     "caca");
    defer caca::free_canvas(imported_cv);

    int imported_width = caca::get_canvas_width(imported_cv);
    int imported_height = caca::get_canvas_height(imported_cv);

    static int [TABLE_SIZE]cos_tab;
    static int [TABLE_SIZE]sin_tab;

    static int angle_fixed;
    static int time_fixed;
    int scale_fixed;
    uint x_coord, y_coord;
    uint x_transform_fixed;
    uint y_transform_fixed;
    uint texture_u_fixed;
    uint texture_v_fixed;
    uint prev_texture_u_fixed;
    uint prev_texture_v_fixed;

    uint texture_u_int;
    uint texture_v_int;

      /* Prepare */
    for(int x = 0; x < TABLE_SIZE; x++) {
	cos_tab[x] = @tofix(math::cos(x * (360.0f / (float)TABLE_SIZE)));
	sin_tab[x] = @tofix(math::sin(x * (360.0f / (float)TABLE_SIZE)));
    }
    // for (int i = 0; i < 10; ++i) {
    // 	io::eprintfn("sin_tab[%d]: %d", i, sin_tab[i]);
    // }

    while (!caca::get_event(dp, caca::EVENT_KEY_PRESS | caca::EVENT_QUIT, ev, 0)) {
	// create a temporary canvas for writing to
	caca::Canvas *temp_cv = caca::create_canvas(width, height);

	angle_fixed += 4;
	time_fixed += 3;

	scale_fixed = @fmul(sin_tab[time_fixed & 0xFFFF], @tofix(3)) + (@tofix(4));

	x_transform_fixed = @fmul(cos_tab[(angle_fixed) & 0xFFFF], scale_fixed);
	y_transform_fixed = @fmul(sin_tab[(angle_fixed) & 0xFFFF], scale_fixed);
	// io::eprintfn("scale_fixed: %d", scale_fixed);

	for (y_coord = 0; y_coord < height; y_coord++) {
	    for (x_coord = 0; x_coord < width; x_coord++) {
		texture_u_fixed += x_transform_fixed;
		texture_v_fixed += y_transform_fixed;

		texture_u_int = @toint(texture_u_fixed);
		texture_v_int = @toint(texture_v_fixed);

		// Ensure texture_u_int and texture_v_int are within bounds of the imported image
		texture_u_int = (texture_u_int % imported_width + imported_width) % imported_width;
		texture_v_int = (texture_v_int % imported_height + imported_height) % imported_height;

		// Get the character and attributes from the imported canvas
		uint ch = caca::get_char(imported_cv, texture_u_int, texture_v_int);
		uint attr = caca::get_attr(imported_cv, texture_u_int, texture_v_int);

		// Put the character and attributes on the temporary canvas
		caca::put_char(temp_cv, x_coord, y_coord, ch);
		caca::put_attr(temp_cv, x_coord, y_coord, attr);
	    }
	    texture_u_fixed = prev_texture_u_fixed -= y_transform_fixed;
	    texture_v_fixed = prev_texture_v_fixed += x_transform_fixed;
	}
	caca::clear_canvas(cv);
	caca::blit(cv, 0, 0, temp_cv, null);
	caca::free_canvas(temp_cv);
	caca::refresh_display(dp);
	thread::sleep(80_000);
    }
    return 0;
}
