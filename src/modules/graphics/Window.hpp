#pragma once
#include <SDL.h>
#include <string>

namespace app {
    class Window {
    public:
        Window(const std::string& title, int width, int height);
        ~Window();

        SDL_Window* getSDLWindow() const;
        SDL_Renderer* getSDLRenderer() const;
        int getWidth() const;
        int getHeight() const;

    private:
        SDL_Window* window;
        SDL_Renderer* renderer;
        int width;
        int height;
    };
} // namespace app
