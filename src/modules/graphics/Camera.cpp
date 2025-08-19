#include "Camera.hpp"
#include <stdexcept>

namespace graphics {
    template<typename T>
    Camera<T>::Camera(const math::Vector3<T>& pos, const math::Vector3<T>& target, const math::Vector3<T>& up) :
        position(pos), worldUp(up.normalize()), front(0.0f, 0.0f, -1.0f), yaw(-90.0f), pitch(0.0f), movementSpeed(20.5f), mouseSensitivity(0.1f) {
        updateVectors();
    }
    
    template<typename T>
    void Camera<T>::update(T deltaTime) {
    }
    
    template<typename T>
    void Camera<T>::handleKeyboard(const Uint8* state, T deltaTime) {
        T velocity = movementSpeed * deltaTime;
        if (state[SDL_SCANCODE_W]) {
            position = position + front * velocity;
        }
        if (state[SDL_SCANCODE_S]) {
            position = position - front * velocity;
        }
        if (state[SDL_SCANCODE_A]) {
            position = position - right * velocity;
        }
        if (state[SDL_SCANCODE_D]) {
            position = position + right * velocity;
        }
        if (state[SDL_SCANCODE_Q]) {
            position = position - worldUp * velocity;
        }
        if (state[SDL_SCANCODE_E]) {
            position = position + worldUp * velocity;
        }
    }

    template<typename T>
    void Camera<T>::handleMouseMovement(T xoffset, T yoffset) {
        xoffset *= mouseSensitivity;
        yoffset *= mouseSensitivity;

        yaw += xoffset;
        pitch += yoffset;

        if (pitch > 89.0f) {
            pitch = 89.0f;
        }
        if (pitch < -89.0f) {
            pitch = -89.0f;
        }

        updateVectors();
    }

    template<typename T>
    math::Matrix4<T> Camera<T>::getViewMatrix() const {
        return math::Matrix4<T>::lookAt(position, position + front, up);
    }
    
    template<typename T>
    math::Matrix4<T> Camera<T>::getProjectionMatrix(int screenWidth, int screenHeight) const {
        T fovY = static_cast<T>(45.0) * static_cast<T>(3.141592653589793) / static_cast<T>(180.0);
        T aspect = static_cast<T>(screenWidth) / static_cast<T>(screenHeight);
        T nearZ = static_cast<T>(0.1);
        T farZ = static_cast<T>(100.0);
        return math::Matrix4<T>::perspective(fovY, aspect, nearZ, farZ);
    }

    template<typename T>
    void Camera<T>::updateVectors() {
        T yawRad = yaw * static_cast<T>(3.141592653589793) / static_cast<T>(180.0);
        T pitchRad = pitch * static_cast<T>(3.141592653589793) / static_cast<T>(180.0);

        math::Vector3<T> newFront;
        newFront.x = std::cos(yawRad) * std::cos(pitchRad);
        newFront.y = std::sin(pitchRad);
        newFront.z = std::sin(yawRad) * std::cos(pitchRad);
        front = newFront.normalize();
        
        right = front.cross(worldUp).normalize();
        up = right.cross(front).normalize();
    }

    template class Camera<float>;
    template class Camera<double>;
    
} // namespace graphics
