#pragma once
#include "UIComponent.hpp"
#include "Button.hpp"
#include <vector>
#include <string>
#include <filesystem>
#include <functional>

namespace ui {
    
    class FileDialog : public UIComponent {
    public:
        using FileSelectedCallback = std::function<void(const std::string&)>;
        
        FileDialog(const Rect& bounds, FileSelectedCallback onFileSelected = nullptr)
            : UIComponent(bounds), onFileSelected(onFileSelected), 
              selectedIndex(-1), scrollOffset(0) {
            
            
            backgroundColor = Color(30, 30, 30, 240); 
            itemColor = Color(50, 50, 50, 255);
            selectedItemColor = Color(0, 120, 215, 255); 
            textColor = Color(255, 255, 255, 255);
            
            
            refreshFileList();
            
            
            int buttonY = bounds.y + bounds.h - 40;
            int buttonWidth = 80;
            int buttonHeight = 30;
            
            selectButton = std::make_unique<Button>(
                Rect(bounds.x + bounds.w - buttonWidth * 2 - 20, buttonY, buttonWidth, buttonHeight),
                "Select",
                [this]() { selectFile(); }
            );
            
            cancelButton = std::make_unique<Button>(
                Rect(bounds.x + bounds.w - buttonWidth - 10, buttonY, buttonWidth, buttonHeight),
                "Cancel", 
                [this]() { close(); }
            );
        }
        
        void handleEvent(const SDL_Event& event) override {
            if (!visible) return;
            
            
            selectButton->handleEvent(event);
            cancelButton->handleEvent(event);
            
            switch (event.type) {
                case SDL_MOUSEBUTTONDOWN: {
                    if (event.button.button == SDL_BUTTON_LEFT) {
                        int x = event.button.x;
                        int y = event.button.y;
                        
                        
                        int listY = bounds.y + 30; 
                        int listHeight = bounds.h - 80; 
                        int itemHeight = 25;
                        
                        if (x >= bounds.x + 10 && x < bounds.x + bounds.w - 10 &&
                            y >= listY && y < listY + listHeight) {
                            
                            int clickedIndex = (y - listY) / itemHeight + scrollOffset;
                            if (clickedIndex >= 0 && clickedIndex < static_cast<int>(files.size())) {
                                selectedIndex = clickedIndex;
                            }
                        }
                    }
                    break;
                }
                
                case SDL_MOUSEWHEEL: {
                    
                    if (bounds.contains(event.button.x, event.button.y)) {
                        scrollOffset -= event.wheel.y;
                        if (scrollOffset < 0) scrollOffset = 0;
                        
                        int maxVisible = (bounds.h - 80) / 25; 
                        int maxScroll = static_cast<int>(files.size()) - maxVisible;
                        if (scrollOffset > maxScroll && maxScroll > 0) {
                            scrollOffset = maxScroll;
                        }
                    }
                    break;
                }
                
                case SDL_KEYDOWN: {
                    switch (event.key.keysym.sym) {
                        case SDLK_ESCAPE:
                            close();
                            break;
                            
                        case SDLK_RETURN:
                        case SDLK_KP_ENTER:
                            selectFile();
                            break;
                            
                        case SDLK_UP:
                            if (selectedIndex > 0) selectedIndex--;
                            break;
                            
                        case SDLK_DOWN:
                            if (selectedIndex < static_cast<int>(files.size()) - 1) selectedIndex++;
                            break;
                    }
                    break;
                }
            }
        }
        
        void render(SDL_Renderer* renderer, TTF_Font* font) override {
            if (!visible) return;
            
            
            renderRect(renderer, bounds, backgroundColor);
            
            
            SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
            SDL_Rect borderRect = {bounds.x, bounds.y, bounds.w, bounds.h};
            SDL_RenderDrawRect(renderer, &borderRect);
            
            
            if (font) {
                renderText(renderer, font, "Select OBJ File (models/ folder)", 
                          bounds.x + 10, bounds.y + 5, textColor);
            }
            
            
            int listY = bounds.y + 30;
            int listHeight = bounds.h - 80;
            int itemHeight = 25;
            int maxVisible = listHeight / itemHeight;
            
            for (int i = 0; i < maxVisible && (i + scrollOffset) < static_cast<int>(files.size()); ++i) {
                int fileIndex = i + scrollOffset;
                int itemY = listY + i * itemHeight;
                
                
                if (fileIndex == selectedIndex) {
                    Rect itemRect(bounds.x + 5, itemY, bounds.w - 10, itemHeight);
                    renderRect(renderer, itemRect, selectedItemColor);
                }
                
                
                if (font) {
                    Color currentTextColor = (fileIndex == selectedIndex) ? Color(255, 255, 255) : textColor;
                    renderText(renderer, font, files[fileIndex], bounds.x + 10, itemY + 3, currentTextColor);
                }
            }
            
            
            if (static_cast<int>(files.size()) > maxVisible) {
                int scrollbarX = bounds.x + bounds.w - 15;
                int scrollbarY = listY;
                int scrollbarHeight = listHeight;
                
                
                SDL_SetRenderDrawColor(renderer, 60, 60, 60, 255);
                SDL_Rect trackRect = {scrollbarX, scrollbarY, 10, scrollbarHeight};
                SDL_RenderFillRect(renderer, &trackRect);
                
                
                float thumbHeight = (float)maxVisible / files.size() * scrollbarHeight;
                float thumbY = (float)scrollOffset / files.size() * scrollbarHeight;
                
                SDL_SetRenderDrawColor(renderer, 120, 120, 120, 255);
                SDL_Rect thumbRect = {scrollbarX, (int)(scrollbarY + thumbY), 10, (int)thumbHeight};
                SDL_RenderFillRect(renderer, &thumbRect);
            }
            
            
            selectButton->render(renderer, font);
            cancelButton->render(renderer, font);
            
            
            selectButton->setEnabled(selectedIndex >= 0);
        }
        
        void show() {
            visible = true;
            refreshFileList();
            selectedIndex = -1;
            scrollOffset = 0;
        }
        
        void close() {
            visible = false;
        }
        
    private:
        FileSelectedCallback onFileSelected;
        std::vector<std::string> files;
        int selectedIndex;
        int scrollOffset;
        
        std::unique_ptr<Button> selectButton;
        std::unique_ptr<Button> cancelButton;
        
        Color backgroundColor;
        Color itemColor;
        Color selectedItemColor;
        Color textColor;
        
        void refreshFileList() {
            files.clear();
            
            try {
                
                std::filesystem::path modelsPath = "models";
                if (std::filesystem::exists(modelsPath) && std::filesystem::is_directory(modelsPath)) {
                    for (const auto& entry : std::filesystem::directory_iterator(modelsPath)) {
                        if (entry.is_regular_file() && entry.path().extension() == ".obj") {
                            files.push_back(entry.path().filename().string());
                        }
                    }
                }
            } catch (const std::exception& e) {
                
                files.push_back("teapot.obj");
                files.push_back("cube.obj");
                files.push_back("sphere.obj");
            }
            
            
            std::sort(files.begin(), files.end());
        }
        
        void selectFile() {
            if (selectedIndex >= 0 && selectedIndex < static_cast<int>(files.size())) {
                std::string fullPath = "models/" + files[selectedIndex];
                if (onFileSelected) {
                    onFileSelected(fullPath);
                }
                close();
            }
        }
    };
    
} // namespace ui