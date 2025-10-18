#pragma once

#include <cmath>

namespace simplegl
{

struct vec2_t;
double length(vec2_t const & vec);

struct vec2_t {
    double x = 0;
    double y = 0;

    vec2_t& normalize() {
        const double len = length(*this);
        x /= len;
        y /= len;
        return *this;
    }
};

inline double length(vec2_t const & vec) {
    return std::sqrt(
            vec.x*vec.x +
            vec.y*vec.y
        );
}

inline vec2_t operator*(vec2_t const & vec, double scalar) {
    return vec2_t{
        vec.x * scalar,
        vec.y * scalar
    };
}

inline vec2_t operator*(double scalar, vec2_t const & vec) {
    return vec * scalar;
}

inline vec2_t operator/(double scalar, vec2_t const & vec) {
    return vec2_t{
        vec.x / scalar,
        vec.y / scalar
    };
}

inline vec2_t operator+(vec2_t const & a, vec2_t const & b) {
    return vec2_t{
        a.x + b.x,
        a.y + b.y
    };
}

inline vec2_t operator-(vec2_t const & a, vec2_t const & b) {
    return vec2_t{
        a.x - b.x,
        a.y - b.y
    };
}

inline vec2_t operator-(vec2_t const & vec) {
    return vec2_t{
        -vec.x,
        -vec.y
    };
}

inline vec2_t normalize(vec2_t const & vec) {
    vec2_t normal = vec;
    return normal.normalize();
}

inline double dot(vec2_t const & a, vec2_t const & b) {
    return  (a.x * b.x) +
            (a.y * b.y);
}

struct vec3_t;
double length(vec3_t const & vec);

struct vec3_t {
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;

    vec3_t & normalize() {
        const double len = length(*this);
        x /= len;
        y /= len;
        z /= len;
        return *this;
    }

    vec2_t xy() {
        return vec2_t {
            x,
            y
        };
    }
};

using point3_t = vec3_t;

inline double length(vec3_t const & vec) {
    return std::sqrt(
            vec.x*vec.x +
            vec.y*vec.y +
            vec.z*vec.z
        );
}

struct camera_t {
    point3_t position;
    vec3_t rotation;
    double pov;
};

inline vec3_t operator*(vec3_t const & vec, double scalar) {
    return vec3_t{
        vec.x * scalar,
        vec.y * scalar,
        vec.z * scalar
    };
}

inline vec3_t operator*(double scalar, vec3_t const & vec) {
    return vec * scalar;
}

inline vec3_t operator/(double scalar, vec3_t const & vec) {
    return vec3_t{
        vec.x / scalar,
        vec.y / scalar,
        vec.z / scalar
    };
}

inline vec3_t operator+(vec3_t const & a, vec3_t const & b) {
    return vec3_t{
        a.x + b.x,
        a.y + b.y,
        a.z + b.z
    };
}

inline vec3_t operator-(vec3_t const & a, vec3_t const & b) {
    return vec3_t{
        a.x - b.x,
        a.y - b.y,
        a.z - b.z
    };
}

inline vec3_t operator-(vec3_t const & vec) {
    return vec3_t{
        -vec.x,
        -vec.y,
        -vec.z
    };
}

inline vec3_t normalize(vec3_t const & vec) {
    vec3_t normal = vec;
    return normal.normalize();
}

inline double dot(vec3_t const & a, vec3_t const & b) {
        return  (a.x * b.x) +
                (a.y * b.y) +
                (a.z * b.z);
}

inline vec3_t cross(vec3_t const & a, vec3_t const & b) {
    return vec3_t{
        (a.y * b.z) - (a.z * b.y),
        (a.z * b.x) - (a.x * b.z),
        (a.x * b.y) - (a.y * b.x)
    };
}

}