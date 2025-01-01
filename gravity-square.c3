import std::io;
import caca;

// TODO: Can probably use floats for these
struct Square {
    int width, height, x, y;
}

fn void main() {
    caca::Display *dp;
    caca::Canvas *cv;
    caca::Event ev;
    dp = caca::create_display(null);
    cv = caca::get_canvas(dp);
    int dt;
    while(!caca::get_event(dp, caca::EVENT_KEY_PRESS, ev, 0)) {
	dt = caca::get_frame_count(cv);
    }
    caca::free_canvas(cv);
    caca::free_display(dp);
    io::printfn("dt: %d", dt);;
}
