#include <stdio.h>
#include "math3d.h"

int main() {
    // Define cube vertices centered at origin
    vec3_t cube[8] = {
        {-1, -1, -1}, { 1, -1, -1},
        { 1,  1, -1}, {-1,  1, -1},
        {-1, -1,  1}, { 1, -1,  1},
        { 1,  1,  1}, {-1,  1,  1},
    };

    // Build transform: scale -> rotate -> translate
    mat4_t scale = mat4_scale(0.5f, 0.5f, 0.5f);
    mat4_t rotate = mat4_rotate_xyz(0.5f, 1.0f, 0.2f);
    mat4_t translate = mat4_translate(0, 0, -5);

    // Combined model matrix (approximate multiplication manually)
    mat4_t model = translate; // Skip real mat4_mul for now

    // Projection
    mat4_t proj = mat4_frustum_asymmetric(-1, 1, -1, 1, 1, 10);

    printf("Projected 2D coordinates:\n");
    for (int i = 0; i < 8; ++i) {
        vec3_t p = mat4_transform_vec3(scale, cube[i]);
        p = mat4_transform_vec3(rotate, p);
        p = mat4_transform_vec3(model, p);
        vec3_t projected = mat4_transform_vec3(proj, p);

        printf("Vertex %d: x = %.2f, y = %.2f\n", i, projected.x, projected.y);
    }

    return 0;
}
