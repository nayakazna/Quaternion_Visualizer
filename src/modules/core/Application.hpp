#pragma once
#include "Window.hpp"
#include "../graphics/Renderer.hpp"
#include "../graphics/Camera.hpp"
#include "../graphics/Mesh.hpp"
#include "../math/Quaternion.hpp"
#include "../math/Vector3.hpp"    
#include "../math/Matrix4.hpp" 
#include "../ui/UIManager.hpp"

namespace app {
    class Application {
    public:
        Application();
        ~Application();
        void run();

    private:
        void handleEvents();
        void handleKeyboard(const Uint8* state, float deltaTime);
        void update(float deltaTime);
        void render();

        Window* mainWindow;
        graphics::Renderer<float>* mainRenderer;
        graphics::Camera<float>* mainCamera;
        graphics::Mesh<float> mesh;
        bool quit;
        bool mouseCapture = false;
        float rotationAngle;

        std::unique_ptr<ui::UIManager> uiManager;
        
        math::Matrix4<float> originalModelMatrix;
        math::Matrix4<float> rotatedModelMatrix;
        bool hasRotation = false;
        
        void onFileSelected(const std::string& filename);
        void onApplyRotation();
        void onResetRotation();
    };
} // namespace app
