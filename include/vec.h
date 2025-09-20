#pragma once

#include <tuple>

namespace simplegl
{

struct vec2_t {
    double x = 0;
    double y = 0;
};

using point2_t = vec2_t ;

struct vec3_t {
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;
};

using point3_t = vec3_t;

struct camera_t {
    point3_t position;
    vec3_t rotation;
    double pov;
};

inline vec3_t operator+(vec3_t lhs, vec3_t rhs) {
    return vec3_t{
        lhs.x + rhs.x,
        lhs.y + rhs.y,
        lhs.z + rhs.z
    };
}

inline vec3_t operator-(vec3_t lhs, vec3_t rhs) {
    return vec3_t{
        lhs.x - rhs.x,
        lhs.y - rhs.y,
        lhs.z - rhs.z
    };
}

}