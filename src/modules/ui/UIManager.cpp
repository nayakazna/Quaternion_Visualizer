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
        
        int panelWidth = 510;
        int panelX = screenWidth - panelWidth - 10;
        int panelY = 10;
        int panelHeight = screenHeight - 20;
        
        mainPanel = createPanel(Rect(panelX, panelY, panelWidth, panelHeight), "Quaternion Visualizer");
        addComponent(mainPanel);
        
        createFileSection();
        createMethodSelection();
        createRotationSection();
        createEulerSection();
        createTaitBryanSection();
        createControlButtons();
        createInfoSection();
        
        updateVisiblePanels();
        
        statusLabel = createLabel(Rect(panelX + 10, panelY + panelHeight - 30, panelWidth - 20, 20), 
                                 "Ready");
        statusLabel->setTextColor(Color(150, 150, 150, 255));
        addComponent(statusLabel);
    }

    void UIManager::updateVisiblePanels() {
        rotationPanel->setVisible(currentMethod == RotationMethod::QUATERNION);
        eulerPanel->setVisible(currentMethod == RotationMethod::EULER_ANGLES);
        taitBryanPanel->setVisible(currentMethod == RotationMethod::TAIT_BRYAN);
    }

    RotationMethod UIManager::getRotationMethod() const {
        return currentMethod;
    }

    void UIManager::getEulerAngles(float& alpha, float& beta, float& gamma) const {
        alpha = alphaInput ? alphaInput->getFloatValue() : 0.0f;
        beta = betaInput ? betaInput->getFloatValue() : 0.0f;
        gamma = gammaInput ? gammaInput->getFloatValue() : 0.0f;
    }

    void UIManager::getTaitBryanAngles(float& yaw, float& pitch, float& roll) const {
        yaw = yawInput ? yawInput->getFloatValue() : 0.0f;
        pitch = pitchInput ? pitchInput->getFloatValue() : 0.0f;
        roll = rollInput ? rollInput->getFloatValue() : 0.0f;
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
            Rect(contentArea.x, contentArea.y + 220, contentArea.w, 200), 
            "Parameter Rotasi"
        );
        mainPanel->addChild(rotationPanel);
        
        Rect rotContentArea = rotationPanel->getContentArea();
        int inputWidth = (rotContentArea.w - 20) / 3; 
        
        
        auto axisLabel = createLabel(Rect(rotContentArea.x, rotContentArea.y, rotContentArea.w, 20), 
                                    "Sumbu Rotasi (Vektor Satuan):");
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
                                     "Sudut Rotasi (degrees):");
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

    void UIManager::createMethodSelection() {
        Rect contentArea = mainPanel->getContentArea();
        
        auto methodPanel = createPanel(Rect(contentArea.x, contentArea.y + 90, contentArea.w, 120), 
                                    "Metode Rotasi");
        mainPanel->addChild(methodPanel);
        
        Rect methodContentArea = methodPanel->getContentArea();
        
        std::vector<std::string> methods = {
            "Quaternion",
            "Euler Angles",
            "Tait-Bryan"
        };
        
        methodSelector = std::make_shared<RadioButton>(
            Rect(methodContentArea.x, methodContentArea.y, methodContentArea.w, 60),
            methods,
            0,
            [this](int index) { onMethodChanged(index); }
        );
        
        methodPanel->addChild(methodSelector);
    }

    void UIManager::createEulerSection() {
        Rect contentArea = mainPanel->getContentArea();
        
        
        eulerPanel = createPanel(Rect(contentArea.x, contentArea.y + 220, contentArea.w, 150), 
                                "Euler Angles (Z-Y-X)");
        mainPanel->addChild(eulerPanel);
        
        Rect eulerContentArea = eulerPanel->getContentArea();
        int inputWidth = (eulerContentArea.w - 20) / 3;
        
        
        auto alphaLabel = createLabel(Rect(eulerContentArea.x, eulerContentArea.y, eulerContentArea.w, 20), 
                                    "alpha, beta, gamma:");
        eulerPanel->addChild(alphaLabel);
        
        auto alphaLbl = createLabel(Rect(eulerContentArea.x, eulerContentArea.y + 25, 15, 20), "alpha:");
        eulerPanel->addChild(alphaLbl);
        alphaInput = createInputField(Rect(eulerContentArea.x + 20, eulerContentArea.y + 25, inputWidth - 25, 25), "0.0");
        alphaInput->setText("0.0");
        eulerPanel->addChild(alphaInput);
        
        
        auto betaLbl = createLabel(Rect(eulerContentArea.x + inputWidth, eulerContentArea.y + 25, 15, 20), "beta:");
        eulerPanel->addChild(betaLbl);
        betaInput = createInputField(Rect(eulerContentArea.x + inputWidth + 20, eulerContentArea.y + 25, inputWidth - 25, 25), "0.0");
        betaInput->setText("45.0");
        eulerPanel->addChild(betaInput);
        
        
        auto gammaLbl = createLabel(Rect(eulerContentArea.x + inputWidth * 2, eulerContentArea.y + 25, 15, 20), "gamma:");
        eulerPanel->addChild(gammaLbl);
        gammaInput = createInputField(Rect(eulerContentArea.x + inputWidth * 2 + 20, eulerContentArea.y + 25, inputWidth - 25, 25), "0.0");
        gammaInput->setText("0.0");
        eulerPanel->addChild(gammaInput);
        
        
        auto descLabel = createLabel(Rect(eulerContentArea.x, eulerContentArea.y + 60, eulerContentArea.w, 20), 
                                    "Urutan rotasi: Z(alpha) -> Y(beta) -> X(gamma)");
        descLabel->setTextColor(Color(150, 150, 150, 255));
        eulerPanel->addChild(descLabel);
    }

    void UIManager::createTaitBryanSection() {
        Rect contentArea = mainPanel->getContentArea();
        
        
        taitBryanPanel = createPanel(Rect(contentArea.x, contentArea.y + 220, contentArea.w, 150), 
                                    "Tait-Bryan Angles");
        mainPanel->addChild(taitBryanPanel);
        
        Rect tbContentArea = taitBryanPanel->getContentArea();
        int inputWidth = (tbContentArea.w - 20) / 3;
        
        
        auto headerLabel = createLabel(Rect(tbContentArea.x, tbContentArea.y, tbContentArea.w, 20), 
                                    "Yaw (Z), Pitch (Y), Roll (X):");
        taitBryanPanel->addChild(headerLabel);
        
        
        auto yawLbl = createLabel(Rect(tbContentArea.x, tbContentArea.y + 25, 25, 20), "Yaw:");
        taitBryanPanel->addChild(yawLbl);
        yawInput = createInputField(Rect(tbContentArea.x + 30, tbContentArea.y + 25, inputWidth - 35, 25), "0.0");
        yawInput->setText("0.0");
        taitBryanPanel->addChild(yawInput);
        
        
        auto pitchLbl = createLabel(Rect(tbContentArea.x + inputWidth, tbContentArea.y + 25, 30, 20), "Pitch:");
        taitBryanPanel->addChild(pitchLbl);
        pitchInput = createInputField(Rect(tbContentArea.x + inputWidth + 35, tbContentArea.y + 25, inputWidth - 40, 25), "0.0");
        pitchInput->setText("45.0");
        taitBryanPanel->addChild(pitchInput);
        
        
        auto rollLbl = createLabel(Rect(tbContentArea.x + inputWidth * 2, tbContentArea.y + 25, 25, 20), "Roll:");
        taitBryanPanel->addChild(rollLbl);
        rollInput = createInputField(Rect(tbContentArea.x + inputWidth * 2 + 30, tbContentArea.y + 25, inputWidth - 35, 25), "0.0");
        rollInput->setText("0.0");
        taitBryanPanel->addChild(rollInput);
        
        
        auto descLabel = createLabel(Rect(tbContentArea.x, tbContentArea.y + 60, tbContentArea.w, 20), 
                                    "Rotasi intrinsik: Z(yaw) -> Y'(pitch) -> X''(roll)");
        descLabel->setTextColor(Color(150, 150, 150, 255));
        taitBryanPanel->addChild(descLabel);
    }

    void UIManager::onMethodChanged(int methodIndex) {
        currentMethod = static_cast<RotationMethod>(methodIndex);
        updateVisiblePanels();
        
        std::string methodName;
        switch (currentMethod) {
            case RotationMethod::QUATERNION: methodName = "Quaternion"; break;
            case RotationMethod::EULER_ANGLES: methodName = "Euler Angles"; break;
            case RotationMethod::TAIT_BRYAN: methodName = "Tait-Bryan"; break;
        }
        
        statusLabel->setText("Metode: " + methodName);
    }

    void UIManager::createControlButtons() {
        Rect contentArea = mainPanel->getContentArea();
        
        int buttonY = contentArea.y + 425;
        int buttonWidth = (contentArea.w - 10) / 2;
        
        
        applyButton = createButton(
            Rect(contentArea.x, buttonY, buttonWidth, 35),
            "Apply",
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
        statusLabel->setText("Rotasi diterapkan");
    }
    
    void UIManager::onResetClicked() {
        if (onResetRotation) {
            onResetRotation();
        }
        statusLabel->setText("Rotasi direset");
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
    
    void UIManager::createInfoSection() {
        Rect contentArea = mainPanel->getContentArea();
        
        
        infoPanel = createPanel(
            Rect(contentArea.x, contentArea.y + 500, contentArea.w, 450), 
            "Info"
        );
        mainPanel->addChild(infoPanel);
        
        Rect infoContentArea = infoPanel->getContentArea();
        int currentY = infoContentArea.y;
        int lineHeight = 18;
        int sectionSpacing = 8;
        
        
        auto controlsTitle = createLabel(Rect(infoContentArea.x, currentY, infoContentArea.w, lineHeight), 
                                        "Kontrol Kamera:");
        controlsTitle->setTextColor(Color(200, 200, 100, 255)); 
        infoPanel->addChild(controlsTitle);
        currentY += lineHeight + 3;
        
        auto wasdLabel = createLabel(Rect(infoContentArea.x, currentY, infoContentArea.w, lineHeight), 
                                    "WASD - Gerakin kamera");
        wasdLabel->setTextColor(Color(180, 180, 180, 255));
        infoPanel->addChild(wasdLabel);
        currentY += lineHeight;
        
        auto qeLabel = createLabel(Rect(infoContentArea.x, currentY, infoContentArea.w, lineHeight), 
                                "Q/E - Atas/Bawah");
        qeLabel->setTextColor(Color(180, 180, 180, 255));
        infoPanel->addChild(qeLabel);
        currentY += lineHeight;
        
        auto zcLabel = createLabel(Rect(infoContentArea.x, currentY, infoContentArea.w, lineHeight), 
                                "Z/C - Rotasi yaw");
        zcLabel->setTextColor(Color(180, 180, 180, 255));
        infoPanel->addChild(zcLabel);
        currentY += lineHeight;
        
        auto xvLabel = createLabel(Rect(infoContentArea.x, currentY, infoContentArea.w, lineHeight), 
                                "X/V - Rotasi pitch");
        xvLabel->setTextColor(Color(180, 180, 180, 255));
        infoPanel->addChild(xvLabel);
        currentY += lineHeight;
        
        auto escLabel = createLabel(Rect(infoContentArea.x, currentY, infoContentArea.w, lineHeight), 
                                "ESC - Nyala/matiin kontrol mouse");
        escLabel->setTextColor(Color(180, 180, 180, 255));
        infoPanel->addChild(escLabel);
        currentY += lineHeight + sectionSpacing;
        
        
        auto colorsTitle = createLabel(Rect(infoContentArea.x, currentY, infoContentArea.w, lineHeight), 
                                    "Panduan warna:");
        colorsTitle->setTextColor(Color(200, 200, 100, 255)); 
        infoPanel->addChild(colorsTitle);
        currentY += lineHeight + 3;
        
        auto greenLabel = createLabel(Rect(infoContentArea.x, currentY, infoContentArea.w, lineHeight), 
                                    "Abu-abu - sebelum rotasi");
        greenLabel->setTextColor(Color(100, 100, 100, 255)); 
        infoPanel->addChild(greenLabel);
        currentY += lineHeight;

        auto whiteLabel = createLabel(Rect(infoContentArea.x, currentY, infoContentArea.w, lineHeight), 
                                    "Putih - setelah rotasi");
        whiteLabel->setTextColor(Color(255, 255, 255, 255)); 
        infoPanel->addChild(whiteLabel);
        currentY += lineHeight + sectionSpacing;;
        
        auto axesTitle = createLabel(Rect(infoContentArea.x, currentY, infoContentArea.w, lineHeight), 
                                    "Sumbu Koordinat:");
        axesTitle->setTextColor(Color(200, 200, 100, 255)); 
        infoPanel->addChild(axesTitle);
        currentY += lineHeight + 3;
        
        auto xAxisLabel = createLabel(Rect(infoContentArea.x, currentY, infoContentArea.w, lineHeight), 
                                    "Merah - Sumbu-x");
        xAxisLabel->setTextColor(Color(255, 100, 100, 255)); 
        infoPanel->addChild(xAxisLabel);
        currentY += lineHeight;
        
        auto yAxisLabel = createLabel(Rect(infoContentArea.x, currentY, infoContentArea.w, lineHeight), 
                                    "Hijau - Sumbu-y");
        yAxisLabel->setTextColor(Color(100, 255, 100, 255)); 
        infoPanel->addChild(yAxisLabel);
        currentY += lineHeight;
        
        auto zAxisLabel = createLabel(Rect(infoContentArea.x, currentY, infoContentArea.w, lineHeight), 
                                    "Biru - Sumbu-z");
        zAxisLabel->setTextColor(Color(100, 150, 255, 255)); 
        infoPanel->addChild(zAxisLabel);
        currentY += lineHeight;

        auto methodTitle = createLabel(Rect(infoContentArea.x, currentY, infoContentArea.w, lineHeight), 
                                    "Sumbu berdasarkan metode rotasi: ");
        methodTitle->setTextColor(Color(200, 200, 100, 255));
        infoPanel->addChild(methodTitle);
        currentY += lineHeight + 3;
        
        auto quatLabel = createLabel(Rect(infoContentArea.x, currentY, infoContentArea.w, lineHeight), 
                                    "Quaternion - Ungu");
        quatLabel->setTextColor(Color(128, 0, 128, 255));
        infoPanel->addChild(quatLabel);
        currentY += lineHeight;
        
        auto eulerLabel = createLabel(Rect(infoContentArea.x, currentY, infoContentArea.w, lineHeight), 
                                    "Euler - Ungu(Z), Pink(Y), Orange(X)");
        eulerLabel->setTextColor(Color(255, 200, 100, 255));
        infoPanel->addChild(eulerLabel);
        currentY += lineHeight;
        
        auto taitLabel = createLabel(Rect(infoContentArea.x, currentY, infoContentArea.w, lineHeight), 
                                    "Tait-Bryan - Ungu(Yaw), Pink(Pitch), Orange(Roll)");
        taitLabel->setTextColor(Color(150, 200, 255, 255));
        infoPanel->addChild(taitLabel);
    }
} // namespace