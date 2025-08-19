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
            std::cerr << "Error: Tidak dapat membuka file " << filePath << std::endl;
            std::cerr << "Current working directory mungkin berbeda dari yang diharapkan." << std::endl;
            return mesh;
        }

        std::string line;
        int lineNumber = 0;
        
        while (std::getline(file, line)) {
            lineNumber++;
            
            if (line.empty() || line[0] == '#') {
                continue;
            }
            
            std::stringstream ss(line);
            std::string type;
            ss >> type;

            if (type == "v") {
                T x, y, z;
                if (ss >> x >> y >> z) {
                    mesh.vertices.emplace_back(x, y, z);
                } else {
                    std::cerr << "Warning: Invalid vertex at line " << lineNumber << std::endl;
                }
            } else if (type == "f") {
                std::vector<int> faceIndices;
                std::string vertStr;
                
                while (ss >> vertStr) {
                    // "v", "v/vt", "v/vt/vn", "v//vn"
                    size_t slashPos = vertStr.find('/');
                    std::string vertexIndexStr;
                    
                    if (slashPos != std::string::npos) {
                        vertexIndexStr = vertStr.substr(0, slashPos);
                    } else {
                        vertexIndexStr = vertStr;
                    }
                    
                    try {
                        int vertexIndex = std::stoi(vertexIndexStr) - 1;
                        
                        if (vertexIndex >= 0 && vertexIndex < static_cast<int>(mesh.vertices.size())) {
                            faceIndices.push_back(vertexIndex);
                        } else {
                            std::cerr << "Warning: index vertex invalid " << (vertexIndex + 1) 
                                      << " di line " << lineNumber << std::endl;
                        }
                    } catch (const std::exception& e) {
                        std::cerr << "Warning: format face invalid di line " << lineNumber << std::endl;
                    }
                }
                
                if (faceIndices.size() >= 3) {
                    mesh.faces.push_back(faceIndices);
                } else {
                    std::cerr << "Warning: Face dengan vertices kurang dari 3 di line " << lineNumber << std::endl;
                }
            }
            // TODO: handle tipe lain (vt, vn, dll)
        }

        file.close();
        
        std::cout << "Behasil memuat file OBJ: " << filePath << std::endl;
        std::cout << "Vertices: " << mesh.vertices.size() << std::endl;
        std::cout << "Faces: " << mesh.faces.size() << std::endl;
        
        return mesh;
    }

    template class ObjLoader<float>;
    template class ObjLoader<double>;

} // namespace graphics