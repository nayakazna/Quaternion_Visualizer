#pragma once
#include "Window.hpp"
#include "../graphics/Renderer.hpp"
#include "../graphics/Camera.hpp"
#include "../graphics/Mesh.hpp"
#include "../math/Quaternion.hpp"
#include "../math/Vector3.hpp"    
#include "../math/Matrix4.hpp" 
#include "../ui/UIManager.hpp"
#include <memory>                        
#include <sstream>                       
#include <iomanip>                       

namespace app {
    class Application {
    public:
        Application();
        ~Application();
        void run();

    private:
        Window* mainWindow;
        graphics::Renderer<float>* mainRenderer;
        graphics::Camera<float>* mainCamera;
        graphics::Mesh<float> mesh;

        std::unique_ptr<ui::UIManager> uiManager;

        void handleEvents();
        void handleKeyboard(const Uint8* state, float deltaTime);
        void update(float deltaTime);
        void render();

        bool quit;
        float rotationAngle;
        bool mouseCapture = false;
        bool mouseControlEnabled = false;

        
        math::Matrix4<float> originalModelMatrix;
        math::Matrix4<float> rotatedModelMatrix;
        bool hasRotation = false;
        
        void onFileSelected(const std::string& filename);
        void onApplyRotation();
        void onResetRotation();

        void drawRotationAxis(const math::Matrix4<float>& viewProjectionMatrix);
        void drawAngleLabel(const math::Matrix4<float>& viewProjectionMatrix);
    };
} // namespace app
