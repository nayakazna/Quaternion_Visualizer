#pragma once
#include <vector>
#include "../math/Vector3.hpp"

namespace graphics {
    template<typename T>
    struct Mesh {
        std::vector<math::Vector3<T>> vertices;
        std::vector<std::vector<int>> faces;
    };
} // namespace graphics