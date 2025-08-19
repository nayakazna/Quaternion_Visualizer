#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <functional>
#include <string>

namespace ui {
    
    struct Color {
        Uint8 r, g, b, a;
        Color(Uint8 r = 255, Uint8 g = 255, Uint8 b = 255, Uint8 a = 255) : r(r), g(g), b(b), a(a) {}
    };
    
    struct Rect {
        int x, y, w, h;
        Rect(int x = 0, int y = 0, int w = 0, int h = 0) : x(x), y(y), w(w), h(h) {}
        
        bool contains(int px, int py) const {
            return px >= x && px < x + w && py >= y && py < y + h;
        }
    };
    
    class UIComponent {
    public:
        UIComponent(const Rect& bounds) : bounds(bounds), visible(true), enabled(true) {}
        virtual ~UIComponent() = default;
        
        virtual void handleEvent(const SDL_Event& event) = 0;
        virtual void update(float deltaTime) {}
        virtual void render(SDL_Renderer* renderer, TTF_Font* font) = 0;
        
        void setPosition(int x, int y) { bounds.x = x; bounds.y = y; }
        void setSize(int w, int h) { bounds.w = w; bounds.h = h; }
        void setVisible(bool v) { visible = v; }
        void setEnabled(bool e) { enabled = e; }
        
        const Rect& getBounds() const { return bounds; }
        bool isVisible() const { return visible; }
        bool isEnabled() const { return enabled; }
        
    protected:
        Rect bounds;
        bool visible;
        bool enabled;
        
        void renderRect(SDL_Renderer* renderer, const Rect& rect, const Color& color) {
            SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
            SDL_Rect sdlRect = {rect.x, rect.y, rect.w, rect.h};
            SDL_RenderFillRect(renderer, &sdlRect);
        }
        
        void renderText(SDL_Renderer* renderer, TTF_Font* font, const std::string& text, 
                       int x, int y, const Color& color) {
            if (!font || text.empty()) return;
            
            SDL_Color sdlColor = {color.r, color.g, color.b, color.a};
            SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), sdlColor);
            if (!surface) return;
            
            SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
            if (texture) {
                SDL_Rect destRect = {x, y, surface->w, surface->h};
                SDL_RenderCopy(renderer, texture, nullptr, &destRect);
                SDL_DestroyTexture(texture);
            }
            SDL_FreeSurface(surface);
        }
    };
    
} // namespace ui