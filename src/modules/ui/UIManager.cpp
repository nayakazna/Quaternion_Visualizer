#include "UIManager.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <cmath>

namespace ui {
    
    UIManager::UIManager(SDL_Renderer* renderer, int screenWidth, int screenHeight)
        : renderer(renderer), font(nullptr), screenWidth(screenWidth), screenHeight(screenHeight) {
        
        if (!initializeFont()) {
            std::cerr << "Gagal inisialisasi font buat UI!" << std::endl;
        }
        
        createMainLayout();
    }
    
    UIManager::~UIManager() {
        if (font) {
            TTF_CloseFont(font);
        }
    }
    
    bool UIManager::initializeFont() {
        
        const char* fontPaths[] = {
            "../assets/fonts/Miracode.ttf",
            "assets/fonts/Miracode.ttf",
            nullptr
        };
        
        for (int i = 0; fontPaths[i] != nullptr; ++i) {
            font = TTF_OpenFont(fontPaths[i], theme.fontSize);
            if (font) {
                std::cout << "Berhasil load font: " << fontPaths[i] << std::endl;
                return true;
            }
        }
        
        std::cerr << "Gagal load font, gaada teks!" << std::endl;
        return false;
    }
    
    void UIManager::handleEvent(const SDL_Event& event) {
        
        if (fileDialog && fileDialog->isVisible()) {
            fileDialog->handleEvent(event);
            return;
        }
        
        
        for (auto& component : components) {
            if (component && component->isVisible() && component->isEnabled()) {
                component->handleEvent(event);
            }
        }
    }
    
    void UIManager::update(float deltaTime) {
        
        if (fileDialog && fileDialog->isVisible()) {
            fileDialog->update(deltaTime);
        }
        
        
        for (auto& component : components) {
            if (component && component->isVisible()) {
                component->update(deltaTime);
            }
        }
        
        
        updateQuaternionDisplay();
    }
    
    void UIManager::render() {
        
        for (auto& component : components) {
            if (component && component->isVisible()) {
                component->render(renderer, font);
            }
        }
        
        
        if (fileDialog && fileDialog->isVisible()) {
            fileDialog->render(renderer, font);
        }
    }
    
    void UIManager::addComponent(std::shared_ptr<UIComponent> component) {
        if (component) {
            components.push_back(component);
        }
    }
    
    void UIManager::removeComponent(std::shared_ptr<UIComponent> component) {
        components.erase(
            std::remove(components.begin(), components.end(), component),
            components.end()
        );
    }
    
    void UIManager::clearComponents() {
        components.clear();
    }
    
    
    std::shared_ptr<Button> UIManager::createButton(const Rect& bounds, const std::string& text, 
                                                   std::function<void()> onClick) {
        auto button = std::make_shared<Button>(bounds, text, onClick);
        button->setColors(theme.panelColor, Color(90, 90, 90, 255), Color(50, 50, 50, 255));
        return button;
    }
    
    std::shared_ptr<InputField> UIManager::createInputField(const Rect& bounds, const std::string& placeholder) {
        auto input = std::make_shared<InputField>(bounds, placeholder);
        input->setColors(Color(30, 30, 30, 255), theme.borderColor, theme.accentColor);
        return input;
    }
    
    std::shared_ptr<Label> UIManager::createLabel(const Rect& bounds, const std::string& text) {
        auto label = std::make_shared<Label>(bounds, text);
        label->setTextColor(theme.textColor);
        return label;
    }
    
    std::shared_ptr<Panel> UIManager::createPanel(const Rect& bounds, const std::string& title) {
        auto panel = std::make_shared<Panel>(bounds, title);
        panel->setColors(theme.panelColor, theme.borderColor);
        return panel;
    }
    
    void UIManager::showFileDialog(std::function<void(const std::string&)> onSelected) {
        if (!fileDialog) {
            int dialogW = 500;
            int dialogH = 400;
            int dialogX = (screenWidth - dialogW) / 2;
            int dialogY = (screenHeight - dialogH) / 2;
            
            fileDialog = std::make_unique<FileDialog>(
                Rect(dialogX, dialogY, dialogW, dialogH),
                onSelected
            );
        } else {
            
            fileDialog = std::make_unique<FileDialog>(
                fileDialog->getBounds(),
                onSelected
            );
        }
        
        fileDialog->show();
    }
    
