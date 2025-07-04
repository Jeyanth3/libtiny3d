#include <math.h>
#include "canvas.h"
#include <math.h>
#include <stdlib.h>

// Clamp brightness to [0.0, 1.0]
static float clamp(float value) {
    if (value < 0.0f) return 0.0f;
    if (value > 1.0f) return 1.0f;
    return value;
}

// Set brightness at floating-point (x, y) using bilinear filtering
void set_pixel_f(Canvas* canvas, float x, float y, float intensity) {
    int x0 = (int)floorf(x);
    int y0 = (int)floorf(y);
    int x1 = x0 + 1;
    int y1 = y0 + 1;

    float dx = x - x0;
    float dy = y - y0;

    // Contributions to 4 surrounding pixels
    float w00 = (1 - dx) * (1 - dy);  // Top-left
    float w10 = dx * (1 - dy);        // Top-right
    float w01 = (1 - dx) * dy;        // Bottom-left
    float w11 = dx * dy;              // Bottom-right

    if (x0 >= 0 && y0 >= 0 && x0 < canvas->width && y0 < canvas->height)
        canvas->pixels[y0][x0] = clamp(canvas->pixels[y0][x0] + intensity * w00);
    if (x1 >= 0 && y0 >= 0 && x1 < canvas->width && y0 < canvas->height)
        canvas->pixels[y0][x1] = clamp(canvas->pixels[y0][x1] + intensity * w10);
    if (x0 >= 0 && y1 >= 0 && x0 < canvas->width && y1 < canvas->height)
        canvas->pixels[y1][x0] = clamp(canvas->pixels[y1][x0] + intensity * w01);
    if (x1 >= 0 && y1 >= 0 && x1 < canvas->width && y1 < canvas->height)
        canvas->pixels[y1][x1] = clamp(canvas->pixels[y1][x1] + intensity * w11);
}

// Draw a thick line using the DDA algorithm and bilinear pixel setting
void draw_line_f(Canvas* canvas, float x0, float y0, float x1, float y1, float thickness) {
    float dx = x1 - x0;
    float dy = y1 - y0;
    float length = fmaxf(fabsf(dx), fabsf(dy));

    if (length == 0) return;

    float step_x = dx / length;
    float step_y = dy / length;

    for (int i = 0; i <= (int)length; ++i) {
        float x = x0 + step_x * i;
        float y = y0 + step_y * i;

        // Draw a small square around (x, y) based on thickness
        int radius = (int)(thickness / 2.0f);
        for (int dy = -radius; dy <= radius; ++dy) {
            for (int dx = -radius; dx <= radius; ++dx) {
                float dist = sqrtf(dx*dx + dy*dy);
                if (dist <= thickness / 2.0f) {
                    set_pixel_f(canvas, x + dx, y + dy, 1.0f);  // Full intensity
                }
            }
        }
    }
}


// Allocate and initialize a new canvas
Canvas* create_canvas(int width, int height) {
    Canvas* canvas = (Canvas*)malloc(sizeof(Canvas));
    if (!canvas) return NULL;

    canvas->width = width;
    canvas->height = height;

    // Allocate 2D array of float pixels
    canvas->pixels = (float**)malloc(height * sizeof(float*));
    if (!canvas->pixels) {
        free(canvas);
        return NULL;
    }

    for (int y = 0; y < height; ++y) {
        canvas->pixels[y] = (float*)calloc(width, sizeof(float));
        if (!canvas->pixels[y]) {
            // cleanup in case of allocation failure
            for (int j = 0; j < y; ++j)
                free(canvas->pixels[j]);
            free(canvas->pixels);
            free(canvas);
            return NULL;
        }
    }

    return canvas;
}

// Free memory used by the canvas
void free_canvas(Canvas* canvas) {
    if (!canvas) return;

    for (int y = 0; y < canvas->height; ++y)
        free(canvas->pixels[y]);

    free(canvas->pixels);
    free(canvas);
}
