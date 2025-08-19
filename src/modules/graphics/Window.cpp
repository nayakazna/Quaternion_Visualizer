// Window.cpp
#include "Window.hpp"
#include <iostream>

namespace app {
    Window::Window(const std::string& title, int width, int height) :
        window(nullptr), renderer(nullptr), width(width), height(height) {

        window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                    width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
        if (window == nullptr) {
            std::cerr << "Jendela tidak dapat dibuat! SDL_Error: " << SDL_GetError() << std::endl;
            exit(1);
        }

        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        if (renderer == nullptr) {
            std::cerr << "Renderer tidak dapat dibuat! SDL_Error: " << SDL_GetError() << std::endl;
            SDL_DestroyWindow(window);
            exit(1);
        }
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    }

    Window::~Window() {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
    }

    SDL_Window* Window::getSDLWindow() const {
        return window;
    }

    SDL_Renderer* Window::getSDLRenderer() const {
        return renderer;
    }

    int Window::getWidth() const {
        return width;
    }

    int Window::getHeight() const {
        return height;
    }
} // namespace app