    void UIManager::hideFileDialog() {
        if (fileDialog) {
            fileDialog->close();
        }
    }
    
    void UIManager::createMainLayout() {
        
        int panelWidth = 300;
        int panelX = screenWidth - panelWidth - 10;
        int panelY = 10;
        int panelHeight = screenHeight - 20;
        
        mainPanel = createPanel(Rect(panelX, panelY, panelWidth, panelHeight), "Quaternion Visualizer");
        addComponent(mainPanel);
        
        createFileSection();
        createRotationSection();
        createControlButtons();
        
        
        statusLabel = createLabel(Rect(panelX + 10, panelY + panelHeight - 30, panelWidth - 20, 20), 
                                 "Ready");
        statusLabel->setTextColor(Color(150, 150, 150, 255));
        addComponent(statusLabel);
    }
    
    void UIManager::createFileSection() {
        Rect contentArea = mainPanel->getContentArea();
        
        
        filePanel = createPanel(Rect(contentArea.x, contentArea.y, contentArea.w, 80), "File Operations");
        mainPanel->addChild(filePanel);
        
        Rect fileContentArea = filePanel->getContentArea();
        
        
        chooseFileButton = createButton(
            Rect(fileContentArea.x, fileContentArea.y, fileContentArea.w, 25),
            "Choose OBJ File",
            [this]() { onChooseFileClicked(); }
        );
        filePanel->addChild(chooseFileButton);
        
        
        fileNameLabel = createLabel(
            Rect(fileContentArea.x, fileContentArea.y + 30, fileContentArea.w, 20),
            "No file selected"
        );
        fileNameLabel->setTextColor(Color(180, 180, 180, 255));
        filePanel->addChild(fileNameLabel);
    }
    
    void UIManager::createRotationSection() {
        Rect contentArea = mainPanel->getContentArea();
        
        
        rotationPanel = createPanel(
            Rect(contentArea.x, contentArea.y + 90, contentArea.w, 200), 
            "Rotation Parameters"
        );
        mainPanel->addChild(rotationPanel);
        
        Rect rotContentArea = rotationPanel->getContentArea();
        int inputWidth = (rotContentArea.w - 20) / 3; 
        
        
        auto axisLabel = createLabel(Rect(rotContentArea.x, rotContentArea.y, rotContentArea.w, 20), 
                                    "Rotation Axis (Unit Vector):");
        rotationPanel->addChild(axisLabel);
        
        
        auto xLabel = createLabel(Rect(rotContentArea.x, rotContentArea.y + 25, 15, 20), "X:");
        rotationPanel->addChild(xLabel);
        axisXInput = createInputField(Rect(rotContentArea.x + 20, rotContentArea.y + 25, inputWidth - 25, 25), "1.0");
        axisXInput->setText("1.0");
        rotationPanel->addChild(axisXInput);
        
        
        auto yLabel = createLabel(Rect(rotContentArea.x + inputWidth, rotContentArea.y + 25, 15, 20), "Y:");
        rotationPanel->addChild(yLabel);
        axisYInput = createInputField(Rect(rotContentArea.x + inputWidth + 20, rotContentArea.y + 25, inputWidth - 25, 25), "0.0");
        axisYInput->setText("0.0");
        rotationPanel->addChild(axisYInput);
        
        
        auto zLabel = createLabel(Rect(rotContentArea.x + inputWidth * 2, rotContentArea.y + 25, 15, 20), "Z:");
        rotationPanel->addChild(zLabel);
        axisZInput = createInputField(Rect(rotContentArea.x + inputWidth * 2 + 20, rotContentArea.y + 25, inputWidth - 25, 25), "0.0");
        axisZInput->setText("0.0");
        rotationPanel->addChild(axisZInput);
        
        
        auto angleLabel = createLabel(Rect(rotContentArea.x, rotContentArea.y + 60, rotContentArea.w, 20), 
                                     "Rotation Angle (degrees):");
        rotationPanel->addChild(angleLabel);
        
        angleInput = createInputField(Rect(rotContentArea.x, rotContentArea.y + 85, rotContentArea.w, 25), "45.0");
        angleInput->setText("45.0");
        rotationPanel->addChild(angleInput);
        
        
        auto quatLabel = createLabel(Rect(rotContentArea.x, rotContentArea.y + 120, rotContentArea.w, 20), 
                                    "Quaternion:");
        rotationPanel->addChild(quatLabel);
        
        quaternionDisplay = createLabel(Rect(rotContentArea.x, rotContentArea.y + 145, rotContentArea.w, 20), 
                                       "q = (1.0, 0.0, 0.0, 0.0)");
        quaternionDisplay->setTextColor(Color(100, 200, 100, 255)); 
        rotationPanel->addChild(quaternionDisplay);
    }
    
