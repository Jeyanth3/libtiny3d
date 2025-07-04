#include <math.h>
#include <stdint.h>
#include "math3d.h"

/// ─────────────────────────────────────────────────────────────
/// VEC3 IMPLEMENTATION
/// ─────────────────────────────────────────────────────────────

void vec3_update_spherical(vec3_t* v) {
    v->r = sqrtf(v->x * v->x + v->y * v->y + v->z * v->z);
    if (v->r == 0.0f) {
        v->theta = 0.0f;
        v->phi = 0.0f;
    } else {
        v->theta = acosf(v->z / v->r);
        v->phi = atan2f(v->y, v->x);
    }
}

vec3_t vec3_from_spherical(float r, float theta, float phi) {
    vec3_t v;

    v.r = r;
    v.theta = theta;
    v.phi = phi;

    v.x = r * sinf(theta) * cosf(phi);
    v.y = r * sinf(theta) * sinf(phi);
    v.z = r * cosf(theta);

    return v;
}

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

vec3_t vec3_normalize_fast(vec3_t v) {
    float len_sq = v.x * v.x + v.y * v.y + v.z * v.z;
    float inv_len = Q_rsqrt(len_sq);

    v.x *= inv_len;
    v.y *= inv_len;
    v.z *= inv_len;

    vec3_update_spherical(&v);
    return v;
}

vec3_t vec3_slerp(vec3_t a, vec3_t b, float t) {
    a = vec3_normalize_fast(a);
    b = vec3_normalize_fast(b);

    float dot = a.x * b.x + a.y * b.y + a.z * b.z;
    if (dot > 1.0f) dot = 1.0f;
    if (dot < -1.0f) dot = -1.0f;

    float theta = acosf(dot) * t;

    vec3_t relative = {
        .x = b.x - a.x * dot,
        .y = b.y - a.y * dot,
        .z = b.z - a.z * dot,
        .r = 0, .theta = 0, .phi = 0
    };
    relative = vec3_normalize_fast(relative);

    vec3_t result = {
        .x = a.x * cosf(theta) + relative.x * sinf(theta),
        .y = a.y * cosf(theta) + relative.y * sinf(theta),
        .z = a.z * cosf(theta) + relative.z * sinf(theta),
        .r = 0, .theta = 0, .phi = 0
    };
    return result;
}

/// ─────────────────────────────────────────────────────────────
/// MAT4 IMPLEMENTATION
/// ─────────────────────────────────────────────────────────────

mat4_t mat4_identity() {
    mat4_t m = {0};
    m.m[0] = m.m[5] = m.m[10] = m.m[15] = 1.0f;
    return m;
}

mat4_t mat4_translate(float tx, float ty, float tz) {
    mat4_t m = mat4_identity();
    m.m[12] = tx;
    m.m[13] = ty;
    m.m[14] = tz;
    return m;
}

mat4_t mat4_scale(float sx, float sy, float sz) {
    mat4_t m = {0};
    m.m[0] = sx;
    m.m[5] = sy;
    m.m[10] = sz;
    m.m[15] = 1.0f;
    return m;
}

mat4_t mat4_rotate_xyz(float rx, float ry, float rz) {
    float cx = cosf(rx), sx = sinf(rx);
    float cy = cosf(ry), sy = sinf(ry);
    float cz = cosf(rz), sz = sinf(rz);

    mat4_t m = {0};

    m.m[0]  = cy * cz;
    m.m[1]  = cx * sz + sx * sy * cz;
    m.m[2]  = sx * sz - cx * sy * cz;
    m.m[4]  = -cy * sz;
    m.m[5]  = cx * cz - sx * sy * sz;
    m.m[6]  = sx * cz + cx * sy * sz;
    m.m[8]  = sy;
    m.m[9]  = -sx * cy;
    m.m[10] = cx * cy;
    m.m[15] = 1.0f;

    return m;
}

mat4_t mat4_frustum_asymmetric(float l, float r, float b, float t, float n, float f) {
    mat4_t m = {0};

    m.m[0]  = (2 * n) / (r - l);
    m.m[5]  = (2 * n) / (t - b);
    m.m[8]  = (r + l) / (r - l);
    m.m[9]  = (t + b) / (t - b);
    m.m[10] = -(f + n) / (f - n);
    m.m[11] = -1.0f;
    m.m[14] = -(2 * f * n) / (f - n);

    return m;
}

vec3_t mat4_transform_vec3(mat4_t m, vec3_t v) {
    vec3_t result;
    float x = v.x, y = v.y, z = v.z;

    float w = m.m[3] * x + m.m[7] * y + m.m[11] * z + m.m[15];
    if (w == 0.0f) w = 1.0f;

    result.x = (m.m[0] * x + m.m[4] * y + m.m[8]  * z + m.m[12]) / w;
    result.y = (m.m[1] * x + m.m[5] * y + m.m[9]  * z + m.m[13]) / w;
    result.z = (m.m[2] * x + m.m[6] * y + m.m[10] * z + m.m[14]) / w;

    vec3_update_spherical(&result);
    return result;
}
