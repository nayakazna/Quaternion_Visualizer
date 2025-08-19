#pragma once
#include <SDL.h>
#include "../math/Matrix4.hpp"
#include "../math/Vector2.hpp"
#include "../math/Vector3.hpp"
#include "../graphics/Mesh.hpp"

namespace graphics {
    template<typename T>
    class Renderer {
    public:
        Renderer(SDL_Renderer* renderer, int screenWidth, int screenHeight);
        
        void clearScreen(Uint8 r, Uint8 g, Uint8 b, Uint8 a);
        void present();
        math::Vector3<T> project(const math::Vector3<T>& worldPoint, const math::Matrix4<T>& mvpMatrix) const;
        void drawLine(const math::Vector3<T>& p1, const math::Vector3<T>& p2, const math::Matrix4<T>& mvpMatrix, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
        void drawMesh(const graphics::Mesh<T>& mesh, const math::Matrix4<T>& modelMatrix, const math::Matrix4<T>& viewMatrix, const math::Matrix4<T>& projectionMatrix, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
        void drawAxes(const math::Matrix4<T>& viewProjectionMatrix);
        
    private:

        SDL_Renderer* renderer;
        int screenWidth;
        int screenHeight;
    };
} // namespace graphics