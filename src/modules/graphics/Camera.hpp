#pragma once
#include "../math/Vector3.hpp"
#include "../math/Matrix4.hpp"

namespace graphics {
    template<typename T>
    class Camera {
    public:
        Camera(const math::Vector3<T>& pos, const math::Vector3<T>& target, const math::Vector3<T>& up);
        ~Camera() {}

        void update(T deltaTime);
        void setPosition(const math::Vector3<T>& pos);
        void setTarget(const math::Vector3<T>& target);
        void setUp(const math::Vector3<T>& up);

        math::Matrix4<T> getViewMatrix() const;
        math::Matrix4<T> getProjectionMatrix(int screenWidth, int screenHeight) const;

    private:
        math::Vector3<T> position;
        math::Vector3<T> target;
        math::Vector3<T> up;
        
        void updateVectors();
    };
} // namespace graphics