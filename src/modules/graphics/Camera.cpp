// Camera.cpp
#include "Camera.hpp"
#include <stdexcept>

namespace graphics {
    template<typename T>
    Camera<T>::Camera(const math::Vector3<T>& pos, const math::Vector3<T>& target, const math::Vector3<T>& up) :
        position(pos), target(target), up(up.normalize()) {
        updateVectors();
    }
    
    template<typename T>
    void Camera<T>::update(T deltaTime) {
// TODO: ini ntaran
    }

    template<typename T>
    void Camera<T>::setPosition(const math::Vector3<T>& pos) {
        position = pos;
        updateVectors();
    }
    
    template<typename T>
    void Camera<T>::setTarget(const math::Vector3<T>& target) {
        this->target = target;
        updateVectors();
    }
    
    template<typename T>
    void Camera<T>::setUp(const math::Vector3<T>& up) {
        this->up = up.normalize();
        updateVectors();
    }

    template<typename T>
    math::Matrix4<T> Camera<T>::getViewMatrix() const {
        return math::Matrix4<T>::lookAt(position, target, up);
    }
    
    template<typename T>
    math::Matrix4<T> Camera<T>::getProjectionMatrix(int screenWidth, int screenHeight) const {
        T fovY = static_cast<T>(45.0) * static_cast<T>(3.141592653589793) / static_cast<T>(180.0);
        T aspect = static_cast<T>(screenWidth) / static_cast<T>(screenHeight);
        T nearZ = static_cast<T>(0.1);
        T farZ = static_cast<T>(1000.0);
        return math::Matrix4<T>::perspective(fovY, aspect, nearZ, farZ);
    }

    template<typename T>
    void Camera<T>::updateVectors() {
// TODO: ini ntaran
    }

    template class Camera<float>;
    template class Camera<double>;
    
} // namespace graphics
