#pragma once
#include "UIComponent.hpp"
#include "Button.hpp"
#include "InputField.hpp"
#include "Label.hpp"
#include "Panel.hpp"
#include "FileDialog.hpp"
#include "RadioButton.hpp"
#include <SDL_ttf.h>
#include <vector>
#include <memory>
#include <string>
#include <functional>

namespace ui {
    enum class RotationMethod {
        QUATERNION = 0,
        EULER_ANGLES = 1,
        TAIT_BRYAN = 2
    };
    
    struct UITheme {
        Color backgroundColor = Color(30, 30, 30, 255);
        Color panelColor = Color(40, 40, 40, 255);
        Color borderColor = Color(70, 70, 70, 255);
        Color textColor = Color(255, 255, 255, 255);
        Color accentColor = Color(0, 120, 215, 255); 
        
        int fontSize = 14;
        int padding = 10;
        int spacing = 5;
    };
    
    class UIManager {
    public:
        UIManager(SDL_Renderer* renderer, int screenWidth, int screenHeight);
        ~UIManager();
        
        
        void handleEvent(const SDL_Event& event);
        void update(float deltaTime);
        void render();
        
        
        void addComponent(std::shared_ptr<UIComponent> component);
        void removeComponent(std::shared_ptr<UIComponent> component);
        void clearComponents();
        
        
        std::shared_ptr<Button> createButton(const Rect& bounds, const std::string& text, 
                                           std::function<void()> onClick = nullptr);
        std::shared_ptr<InputField> createInputField(const Rect& bounds, const std::string& placeholder = "");
        std::shared_ptr<Label> createLabel(const Rect& bounds, const std::string& text);
        std::shared_ptr<Panel> createPanel(const Rect& bounds, const std::string& title = "");
        
        
        void showFileDialog(std::function<void(const std::string&)> onFileSelected);
        void hideFileDialog();
        
        
        void setupQuaternionUI();
        void updateRotationDisplay(float angle, float axisX, float axisY, float axisZ);
        void setLoadedFileName(const std::string& filename);
        
        
        float getRotationAngle() const;
        void getRotationAxis(float& x, float& y, float& z) const;
        void setRotationAngle(float angle);
        void setRotationAxis(float x, float y, float z);

        RotationMethod getRotationMethod() const;
        void setRotationMethod(RotationMethod method);
        
        void getEulerAngles(float& alpha, float& beta, float& gamma) const;
        void setEulerAngles(float alpha, float beta, float gamma);
        
        void getTaitBryanAngles(float& yaw, float& pitch, float& roll) const;
        void setTaitBryanAngles(float yaw, float pitch, float roll);
        
        std::function<void(const std::string&)> onFileSelected;
        std::function<void()> onApplyRotation;
        std::function<void()> onResetRotation;
        
    private:
        SDL_Renderer* renderer;
        TTF_Font* font;
        UITheme theme;
        int screenWidth, screenHeight;
        
        std::vector<std::shared_ptr<UIComponent>> components;
        std::unique_ptr<FileDialog> fileDialog;
        
        
        std::shared_ptr<Panel> mainPanel;
        std::shared_ptr<Panel> filePanel;
        std::shared_ptr<Panel> rotationPanel;
        std::shared_ptr<Panel> infoPanel;
        
        std::shared_ptr<Button> chooseFileButton;
        std::shared_ptr<Label> fileNameLabel;
        
        std::shared_ptr<InputField> axisXInput;
        std::shared_ptr<InputField> axisYInput;
        std::shared_ptr<InputField> axisZInput;
        std::shared_ptr<InputField> angleInput;
        
        std::shared_ptr<Button> applyButton;
        std::shared_ptr<Button> resetButton;
        
        std::shared_ptr<Label> quaternionDisplay;
        std::shared_ptr<Label> statusLabel;
        
        bool initializeFont();
        void createMainLayout();
        void createFileSection();
        void createRotationSection();
        void createInfoSection();
        void createControlButtons();
        
        void onChooseFileClicked();
        void onApplyClicked();
        void onResetClicked();
        void onAxisChanged();
        
        void normalizeAxis();
        void updateQuaternionDisplay();

        
        RotationMethod currentMethod = RotationMethod::QUATERNION;
        std::shared_ptr<RadioButton> methodSelector;
        
        
        std::shared_ptr<Panel> eulerPanel;
        std::shared_ptr<InputField> alphaInput;  
        std::shared_ptr<InputField> betaInput;   
        std::shared_ptr<InputField> gammaInput;  
        
        
        std::shared_ptr<Panel> taitBryanPanel;
        std::shared_ptr<InputField> yawInput;
        std::shared_ptr<InputField> pitchInput;
        std::shared_ptr<InputField> rollInput;
        
        void createMethodSelection();
        void createEulerSection();
        void createTaitBryanSection();
        void onMethodChanged(int methodIndex);
        void updateVisiblePanels();
    };
    
} 