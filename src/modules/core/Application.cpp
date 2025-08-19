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
            Vector3f<float>(0.0f, 0.0f, 10.0f),
            Vector3f<float>(0.0f, 0.0f, 0.0f),
            Vector3f<float>(0.0f, 1.0f, 0.0f)
        );
        
        // TODO: ini nanti diganti, di app-nya ada buat choose file
        // mesh = graphics::ObjLoader<float>::loadObj("../models/teapot.obj");
        // Tes pake kubus
        mesh.vertices = {
            {-1,-1,-1}, {1,-1,-1}, {1,1,-1}, {-1,1,-1},
            {-1,-1,1}, {1,-1,1}, {1,1,1}, {-1,1,1}
        };
        mesh.faces = {
            {0,1,2,3}, // front
            {4,5,6,7}, // back
            {0,1,5,4}, // bottom
            {2,3,7,6}, // top
            {1,2,6,5}, // right
            {0,3,7,4}  // left
        };


        // buat debug doang
        std::cout << "Mesh loaded successfully\n";
        std::cout << "Vertex count: " << mesh.vertices.size() << "\n";
        std::cout << "Face count: " << mesh.faces.size() << "\n";

        if (!mesh.vertices.empty()) {
            std::cout << "First vertex: " << mesh.vertices[0].x << ", " 
                    << mesh.vertices[0].y << ", " << mesh.vertices[0].z << "\n";
            std::cout << "Last vertex: " << mesh.vertices.back().x << ", " 
                    << mesh.vertices.back().y << ", " << mesh.vertices.back().z << "\n";
        }

        if (!mesh.faces.empty()) {
            std::cout << "First face indices: ";
            for (auto idx : mesh.faces[0]) std::cout << idx << " ";
            std::cout << "\n";
        }
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
        }
    }

    void Application::update(float deltaTime) {
        // TODO: ini biar muter-muter aja, testing
        rotationAngle += 45.0f * deltaTime;
        if (rotationAngle >= 360.0f) {
            rotationAngle -= 360.0f;
        }
    }


    void Application::render() {
        mainRenderer->clearScreen(0x33, 0x33, 0x33, 0xFF);
        Matrix4f<float> viewMatrix = mainCamera->getViewMatrix();
        Matrix4f<float> projectionMatrix = mainCamera->getProjectionMatrix(mainWindow->getWidth(), mainWindow->getHeight());
        Matrix4f<float> viewProjectionMatrix = projectionMatrix * viewMatrix;

        Vector3f<float> testPoint(0,0,0); 
        auto projected = mainRenderer->project(testPoint, viewProjectionMatrix);

        // sumbu koordinat (XYZ)
        mainRenderer->drawAxes(viewProjectionMatrix);
        

        // sblm rotasi (ijo)
        Matrix4f<float> modelMatrixOriginal = Matrix4f<float>::identity();
        mainRenderer->drawMesh(mesh, modelMatrixOriginal, viewMatrix, projectionMatrix, 0, 255, 0, 255);

        // quaternion rotasi
        Vector3f<float> rotationAxis(0.0f, 1.0f, 0.0f); 
        float rotationAngleRad = rotationAngle * (3.141592653589793f / 180.0f);
        Quaternionf<float> rotation = Quaternionf<float>::fromAxisAngle(rotationAxis, rotationAngleRad);

        // setelah rotasi (putih)
        Matrix4f<float> modelMatrixRotated = Matrix4f<float>::fromQuaternion(rotation);
        mainRenderer->drawMesh(mesh, modelMatrixRotated, viewMatrix, projectionMatrix, 255, 255, 255, 255);

        mainRenderer->present();
    }
} // namespace app
