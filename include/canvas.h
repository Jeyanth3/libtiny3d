#ifndef CANVAS_H
#define CANVAS_H

// Structure to hold a canvas
typedef struct {
    int width;       // Width of the canvas
    int height;      // Height of the canvas
    float **pixels;  // 2D array of brightness values (0.0 to 1.0)
} Canvas;

// Function declarations (optional if you're adding more later)
Canvas* create_canvas(int width, int height);
void free_canvas(Canvas* canvas);

#endif // CANVAS_H
