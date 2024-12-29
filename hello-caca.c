#include <caca.h>

int main(void)
{
  caca_canvas_t *cv;
  caca_display_t *dp;
  caca_event_t ev;
  dp = caca_create_display(NULL);
  if (!dp) return 1;
  int square_w = 10;
  int square_h = 10;
  cv = caca_get_canvas(dp);
  while (!caca_get_event(dp, CACA_EVENT_KEY_PRESS, &ev, 0)) {
    int w = caca_get_canvas_width(cv);
    int h = caca_get_canvas_height(cv);
    int xa = (w / 2) - 50;
    int ya = (h / 2) - 10;
    int xb = xa + square_w;
    int yb = ya + square_h;
 
    caca_set_color_ansi(cv, CACA_BLACK, CACA_WHITE);
    caca_put_str(cv, 0, 0, "Hello, libcaca!");
    caca_set_color_ansi(cv, caca_rand(0, 16), CACA_BLACK);
    caca_fill_box(cv, xa, ya, xb, yb, '#');
    caca_refresh_display(dp);
    usleep(50000);
  }
  caca_free_display(dp);
  return 0;
}
