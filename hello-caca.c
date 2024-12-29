#include <stdio.h>
#include <caca.h>

int main(void)
{
  caca_canvas_t *cv;
  caca_display_t *dp;
  caca_event_t ev;
  dp = caca_create_display(NULL);
  if (!dp) return 1;
  cv = caca_get_canvas(dp);
    
  int w = caca_get_canvas_width(cv);
  int h = caca_get_canvas_height(cv);
  int square_w = 10;
  int square_h = 10;
  int square_x = (w / 2) - 50;
  int square_y = (h / 2) - 10;

  while (!caca_get_event(dp, CACA_EVENT_KEY_PRESS, NULL, 0)) {
    /* int xa = (w / 2) - 50; */
    /* int ya = (h / 2) - 10; */
    int xb = square_x + 10;
    int yb = square_y + 10;
 
    caca_set_color_ansi(cv, CACA_BLACK, CACA_WHITE);
    caca_put_str(cv, 0, 0, "Hello, libcaca!");
    caca_set_color_ansi(cv, caca_rand(0, 16), CACA_BLACK);
    caca_fill_box(cv, square_x, square_y, xb, yb, '#');
    caca_refresh_display(dp);
    usleep(50000);
  }
  caca_free_display(dp);
  return 0;
}
