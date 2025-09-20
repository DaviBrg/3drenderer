#pragma once

#include <optional>
#include <string_view>

#include "mesh.h"

namespace simplegl
{

class ObjLoader {

public:

    static std::optional<Mesh> load(std::string_view path);

};

}

