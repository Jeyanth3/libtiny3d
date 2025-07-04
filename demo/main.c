#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "canvas.h"

#define M_PI 3.14159265358979323846

// Function prototypes (defined in canvas.c)
extern void set_pixel_f(Canvas* canvas, float x, float y, float intensity);
extern void draw_line_f(Canvas* canvas, float x0, float y0, float x1, float y1, float thickness);

// Save canvas to a PGM file for visualization
void save_canvas_to_pgm(Canvas* canvas, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        perror("Failed to open file");
        return;
    }

    // Write PGM header
    fprintf(file, "P2\n%d %d\n255\n", canvas->width, canvas->height);

    for (int y = 0; y < canvas->height; ++y) {
        for (int x = 0; x < canvas->width; ++x) {
            int value = (int)((1.0f - canvas->pixels[y][x]) * 255.0f); // convert to grayscale
            fprintf(file, "%d ", value);
        }
        fprintf(file, "\n");
    }

    fclose(file);
}


int main() {
    int width = 200;
    int height = 200;
    Canvas* canvas = create_canvas(width, height);

    if (!canvas) {
        fprintf(stderr, "Failed to create canvas\n");
        return 1;
    }

    float center_x = width / 2.0f;
    float center_y = height / 2.0f;
    float radius = 80.0f;
    float thickness = 1.0f;

    // Draw radial lines every 15 degrees
    for (int angle = 0; angle < 360; angle += 15) {
        float radians = angle * M_PI / 180.0f;
        float end_x = center_x + radius * cosf(radians);
        float end_y = center_y + radius * sinf(radians);

        draw_line_f(canvas, center_x, center_y, end_x, end_y, thickness);
    }
    
     
    // Save canvas to file
    save_canvas_to_pgm(canvas, "clock_lines.pgm");
    printf("Clock face saved to clock_lines.pgm\n");

    // Cleanup
    free_canvas(canvas);
    return 0;
}
