#include <caca.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
  caca_canvas_t *cv;
  caca_display_t *dp;
  dp = caca_create_display(NULL);
  if (!dp) return 1;
  cv = caca_get_canvas(dp);

  // Load the file
  caca_canvas_t *imported_cv = caca_create_canvas(0, 0);
  ssize_t size = caca_import_canvas_from_file(imported_cv, "reply.txt", "ansi");
  if (!imported_cv) {
    fprintf(stderr, "Error importing ANSI file\n");
    caca_free_display(dp);
    caca_free_canvas(cv);
    return 1;
  }

  int width = caca_get_canvas_width(imported_cv);
  int height = caca_get_canvas_height(imported_cv);

  caca_blit(cv, 0, 0, imported_cv, NULL);

  caca_refresh_display(dp);
  caca_get_event(dp, CACA_EVENT_KEY_PRESS, NULL, -1);

  caca_free_canvas(imported_cv); // Free the imported canvas
  caca_free_canvas(cv);
  caca_free_display(dp);

  return 0;

}
