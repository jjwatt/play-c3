import std::io;
import std::thread;
import caca;

struct Square {
    int width;
    int height;
    int x;
    int y;
    int dx;
    int dy;
}

fn void main()
{
    caca::Display *dp;
    caca::Canvas *cv;
    dp = caca::create_display(null);
    cv = caca::get_canvas(dp);

    int c_width = caca::get_canvas_width(cv);
    int c_height = caca::get_canvas_height(cv);
    Square square = {
	.width = 20,
	.height = 10,
	.x = 10,
	.y = 10,
	.dx = 1,
	.dy = 1
    };
    int dt;
    caca::set_color_ansi(cv, caca::BLUE, caca::BLACK);
    caca::fill_box(cv, square.x, square.y, square.width, square.height, '#');
    caca::refresh_display(dp);
    while(!caca::get_event(dp, caca::EVENT_KEY_PRESS, null, 0)) {
	caca::clear_canvas(cv);
	square.x += square.dx;
	square.y += square.dy;
	if (square.x >= c_width - square.width || square.x <= 1) {
	    caca::set_color_ansi(cv, caca::rand(1, 16), caca::BLACK);
	    square.dx *= -1;            
	}
	if (square.y >= c_height - square.height || square.y <= 1) {
	    caca::set_color_ansi(cv, caca::rand(1, 16), caca::BLACK);
	    square.dy *= -1;            
	}
	caca::fill_box(cv, square.x, square.y, square.width, square.height, '#');
	caca::refresh_display(dp);
	thread::sleep(80000);
	dt += 1;
    }
    caca::free_canvas(cv);
    caca::free_display(dp);
    io::printfn("c_width: %d", c_width);
    io::printfn("c_height: %d", c_height);
    io::printfn("dt: %d", dt);
}
