#pragma once

#include "vec.h"

namespace simplegl
{

struct Triangle {

    Triangle(vec3_t a, vec3_t b, vec3_t c) :
    a{a}, b{b}, c{c} {}

    vec3_t a;
    vec3_t b;
    vec3_t c;
};

}