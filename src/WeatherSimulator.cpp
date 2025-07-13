#include "WeatherSimulator.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <cmath>

WeatherSimulator::WeatherSimulator()
    : rng(std::random_device{}())
    , tempVariation(-2.0, 2.0)
    , humidityVariation(-5.0, 5.0)
    , windVariation(0.0, 10.0)
    , seasonalBaseTemp(20.0)
    , seasonalHumidity(50.0)
    , seasonalCondition(WeatherCondition::SUNNY)
    , lastUpdate(std::chrono::system_clock::now())
{
    // Initialize current weather
    currentWeather.temperature = 20.0;
    currentWeather.humidity = 50.0;
    currentWeather.windSpeed = 5.0;
    currentWeather.condition = WeatherCondition::SUNNY;
    currentWeather.pressure = 1013.25;
    currentWeather.description = "Sunny";
    
    // Initialize forecast
    for (int i = 0; i < 24; ++i) {
        forecastWeather[i] = currentWeather;
    }
}

void WeatherSimulator::initialize() {
    std::cout << "Initializing weather simulator..." << std::endl;
    generateRandomWeather();
    generateForecast();
    std::cout << "Weather simulator initialized successfully!" << std::endl;
}

void WeatherSimulator::update() {
    if (shouldUpdate()) {
        generateRandomWeather();
        updateForecast();
        lastUpdate = std::chrono::system_clock::now();
    }
}

void WeatherSimulator::shutdown() {
    std::cout << "Weather simulator shutdown." << std::endl;
}

WeatherData WeatherSimulator::getCurrentWeather() const {
    return currentWeather;
}

WeatherData WeatherSimulator::getForecast(int hour) const {
    if (hour >= 0 && hour < 24) {
        return forecastWeather[hour];
    }
    return currentWeather;
}

double WeatherSimulator::getOutdoorTemperature() const {
    return currentWeather.temperature;
}

double WeatherSimulator::getOutdoorHumidity() const {
    return currentWeather.humidity;
}

WeatherCondition WeatherSimulator::getWeatherCondition() const {
    return currentWeather.condition;
}

std::string WeatherSimulator::getWeatherDescription() const {
    return currentWeather.description;
}

void WeatherSimulator::setSeasonalParameters(double baseTemp, double humidity, WeatherCondition condition) {
    seasonalBaseTemp = baseTemp;
    seasonalHumidity = humidity;
    seasonalCondition = condition;
    
    std::cout << "Seasonal parameters updated: " << baseTemp << "°C, " 
              << humidity << "% humidity, " << getConditionString(condition) << std::endl;
}

void WeatherSimulator::simulateWeatherChange(double targetTemp, double targetHumidity, WeatherCondition condition) {
    currentWeather.temperature = targetTemp;
    currentWeather.humidity = targetHumidity;
    currentWeather.condition = condition;
    currentWeather.description = getConditionString(condition);
    
    std::cout << "Weather changed to: " << targetTemp << "°C, " 
              << targetHumidity << "% humidity, " << currentWeather.description << std::endl;
}

void WeatherSimulator::generateForecast() {
    for (int hour = 0; hour < 24; ++hour) {
        WeatherData& forecast = forecastWeather[hour];
        
        // Simulate gradual weather changes
        double timeFactor = hour / 24.0;
        double tempChange = (tempVariation(rng) - 1.0) * timeFactor;
        double humidityChange = humidityVariation(rng) * timeFactor;
        
        forecast.temperature = currentWeather.temperature + tempChange;
        forecast.humidity = std::max(0.0, std::min(100.0, currentWeather.humidity + humidityChange));
        forecast.windSpeed = std::max(0.0, currentWeather.windSpeed + windVariation(rng) * 0.1);
        forecast.pressure = currentWeather.pressure + (tempVariation(rng) * 2.0);
        
        // Update weather condition based on temperature and humidity
        updateWeatherCondition(forecast);
    }
}

double WeatherSimulator::getHeatLoad() const {
    return calculateHeatLoad();
}

double WeatherSimulator::getCoolingLoad() const {
    return calculateCoolingLoad();
}

bool WeatherSimulator::shouldDefrost() const {
    return currentWeather.temperature < 2.0 && currentWeather.humidity > 80.0;
}

