#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "canvas.h"
#include "renderer.h"
#include "soccerball.h"

#define M_PI 3.14159265358979323846

// Save canvas to a PGM file for visualization
void save_canvas_to_pgm(Canvas* canvas, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        perror("Failed to open file");
        return;
    }

    fprintf(file, "P2\n%d %d\n255\n", canvas->width, canvas->height);

    for (int y = 0; y < canvas->height; ++y) {
        for (int x = 0; x < canvas->width; ++x) {
            int value = (int)((1.0f - canvas->pixels[y][x]) * 255.0f);
            fprintf(file, "%d ", value);
        }
        fprintf(file, "\n");
    }

    fclose(file);
}

int main() {
    int width = 512, height = 512;
    float center_x = width / 2.0f, center_y = height / 2.0f;
    float radius = 80.0f, thickness = 1.0f;

    // ========== Task 1: Clock face ==========
    Canvas* clock_canvas = create_canvas(width, height);
    if (!clock_canvas) {
        fprintf(stderr, "Failed to create clock canvas\n");
        return 1;
    }

    for (int angle = 0; angle < 360; angle += 15) {
        float radians = angle * M_PI / 180.0f;
        float end_x = center_x + radius * cosf(radians);
        float end_y = center_y + radius * sinf(radians);
        draw_line_f(clock_canvas, center_x, center_y, end_x, end_y, thickness);
    }


    save_canvas_to_pgm(clock_canvas, "clock_lines.pgm");
    printf("Clock face saved to clock_lines.pgm\n");
    canvas_free(clock_canvas);

    // ========== Task 3: Cube Rendering ==========
    Canvas* cube_canvas = create_canvas(width, height);
    if (!cube_canvas) {
        fprintf(stderr, "Failed to create cube canvas\n");
        return 1;
    }

    vec3_t vertices[8] = {
        vec3_init(-1, -1, -1), vec3_init(1, -1, -1),
        vec3_init(1,  1, -1), vec3_init(-1,  1, -1),
        vec3_init(-1, -1,  1), vec3_init(1, -1,  1),
        vec3_init(1,  1,  1), vec3_init(-1,  1,  1)
    };

    printf("Cube vertex coordinates:\n");
    for (int i = 0; i < 8; ++i) {
        printf("Vertex %d: (%.3f, %.3f, %.3f)\n", i,
            vertices[i].x, vertices[i].y, vertices[i].z);
    }

    int edges[][2] = {
        {0, 1}, {1, 2}, {2, 3}, {3, 0},
        {4, 5}, {5, 6}, {6, 7}, {7, 4},
        {0, 4}, {1, 5}, {2, 6}, {3, 7}
    };

    mat4_t model = mat4_rotate_xyz(0.5f, 1.0f, 0.3f);
    mat4_t view = mat4_translate(0.0f, 0.0f, -5.0f);
    mat4_t projection = mat4_frustum_asymmetric(-1, 1, -1, 1, 1, 10);

    render_wireframe(cube_canvas, vertices, 8, edges, 12, model, view, projection);
    canvas_save(cube_canvas, "output.pgm");

    printf("Cube wireframe saved to output.pgm\n");
    canvas_free(cube_canvas);

    // ========== Soccer Ball Rendering ==========

    Canvas* soccer_canvas = create_canvas(width, height);
    if (!soccer_canvas) {
        fprintf(stderr, "Failed to create soccer canvas\n");
        return 1;
    }

    vec3_t* soccer_vertices = NULL;
    int soccer_vertex_count = 0;
    int (*soccer_edges)[2] = NULL;
    int soccer_edge_count = 0;

    generate_soccer_ball(&soccer_vertices, &soccer_vertex_count, &soccer_edges, &soccer_edge_count);

    
    float aspect_ratio = (float)width / height;
    mat4_t soccer_proj = mat4_frustum_asymmetric(-aspect_ratio, aspect_ratio, -1.0f, 1.0f, 1.0f, 10.0f);
    mat4_t soccer_model = mat4_identity(); // no rotation
    mat4_t soccer_view = mat4_translate(0.0f, 0.0f, -4.0f);
    
    
    render_wireframe(soccer_canvas, soccer_vertices, soccer_vertex_count, soccer_edges, soccer_edge_count,soccer_model, soccer_view, soccer_proj);
    
    char filename[64];
    for (int frame = 0; frame < 60; ++frame) {
        float angle = frame * (2.0f * M_PI / 60);  // One full rotation
        mat4_t soccer_model = mat4_rotate_xyz(0.0f, angle, 0.0f);  // Y-axis rotation

        Canvas* frame_canvas = create_canvas(width, height);
        render_wireframe(frame_canvas, soccer_vertices, soccer_vertex_count,
                        soccer_edges, soccer_edge_count, soccer_model, soccer_view, soccer_proj);

        sprintf(filename, "soccer_%03d.pgm", frame);
        canvas_save(frame_canvas, filename);
        canvas_free(frame_canvas);
    }
    canvas_save(soccer_canvas, "soccer.pgm");
    printf("Soccer ball saved to soccer.pgm\n");

    free(soccer_vertices);
    free(soccer_edges);
    canvas_free(soccer_canvas);


    return 0;
}