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
        
        mainWindow = new Window("Quaternion Visualizer", 1280, 720);
        mainRenderer = new graphics::Renderer<float>(mainWindow->getSDLRenderer(), mainWindow->getWidth(), mainWindow->getHeight());
        mainCamera = new graphics::Camera<float>(
            Vector3f(0.0f, 0.0f, 5.0f),
            Vector3f(0.0f, 0.0f, 0.0f),
            Vector3f(0.0f, 1.0f, 0.0f)
        );
        
        // TODO: ini nanti diganti, di app-nya ada buat choose file
        mesh = graphics::ObjLoader<float>::loadObj("models/teapot.obj");
    }

    Application::~Application() {
        delete mainWindow;
        delete mainRenderer;
        delete mainCamera;
        SDL_Quit();
    }

    void Application::run() {
        Uint32 lastFrameTime = SDL_GetTicks();
        while (!quit) {
            Uint32 currentFrameTime = SDL_GetTicks();
            float deltaTime = static_cast<float>(currentFrameTime - lastFrameTime) / 1000.0f;
            lastFrameTime = currentFrameTime;

            handleEvents();
            update(deltaTime);
            render();
        }
    }

    void Application::handleEvents() {
        SDL_Event e;
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            } 
            else if (e.type == SDL_MOUSEMOTION) {
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

    void Application::update(float deltaTime) {
        const Uint8* state = SDL_GetKeyboardState(NULL);
        mainCamera->handleKeyboard(state, deltaTime);
    }

    void Application::render() {
        mainRenderer->clearScreen(0x1A, 0x1A, 0x1A, 0xFF);

        Matrix4f viewMatrix = mainCamera->getViewMatrix();
        Matrix4f projectionMatrix = mainCamera->getProjectionMatrix(mainWindow->getWidth(), mainWindow->getHeight());
        Matrix4f viewProjectionMatrix = projectionMatrix * viewMatrix;

        mainRenderer->drawAxes(viewProjectionMatrix);

        Matrix4f modelMatrixOriginal = Matrix4f::identity();
        mainRenderer->drawMesh(mesh, modelMatrixOriginal, viewMatrix, projectionMatrix, 0, 255, 0, 255);

        Vector3f rotationAxis(0.0f, 1.0f, 0.0f);
        float rotationAngleRad = rotationAngle * (3.141592653589793f / 180.0f);
        Quaternionf rotation = Quaternionf::fromAxisAngle(rotationAxis, rotationAngleRad);

        Matrix4f modelMatrixRotated = Matrix4f::fromQuaternion(rotation);
        mainRenderer->drawMesh(mesh, modelMatrixRotated, viewMatrix, projectionMatrix, 255, 255, 255, 255);

        mainRenderer->present();
    }
} // namespace app
