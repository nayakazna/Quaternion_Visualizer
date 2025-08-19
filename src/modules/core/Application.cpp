// Application.cpp
#include "Application.hpp"
#include <iostream>
#include <SDL_ttf.h>
#include <vector>

#include "../graphics/Camera.hpp"
#include "../graphics/Renderer.hpp"
#include "../graphics/ObjLoader.hpp"
#include "../graphics/Mesh.hpp"
#include "../math/Matrix4.hpp"
#include "../math/Vector3.hpp"
#include "../math/Quaternion.hpp"
#include "../math/Vector2.hpp"


namespace app {
    using Quaternionf = math::Quaternion<float>;
    using Vector3f = math::Vector3<float>;
    using Matrix4f = math::Matrix4<float>;

    Application::Application() : quit(false), rotationAngle(0.0f) {
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0) {
            std::cerr << "SDL tidak dapat diinisialisasi! SDL_Error: " << SDL_GetError() << std::endl;
            exit(1);
        }

        if (TTF_Init() == -1) {
            std::cerr << "SDL_ttf tidak dapat diinisialisasi! TTF_Error: " << TTF_GetError() << std::endl;
            SDL_Quit();
            exit(1);
        }
        
        mainWindow = new Window("Quaternion Visualizer", 1366, 768);
        mainRenderer = new graphics::Renderer<float>(mainWindow->getSDLRenderer(), mainWindow->getWidth(), mainWindow->getHeight());
        mainCamera = new graphics::Camera<float>(
            Vector3f(0.0f, 0.0f, 5.0f),
            Vector3f(0.0f, 0.0f, 0.0f),
            Vector3f(0.0f, 1.0f, 0.0f)
        );
        
        uiManager = std::make_unique<ui::UIManager>(
            mainWindow->getSDLRenderer(), 
            mainWindow->getWidth(), 
            mainWindow->getHeight()
        );
        
        uiManager->onFileSelected = [this](const std::string& filename) {
            onFileSelected(filename);
        };
        
        uiManager->onApplyRotation = [this]() {
            onApplyRotation();
        };
        
        uiManager->onResetRotation = [this]() {
            onResetRotation();
        };
        
