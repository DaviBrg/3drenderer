#include "objloader.h"

#include <fstream>
#include <iostream>
#include <string>
#include <ranges>

namespace simplegl
{
    
std::optional<Mesh> ObjLoader::load(std::string_view path)
{
    auto result = Mesh{};
    auto ifs = std::ifstream{path.data()};

    char op = 0;
    double x, y, z = 0;
    int f0, f1, f2 = 0;

    while( ifs >> op ) {
    
        switch (op)
        {

        case 'v':
            {
                ifs >> x;
                ifs >> y;
                ifs >> z;

                const auto vertex = vec3_t{
                    x,
                    y,
                    z
                };
                result.addVertex(vertex);
            }
            break;
    
            case 'f':
            {
                ifs >> f0;
                ifs >> f1;
                ifs >> f2;

                const Face face = {
                    f0 - 1,
                    f1 - 1,
                    f2 - 1
                };

                result.addFace(face);
    
            }
            break;
        
        default:
            return std::nullopt;
        }

    }

    return result;
}

}

