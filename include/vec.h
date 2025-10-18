#pragma once

#include <cmath>

namespace simplegl
{

struct vec2_t {
    double x = 0;
    double y = 0;

    double length() {
        return std::sqrt(
            x*x +
            y*y
        );
    }

    vec2_t normalize() {
        const double len = length();
        return vec2_t {
            x/len,
            y/len
        };
    }
};

inline vec2_t operator*(vec2_t vec, double scalar) {
    return vec2_t{
        vec.x * scalar,
        vec.y * scalar
    };
}

inline vec2_t operator*(double scalar, vec2_t vec) {
    return vec * scalar;
}

inline vec2_t operator/(double scalar, vec2_t vec) {
    return vec2_t{
        vec.x / scalar,
        vec.y / scalar
    };
}

inline vec2_t operator+(vec2_t a, vec2_t b) {
    return vec2_t{
        a.x + b.x,
        a.y + b.y
    };
}

inline vec2_t operator-(vec2_t a, vec2_t b) {
    return vec2_t{
        a.x - b.x,
        a.y - b.y
    };
}

inline vec2_t operator-(vec2_t vec) {
    return vec2_t{
        -vec.x,
        -vec.y
    };
}

inline double dot(vec2_t a, vec2_t b) {
    return  (a.x * b.x) +
            (a.y * b.y);
}

struct vec3_t {
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;

    double length() {
        return std::sqrt(
            x*x +
            y*y +
            z*z
        );
    }

    vec3_t normalize() {
        const double len = length();
        return vec3_t {
            x/len,
            y/len,
            z/len,
        };
    }

    vec2_t xy() {
        return vec2_t {
            x,
            y
        };
    }
};

using point3_t = vec3_t;

struct camera_t {
    point3_t position;
    vec3_t rotation;
    double pov;
};

inline vec3_t operator*(vec3_t vec, double scalar) {
    return vec3_t{
        vec.x * scalar,
        vec.y * scalar,
        vec.z * scalar
    };
}

inline vec3_t operator*(double scalar, vec3_t vec) {
    return vec * scalar;
}

inline vec3_t operator/(double scalar, vec3_t vec) {
    return vec3_t{
        vec.x / scalar,
        vec.y / scalar,
        vec.z / scalar
    };
}

inline vec3_t operator+(vec3_t a, vec3_t b) {
    return vec3_t{
        a.x + b.x,
        a.y + b.y,
        a.z + b.z
    };
}

inline vec3_t operator-(vec3_t a, vec3_t b) {
    return vec3_t{
        a.x - b.x,
        a.y - b.y,
        a.z - b.z
    };
}

inline vec3_t operator-(vec3_t vec) {
    return vec3_t{
        -vec.x,
        -vec.y,
        -vec.z
    };
}

inline double dot(vec3_t a, vec3_t b) {
        return  (a.x * b.x) +
                (a.y * b.y) +
                (a.z * b.z);
}

inline vec3_t cross(vec3_t a, vec3_t b) {
    return vec3_t{
        (a.y * b.z) - (a.z * b.y),
        (a.z * b.x) - (a.x * b.z),
        (a.x * b.y) - (a.y * b.x)
    };
}

}