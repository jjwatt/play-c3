module caca;

const int CACA_BLUE = 0x01;
const int CACA_BLACK = 0x00;

const int CACA_EVENT_KEY_PRESS = 0x0001;

def Caca_display = void*;
def Caca_canvas = void*;
def Caca_event = void*;

extern fn Caca_display create_display(Caca_canvas*) @extern("caca_create_display");
extern fn CInt free_display(Caca_display*) @extern("caca_free_display");
extern fn Caca_canvas caca_get_canvas(Caca_display*);
extern fn CInt caca_free_canvas(Caca_canvas*);
extern fn CInt caca_get_canvas_width(Caca_canvas*);
extern fn CInt caca_get_canvas_height(Caca_canvas*);
extern fn CInt caca_set_color_ansi(Caca_canvas*, uint, uint);
extern fn CInt caca_get_event(Caca_display*, CInt, Caca_event*, int);
extern fn CInt caca_fill_box(Caca_canvas*, CInt, CInt, CInt, CInt, uint);
extern fn CInt caca_refresh_display(Caca_display*);
