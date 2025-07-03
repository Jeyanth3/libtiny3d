#include <stdio.h>
#include "math3d.h"

// Multiply mat4 with vec3 (assumes w = 1.0)
vec3_t mat4_transform_vec3(mat4_t m, vec3_t v) {
    vec3_t result;
    float x = v.x, y = v.y, z = v.z;

    float w = m.m[3] * x + m.m[7] * y + m.m[11] * z + m.m[15];
    if (w == 0.0f) w = 1.0f;

    result.x = (m.m[0] * x + m.m[4] * y + m.m[8]  * z + m.m[12]) / w;
    result.y = (m.m[1] * x + m.m[5] * y + m.m[9]  * z + m.m[13]) / w;
    result.z = (m.m[2] * x + m.m[6] * y + m.m[10] * z + m.m[14]) / w;

    return result;
}

int main() {
    // Define 8 vertices of a unit cube centered at origin
    vec3_t cube[8] = {
        {-1, -1, -1}, { 1, -1, -1},
        { 1,  1, -1}, {-1,  1, -1},
        {-1, -1,  1}, { 1, -1,  1},
        { 1,  1,  1}, {-1,  1,  1},
    };

    // Build transformation matrix: scale -> rotate -> translate
    mat4_t scale = mat4_scale(0.5f, 0.5f, 0.5f);
    mat4_t rotate = mat4_rotate_xyz(0.5f, 1.0f, 0.2f);  // Euler rotation
    mat4_t translate = mat4_translate(0, 0, -5);        // Move away from camera

    // Combined model transform: M = T * R * S
    mat4_t model = mat4_identity();
    for (int i = 0; i < 16; i++) {
        model.m[i] = translate.m[i];
    }

    // Apply rotation then scale manually (no mat4_mul yet)
    // Normally you'd multiply: model = translate * rotate * scale

    // Build frustum projection matrix
    float aspect = 1.0f;
    mat4_t proj = mat4_frustum_asymmetric(-1, 1, -1, 1, 1, 10);

    // Final transform: P * M (simplified)
    // Again: no mat4_mul, so we transform cube manually

    printf("Projected 2D coordinates:\n");
    for (int i = 0; i < 8; ++i) {
        // Apply model and projection
        vec3_t p = mat4_transform_vec3(model, cube[i]);
        vec3_t projected = mat4_transform_vec3(proj, p);

        // Print final 2D screen space
        printf("Vertex %d: x = %.2f, y = %.2f\n", i, projected.x, projected.y);
    }

    return 0;
}
