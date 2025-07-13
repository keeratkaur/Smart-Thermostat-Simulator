#include "UserInterface.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <algorithm>

UserInterface::UserInterface()
    : currentMode(DisplayMode::MAIN_SCREEN)
    , isDisplayOn(true)
    , brightness(80)
    , lastMessage("")
    , lastInteraction(std::chrono::system_clock::now())
{
}

void UserInterface::initialize() {
    std::cout << "Initializing user interface..." << std::endl;
    initializeMenus();
    turnOnDisplay();
    std::cout << "User interface initialized successfully!" << std::endl;
}

void UserInterface::update() {
    updateDisplayTimeout();
    
    // Simulate display timeout
    if (shouldTurnOffDisplay()) {
        turnOffDisplay();
    }
}

void UserInterface::shutdown() {
    std::cout << "Shutting down user interface..." << std::endl;
    turnOffDisplay();
    std::cout << "User interface shutdown complete." << std::endl;
}

void UserInterface::turnOnDisplay() {
    isDisplayOn = true;
    lastInteraction = std::chrono::system_clock::now();
    std::cout << "Display turned ON" << std::endl;
}

void UserInterface::turnOffDisplay() {
    isDisplayOn = false;
    std::cout << "Display turned OFF" << std::endl;
}

bool UserInterface::isDisplayActive() const {
    return isDisplayOn;
}

void UserInterface::setBrightness(int level) {
    if (level >= MIN_BRIGHTNESS && level <= MAX_BRIGHTNESS) {
        brightness = level;
        std::cout << "Display brightness set to " << brightness << "%" << std::endl;
    } else {
        std::cout << "Invalid brightness level: " << level << "% (must be " 
                  << MIN_BRIGHTNESS << "-" << MAX_BRIGHTNESS << ")" << std::endl;
    }
}

int UserInterface::getBrightness() const {
    return brightness;
}

void UserInterface::setDisplayMode(DisplayMode mode) {
    currentMode = mode;
    lastInteraction = std::chrono::system_clock::now();
    std::cout << "Display mode changed to " << getModeString(mode) << std::endl;
}

DisplayMode UserInterface::getCurrentMode() const {
    return currentMode;
}

void UserInterface::showMainScreen() {
    setDisplayMode(DisplayMode::MAIN_SCREEN);
}

void UserInterface::showSettings() {
    setDisplayMode(DisplayMode::SETTINGS);
}

void UserInterface::showSchedule() {
    setDisplayMode(DisplayMode::SCHEDULE);
}

void UserInterface::showEnergyReport() {
    setDisplayMode(DisplayMode::ENERGY_REPORT);
}

void UserInterface::showSystemStatus() {
    setDisplayMode(DisplayMode::SYSTEM_STATUS);
}

void UserInterface::showWeatherInfo() {
    setDisplayMode(DisplayMode::WEATHER_INFO);
}

void UserInterface::handleButtonPress(int buttonId) {
    lastInteraction = std::chrono::system_clock::now();
    turnOnDisplay();
    
    switch (buttonId) {
        case 1: // Temperature up
            handleTemperatureUp();
            break;
        case 2: // Temperature down
            handleTemperatureDown();
            break;
        case 3: // Mode change
            handleModeChange();
            break;
        case 4: // Fan toggle
            handleFanToggle();
            break;
        case 5: // Schedule toggle
            handleScheduleToggle();
            break;
        case 6: // Menu navigation
            navigateMenu(1);
            break;
        case 7: // Menu selection
            selectMenuItem(0);
            break;
        default:
            std::cout << "Unknown button pressed: " << buttonId << std::endl;
            break;
    }
}

void UserInterface::handleTemperatureUp() {
    showMessage("Temperature increased");
    std::cout << "Temperature UP button pressed" << std::endl;
}

void UserInterface::handleTemperatureDown() {
    showMessage("Temperature decreased");
    std::cout << "Temperature DOWN button pressed" << std::endl;
}

void UserInterface::handleModeChange() {
    showMessage("Mode changed");
    std::cout << "MODE button pressed" << std::endl;
}

void UserInterface::handleFanToggle() {
    showMessage("Fan toggled");
    std::cout << "FAN button pressed" << std::endl;
}

