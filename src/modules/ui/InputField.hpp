#pragma once
#include "UIComponent.hpp"
#include <string>
#include <sstream>

namespace ui {
    
    class InputField : public UIComponent {
    public:
        InputField(const Rect& bounds, const std::string& placeholder = "")
            : UIComponent(bounds), placeholder(placeholder), 
              isFocused(false), cursorPos(0), cursorBlinkTime(0.0f) {
            
            
            backgroundColor = Color(40, 40, 40, 255);
            borderColor = Color(100, 100, 100, 255);
            focusedBorderColor = Color(0, 120, 215, 255); 
            textColor = Color(255, 255, 255, 255);
            placeholderColor = Color(128, 128, 128, 255);
        }
        
        void handleEvent(const SDL_Event& event) override {
            if (!visible || !enabled) return;
            
            switch (event.type) {
                case SDL_MOUSEBUTTONDOWN: {
                    if (event.button.button == SDL_BUTTON_LEFT) {
                        int x = event.button.x;
                        int y = event.button.y;
                        bool wasInBounds = bounds.contains(x, y);
                        
                        if (wasInBounds && !isFocused) {
                            
                            isFocused = true;
                            SDL_StartTextInput();
                        } else if (!wasInBounds && isFocused) {
                            
                            isFocused = false;
                            SDL_StopTextInput();
                        }
                    }
                    break;
                }
                
                case SDL_TEXTINPUT: {
                    if (isFocused) {
                        
                        std::string input = event.text.text;
                        text.insert(cursorPos, input);
                        cursorPos += input.length();
                    }
                    break;
                }
                
                case SDL_KEYDOWN: {
                    if (isFocused) {
                        switch (event.key.keysym.sym) {
                            case SDLK_BACKSPACE:
                                if (cursorPos > 0) {
                                    text.erase(cursorPos - 1, 1);
                                    cursorPos--;
                                }
                                break;
                                
                            case SDLK_DELETE:
                                if (cursorPos < text.length()) {
                                    text.erase(cursorPos, 1);
                                }
                                break;
                                
                            case SDLK_LEFT:
                                if (cursorPos > 0) cursorPos--;
                                break;
                                
                            case SDLK_RIGHT:
                                if (cursorPos < text.length()) cursorPos++;
                                break;
                                
                            case SDLK_HOME:
                                cursorPos = 0;
                                break;
                                
                            case SDLK_END:
                                cursorPos = text.length();
                                break;
                                
                            case SDLK_RETURN:
                            case SDLK_KP_ENTER:
                                
                                isFocused = false;
                                SDL_StopTextInput();
                                break;
                        }
                        
                        
                        if (cursorPos > text.length()) cursorPos = text.length();
                    }
                    break;
                }
            }
        }
        
        void update(float deltaTime) override {
            
            if (isFocused) {
                cursorBlinkTime += deltaTime;
                if (cursorBlinkTime >= 1.0f) { 
                    cursorBlinkTime = 0.0f;
                }
            }
        }
        
        void render(SDL_Renderer* renderer, TTF_Font* font) override {
            if (!visible) return;
            
            
            renderRect(renderer, bounds, backgroundColor);
            
            
            Color currentBorderColor = isFocused ? focusedBorderColor : borderColor;
            SDL_SetRenderDrawColor(renderer, currentBorderColor.r, currentBorderColor.g, currentBorderColor.b, currentBorderColor.a);
            SDL_Rect borderRect = {bounds.x, bounds.y, bounds.w, bounds.h};
            SDL_RenderDrawRect(renderer, &borderRect);
            
            
            if (font) {
                std::string displayText = text.empty() ? placeholder : text;
                Color currentTextColor = text.empty() ? placeholderColor : textColor;
                
                if (!displayText.empty()) {
                    
                    int textX = bounds.x + 8; 
                    int textY = bounds.y + (bounds.h - TTF_FontHeight(font)) / 2;
                    
                    renderText(renderer, font, displayText, textX, textY, currentTextColor);
                }
                
                
                if (isFocused && cursorBlinkTime < 0.5f) { 
                    
                    std::string beforeCursor = text.substr(0, cursorPos);
                    int cursorX = bounds.x + 8;
                    
                    if (!beforeCursor.empty()) {
                        int textW;
                        TTF_SizeText(font, beforeCursor.c_str(), &textW, nullptr);
                        cursorX += textW;
                    }
                    
                    int cursorY = bounds.y + 4;
                    int cursorHeight = bounds.h - 8;
                    
                    SDL_SetRenderDrawColor(renderer, textColor.r, textColor.g, textColor.b, textColor.a);
                    SDL_RenderDrawLine(renderer, cursorX, cursorY, cursorX, cursorY + cursorHeight);
                }
            }
        }
        
        
        const std::string& getText() const { return text; }
        void setText(const std::string& newText) { 
            text = newText; 
            cursorPos = text.length();
        }
        
        
        float getFloatValue() const {
            try {
                return std::stof(text);
            } catch (...) {
                return 0.0f;
            }
        }
        
        void setFloatValue(float value) {
            std::ostringstream oss;
            oss << value;
            setText(oss.str());
        }
        
        
        void setColors(const Color& bg, const Color& border, const Color& focusedBorder) {
            backgroundColor = bg;
            borderColor = border;
            focusedBorderColor = focusedBorder;
        }
        
    private:
        std::string text;
        std::string placeholder;
        bool isFocused;
        size_t cursorPos;
        float cursorBlinkTime;
        
        Color backgroundColor;
        Color borderColor;
        Color focusedBorderColor;
        Color textColor;
        Color placeholderColor;
    };
    
} // namespace ui