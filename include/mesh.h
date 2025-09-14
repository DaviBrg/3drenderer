#pragma once

#include <vector>

#include "triangle.h"
#include "vec.h"

namespace simplegl
{

struct Face {
    unsigned int a = 0;
    unsigned int b = 0;
    unsigned int c = 0;
};

class Mesh {

public:

    std::vector<vec3_t> const & vertexes() const {
        return _vertexes;
    }

    std::vector<Face> const & faces() const {
        return _faces;
    }

    void addVertex(vec3_t const & vertex) {
        _vertexes.emplace_back(vertex);
    }

    void addFace(Face const & face) {
        _faces.emplace_back(face);
    }

    void addTriangle(Triangle triangle) {
        unsigned int numFaces = _faces.size();
        addVertex(triangle.a);
        addVertex(triangle.b);
        addVertex(triangle.c);
        addFace(Face{numFaces, numFaces + 1, numFaces + 2});
    }

private:
    std::vector<vec3_t> _vertexes;
    std::vector<Face> _faces;

};

}
