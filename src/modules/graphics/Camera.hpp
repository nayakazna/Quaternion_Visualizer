#pragma once
#include "../math/Vector3.hpp"
#include "../math/Matrix4.hpp"
#include <SDL.h>

namespace graphics {
    template<typename T>
    class Camera {
    public:
        Camera(const math::Vector3<T>& pos, const math::Vector3<T>& target, const math::Vector3<T>& up);
        ~Camera() {}

        void update(T deltaTime);
        void handleMouseMovement(T xoffset, T yoffset);
        void handleKeyboard(const Uint8* state, T deltaTime);

        math::Matrix4<T> getViewMatrix() const;
        math::Matrix4<T> getProjectionMatrix(int screenWidth, int screenHeight) const;

    private:
        math::Vector3<T> position;
        math::Vector3<T> front;
        math::Vector3<T> up;
        math::Vector3<T> right;
        math::Vector3<T> worldUp;

        T yaw;  
        T pitch;

        T movementSpeed;
        T mouseSensitivity;

        void updateVectors();
    };
} // namespace graphics
