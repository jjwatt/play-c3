#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define WIDTH 80
#define HEIGHT 25

// Simple noise function (you can use more sophisticated ones like Perlin noise)
double noise(double x, double y) {
    return sin(x) + cos(y); 
}

int main() {
    char frame[HEIGHT][WIDTH]; 

    while (1) { // Infinite loop for continuous animation
        double x_offset = sin(clock() / 1000.0); // Simple animation for x-offset
        double y_offset = cos(clock() / 1500.0); // Simple animation for y-offset

        for (int y = 0; y < HEIGHT; y++) {
            for (int x = 0; x < WIDTH; x++) {
                double n = noise(x + x_offset, y + y_offset); 
                n = (n + 1.0) * 0.5; // Normalize to 0.0 - 1.0
                int intensity = n * 7; // Map to character intensity (0-7)
                frame[y][x] = " .:-=+*#%@"[intensity]; 
            }
        }

        // Clear the screen (system-specific)
        system("clear"); 

        // Print the frame
        for (int y = 0; y < HEIGHT; y++) {
            for (int x = 0; x < WIDTH; x++) {
                printf("%c", frame[y][x]);
            }
            printf("\n");
        }
    }
    return 0;
}