    void UIManager::createControlButtons() {
        Rect contentArea = mainPanel->getContentArea();
        
        int buttonY = contentArea.y + 300;
        int buttonWidth = (contentArea.w - 10) / 2;
        
        
        applyButton = createButton(
            Rect(contentArea.x, buttonY, buttonWidth, 35),
            "Apply Rotation",
            [this]() { onApplyClicked(); }
        );
        applyButton->setColors(Color(0, 120, 50, 255), Color(0, 140, 60, 255), Color(0, 100, 40, 255)); 
        mainPanel->addChild(applyButton);
        
        
        resetButton = createButton(
            Rect(contentArea.x + buttonWidth + 10, buttonY, buttonWidth, 35),
            "Reset",
            [this]() { onResetClicked(); }
        );
        resetButton->setColors(Color(120, 50, 0, 255), Color(140, 60, 0, 255), Color(100, 40, 0, 255)); 
        mainPanel->addChild(resetButton);
    }
    
    
    void UIManager::onChooseFileClicked() {
        showFileDialog([this](const std::string& filename) {
            if (onFileSelected) {
                onFileSelected(filename);
            }
            setLoadedFileName(filename);
        });
    }
    
    void UIManager::onApplyClicked() {
        normalizeAxis();
        if (onApplyRotation) {
            onApplyRotation();
        }
        statusLabel->setText("Rotation applied");
    }
    
    void UIManager::onResetClicked() {
        if (onResetRotation) {
            onResetRotation();
        }
        statusLabel->setText("Rotation reset");
    }
    
    void UIManager::normalizeAxis() {
        float x = axisXInput->getFloatValue();
        float y = axisYInput->getFloatValue();
        float z = axisZInput->getFloatValue();
        
        float length = std::sqrt(x*x + y*y + z*z);
        if (length > 0.001f) { 
            x /= length;
            y /= length;
            z /= length;
            
            axisXInput->setFloatValue(x);
            axisYInput->setFloatValue(y);
            axisZInput->setFloatValue(z);
        }
    }
    
    void UIManager::updateQuaternionDisplay() {
        float angle = getRotationAngle();
        float x, y, z;
        getRotationAxis(x, y, z);
        
        
        float halfAngle = angle * 0.5f * (3.14159265f / 180.0f); 
        float sinHalf = std::sin(halfAngle);
        float cosHalf = std::cos(halfAngle);
        
        float qw = cosHalf;
        float qx = x * sinHalf;
        float qy = y * sinHalf;
        float qz = z * sinHalf;
        
        
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(3);
        oss << "q = (" << qw << ", " << qx << ", " << qy << ", " << qz << ")";
        
        quaternionDisplay->setText(oss.str());
    }
    
    
    float UIManager::getRotationAngle() const {
        return angleInput ? angleInput->getFloatValue() : 0.0f;
    }
    
    void UIManager::getRotationAxis(float& x, float& y, float& z) const {
        x = axisXInput ? axisXInput->getFloatValue() : 1.0f;
        y = axisYInput ? axisYInput->getFloatValue() : 0.0f;
        z = axisZInput ? axisZInput->getFloatValue() : 0.0f;
    }
    
    void UIManager::setRotationAngle(float angle) {
        if (angleInput) {
            angleInput->setFloatValue(angle);
        }
    }
    
    void UIManager::setRotationAxis(float x, float y, float z) {
        if (axisXInput) axisXInput->setFloatValue(x);
        if (axisYInput) axisYInput->setFloatValue(y);
        if (axisZInput) axisZInput->setFloatValue(z);
    }
    
    void UIManager::setLoadedFileName(const std::string& filename) {
        if (fileNameLabel) {
            
            size_t lastSlash = filename.find_last_of("/\\");
            std::string displayName = (lastSlash != std::string::npos) ? 
                filename.substr(lastSlash + 1) : filename;
            
            fileNameLabel->setText(displayName);
            fileNameLabel->setTextColor(Color(100, 255, 100, 255)); 
        }
    }
    
} // namespace