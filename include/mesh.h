#pragma once

#include <array>
#include <vector>

#include "triangle.h"
#include "vec.h"

namespace simplegl
{

using Face = std::array<int,3>;

class Mesh {

public:

    static Mesh buildSphere(unsigned int geometryLevel);

    static Mesh buildCylinder(unsigned int geometryLevel);

    std::vector<vec3_t> const & vertexes() const {
        return _vertexes;
    }

    std::vector<Face> const & faces() const {
        return _faces;
    }

    void addVertex(vec3_t const & vertex);

    void addFace(Face const & face);

    void addTriangle(Triangle const & triangle);

private:
    std::vector<vec3_t> _vertexes;
    std::vector<Face> _faces;

};

}