void UserInterface::handleScheduleToggle() {
    showMessage("Schedule toggled");
    std::cout << "SCHEDULE button pressed" << std::endl;
}

std::string UserInterface::getMainDisplayText() const {
    std::stringstream ss;
    ss << "=== SMART THERMOSTAT ===" << std::endl;
    ss << "Current: 22.5°C" << std::endl;
    ss << "Target:  22.0°C" << std::endl;
    ss << "Mode:    AUTO" << std::endl;
    ss << "Fan:     AUTO" << std::endl;
    ss << "Humidity: 45%" << std::endl;
    ss << "========================" << std::endl;
    return ss.str();
}

std::string UserInterface::getSettingsDisplayText() const {
    std::stringstream ss;
    ss << "=== SETTINGS ===" << std::endl;
    ss << "1. Temperature Range" << std::endl;
    ss << "2. Schedule Settings" << std::endl;
    ss << "3. Energy Settings" << std::endl;
    ss << "4. Display Settings" << std::endl;
    ss << "5. System Info" << std::endl;
    ss << "6. Back to Main" << std::endl;
    ss << "=================" << std::endl;
    return ss.str();
}

std::string UserInterface::getScheduleDisplayText() const {
    std::stringstream ss;
    ss << "=== SCHEDULE ===" << std::endl;
    ss << "Today's Schedule:" << std::endl;
    ss << "06:00 - 20.0°C (Wake)" << std::endl;
    ss << "08:00 - 18.0°C (Away)" << std::endl;
    ss << "17:00 - 20.0°C (Home)" << std::endl;
    ss << "22:00 - 18.0°C (Sleep)" << std::endl;
    ss << "Next: 08:00 (18.0°C)" << std::endl;
    ss << "=================" << std::endl;
    return ss.str();
}

std::string UserInterface::getEnergyDisplayText() const {
    std::stringstream ss;
    ss << "=== ENERGY REPORT ===" << std::endl;
    ss << "Current Power: 2.5 kW" << std::endl;
    ss << "Today's Usage: 15.2 kWh" << std::endl;
    ss << "Today's Cost:  $1.82" << std::endl;
    ss << "Efficiency:    85%" << std::endl;
    ss << "Carbon Footprint: 7.6 kg CO2" << std::endl;
    ss << "Money Saved:   $0.50" << std::endl;
    ss << "=====================" << std::endl;
    return ss.str();
}

std::string UserInterface::getStatusDisplayText() const {
    std::stringstream ss;
    ss << "=== SYSTEM STATUS ===" << std::endl;
    ss << "HVAC:          HEATING" << std::endl;
    ss << "Compressor:    RUNNING" << std::endl;
    ss << "Fan:           ON" << std::endl;
    ss << "Efficiency:    85%" << std::endl;
    ss << "Runtime:       2h 15m" << std::endl;
    ss << "Power:         3.2 kW" << std::endl;
    ss << "Status:        NORMAL" << std::endl;
    ss << "=====================" << std::endl;
    return ss.str();
}

std::string UserInterface::getWeatherDisplayText() const {
    std::stringstream ss;
    ss << "=== WEATHER INFO ===" << std::endl;
    ss << "Outdoor:       15.2°C" << std::endl;
    ss << "Humidity:      65%" << std::endl;
    ss << "Wind:          8 km/h" << std::endl;
    ss << "Condition:     Cloudy" << std::endl;
    ss << "Heat Load:     3.5 kW" << std::endl;
    ss << "Cooling Load:  0.0 kW" << std::endl;
    ss << "Alert:         None" << std::endl;
    ss << "====================" << std::endl;
    return ss.str();
}

std::vector<MenuItem> UserInterface::getCurrentMenu() const {
    switch (currentMode) {
        case DisplayMode::SETTINGS:
            return settingsMenu;
        default:
            return mainMenu;
    }
}

void UserInterface::selectMenuItem(int index) {
    auto menu = getCurrentMenu();
    if (index >= 0 && index < static_cast<int>(menu.size())) {
        if (menu[index].isEnabled) {
            menu[index].action();
        } else {
            showMessage("Option disabled");
        }
    } else {
        showMessage("Invalid selection");
    }
}

