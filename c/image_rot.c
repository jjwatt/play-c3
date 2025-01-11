#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <caca.h>

#define TABLE_SIZE 65536

#define PRECISION 8

#define FMUL(a, b) (((a)*(b))>>PRECISION)
#define TOFIX(d)   ((int)( (d)*(double)(1<<PRECISION) ))
#define TOINT(a)   (a>>PRECISION);

int main() {
  caca_canvas_t *cv;
  caca_display_t *dp;
  caca_event_t ev;
  dp = caca_create_display(NULL);
  if (!dp) return 1;
  cv = caca_get_canvas(dp);
  int width = caca_get_canvas_width(cv);
  int height = caca_get_canvas_height(cv);

// Load the file
  caca_canvas_t *imported_cv = caca_create_canvas(0, 0);
  ssize_t size = caca_import_canvas_from_file(imported_cv, "reply.caca", "caca");
  if (!size) {
    fprintf(stderr, "Error importing ANSI file\n");
    caca_free_display(dp);
    caca_free_canvas(cv);
    return 1;
  }
  int imported_width = caca_get_canvas_width(imported_cv);
  int imported_height = caca_get_canvas_height(imported_cv);

  static int cos_tab[TABLE_SIZE];
  static int sin_tab[TABLE_SIZE];

  static int angle_fixed;             // Rotation angle (fixed-point)
  static int time_fixed;              // Time variable for animation (fixed-point)
  int scale_fixed;                    // Scaling factor (fixed-point)

  unsigned int x_coord, y_coord;      // Coordinates of the current pixel in the output

  // Fixed-point components of the transformation (rotation and scaling)
  int x_transform_fixed;            // cos(angle) * scale (fixed-point)
  int y_transform_fixed;            // sin(angle) * scale (fixed-point)

  // Fixed-point texture coordinates
  int texture_u_fixed;               // U (horizontal) texture coordinate (fixed-point)
  int texture_v_fixed;               // V (vertical) texture coordinate (fixed-point)
  int prev_texture_u_fixed;           // Previous U coordinate at the start of the row (fixed-point)
  int prev_texture_v_fixed;           // Previous V coordinate at the start of the row (fixed-point)

  // Integer parts of texture coordinates, used for indexing
  unsigned int texture_u_int;
  unsigned int texture_v_int;

  /* Prepare */
  for(int x = 0; x < TABLE_SIZE; x++) {
      cos_tab[x] = TOFIX(cos(x * (360.0f / (float)TABLE_SIZE)));
      sin_tab[x] = TOFIX(sin(x * (360.0f / (float)TABLE_SIZE)));
  }

  while (!caca_get_event(dp, CACA_EVENT_KEY_PRESS | CACA_EVENT_QUIT, &ev, 0)) {
    // create temporary canvas for writing to
    caca_canvas_t *temp_cv = caca_create_canvas(width, height);

    angle_fixed += 4;
    time_fixed += 3;
    scale_fixed = FMUL(sin_tab[time_fixed & 0xFFFF], TOFIX(3)) + (TOFIX(4));
    x_transform_fixed    = FMUL(cos_tab[(angle_fixed) & 0xFFFF], scale_fixed);
    y_transform_fixed    = FMUL(sin_tab[(angle_fixed) & 0xFFFF], scale_fixed);
    texture_u_fixed  = texture_v_fixed  = 0;
    prev_texture_u_fixed = prev_texture_v_fixed = 0;

    for (y_coord = 0; y_coord < height; y_coord++) {
      for (x_coord = 0; x_coord < width; x_coord++) {
	texture_u_fixed += x_transform_fixed;
	texture_v_fixed += y_transform_fixed;

	texture_u_int = TOINT(texture_u_fixed);
	texture_v_int = TOINT(texture_v_fixed);

	// Ensure texture_u_int and texture_v_int are within bounds of the imported image
	texture_u_int = (texture_u_int % imported_width + imported_width) % imported_width;
	texture_v_int = (texture_v_int % imported_height + imported_height) % imported_height;

	// Get the character and attributes from the imported canvas
	uint32_t ch = caca_get_char(imported_cv, texture_u_int, texture_v_int);
	uint32_t attr = caca_get_attr(imported_cv, texture_u_int, texture_v_int);

	// Put the character and attributes on the temporary canvas
	caca_put_char(temp_cv, x_coord, y_coord, ch);
	caca_put_attr(temp_cv, x_coord, y_coord, attr);
      }
      texture_u_fixed = prev_texture_u_fixed -= y_transform_fixed;
      texture_v_fixed = prev_texture_v_fixed += x_transform_fixed;
    }
    caca_clear_canvas(cv);
    // blit rendered canvas
    caca_blit(cv, 0, 0, temp_cv, NULL);

    caca_free_canvas(temp_cv);

    caca_refresh_display(dp);
    usleep(50000);
  }
  caca_free_canvas(cv);
  caca_free_display(dp);
  return 0;
}
