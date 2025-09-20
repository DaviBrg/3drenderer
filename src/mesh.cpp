#include "mesh.h"

#include <numbers>

namespace
{

simplegl::vec3_t getPointFromUV(double u, double v) {
    const double cosU = std::cos(u);
    return simplegl::vec3_t{
        cosU*std::cos(v),
        std::sin(u),
        cosU*std::sin(v)};
}

}

namespace simplegl
{

const int kNoIndex = -1;

Mesh Mesh::buildSphere(unsigned int geometryLevel)
{
    auto result = simplegl::Mesh();

    if (geometryLevel < 2) geometryLevel = 2;

    constexpr double uStart = std::numbers::pi*0.5;
    constexpr double vStart = 0.0;
    double step = (std::numbers::pi)/geometryLevel;

    double previousU = uStart;
    double previousV = vStart;

    for (unsigned int uIdx = 1; uIdx <= geometryLevel; ++uIdx) {

        const double u = uStart - (uIdx*step);

        for (unsigned int vIdx = 1; vIdx <= geometryLevel*2; ++vIdx) {

            const double v = vStart + (vIdx*step);
            result.addSimpleTriangle(getPointFromUV(previousU, previousV), getPointFromUV(previousU, v), getPointFromUV(u, v));
            result.addSimpleTriangle(getPointFromUV(u, v), getPointFromUV(u, previousV), getPointFromUV(previousU, previousV));

            previousV = v;
        }
        previousU = u;
    }

    return result;
}

Mesh Mesh::buildCylinder(unsigned int geometryLevel)
{
    simplegl::Mesh result;

    if (geometryLevel < 3) geometryLevel = 3;

    constexpr double x0 = 1.0; //cos(0)
    constexpr double z0 = 0.0; //sin(0)
    
    const double cylinderSize = std::sqrt(2)/2;
    const double step = (std::numbers::pi*2.0)/geometryLevel;
    const auto upOrigin = simplegl::vec3_t{0.0, cylinderSize, 0.0};
    const auto downOrigin = simplegl::vec3_t{0.0, -cylinderSize, 0.0};
    
    auto up0 = simplegl::vec3_t{x0, cylinderSize, z0};
    auto down0 = simplegl::vec3_t{x0, -cylinderSize, z0};

    for (unsigned int i = 1; i <= geometryLevel; ++i) {

        const double u = i*step;

        const double x1 = std::cos(u);
        const double z1 = std::sin(u);

        const auto up1 = simplegl::vec3_t{x1, cylinderSize, z1};
        const auto down1 = simplegl::vec3_t{x1, -cylinderSize, z1};

        result.addSimpleTriangle(upOrigin, up1, up0);
        result.addSimpleTriangle(downOrigin, down0, down1);
        result.addSimpleTriangle(up0, up1, down1);
        result.addSimpleTriangle(down1, down0, up0);

        up0 = up1;
        down0 = down1;

    }
    return result;
}

void Mesh::addVertex(vec3_t const & vertex) {
    _vertexes.emplace_back(vertex);
}

void Mesh::addTextureUV(vec2_t const & textureUV) {
    _textureUVs.emplace_back(textureUV);
}

void Mesh::addNormal(vec3_t const & normal) {
    _normals.emplace_back(normal);
}

void Mesh::addFace(Face const & face) {
    _faces.emplace_back(face);
}

void Mesh::addSimpleTriangle(vec3_t v1, vec3_t v2, vec3_t v3) {
    int numFaces = static_cast<int>(_vertexes.size());

    addVertex(v1);
    addVertex(v2);
    addVertex(v3);

    Face face;
    face.indexes[0].vertex = numFaces;
    face.indexes[1].vertex = numFaces + 1;
    face.indexes[2].vertex = numFaces + 2;

    addFace(face);

}

}