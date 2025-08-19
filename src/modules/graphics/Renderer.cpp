#include "Renderer.hpp"
#include <limits>
#include <iostream>

namespace graphics {
    template<typename T>
    Renderer<T>::Renderer(SDL_Renderer* renderer, int screenWidth, int screenHeight) :
        renderer(renderer), screenWidth(screenWidth), screenHeight(screenHeight) {}

    template<typename T>
    void Renderer<T>::clearScreen(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
        SDL_SetRenderDrawColor(renderer, r, g, b, a);
        SDL_RenderClear(renderer);
    }

    template<typename T>
    void Renderer<T>::present() {
        SDL_RenderPresent(renderer);
    }

    template<typename T>
    math::Vector3<T> Renderer<T>::project(const math::Vector3<T>& worldPoint, const math::Matrix4<T>& mvpMatrix) const {
        math::Vector4<T> clipCoords(worldPoint.x, worldPoint.y, worldPoint.z, static_cast<T>(1));
        math::Vector4<T> projected = mvpMatrix * clipCoords;

        if (std::abs(projected.w()) < std::numeric_limits<T>::epsilon()) {
            return math::Vector3<T>(
                std::numeric_limits<T>::quiet_NaN(),
                std::numeric_limits<T>::quiet_NaN(),
                std::numeric_limits<T>::quiet_NaN()
            );
        }

        math::Vector3<T> ndcCoords(
            projected.x() / projected.w(), 
            projected.y() / projected.w(), 
            projected.z() / projected.w()
        );

        T screenX = (ndcCoords.x + static_cast<T>(1)) * static_cast<T>(0.5) * static_cast<T>(screenWidth);
        T screenY = (static_cast<T>(1) - ndcCoords.y) * static_cast<T>(0.5) * static_cast<T>(screenHeight);

        return math::Vector3<T>(screenX, screenY, ndcCoords.z);
    }

    template<typename T>
    bool Renderer<T>::isValidScreenPoint(const math::Vector3<T>& screenPoint) const {
        return !std::isnan(screenPoint.x) && !std::isnan(screenPoint.y) &&
               screenPoint.x >= 0 && screenPoint.x < screenWidth &&
               screenPoint.y >= 0 && screenPoint.y < screenHeight &&
               screenPoint.z >= -1 && screenPoint.z <= 1; 
    }

    template<typename T>
    int Renderer<T>::computeOutCode(const math::Vector3<T>& point) const {
        int code = 0;
        if (point.x < 0) code |= 1;        
        if (point.x >= screenWidth) code |= 2;   
        if (point.y < 0) code |= 4;        
        if (point.y >= screenHeight) code |= 8;  
        return code;
    }

    template<typename T>
    bool Renderer<T>::clipLine(math::Vector3<T>& p1, math::Vector3<T>& p2) const {
        if (std::isnan(p1.x) || std::isnan(p1.y) || std::isnan(p2.x) || std::isnan(p2.y)) {
            return false;
        }

        int outcode1 = computeOutCode(p1);
        int outcode2 = computeOutCode(p2);

        while (true) {
            if ((outcode1 | outcode2) == 0) {
                return true; 
            }
            if ((outcode1 & outcode2) != 0) {
                return false; 
            }

            int outcodeOut = outcode1 ? outcode1 : outcode2;
            T x, y;

            if (outcodeOut & 8) {
                x = p1.x + (p2.x - p1.x) * (screenHeight - 1 - p1.y) / (p2.y - p1.y);
                y = screenHeight - 1;
            } else if (outcodeOut & 4) {
                x = p1.x + (p2.x - p1.x) * (0 - p1.y) / (p2.y - p1.y);
                y = 0;
            } else if (outcodeOut & 2) {
                y = p1.y + (p2.y - p1.y) * (screenWidth - 1 - p1.x) / (p2.x - p1.x);
                x = screenWidth - 1;
            } else if (outcodeOut & 1) {
                y = p1.y + (p2.y - p1.y) * (0 - p1.x) / (p2.x - p1.x);
                x = 0;
            }

            if (outcodeOut == outcode1) {
                p1.x = x; p1.y = y;
                outcode1 = computeOutCode(p1);
            } else {
                p2.x = x; p2.y = y;
                outcode2 = computeOutCode(p2);
            }
        }
    }

