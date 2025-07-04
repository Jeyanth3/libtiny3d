#include <math.h>
#include <stdlib.h>
#include "../include/renderer.h"
#include "../include/math3d.h"
#include "../include/canvas.h"

vec3_t project_vertex(const vec3_t* v, mat4_t mvp, int width, int height) {
    vec3_t result;

    float x = v->x, y = v->y, z = v->z;
    float tx = mvp.m[0]*x + mvp.m[4]*y + mvp.m[8]*z + mvp.m[12];
    float ty = mvp.m[1]*x + mvp.m[5]*y + mvp.m[9]*z + mvp.m[13];
    float tz = mvp.m[2]*x + mvp.m[6]*y + mvp.m[10]*z + mvp.m[14];
    float tw = mvp.m[3]*x + mvp.m[7]*y + mvp.m[11]*z + mvp.m[15];

    if (tw != 0.0f) {
        tx /= tw;
        ty /= tw;
        tz /= tw;
    }

    result.x = (tx * 0.5f + 0.5f) * width;
    result.y = (1.0f - (ty * 0.5f + 0.5f)) * height;
    result.z = tz;

    return result;
}

int clip_to_circular_viewport(Canvas* canvas, float x, float y) {
    float cx = canvas->width / 2.0f;
    float cy = canvas->height / 2.0f;
    float radius = fminf(cx, cy);
    float dx = x - cx;
    float dy = y - cy;
    return (dx * dx + dy * dy) <= (radius * radius);
}

void render_wireframe(Canvas* canvas, const vec3_t* vertices, int num_vertices,
                      const int (*edges)[2], int num_edges, mat4_t mvp) {
    if (!canvas || !vertices || !edges || num_vertices <= 0 || num_edges <= 0) return;

    vec3_t* projected = (vec3_t*)malloc(sizeof(vec3_t) * num_vertices);
    if (!projected) return;

    for (int i = 0; i < num_vertices; i++) {
        projected[i] = project_vertex(&vertices[i], mvp, canvas->width, canvas->height);
    }

    for (int i = 0; i < num_edges; i++) {
        int a = edges[i][0];
        int b = edges[i][1];

        if (a < 0 || a >= num_vertices || b < 0 || b >= num_vertices) continue;

        vec3_t p0 = projected[a];
        vec3_t p1 = projected[b];

        if (clip_to_circular_viewport(canvas, p0.x, p0.y) ||
            clip_to_circular_viewport(canvas, p1.x, p1.y)) {
            draw_line_f(canvas, p0.x, p0.y, p1.x, p1.y, 1.0f);
        }
    }

    free(projected);
}