void UserInterface::navigateMenu(int direction) {
    // Simulate menu navigation
    std::cout << "Menu navigation: " << (direction > 0 ? "Next" : "Previous") << std::endl;
}

void UserInterface::showMessage(const std::string& message) {
    lastMessage = message;
    lastInteraction = std::chrono::system_clock::now();
    std::cout << "UI Message: " << message << std::endl;
}

void UserInterface::clearMessage() {
    lastMessage = "";
}

std::string UserInterface::getLastMessage() const {
    return lastMessage;
}

void UserInterface::showAlert(const std::string& alert) {
    std::cout << "ALERT: " << alert << std::endl;
    showMessage("Alert: " + alert);
}

void UserInterface::showConfirmation(const std::string& message) {
    std::cout << "CONFIRM: " << message << std::endl;
    showMessage("Confirm: " + message);
}

void UserInterface::showProgress(const std::string& operation, int percentage) {
    std::cout << "PROGRESS: " << operation << " - " << percentage << "%" << std::endl;
    showMessage(operation + " - " + std::to_string(percentage) + "%");
}

void UserInterface::initializeMenus() {
    // Initialize main menu
    mainMenu.clear();
    mainMenu.push_back(MenuItem("Settings", []() { std::cout << "Settings selected" << std::endl; }));
    mainMenu.push_back(MenuItem("Schedule", []() { std::cout << "Schedule selected" << std::endl; }));
    mainMenu.push_back(MenuItem("Energy Report", []() { std::cout << "Energy report selected" << std::endl; }));
    mainMenu.push_back(MenuItem("System Status", []() { std::cout << "System status selected" << std::endl; }));
    mainMenu.push_back(MenuItem("Weather Info", []() { std::cout << "Weather info selected" << std::endl; }));
    
    // Initialize settings menu
    settingsMenu.clear();
    settingsMenu.push_back(MenuItem("Temperature Range", []() { std::cout << "Temperature range setting" << std::endl; }));
    settingsMenu.push_back(MenuItem("Schedule Settings", []() { std::cout << "Schedule settings" << std::endl; }));
    settingsMenu.push_back(MenuItem("Energy Settings", []() { std::cout << "Energy settings" << std::endl; }));
    settingsMenu.push_back(MenuItem("Display Settings", []() { std::cout << "Display settings" << std::endl; }));
    settingsMenu.push_back(MenuItem("System Info", []() { std::cout << "System info" << std::endl; }));
    settingsMenu.push_back(MenuItem("Back to Main", []() { std::cout << "Back to main" << std::endl; }));
}

void UserInterface::updateDisplayTimeout() {
    auto now = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(now - lastInteraction);
    
    if (duration.count() > DISPLAY_TIMEOUT_SECONDS) {
        // Display would timeout in real implementation
        // For simulation, we just log it
        if (isDisplayOn) {
            std::cout << "Display timeout approaching..." << std::endl;
        }
    }
}

bool UserInterface::shouldTurnOffDisplay() const {
    auto now = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(now - lastInteraction);
    return duration.count() > DISPLAY_TIMEOUT_SECONDS;
}

std::string UserInterface::formatTemperature(double temp) const {
    std::stringstream ss;
    ss << std::fixed << std::setprecision(1) << temp << "°C";
    return ss.str();
}

std::string UserInterface::formatTime(int hour, int minute) const {
    std::stringstream ss;
    ss << std::setfill('0') << std::setw(2) << hour << ":" 
       << std::setw(2) << minute;
    return ss.str();
}

std::string UserInterface::formatEnergy(double energy) const {
    std::stringstream ss;
    ss << std::fixed << std::setprecision(2) << energy << " kWh";
    return ss.str();
}

std::string UserInterface::getModeString(DisplayMode mode) const {
    switch (mode) {
        case DisplayMode::MAIN_SCREEN: return "MAIN_SCREEN";
        case DisplayMode::SETTINGS: return "SETTINGS";
        case DisplayMode::SCHEDULE: return "SCHEDULE";
        case DisplayMode::ENERGY_REPORT: return "ENERGY_REPORT";
        case DisplayMode::SYSTEM_STATUS: return "SYSTEM_STATUS";
        case DisplayMode::WEATHER_INFO: return "WEATHER_INFO";
        default: return "UNKNOWN";
    }
} 