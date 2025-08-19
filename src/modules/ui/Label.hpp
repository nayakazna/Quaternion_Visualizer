#pragma once
#include "UIComponent.hpp"
#include <iostream>
#include <string>
#include <memory>

namespace ui {
    
    enum class TextAlign {
        LEFT,
        CENTER,
        RIGHT
    };
    
    class Label : public UIComponent {
    private:
        std::string text;
        TextAlign textAlign;
        Color textColor;
        Color backgroundColor;
        bool hasBorder;
        Color borderColor;
        int padding = 5; 
    public:
        Label(const Rect& bounds, const std::string& text = "")
            : UIComponent(bounds), text(text), textAlign(TextAlign::LEFT) {
            
            
            textColor = Color(255, 255, 255, 255);     
            backgroundColor = Color(0, 0, 0, 0);       
            hasBorder = false;
            borderColor = Color(100, 100, 100, 255);
        }
        
        void handleEvent(const SDL_Event& event) override {
            
            
        }
        
        void render(SDL_Renderer* renderer, TTF_Font* font) override {
            if (!visible) return;
            
            
            if (backgroundColor.a > 0) {
                renderRect(renderer, bounds, backgroundColor);
            }
            
            
            if (hasBorder) {
                SDL_SetRenderDrawColor(renderer, borderColor.r, borderColor.g, borderColor.b, borderColor.a);
                SDL_Rect borderRect = {bounds.x, bounds.y, bounds.w, bounds.h};
                SDL_RenderDrawRect(renderer, &borderRect);
            }
            
            
            if (font && !text.empty()) {
                int textW, textH;
                TTF_SizeText(font, text.c_str(), &textW, &textH);
                
                int textX, textY;
                
                
                switch (textAlign) {
                    case TextAlign::LEFT:
                        textX = bounds.x + padding;
                        break;
                    case TextAlign::CENTER:
                        textX = bounds.x + (bounds.w - textW) / 2;
                        break;
                    case TextAlign::RIGHT:
                        textX = bounds.x + bounds.w - textW - padding;
                        break;
                }
                
                
                textY = bounds.y + (bounds.h - textH) / 2;
                
                
                if (textX < bounds.x + padding) textX = bounds.x + padding;
                if (textX + textW > bounds.x + bounds.w - padding) {
                    textX = bounds.x + bounds.w - textW - padding;
                }
                
                renderText(renderer, font, text, textX, textY, textColor);
            }
        }
        
        
        void setText(const std::string& newText) { text = newText; }
        const std::string& getText() const { return text; }
        
        
        void setTextAlign(TextAlign align) { textAlign = align; }
        void setTextColor(const Color& color) { textColor = color; }
        void setBackgroundColor(const Color& color) { backgroundColor = color; }
        void setBorder(bool border, const Color& color = Color(100, 100, 100, 255)) {
            hasBorder = border;
            borderColor = color;
        }
        void setPadding(int p) { padding = p; }
        
        
        static std::unique_ptr<Label> createTitle(const Rect& bounds, const std::string& text) {
            auto label = std::make_unique<Label>(bounds, text);
            label->setTextAlign(TextAlign::CENTER);
            label->setTextColor(Color(220, 220, 220, 255)); 
            return label;
        }
        
        static std::unique_ptr<Label> createValue(const Rect& bounds, const std::string& text) {
            auto label = std::make_unique<Label>(bounds, text);
            label->setTextAlign(TextAlign::RIGHT);
            label->setTextColor(Color(255, 255, 255, 255)); 
            return label;
        }
        

    };
    
} 