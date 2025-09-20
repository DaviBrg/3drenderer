#pragma once

#include <array>
#include <vector>

#include "triangle.h"
#include "vec.h"

namespace simplegl
{

extern const int kNoIndex;

struct VertexIndex {
    int vertex = 0;
    int texture = 0;
    int normal = 0;
};

struct Face {
    std::array<VertexIndex,3> indexes = {};
};

class Mesh {

public:

    static Mesh buildSphere(unsigned int geometryLevel);

    static Mesh buildCylinder(unsigned int geometryLevel);

    std::vector<vec3_t> const & vertices() const {
        return _vertexes;
    }
    
    std::vector<vec2_t> const & textureUVs() const {
        return _textureUVs;
    }
    
    std::vector<vec3_t> const & normals() const {
        return _normals;
    }
    
    std::vector<Face> const & faces() const {
        return _faces;
    }
    
    void addVertex(vec3_t const & vertex);

    void addTextureUV(vec2_t const & textureUV);

    void addNormal(vec3_t const & normal);
    
    void addFace(Face const & face);

    void addSimpleTriangle(vec3_t v1, vec3_t v2, vec3_t v3);

private:
    std::vector<vec3_t> _vertexes;
    std::vector<vec2_t> _textureUVs;
    std::vector<vec3_t> _normals;
    std::vector<Face> _faces;

};

}
