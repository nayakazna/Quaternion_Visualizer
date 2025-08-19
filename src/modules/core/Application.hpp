#pragma once
#include "Window.hpp"
#include "../graphics/Renderer.hpp"
#include "../graphics/Camera.hpp"
#include "../graphics/Mesh.hpp"
#include "../math/Quaternion.hpp"
#include "../math/Vector3.hpp"    
#include "../math/Matrix4.hpp" 

namespace app {
    template<typename T>
    using Quaternionf = math::Quaternion<T>;
    template<typename T>
    using Vector3f = math::Vector3<T>;
    template<typename T>
    using Matrix4f = math::Matrix4<T>;

    class Application {
    public:
        Application();
        ~Application();
        void run();

    private:
        void handleEvents();
        void update(float deltaTime);
        void render();

        Window* mainWindow;
        graphics::Renderer<float>* mainRenderer;
        graphics::Camera<float>* mainCamera;
        graphics::Mesh<float> mesh;
        bool quit;
        float rotationAngle;
    };
} // namespace app
