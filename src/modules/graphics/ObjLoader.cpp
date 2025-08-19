// ObjLoader.cpp
#include "ObjLoader.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

namespace graphics {
    template<typename T>
    Mesh<T> ObjLoader<T>::loadObj(const std::string& filePath) {
        Mesh<T> mesh;
        std::ifstream file(filePath);

        if (!file.is_open()) {
            std::cerr << "Error: Gabisa buka file " << filePath << std::endl;
            return mesh;
        }

        std::string line;
        while (std::getline(file, line)) {
            std::stringstream ss(line);
            std::string type;
            ss >> type;

            if (type == "v") {
                T x, y, z;
                ss >> x >> y >> z;
                mesh.vertices.emplace_back(x, y, z);
            } else if (type == "f") {
                std::vector<int> faceIndices;
                std::string vertStr;
                while (ss >> vertStr) {
                    size_t slashPos = vertStr.find('/');
                    if (slashPos != std::string::npos) {
                        faceIndices.push_back(std::stoi(vertStr.substr(0, slashPos)) - 1);
                    } else {
                        faceIndices.push_back(std::stoi(vertStr) - 1);
                    }
                }
                mesh.faces.push_back(faceIndices);
            }
        }

        file.close();
        return mesh;
    }

    template class ObjLoader<float>;
    template class ObjLoader<double>;

} // namespace graphics
