#include "objloader.h"

#include <charconv>
#include <fstream>
#include <iostream>
#include <string>
#include <ranges>

namespace
{

std::vector<std::string_view> splitStrView(std::string_view str, char delimiter) {
    std::vector<std::string_view> result;
    result.reserve(4);

    for (auto it : std::views::split(str, delimiter)) {
        result.emplace_back(std::string_view{it.begin(), it.end()});
    }

    return result;
}

std::optional<double> doubleFromStrView(std::string_view sv) {
    std::string str{sv};
    try {
        return std::stod(str);
    } catch (std::exception const & e) {
        std::cerr << "Could not convert " << sv << "to double\n";
        return std::nullopt;
    }
}

std::optional<int> intFromStrView(std::string_view sv) {
    int value;
    auto result = std::from_chars(sv.data(), sv.data() + sv.size(), value);
    if (result.ec == std::errc::invalid_argument || result.ec == std::errc::result_out_of_range) {
        std::cerr << "Could not convert " << sv << "to int\n";
        return std::nullopt;
    } else {
        return value;
    }
}

bool hasElementWithSeparator(std::vector<std::string_view> const & elements) {
    return std::any_of(elements.begin(), elements.end(), [](std::string_view sv) {
        return std::any_of(sv.cbegin(), sv.cend(), [](char c){
            return c == '/';
        });
    });
}

}

namespace simplegl
{

static std::optional<vec3_t> parseVec3FromSplitElements(std::vector<std::string_view> const & elements) {

    auto xOpt = doubleFromStrView(elements[1]);
    auto yOpt = doubleFromStrView(elements[2]);
    auto zOpt = doubleFromStrView(elements[3]);

    if (xOpt.has_value() &&
        yOpt.has_value() &&
        zOpt.has_value()) {

        return vec3_t{
            xOpt.value(),
            yOpt.value(),
            zOpt.value()
        };

    } else {
        std::cerr << "Could not parse split elements to vec3\n";
        return std::nullopt;
    }
}

static std::optional<VertexIndex> parseVertexIndexFromStrView(std::string_view sv) {

    auto indexesStrView = splitStrView(sv, '/');
    
    VertexIndex vertexIndex;

    if (indexesStrView.size() != 3) {
        goto parseFailed;
    }

    {
        auto indexVertexOpt = indexesStrView[0].empty() ? kNoIndex : intFromStrView(indexesStrView[0]);
        auto indexTextureOpt = indexesStrView[1].empty() ? kNoIndex : intFromStrView(indexesStrView[1]);
        auto indexNormalOpt = indexesStrView[2].empty() ? kNoIndex : intFromStrView(indexesStrView[2]);

        if (indexVertexOpt.has_value() &&
            indexTextureOpt.has_value() &&
            indexNormalOpt.has_value()) {

                vertexIndex.vertex = indexVertexOpt.value() - 1;
                vertexIndex.texture = indexTextureOpt.value() - 1;
                vertexIndex.normal = indexNormalOpt.value() - 1;

        } else {
            goto parseFailed;
        }
    }

    return vertexIndex;

    parseFailed:
    std::cerr << "Parsing vertex index from " << sv << " failed";
    return std::nullopt;
}

static std::optional<vec2_t> parseVec2FromSplitElements(std::vector<std::string_view> const & elements) {
    auto xOpt = doubleFromStrView(elements[1]);
    auto yOpt = doubleFromStrView(elements[2]);

    if (xOpt.has_value() &&
       yOpt.has_value()) {

        return vec2_t{
            xOpt.value(),
            yOpt.value()
        };

    } else {
        std::cerr << "Could not parse split elements to vec2\n";
        return std::nullopt;
    }
}

static std::optional<Face> parseSimpleFaceFromSplitElements(std::vector<std::string_view> const & elements) {

    auto idxOpt1 = intFromStrView(elements[1]);
    auto idxOpt2 = intFromStrView(elements[2]);
    auto idxOpt3 = intFromStrView(elements[3]);

    if (idxOpt1.has_value() &&
        idxOpt2.has_value() &&
        idxOpt3.has_value()) {

        VertexIndex v1, v2, v3;
        v1.vertex = idxOpt1.value() - 1;
        v2.vertex = idxOpt2.value() - 1;
        v3.vertex = idxOpt3.value() - 1;

        return Face {
            v1,
            v2,
            v3
        };

    } else {
        std::cerr << "Could not parse split elements to simple Face\n";
        return std::nullopt;
    }
}

static std::optional<Face> parseComplexFaceFromSplitElements(std::vector<std::string_view> const & elements) {

    if (hasElementWithSeparator(elements)) {

        auto indexOpt1 = parseVertexIndexFromStrView(elements[1]);
        auto indexOpt2 = parseVertexIndexFromStrView(elements[2]);
        auto indexOpt3 = parseVertexIndexFromStrView(elements[3]);

        if (indexOpt1.has_value() &&
            indexOpt2.has_value() &&
            indexOpt3.has_value()) {

                Face face;
                face.indexes[0] = indexOpt1.value();
                face.indexes[1] = indexOpt2.value();
                face.indexes[2] = indexOpt3.value();
                return face;

        } else {
            std::cerr << "Could not parse split elements to complex Face";
            return std::nullopt;
        }

    } else {
        return parseSimpleFaceFromSplitElements(elements);
    }
}

std::optional<Mesh> ObjLoader::load(std::string_view path) {
    auto result = Mesh{};
    auto ifs = std::ifstream{path.data()};

    std::string line;
    int lineNr = 1;
    while (std::getline(ifs, line)) {

        auto commentIt = std::find_if(line.begin(), line.end(), [](char c) {
            return c == '#';
        });

        auto lineWithoutComment = std::string_view{line.begin(), commentIt};

        auto splitElements = splitStrView(lineWithoutComment, ' ');

        if (splitElements.empty()) {
            ++lineNr;
            continue;
        }

        auto op = splitElements[0];

        if (op == "v" && splitElements.size() == 4) {

            auto optVertex = parseVec3FromSplitElements(splitElements);
            if (optVertex.has_value()) {
                result.addVertex(optVertex.value());
            } else {
                goto parseFailed;
            }

        } else
        if (op == "vt" && splitElements.size() == 3) {

            auto optTextureUV = parseVec2FromSplitElements(splitElements);
            if (optTextureUV.has_value()) {
                result.addTextureUV(optTextureUV.value());
            } else {
                goto parseFailed;
            }

        } else
        if (op == "vn" && splitElements.size() == 4) {

            auto optNormal = parseVec3FromSplitElements(splitElements);
            if (optNormal.has_value()) {
                result.addNormal(optNormal.value());
            } else {
                goto parseFailed;
            }

        } else
        if (op == "f" && (splitElements.size() == 4 || splitElements.size() == 5)) {
            auto optFace = parseComplexFaceFromSplitElements(splitElements);
            if (optFace.has_value()) {
                result.addFace(optFace.value());
            } else {
                goto parseFailed;
            }

        } else
         {
            ++lineNr;
            continue;
        }
    }
    return result;

    parseFailed:
    std::cerr << "Parsing obj file " << path << " failed on line " <<  lineNr << ": " << line << '\n';
    return std::nullopt;
}

}