    template<typename T>
    bool Renderer<T>::clipToNearPlane(const math::Vector3<T>& worldP1, const math::Vector3<T>& worldP2, 
                                    const math::Matrix4<T>& viewMatrix, const math::Matrix4<T>& modelMatrix,
                                    math::Vector3<T>& clippedP1, math::Vector3<T>& clippedP2) const {
        math::Matrix4<T> modelViewMatrix = viewMatrix * modelMatrix;
        
        math::Vector4<T> viewP1 = modelViewMatrix * math::Vector4<T>(worldP1.x, worldP1.y, worldP1.z, 1.0f);
        math::Vector4<T> viewP2 = modelViewMatrix * math::Vector4<T>(worldP2.x, worldP2.y, worldP2.z, 1.0f);
        
        T nearPlane = static_cast<T>(-0.1);
        
        bool p1Behind = viewP1.z() > nearPlane; 
        bool p2Behind = viewP2.z() > nearPlane;
        
        if (p1Behind && p2Behind) {
            return false; 
        }
        
        if (!p1Behind && !p2Behind) {
            clippedP1 = worldP1;
            clippedP2 = worldP2;
            return true;
        }
        
        T t = (nearPlane - viewP1.z()) / (viewP2.z() - viewP1.z());
        
        math::Vector3<T> intersection(
            worldP1.x + t * (worldP2.x - worldP1.x),
            worldP1.y + t * (worldP2.y - worldP1.y), 
            worldP1.z + t * (worldP2.z - worldP1.z)
        );
        
        if (p1Behind) {
            clippedP1 = intersection;
            clippedP2 = worldP2;
        } else {
            clippedP1 = worldP1;
            clippedP2 = intersection;
        }
        return true;
    }

    template<typename T>
    void Renderer<T>::drawLine(const math::Vector3<T>& p1, const math::Vector3<T>& p2, const math::Matrix4<T>& mvpMatrix, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
        math::Matrix4<T> identityModel = math::Matrix4<T>::identity();

        math::Vector3<T> clippedWorldP1 = p1;
        math::Vector3<T> clippedWorldP2 = p2;
        
        math::Vector3<T> sp1 = project(clippedWorldP1, mvpMatrix);
        math::Vector3<T> sp2 = project(clippedWorldP2, mvpMatrix);

        if (clipLine(sp1, sp2)) {
            SDL_SetRenderDrawColor(renderer, r, g, b, a);
            SDL_RenderDrawLine(renderer, 
                static_cast<int>(sp1.x), static_cast<int>(sp1.y), 
                static_cast<int>(sp2.x), static_cast<int>(sp2.y));
        }
    }

    template<typename T>
    void Renderer<T>::drawMesh(const graphics::Mesh<T>& mesh, const math::Matrix4<T>& modelMatrix, const math::Matrix4<T>& viewMatrix, const math::Matrix4<T>& projectionMatrix, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
        math::Matrix4<T> mvpMatrix = projectionMatrix * viewMatrix * modelMatrix;
        std::vector<SDL_Point> points;
        points.reserve(mesh.faces.size() * 6);
        SDL_SetRenderDrawColor(renderer, r, g, b, a);

        for (const auto& face : mesh.faces) {
            if (face.size() < 3) continue;
            
            for (size_t i = 0; i < face.size(); ++i) {
                int idx1 = face[i];
                int idx2 = face[(i + 1) % face.size()];

                if (idx1 < 0 || idx1 >= static_cast<int>(mesh.vertices.size()) ||
                    idx2 < 0 || idx2 >= static_cast<int>(mesh.vertices.size())) {
                    std::cerr << "Index vertex invalid di face: " << idx1 << " atau " << idx2 << std::endl;
                    continue;
                }

                math::Vector3<T> p1 = mesh.vertices[idx1];
                math::Vector3<T> p2 = mesh.vertices[idx2];
                
                // near-plane clipping
                math::Vector3<T> clippedWorldP1, clippedWorldP2;
                if (!clipToNearPlane(p1, p2, viewMatrix, modelMatrix, clippedWorldP1, clippedWorldP2)) {
                    continue;
                }
                
                math::Vector3<T> sp1 = project(clippedWorldP1, mvpMatrix);
                math::Vector3<T> sp2 = project(clippedWorldP2, mvpMatrix);

                // screen-space clipping
                if (clipLine(sp1, sp2)) {
                    SDL_RenderDrawLine(renderer, 
                        static_cast<int>(sp1.x), static_cast<int>(sp1.y),
                        static_cast<int>(sp2.x), static_cast<int>(sp2.y));
                }
                
                points.push_back({static_cast<int>(sp1.x), static_cast<int>(sp1.y)});
                points.push_back({static_cast<int>(sp2.x), static_cast<int>(sp2.y)});
            }
        }
    SDL_RenderDrawLines(renderer, points.data(), points.size());
    }

    template<typename T>
    void Renderer<T>::drawAxes(const math::Matrix4<T>& viewProjectionMatrix) {
        // x - merah
        drawLine(math::Vector3<T>(0, 0, 0), math::Vector3<T>(200, 0, 0), viewProjectionMatrix, 255, 0, 0, 255);
        // y - ijo
        drawLine(math::Vector3<T>(0, 0, 0), math::Vector3<T>(0, 200, 0), viewProjectionMatrix, 0, 255, 0, 255);
        // z - biru
        drawLine(math::Vector3<T>(0, 0, 0), math::Vector3<T>(0, 0, 200), viewProjectionMatrix, 0, 0, 255, 255);
    }
    
    template class Renderer<float>;
    template class Renderer<double>;

} // namespace graphics