        originalModelMatrix = Matrix4f::identity();
        rotatedModelMatrix = Matrix4f::identity();
    }

    Application::~Application() {
        delete mainWindow;
        delete mainRenderer;
        delete mainCamera;
        SDL_Quit();
    }

    void Application::run() {
        Uint32 lastFrameTime = SDL_GetTicks();
        const float TARGET_FPS = 60.0f;
        const float FRAME_TIME = 1000.0f / TARGET_FPS;
        while (!quit) {
            Uint32 currentFrameTime = SDL_GetTicks();
            float deltaTime = static_cast<float>(currentFrameTime - lastFrameTime) / 1000.0f;
            lastFrameTime = currentFrameTime;

            handleEvents();
            update(deltaTime);
            render();

            Uint32 frameTime = SDL_GetTicks() - currentFrameTime;
            if (frameTime < FRAME_TIME) {
                SDL_Delay(static_cast<Uint32>(FRAME_TIME - frameTime));
            }
        }
    }

    void Application::handleEvents() {
        SDL_Event e;
        while (SDL_PollEvent(&e) != 0) {
            uiManager->handleEvent(e);  
            if (e.type == SDL_QUIT) {
                quit = true;
            } 
            else if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_ESCAPE) {
                    mouseControlEnabled = !mouseControlEnabled;
                    std::cout << "Mouse control: " << (mouseControlEnabled ? "ENABLED" : "DISABLED") << std::endl;
                }
            }
            else if (e.type == SDL_MOUSEMOTION && mouseControlEnabled) {
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);
                int lastX, lastY;
                static bool firstMouse = true;
                static int prevX, prevY;

                if (firstMouse) {
                    prevX = mouseX;
                    prevY = mouseY;
                    firstMouse = false;
                }

                float xoffset = static_cast<float>(mouseX - prevX);
                float yoffset = static_cast<float>(prevY - mouseY);
                prevX = mouseX;
                prevY = mouseY;
                mainCamera->handleMouseMovement(xoffset, yoffset);
            }
        }
    }

    void Application::drawRotationAxis(const Matrix4f& viewProjectionMatrix) {
        float x, y, z;
        uiManager->getRotationAxis(x, y, z);

        Vector3f axisEnd(x * 2.5f, y * 2.5f, z * 2.5f); 
        mainRenderer->drawArrow(Vector3f(0, 0, 0), axisEnd, viewProjectionMatrix, 128, 0, 128, 255);
        
        
        mainRenderer->drawText3D("Sumbu Rotasi", axisEnd + Vector3f(0.2f, 0.2f, 0.2f), 
                                viewProjectionMatrix, 255, 128, 255, 255);
    }

    void Application::drawAngleLabel(const Matrix4f& viewProjectionMatrix) {
        float angle = uiManager->getRotationAngle();
        float x, y, z;
        uiManager->getRotationAxis(x, y, z);
        
        
        Vector3f labelPos(x * 1.5f, y * 1.5f + 0.5f, z * 1.5f);
        
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(1) << angle << " derajat";
        
        mainRenderer->drawText3D(oss.str(), labelPos, viewProjectionMatrix, 255, 255, 100, 255);
    }

    
    void Application::update(float deltaTime) {
        const Uint8* state = SDL_GetKeyboardState(NULL);
        mainCamera->handleKeyboard(state, deltaTime);
        uiManager->update(deltaTime);
    }

    void Application::render() {
        mainRenderer->clearScreen(0x1A, 0x1A, 0x1A, 0xFF);

        Matrix4f viewMatrix = mainCamera->getViewMatrix();
        Matrix4f projectionMatrix = mainCamera->getProjectionMatrix(mainWindow->getWidth(), mainWindow->getHeight());
        Matrix4f viewProjectionMatrix = projectionMatrix * viewMatrix;

        mainRenderer->drawAxesWithLabels(viewProjectionMatrix);

        if (!mesh.vertices.empty()) {
            if (hasRotation) {
                mainRenderer->drawMesh(mesh, originalModelMatrix, viewMatrix, projectionMatrix, 100, 100, 100, 255);
                mainRenderer->drawMesh(mesh, rotatedModelMatrix, viewMatrix, projectionMatrix, 255, 255, 255, 255);
                drawRotationAxis(viewProjectionMatrix);
                drawAngleLabel(viewProjectionMatrix);
            } else {
                mainRenderer->drawMesh(mesh, originalModelMatrix, viewMatrix, projectionMatrix, 100, 100, 100, 255);
            }
        }
        uiManager->render();
        mainRenderer->present();
    }

void Application::onFileSelected(const std::string& filename) {
    std::cout << "Loading file: " << filename << std::endl;
    
    try {
        mesh = graphics::ObjLoader<float>::loadObj(filename);
        std::cout << "Successfully loaded: " << filename << std::endl;
        std::cout << "Vertices: " << mesh.vertices.size() << std::endl;
        std::cout << "Faces: " << mesh.faces.size() << std::endl;
        
        hasRotation = false;
        originalModelMatrix = Matrix4f::identity();
        rotatedModelMatrix = Matrix4f::identity();
        
    } catch (const std::exception& e) {
        std::cerr << "Failed to load " << filename << ": " << e.what() << std::endl;
    }
}

    void Application::onApplyRotation() {
        float angle = uiManager->getRotationAngle();
        float x, y, z;
        uiManager->getRotationAxis(x, y, z);
        
        std::cout << "Rotasi dengan sudut " << angle << "° di sumbu (" << x << ", " << y << ", " << z << ")" << std::endl;
        
        Vector3f rotationAxis(x, y, z);
        float rotationAngleRad = angle * (3.141592653589793f / 180.0f);
        Quaternionf rotation = Quaternionf::fromAxisAngle(rotationAxis, rotationAngleRad);
        
        rotatedModelMatrix = Matrix4f::fromQuaternion(rotation);
        hasRotation = true;
        
        std::cout << "Rotation applied!" << std::endl;
    }

    void Application::onResetRotation() {
        std::cout << "Resetting rotation" << std::endl;
        
        hasRotation = false;
        rotatedModelMatrix = Matrix4f::identity();
        
        uiManager->setRotationAxis(1.0f, 0.0f, 0.0f);
        uiManager->setRotationAngle(45.0f);
        
        std::cout << "Rotation reset!" << std::endl;
    }
} // namespace app
