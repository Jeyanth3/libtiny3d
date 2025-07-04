#ifndef RENDERER_H
#define RENDERER_H

#include "math3d.h"
#include "canvas.h"

vec3_t project_vertex(const vec3_t* v, mat4_t mvp, int width, int height);
int clip_to_circular_viewport(Canvas* canvas, float x, float y);
void render_wireframe(Canvas* canvas, vec3_t* vertices, int num_vertices, 
                      int (*edges)[2], int num_edges, mat4_t mvp);

#endif
