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

// Save the canvas as a grayscale PGM image
void canvas_save(Canvas* canvas, const char* filename);

// Initialize a canvas in-place (non-pointer version)
void canvas_init(Canvas* canvas, int width, int height);

// Free canvas memory (non-pointer version)
void canvas_free(Canvas* canvas);

void draw_line_f(Canvas* canvas, float x0, float y0, float x1, float y1, float thickness);


#endif // CANVAS_H
