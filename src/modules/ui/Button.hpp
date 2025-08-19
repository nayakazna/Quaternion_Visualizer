#pragma once
#include "UIComponent.hpp"
#include <functional>

namespace ui {
    
    class Button : public UIComponent {
    public:
        using ClickCallback = std::function<void()>;
        
        Button(const Rect& bounds, const std::string& text, ClickCallback onClick = nullptr)
            : UIComponent(bounds), text(text), onClick(onClick), 
              isHovered(false), isPressed(false) {
            
            normalColor = Color(70, 70, 70, 255);   
            hoverColor = Color(90, 90, 90, 255);     
            pressedColor = Color(50, 50, 50, 255);   
            textColor = Color(255, 255, 255, 255);   
        }
        
        void handleEvent(const SDL_Event& event) override {
            if (!visible || !enabled) return;
            
            switch (event.type) {
                case SDL_MOUSEMOTION: {
                    int x = event.motion.x;
                    int y = event.motion.y;
                    isHovered = bounds.contains(x, y);
                    break;
                }
                
                case SDL_MOUSEBUTTONDOWN: {
                    if (event.button.button == SDL_BUTTON_LEFT) {
                        int x = event.button.x;
                        int y = event.button.y;
                        if (bounds.contains(x, y)) {
                            isPressed = true;
                        }
                    }
                    break;
                }
                
                case SDL_MOUSEBUTTONUP: {
                    if (event.button.button == SDL_BUTTON_LEFT && isPressed) {
                        int x = event.button.x;
                        int y = event.button.y;
                        if (bounds.contains(x, y) && onClick) {
                            onClick();
                        }
                        isPressed = false;
                    }
                    break;
                }
            }
        }
        
        void render(SDL_Renderer* renderer, TTF_Font* font) override {
            if (!visible) return;
            
            Color currentColor = normalColor;
            if (!enabled) {
                currentColor = Color(50, 50, 50, 128);
            } else if (isPressed) {
                currentColor = pressedColor;
            } else if (isHovered) {
                currentColor = hoverColor;
            }
            
            renderRect(renderer, bounds, currentColor);
            
            SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
            SDL_Rect borderRect = {bounds.x, bounds.y, bounds.w, bounds.h};
            SDL_RenderDrawRect(renderer, &borderRect);
            
            if (font && !text.empty()) {
                int textW, textH;
                TTF_SizeText(font, text.c_str(), &textW, &textH);
                
                int textX = bounds.x + (bounds.w - textW) / 2;
                int textY = bounds.y + (bounds.h - textH) / 2;
                
                Color currentTextColor = enabled ? textColor : Color(128, 128, 128, 255);
                renderText(renderer, font, text, textX, textY, currentTextColor);
            }
        }
        
        void setColors(const Color& normal, const Color& hover, const Color& pressed) {
            normalColor = normal;
            hoverColor = hover;
            pressedColor = pressed;
        }
        
        void setText(const std::string& newText) { text = newText; }
        void setCallback(ClickCallback callback) { onClick = callback; }
        
    private:
        std::string text;
        ClickCallback onClick;
        
        bool isHovered;
        bool isPressed;
        
        Color normalColor;
        Color hoverColor;
        Color pressedColor;
        Color textColor;
    };
    
} // namespace ui