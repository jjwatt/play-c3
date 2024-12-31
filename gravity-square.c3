import std::io;
import caca;

struct Square {
    int w;
    int h;
    int x;
    int y;
}

fn void main() {
    caca::Display *dp;
    caca::Canvas *cv;
    dp = caca::create_display(null);
    cv = caca::get_canvas(dp);
    int dt;
    while(!caca::get_event(dp, caca::EVENT_KEY_PRESS, null, 0)) {
	dt = caca::get_frame_count(cv);
    }
    caca::free_canvas(cv);
    caca::free_display(dp);
    io::printfn("dt: %d", dt);;
}
