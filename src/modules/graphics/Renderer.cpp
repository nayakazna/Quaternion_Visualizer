#include "Renderer.hpp"
#include <limits>

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

        if (projected.w() == static_cast<T>(0)) {
            return math::Vector3<T>(
                std::numeric_limits<T>::quiet_NaN(),
                std::numeric_limits<T>::quiet_NaN(),
                std::numeric_limits<T>::quiet_NaN()
            );
        }

        math::Vector3<T> ndcCoords(projected.x() / projected.w(), projected.y() / projected.w(), projected.z() / projected.w());

        T screenX = (ndcCoords.x + static_cast<T>(1)) * static_cast<T>(0.5) * static_cast<T>(screenWidth);
        T screenY = (static_cast<T>(1) - ndcCoords.y) * static_cast<T>(0.5) * static_cast<T>(screenHeight);

        return math::Vector3<T>(screenX, screenY, ndcCoords.z);
    }

    template<typename T>
    void Renderer<T>::drawLine(const math::Vector3<T>& p1, const math::Vector3<T>& p2, const math::Matrix4<T>& mvpMatrix, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
        math::Vector3<T> sp1 = project(p1, mvpMatrix);
        math::Vector3<T> sp2 = project(p2, mvpMatrix);

        if (!std::isnan(sp1.x) && !std::isnan(sp1.y) && !std::isnan(sp2.x) && !std::isnan(sp2.y)) {
            SDL_SetRenderDrawColor(renderer, r, g, b, a);
            SDL_RenderDrawLine(renderer, static_cast<int>(sp1.x), static_cast<int>(sp1.y), static_cast<int>(sp2.x), static_cast<int>(sp2.y));
        }
    }

    template<typename T>
    void Renderer<T>::drawMesh(const graphics::Mesh<T>& mesh, const math::Matrix4<T>& modelMatrix, const math::Matrix4<T>& viewMatrix, const math::Matrix4<T>& projectionMatrix, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
        math::Matrix4<T> mvpMatrix = projectionMatrix * viewMatrix * modelMatrix;
        
        SDL_SetRenderDrawColor(renderer, r, g, b, a);

        for (const auto& face : mesh.faces) {
            for (size_t i = 0; i < face.size(); ++i) {
                int idx1 = face[i];
                int idx2 = face[(i + 1) % face.size()];

                math::Vector3<T> p1 = mesh.vertices[idx1];
                math::Vector3<T> p2 = mesh.vertices[idx2];
                
                math::Vector3<T> sp1 = project(p1, mvpMatrix);
                math::Vector3<T> sp2 = project(p2, mvpMatrix);

                if (!std::isnan(sp1.x) && !std::isnan(sp1.y) && !std::isnan(sp2.x) && !std::isnan(sp2.y)) {
                    SDL_RenderDrawLine(renderer, static_cast<int>(sp1.x), static_cast<int>(sp1.y),
                                       static_cast<int>(sp2.x), static_cast<int>(sp2.y));
                }
            }
        }
    }
    
    template<typename T>
    void Renderer<T>::drawAxes(const math::Matrix4<T>& viewProjectionMatrix) {
        drawLine(math::Vector3<T>(0, 0, 0), math::Vector3<T>(1, 0, 0), viewProjectionMatrix, 255, 0, 0, 255);
        drawLine(math::Vector3<T>(0, 0, 0), math::Vector3<T>(0, 1, 0), viewProjectionMatrix, 0, 255, 0, 255);
        drawLine(math::Vector3<T>(0, 0, 0), math::Vector3<T>(0, 0, 1), viewProjectionMatrix, 0, 0, 255, 255);
    }
    
    template class Renderer<float>;
    template class Renderer<double>;

} // namespace graphics
