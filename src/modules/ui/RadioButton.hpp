#pragma once
#include "UIComponent.hpp"
#include <functional>
#include <vector>
#include <memory>

namespace ui {
    
    class RadioButton : public UIComponent {
    public:
        using SelectionCallback = std::function<void(int)>;
        
        RadioButton(const Rect& bounds, const std::vector<std::string>& options, 
                   int selectedIndex = 0, SelectionCallback onSelectionChanged = nullptr)
            : UIComponent(bounds), options(options), selectedIndex(selectedIndex), 
              onSelectionChanged(onSelectionChanged) {
            
            
            normalColor = Color(60, 60, 60, 255);
            selectedColor = Color(0, 120, 215, 255); 
            textColor = Color(255, 255, 255, 255);
            hoverColor = Color(80, 80, 80, 255);
            
            hoveredIndex = -1;
        }
        
        void handleEvent(const SDL_Event& event) override {
            if (!visible || !enabled) return;
            
            switch (event.type) {
                case SDL_MOUSEMOTION: {
                    int x = event.motion.x;
                    int y = event.motion.y;
                    
                    hoveredIndex = -1;
                    if (bounds.contains(x, y)) {
                        int itemHeight = bounds.h / static_cast<int>(options.size());
                        int relativeY = y - bounds.y;
                        hoveredIndex = relativeY / itemHeight;
                        if (hoveredIndex >= static_cast<int>(options.size())) {
                            hoveredIndex = -1;
                        }
                    }
                    break;
                }
                
                case SDL_MOUSEBUTTONDOWN: {
                    if (event.button.button == SDL_BUTTON_LEFT) {
                        int x = event.button.x;
                        int y = event.button.y;
                        
                        if (bounds.contains(x, y)) {
                            int itemHeight = bounds.h / static_cast<int>(options.size());
                            int relativeY = y - bounds.y;
                            int clickedIndex = relativeY / itemHeight;
                            
                            if (clickedIndex >= 0 && clickedIndex < static_cast<int>(options.size())) {
                                selectedIndex = clickedIndex;
                                if (onSelectionChanged) {
                                    onSelectionChanged(selectedIndex);
                                }
                            }
                        }
                    }
                    break;
                }
            }
        }
        
        void render(SDL_Renderer* renderer, TTF_Font* font) override {
            if (!visible) return;
            
            int itemHeight = bounds.h / static_cast<int>(options.size());
            
            for (int i = 0; i < static_cast<int>(options.size()); ++i) {
                int itemY = bounds.y + i * itemHeight;
                Rect itemRect(bounds.x, itemY, bounds.w, itemHeight);
                
                
                Color bgColor = normalColor;
                if (i == hoveredIndex) {
                    bgColor = hoverColor;
                }
                
                
                renderRect(renderer, itemRect, bgColor);
                
                
                int circleX = bounds.x + 10;
                int circleY = itemY + itemHeight / 2;
                int circleRadius = 6;
                
                
                SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);
                for (int w = 0; w < circleRadius * 2; w++) {
                    for (int h = 0; h < circleRadius * 2; h++) {
                        int dx = circleRadius - w;
                        int dy = circleRadius - h;
                        if ((dx*dx + dy*dy) <= (circleRadius * circleRadius)) {
                            SDL_RenderDrawPoint(renderer, circleX + dx, circleY + dy);
                        }
                    }
                }
                
                
                if (i == selectedIndex) {
                    SDL_SetRenderDrawColor(renderer, selectedColor.r, selectedColor.g, selectedColor.b, selectedColor.a);
                    int innerRadius = circleRadius - 2;
                    for (int w = 0; w < innerRadius * 2; w++) {
                        for (int h = 0; h < innerRadius * 2; h++) {
                            int dx = innerRadius - w;
                            int dy = innerRadius - h;
                            if ((dx*dx + dy*dy) <= (innerRadius * innerRadius)) {
                                SDL_RenderDrawPoint(renderer, circleX + dx, circleY + dy);
                            }
                        }
                    }
                }
                
                
                if (font && i < static_cast<int>(options.size())) {
                    int textX = bounds.x + 25; 
                    int textY = itemY + (itemHeight - 16) / 2; 
                    
                    renderText(renderer, font, options[i], textX, textY, textColor);
                }
                
                
                if (i < static_cast<int>(options.size()) - 1) {
                    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
                    SDL_RenderDrawLine(renderer, bounds.x, itemY + itemHeight, 
                                     bounds.x + bounds.w, itemY + itemHeight);
                }
            }
            
            
            SDL_SetRenderDrawColor(renderer, 120, 120, 120, 255);
            SDL_Rect borderRect = {bounds.x, bounds.y, bounds.w, bounds.h};
            SDL_RenderDrawRect(renderer, &borderRect);
        }
        
        
        int getSelectedIndex() const { return selectedIndex; }
        void setSelectedIndex(int index) {
            if (index >= 0 && index < static_cast<int>(options.size())) {
                selectedIndex = index;
            }
        }
        
        const std::string& getSelectedOption() const {
            if (selectedIndex >= 0 && selectedIndex < static_cast<int>(options.size())) {
                return options[selectedIndex];
            }
            static std::string empty = "";
            return empty;
        }
        
    private:
        std::vector<std::string> options;
        int selectedIndex;
        int hoveredIndex;
        SelectionCallback onSelectionChanged;
        
        Color normalColor;
        Color selectedColor;
        Color hoverColor;
        Color textColor;
    };
    
} // namespace ui