double WeatherSimulator::getOptimalIndoorTemp() const {
    // Calculate optimal indoor temperature based on outdoor conditions
    double outdoorTemp = currentWeather.temperature;
    double humidity = currentWeather.humidity;
    
    // Base optimal temperature
    double optimal = 22.0;
    
    // Adjust for outdoor temperature
    if (outdoorTemp < 0.0) {
        optimal += 1.0; // Slightly warmer in very cold weather
    } else if (outdoorTemp > 30.0) {
        optimal -= 1.0; // Slightly cooler in very hot weather
    }
    
    // Adjust for humidity
    if (humidity > 70.0) {
        optimal -= 0.5; // Cooler in high humidity
    } else if (humidity < 30.0) {
        optimal += 0.5; // Warmer in low humidity
    }
    
    return optimal;
}

bool WeatherSimulator::hasWeatherAlert() const {
    return currentWeather.windSpeed > 25.0 || 
           currentWeather.temperature < -10.0 || 
           currentWeather.temperature > 40.0 ||
           currentWeather.condition == WeatherCondition::STORMY;
}

std::string WeatherSimulator::getWeatherAlert() const {
    if (!hasWeatherAlert()) {
        return "No weather alerts";
    }
    
    std::stringstream ss;
    ss << "WEATHER ALERT: ";
    
    if (currentWeather.windSpeed > 25.0) {
        ss << "High winds (" << currentWeather.windSpeed << " km/h)";
    } else if (currentWeather.temperature < -10.0) {
        ss << "Extreme cold (" << currentWeather.temperature << "°C)";
    } else if (currentWeather.temperature > 40.0) {
        ss << "Extreme heat (" << currentWeather.temperature << "°C)";
    } else if (currentWeather.condition == WeatherCondition::STORMY) {
        ss << "Storm conditions";
    }
    
    return ss.str();
}

void WeatherSimulator::logWeatherData() {
    std::cout << "Weather logged: " << currentWeather.temperature << "°C, " 
              << currentWeather.humidity << "% humidity, " 
              << currentWeather.windSpeed << " km/h wind" << std::endl;
}

std::string WeatherSimulator::getWeatherTrend() const {
    double tempDiff = forecastWeather[12].temperature - currentWeather.temperature;
    std::stringstream ss;
    
    if (std::abs(tempDiff) < 2.0) {
        ss << "Temperature stable";
    } else if (tempDiff > 2.0) {
        ss << "Temperature warming";
    } else {
        ss << "Temperature cooling";
    }
    
    return ss.str();
}

void WeatherSimulator::generateRandomWeather() {
    // Apply seasonal variation
    applySeasonalVariation();
    
    // Add random variations
    currentWeather.temperature += tempVariation(rng);
    currentWeather.humidity += humidityVariation(rng);
    currentWeather.windSpeed += windVariation(rng);
    
    // Clamp values to reasonable ranges
    currentWeather.temperature = std::max(-30.0, std::min(50.0, currentWeather.temperature));
    currentWeather.humidity = std::max(0.0, std::min(100.0, currentWeather.humidity));
    currentWeather.windSpeed = std::max(0.0, std::min(50.0, currentWeather.windSpeed));
    
    // Update weather condition based on new values
    updateWeatherCondition(currentWeather);
}

void WeatherSimulator::updateForecast() {
    // Update forecast based on current weather trends
    for (int hour = 0; hour < 24; ++hour) {
        WeatherData& forecast = forecastWeather[hour];
        
        // Gradually adjust forecast based on current conditions
        double adjustment = 0.1; // Small adjustment factor
        forecast.temperature += (currentWeather.temperature - forecast.temperature) * adjustment;
        forecast.humidity += (currentWeather.humidity - forecast.humidity) * adjustment;
        
        // Clamp values
        forecast.temperature = std::max(-30.0, std::min(50.0, forecast.temperature));
        forecast.humidity = std::max(0.0, std::min(100.0, forecast.humidity));
        
        updateWeatherCondition(forecast);
    }
}

double WeatherSimulator::calculateHeatLoad() const {
    // Calculate heating load based on outdoor temperature
    double outdoorTemp = currentWeather.temperature;
    double indoorTemp = 22.0; // Target indoor temperature
    double tempDiff = indoorTemp - outdoorTemp;
    
    if (tempDiff <= 0) {
        return 0.0; // No heating needed
    }
    
    // Base heating load in kW
    double baseLoad = tempDiff * 0.5; // 0.5 kW per degree difference
    
    // Adjust for wind (wind chill effect)
    double windFactor = 1.0 + (currentWeather.windSpeed * 0.02);
    
    // Adjust for humidity
    double humidityFactor = 1.0 + (currentWeather.humidity - 50.0) * 0.01;
    
    return baseLoad * windFactor * humidityFactor;
}

