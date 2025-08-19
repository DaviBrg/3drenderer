#ifndef SIMPLE_GL_VECTOR_H
#define SIMPLE_GL_VECTOR_H

typedef struct {
    float x;
    float y;
} vec2_t;

typedef vec2_t point2_t;

typedef struct {
    float x;
    float y;
    float z;
} vec3_t;

typedef vec3_t point3_t;

typedef struct {
    point3_t position;
    vec3_t rotation;
    float pov;
} camera_t;


#endif