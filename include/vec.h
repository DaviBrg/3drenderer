#pragma once

#include <tuple>

namespace simplegl
{

struct vec2_t {
    double x;
    double y;
};

using point2_t = vec2_t ;

struct vec3_t {
    double x;
    double y;
    double z;
};

using point3_t = vec3_t;

struct camera_t {
    point3_t position;
    vec3_t rotation;
    double pov;
};

}