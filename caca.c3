module caca;

const int BLUE = 0x01;
const int BLACK = 0x00;

const int EVENT_KEY_PRESS = 0x0001;

def Display = void*;
def Canvas = void*;
def Event = void*;

extern fn Display create_display(Canvas*) @extern("caca_create_display");
extern fn CInt free_display(Display*) @extern("caca_free_display");
extern fn Canvas get_canvas(Display*) @extern("caca_get_canvas");
extern fn CInt free_canvas(Canvas*) @extern("caca_free_canvas");
extern fn CInt clear_canvas(Canvas*) @extern("caca_clear_canvas");
extern fn CInt get_canvas_width(Canvas*) @extern("caca_get_canvas_width");
extern fn CInt get_canvas_height(Canvas*) @extern("caca_get_canvas_height");
extern fn CInt set_color_ansi(Canvas*, uint, uint) @extern("caca_set_color_ansi");
extern fn CInt get_event(Display*, CInt, Event*, int) @extern("caca_get_event");
extern fn CInt draw_line(Canvas*, CInt, CInt, CInt, CInt, uint) @extern("caca_draw_line");
extern fn CInt fill_box(Canvas*, CInt, CInt, CInt, CInt, uint) @extern("caca_fill_box");
extern fn CInt refresh_display(Display*) @extern("caca_refresh_display");
extern fn CInt rand(CInt, CInt) @extern("caca_rand");
extern fn CInt get_display_time(Display*) @extern("caca_get_display_time");
extern fn CInt get_frame_count(Display*) @extern("caca_get_frame_count");
extern fn CInt put_str(Canvas*, CInt, CInt, CChar*) @extern("caca_put_str");
extern fn CInt put_char(Canvas*, CInt, CInt, uint) @extern("caca_put_char");
