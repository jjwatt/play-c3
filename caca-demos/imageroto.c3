module imageroto;
import std::io;
import std::math;
import std::thread;
import caca;

const int TABLE_SIZE = 65536;
const int PRECISION = 8;

fn int main() {
    caca::Canvas *cv;
    caca::Display *dp;
    caca::Event ev;
    
    caca::Canvas *imported_cv;
    
    dp = caca::create_display(null);
    if (!dp) return 1;
    cv = caca::get_canvas(dp);

    int width = caca::get_canvas_width(cv);
    int height = caca::get_canvas_height(cv);

    imported_cv = caca::create_canvas(0, 0);
    usz size = caca::import_canvas_from_file(imported_cv,
					     "reply.caca",
					     "caca");
    int imported_width = caca::get_canvas_width(imported_cv);
    int imported_height = caca::get_canvas_height(imported_cv);

    int [TABLE_SIZE]cos_tab;
    int [TABLE_SIZE]sin_tab;

    int angle_fixed;
    int time_fixed;
    int scale_fixed;
    uint x_coord, y_coord;
    int x_transform_fixed;
    int y_transform_fixed;
    int texture_u_fixed;
    int texture_v_fixed;
    int prev_texture_u_fixed;
    int prev_texture_v_fixed;

    uint texture_u_int;
    uint texture_v_int;
    
      /* Prepare */
    for(int x = 0; x < TABLE_SIZE; x++) {
	cos_tab[x] = TOFIX(cos(x * (360.0f / (float)TABLE_SIZE)));
	sin_tab[x] = TOFIX(sin(x * (360.0f / (float)TABLE_SIZE)));
    }

    while (1caca::get_event(dp, caca::EVENT_KEY_PRESS | caca::EVENT_QUIT, ev, 0)) {
	// create a temporary canvas for writing to
	caca::Canvas *temp_cv = caca::create_canvas(widht, height);

	angle_fixed += 4;
    }
}
