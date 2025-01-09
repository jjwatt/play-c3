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

  // Get terminal dimensions (in characters)
  int termWidth = caca_get_canvas_width(cv);
  int termHeight = caca_get_canvas_height(cv);

  // 2. Load the image using Imlib2
  Imlib_Image image;
  image = imlib_load_image("reply.jpg"); // Replace with your image path
  if (!image) {
    fprintf(stderr, "Error loading image with Imlib2\n");
    caca_free_display(dp);
    caca_free_canvas(cv);
    return 1;
  
  }

  // 3. Resize the image using Imlib2
  imlib_context_set_image(image);
  int originalWidth = imlib_image_get_width();
  int originalHeight = imlib_image_get_height();

  // Calculate a new size based on terminal dimensions
  // (Example: Scale down to fit within the terminal, maintaining aspect ratio)
  int newWidth, newHeight;
  newWidth = termWidth;
  newHeight = termHeight;
  /* if ((float)originalWidth / termWidth > (float)originalHeight / termHeight) { */
  /*   // Image is wider than the terminal proportionally */
  /*   newWidth = termWidth; */
  /*   newHeight = (int)(originalHeight * ((float)termWidth / originalWidth)); */
  
  /* } else { */
  /*   // Image is taller than the terminal proportionally */
  /*   newHeight = termHeight; */
  /*   newWidth = (int)(originalWidth * ((float)termHeight / originalHeight)); */
  /* } */

  Imlib_Image resizedImage = imlib_create_cropped_scaled_image(0, 0, originalWidth, originalHeight, newWidth, newHeight);
  if (!resizedImage) {
    fprintf(stderr, "Error resizing image with Imlib2\n");
    caca_free_display(dp);
    caca_free_canvas(cv);
    imlib_free_image();
    return 1;
  
  }

  // Free the original image
  imlib_free_image();

  // Set the context to the resized image
  imlib_context_set_image(resizedImage);
  int imageWidth = imlib_image_get_width();
  int imageHeight = imlib_image_get_height();
  DATA32 *imageData = imlib_image_get_data_for_reading_only();

  // 4. Resize the libcaca canvas (to the resized image dimensions)
  caca_set_canvas_size(cv, imageWidth, imageHeight);

  // 5. Transfer Imlib2 data to libcaca canvas
  for (int y = 0; y < imageHeight; y++) {
    for (int x = 0; x < imageWidth; x++) {
      DATA32 pixel = imageData[y * imageWidth + x];
      uint8_t a = (pixel >> 24) & 0xff;
      uint8_t r = (pixel >> 16) & 0xff;
      uint8_t g = (pixel >> 8) & 0xff;
      uint8_t b = pixel & 0xff;

      caca_put_char(cv, x, y, '#');
      uint32_t argb = (a << 24) | (r << 16) | (g << 8) | b;
      caca_set_color_argb(cv, argb, CACA_TRANSPARENT);
    
    }
  
  }

  // 6. Display and wait for a key press
  caca_refresh_display(dp);
  caca_get_event(dp, CACA_EVENT_KEY_PRESS, NULL, -1);

  // 7. Clean up
  imlib_free_image(); // Free Imlib2 image data (resizedImage)
  caca_free_display(dp);
  caca_free_canvas(cv);

  return 0;

}
