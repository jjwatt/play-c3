#include <caca.h>
#include <stdio.h>
#include <stdlib.h>
#include <Imlib2.h>

int main() {
  // 1. Initialize libcaca
  caca_canvas_t *cv = caca_create_canvas(0, 0);
  if (!cv) {
    fprintf(stderr, "Error creating canvas\n");
    return 1;
  
  }

  caca_display_t *dp = caca_create_display(cv);
  if (!dp) {
    fprintf(stderr, "Error creating display\n");
    caca_free_canvas(cv);
    return 1;
  
  }

  // 2. Load the image using Imlib2
  Imlib_Image image;
  image = imlib_load_image("output.jpg"); // Replace with your image path
  if (!image) {
    fprintf(stderr, "Error loading image with Imlib2\n");
    caca_free_display(dp);
    caca_free_canvas(cv);
    return 1;
  
  }

  // 3. Set Imlib2 context and get image data
  imlib_context_set_image(image);
  int imageWidth = imlib_image_get_width();
  int imageHeight = imlib_image_get_height();
  DATA32 *imageData = imlib_image_get_data_for_reading_only();

  // 4. Resize the canvas
  caca_set_canvas_size(cv, imageWidth, imageHeight);

  // 5. Transfer Imlib2 data to libcaca canvas
  for (int y = 0; y < imageHeight; y++) {
    for (int x = 0; x < imageWidth; x++) {
      DATA32 pixel = imageData[y * imageWidth + x];
      // Extract ARGB components (Imlib2 uses ARGB)
      uint8_t a = (pixel >> 24) & 0xff;
      uint8_t r = (pixel >> 16) & 0xff;
      uint8_t g = (pixel >> 8) & 0xff;
      uint8_t b = pixel & 0xff;

      caca_put_char(cv, x, y, '#'); // Or any character you like

      // Correct way to set ARGB color in libcaca:
      uint32_t argb = (a << 24) | (r << 16) | (g << 8) | b;
      caca_set_color_argb(cv, argb, CACA_TRANSPARENT);
    
    }
  
  }

  // 6. Display and wait for a key press
  caca_refresh_display(dp);
  caca_get_event(dp, CACA_EVENT_KEY_PRESS, NULL, -1);

  // 7. Clean up
  imlib_free_image(); // Free Imlib2 image data
  caca_free_display(dp);
  caca_free_canvas(cv);

  return 0;

}