double WeatherSimulator::calculateCoolingLoad() const {
    // Calculate cooling load based on outdoor temperature
    double outdoorTemp = currentWeather.temperature;
    double indoorTemp = 22.0; // Target indoor temperature
    double tempDiff = outdoorTemp - indoorTemp;
    
    if (tempDiff <= 0) {
        return 0.0; // No cooling needed
    }
    
    // Base cooling load in kW
    double baseLoad = tempDiff * 0.4; // 0.4 kW per degree difference
    
    // Adjust for humidity (higher humidity = higher cooling load)
    double humidityFactor = 1.0 + (currentWeather.humidity - 50.0) * 0.02;
    
    // Adjust for solar gain based on weather condition
    double solarFactor = 1.0;
    if (currentWeather.condition == WeatherCondition::SUNNY) {
        solarFactor = 1.3; // 30% more load on sunny days
    } else if (currentWeather.condition == WeatherCondition::CLOUDY) {
        solarFactor = 1.1; // 10% more load on cloudy days
    }
    
    return baseLoad * humidityFactor * solarFactor;
}

bool WeatherSimulator::shouldUpdate() const {
    auto now = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastUpdate);
    return duration.count() >= UPDATE_INTERVAL_MS;
}

void WeatherSimulator::applySeasonalVariation() {
    // Simulate seasonal temperature changes
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    std::tm* tm = std::localtime(&time_t);
    
    int month = tm->tm_mon; // 0-11 (January = 0)
    
    // Seasonal temperature adjustments
    double seasonalAdjustment = 0.0;
    switch (month) {
        case 11: case 0: case 1:  // Winter (Dec, Jan, Feb)
            seasonalAdjustment = -10.0;
            seasonalCondition = WeatherCondition::SNOWY;
            break;
        case 2: case 3: case 4:   // Spring (Mar, Apr, May)
            seasonalAdjustment = 5.0;
            seasonalCondition = WeatherCondition::RAINY;
            break;
        case 5: case 6: case 7:   // Summer (Jun, Jul, Aug)
            seasonalAdjustment = 15.0;
            seasonalCondition = WeatherCondition::SUNNY;
            break;
        case 8: case 9: case 10:  // Fall (Sep, Oct, Nov)
            seasonalAdjustment = 0.0;
            seasonalCondition = WeatherCondition::CLOUDY;
            break;
    }
    
    currentWeather.temperature = seasonalBaseTemp + seasonalAdjustment;
    currentWeather.condition = seasonalCondition;
}

void WeatherSimulator::updateWeatherCondition(WeatherData& weather) {
    // Update weather condition based on temperature, humidity, and pressure
    if (weather.temperature < 0.0 && weather.humidity > 70.0) {
        weather.condition = WeatherCondition::SNOWY;
        weather.description = "Snowy";
    } else if (weather.humidity > 80.0) {
        weather.condition = WeatherCondition::RAINY;
        weather.description = "Rainy";
    } else if (weather.windSpeed > 20.0) {
        weather.condition = WeatherCondition::WINDY;
        weather.description = "Windy";
    } else if (weather.windSpeed > 30.0 || weather.pressure < 1000.0) {
        weather.condition = WeatherCondition::STORMY;
        weather.description = "Stormy";
    } else if (weather.humidity > 60.0) {
        weather.condition = WeatherCondition::CLOUDY;
        weather.description = "Cloudy";
    } else {
        weather.condition = WeatherCondition::SUNNY;
        weather.description = "Sunny";
    }
}

std::string WeatherSimulator::getConditionString(WeatherCondition condition) const {
    switch (condition) {
        case WeatherCondition::SUNNY: return "Sunny";
        case WeatherCondition::CLOUDY: return "Cloudy";
        case WeatherCondition::RAINY: return "Rainy";
        case WeatherCondition::SNOWY: return "Snowy";
        case WeatherCondition::WINDY: return "Windy";
        case WeatherCondition::STORMY: return "Stormy";
        default: return "Unknown";
    }
} 