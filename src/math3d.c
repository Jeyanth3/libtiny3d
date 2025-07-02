#include <stdio.h>
#include <math.h>
#include "math3d.h"

// Convert spherical coordinates (r, theta, phi) to Cartesian (x, y, z)
vec3_t vec3_from_spherical(float r, float theta, float phi) {
    vec3_t v;

    // Store spherical components
    v.r = r;
    v.theta = theta;
    v.phi = phi;

    // Convert to Cartesian
    v.x = r * sinf(theta) * cosf(phi);
    v.y = r * sinf(theta) * sinf(phi);
    v.z = r * cosf(theta);

    return v;
}


// Fast inverse square root function (Quake-style)
static float Q_rsqrt(float number) {
    union {
        float f;
        uint32_t i;
    } conv;

    conv.f = number;
    conv.i = 0x5f3759df - (conv.i >> 1);
    conv.f = conv.f * (1.5f - 0.5f * number * conv.f * conv.f);
    return conv.f;
}

// Normalize a vec3 using fast inverse square root
vec3_t vec3_normalize_fast(vec3_t v) {
    float length_sq = v.x * v.x + v.y * v.y + v.z * v.z;
    float inv_len = Q_rsqrt(length_sq);

    v.x *= inv_len;
    v.y *= inv_len;
    v.z *= inv_len;

    // Optional: Update spherical after normalization
    vec3_update_spherical(&v);

    return v;
}
