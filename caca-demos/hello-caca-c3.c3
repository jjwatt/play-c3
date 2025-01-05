import std::io;

const int CACA_BLUE = 0x01;
const int CACA_BLACK = 0x00;

const int CACA_EVENT_KEY_PRESS = 0x0001;

def Caca_display = void*;
def Caca_canvas = void*;
def Caca_event = void*;

extern fn Caca_display caca_create_display(Caca_canvas*);
extern fn CInt caca_free_display(Caca_display*);
extern fn Caca_canvas caca_get_canvas(Caca_display*);
extern fn CInt caca_free_canvas(Caca_canvas*);
extern fn CInt caca_set_color_ansi(Caca_canvas*, uint, uint);
extern fn CInt caca_get_event(Caca_display*, CInt, Caca_event*, int);
extern fn CInt caca_fill_box(Caca_canvas*, CInt, CInt, CInt, CInt, uint);
extern fn CInt caca_refresh_display(Caca_display*);

fn void main()
{
    Caca_display *dp;
    Caca_canvas *cv;
    dp = caca_create_display(null);
    cv = caca_get_canvas(dp);

    caca_set_color_ansi(cv, CACA_BLUE, CACA_BLACK);
    caca_fill_box(cv, 10, 10, 10, 10, '#');
    caca_refresh_display(dp);

    caca_get_event(dp, CACA_EVENT_KEY_PRESS, null, -1);
    caca_free_canvas(cv);
    caca_free_display(dp);
}
