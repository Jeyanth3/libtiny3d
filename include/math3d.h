#ifndef MATH3D_H
#define MATH3D_H

typedef struct {
    // Cartesian
    float x, y, z;

    // Spherical
    float r;       // radius
    float theta;   // polar angle [0, π]
    float phi;     // azimuthal angle [0, 2π]
} vec3_t;

// Create from Cartesian
vec3_t vec3_from_cartesian(float x, float y, float z);

// Create from Spherical
vec3_t vec3_from_spherical(float r, float theta, float phi);

// Sync functions
void vec3_update_spherical(vec3_t* v);
void vec3_update_cartesian(vec3_t* v);

#endif
