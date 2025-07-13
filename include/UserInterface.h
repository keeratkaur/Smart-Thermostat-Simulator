#pragma once

#include <string>
#include <vector>
#include <functional>

enum class DisplayMode {
    MAIN_SCREEN,
    SETTINGS,
    SCHEDULE,
    ENERGY_REPORT,
    SYSTEM_STATUS,
    WEATHER_INFO
};

struct MenuItem {
    std::string label;
    std::function<void()> action;
    bool isEnabled;
    
    MenuItem(const std::string& l, std::function<void()> a, bool enabled = true)
        : label(l), action(a), isEnabled(enabled) {}
};

class UserInterface {
private:
    DisplayMode currentMode;
    std::vector<MenuItem> mainMenu;
    std::vector<MenuItem> settingsMenu;
    
    bool isDisplayOn;
    int brightness;
    std::string lastMessage;
    std::chrono::system_clock::time_point lastInteraction;
    
    // Display settings
    const int DISPLAY_TIMEOUT_SECONDS = 30;
    const int MAX_BRIGHTNESS = 100;
    const int MIN_BRIGHTNESS = 10;

public:
    UserInterface();
    ~UserInterface() = default;

    // Core functionality
    void initialize();
    void update();
    void shutdown();
    
    // Display control
    void turnOnDisplay();
    void turnOffDisplay();
    bool isDisplayActive() const;
    void setBrightness(int level);
    int getBrightness() const;
    
    // Mode management
    void setDisplayMode(DisplayMode mode);
    DisplayMode getCurrentMode() const;
    void showMainScreen();
    void showSettings();
    void showSchedule();
    void showEnergyReport();
    void showSystemStatus();
    void showWeatherInfo();
    
    // User interaction
    void handleButtonPress(int buttonId);
    void handleTemperatureUp();
    void handleTemperatureDown();
    void handleModeChange();
    void handleFanToggle();
    void handleScheduleToggle();
    
    // Information display
    std::string getMainDisplayText() const;
    std::string getSettingsDisplayText() const;
    std::string getScheduleDisplayText() const;
    std::string getEnergyDisplayText() const;
    std::string getStatusDisplayText() const;
    std::string getWeatherDisplayText() const;
    
    // Menu system
    std::vector<MenuItem> getCurrentMenu() const;
    void selectMenuItem(int index);
    void navigateMenu(int direction);
    
    // Message system
    void showMessage(const std::string& message);
    void clearMessage();
    std::string getLastMessage() const;
    
    // System feedback
    void showAlert(const std::string& alert);
    void showConfirmation(const std::string& message);
    void showProgress(const std::string& operation, int percentage);

private:
    void initializeMenus();
    void updateDisplayTimeout();
    bool shouldTurnOffDisplay() const;
    std::string formatTemperature(double temp) const;
    std::string formatTime(int hour, int minute) const;
    std::string formatEnergy(double energy) const;
}; 