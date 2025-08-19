#pragma once
#include <string>
#include "Mesh.hpp"

namespace graphics {
    template<typename T>
    class ObjLoader {
    public:
        static Mesh<T> loadObj(const std::string& filePath);
    };
} // namespace graphics