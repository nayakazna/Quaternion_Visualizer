#pragma once
#include "UIComponent.hpp"
#include <vector>
#include <memory>

namespace ui {
    
    class Panel : public UIComponent {
    public:
        Panel(const Rect& bounds, const std::string& title = "")
            : UIComponent(bounds), title(title) {
            
            
            backgroundColor = Color(45, 45, 45, 255);     
            borderColor = Color(80, 80, 80, 255);         
            titleColor = Color(200, 200, 200, 255);       
            hasBorder = true;
            hasTitle = !title.empty();
            titleHeight = hasTitle ? 25 : 0;
        }
        
        void handleEvent(const SDL_Event& event) override {
            if (!visible || !enabled) return;
            
            
            for (auto& child : children) {
                if (child && child->isVisible() && child->isEnabled()) {
                    child->handleEvent(event);
                }
            }
        }
        
        void update(float deltaTime) override {
            
            for (auto& child : children) {
                if (child && child->isVisible()) {
                    child->update(deltaTime);
                }
            }
        }
        
        void render(SDL_Renderer* renderer, TTF_Font* font) override {
            if (!visible) return;
            
            
            renderRect(renderer, bounds, backgroundColor);
            
            
            if (hasBorder) {
                SDL_SetRenderDrawColor(renderer, borderColor.r, borderColor.g, borderColor.b, borderColor.a);
                SDL_Rect borderRect = {bounds.x, bounds.y, bounds.w, bounds.h};
                SDL_RenderDrawRect(renderer, &borderRect);
            }
            
            
            if (hasTitle && font && !title.empty()) {
                
                Color titleBg = Color(
                    static_cast<Uint8>(backgroundColor.r * 0.8f),
                    static_cast<Uint8>(backgroundColor.g * 0.8f),
                    static_cast<Uint8>(backgroundColor.b * 0.8f),
                    backgroundColor.a
                );
                
                Rect titleRect(bounds.x + 1, bounds.y + 1, bounds.w - 2, titleHeight);
                renderRect(renderer, titleRect, titleBg);
                
                
                int textW, textH;
                TTF_SizeText(font, title.c_str(), &textW, &textH);
                int textX = bounds.x + (bounds.w - textW) / 2;
                int textY = bounds.y + (titleHeight - textH) / 2;
                
                renderText(renderer, font, title, textX, textY, titleColor);
                
                
                SDL_SetRenderDrawColor(renderer, borderColor.r, borderColor.g, borderColor.b, borderColor.a);
                SDL_RenderDrawLine(renderer, bounds.x + 1, bounds.y + titleHeight, 
                                 bounds.x + bounds.w - 1, bounds.y + titleHeight);
            }
            
            
            for (auto& child : children) {
                if (child && child->isVisible()) {
                    child->render(renderer, font);
                }
            }
        }
        
        
        void addChild(std::shared_ptr<UIComponent> child) {
            if (child) {
                children.push_back(child);
            }
        }
        
        void removeChild(std::shared_ptr<UIComponent> child) {
            children.erase(
                std::remove(children.begin(), children.end(), child),
                children.end()
            );
        }
        
        void clearChildren() {
            children.clear();
        }
        
        
        Rect getContentArea() const {
            return Rect(
                bounds.x + padding,
                bounds.y + titleHeight + padding,
                bounds.w - 2 * padding,
                bounds.h - titleHeight - 2 * padding
            );
        }
        
        
        void layoutVertical(int spacing = 5) {
            Rect contentArea = getContentArea();
            int currentY = contentArea.y;
            
            for (auto& child : children) {
                if (child && child->isVisible()) {
                    Rect childBounds = child->getBounds();
                    child->setPosition(contentArea.x, currentY);
                    currentY += childBounds.h + spacing;
                }
            }
        }
        
        void layoutHorizontal(int spacing = 5) {
            Rect contentArea = getContentArea();
            int currentX = contentArea.x;
            
            for (auto& child : children) {
                if (child && child->isVisible()) {
                    Rect childBounds = child->getBounds();
                    child->setPosition(currentX, contentArea.y);
                    currentX += childBounds.w + spacing;
                }
            }
        }
        
        void layoutGrid(int columns, int spacing = 5) {
            Rect contentArea = getContentArea();
            int col = 0;
            int row = 0;
            
            int childWidth = (contentArea.w - (columns - 1) * spacing) / columns;
            
            for (auto& child : children) {
                if (child && child->isVisible()) {
                    int x = contentArea.x + col * (childWidth + spacing);
                    int y = contentArea.y + row * (child->getBounds().h + spacing);
                    
                    child->setPosition(x, y);
                    child->setSize(childWidth, child->getBounds().h);
                    
                    col++;
                    if (col >= columns) {
                        col = 0;
                        row++;
                    }
                }
            }
        }
        
        
        void setColors(const Color& bg, const Color& border) {
            backgroundColor = bg;
            borderColor = border;
        }
        
        void setTitle(const std::string& newTitle) {
            title = newTitle;
            hasTitle = !title.empty();
            titleHeight = hasTitle ? 25 : 0;
        }
        
        void setPadding(int p) { padding = p; }
        void setBorder(bool border) { hasBorder = border; }
        
        
        static std::unique_ptr<Panel> createControlPanel(const Rect& bounds, const std::string& title) {
            auto panel = std::make_unique<Panel>(bounds, title);
            panel->setColors(Color(40, 40, 40, 255), Color(70, 70, 70, 255));
            return panel;
        }
        
        static std::unique_ptr<Panel> createSection(const Rect& bounds, const std::string& title) {
            auto panel = std::make_unique<Panel>(bounds, title);
            panel->setColors(Color(50, 50, 50, 255), Color(90, 90, 90, 255));
            return panel;
        }
        
    private:
        std::vector<std::shared_ptr<UIComponent>> children;
        std::string title;
        bool hasTitle;
        bool hasBorder;
        int titleHeight;
        int padding = 10;
        
        Color backgroundColor;
        Color borderColor;
        Color titleColor;
    }; // namespace ui
    
} 