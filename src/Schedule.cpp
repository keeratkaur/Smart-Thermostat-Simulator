#include "Schedule.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <ctime>

Schedule::Schedule()
    : isEnabled(true)
    , defaultTemperature(22.0)
    , lastScheduleCheck(std::chrono::system_clock::now())
{
    // Initialize with 7 days (Sunday = 0, Saturday = 6)
    weeklySchedule.resize(7);
    weeklySchedule[0] = DaySchedule("Sunday");
    weeklySchedule[1] = DaySchedule("Monday");
    weeklySchedule[2] = DaySchedule("Tuesday");
    weeklySchedule[3] = DaySchedule("Wednesday");
    weeklySchedule[4] = DaySchedule("Thursday");
    weeklySchedule[5] = DaySchedule("Friday");
    weeklySchedule[6] = DaySchedule("Saturday");
}

void Schedule::initialize() {
    std::cout << "Initializing schedule system..." << std::endl;
    initializeDefaultSchedule();
    std::cout << "Schedule system initialized successfully!" << std::endl;
}

void Schedule::loadDefaultSchedule() {
    // This would typically load from a file, but for simulation we'll use the default
    if (weeklySchedule.empty()) {
        initializeDefaultSchedule();
    }
}

void Schedule::saveSchedule() {
    std::cout << "Schedule saved (simulation mode)" << std::endl;
}

void Schedule::addEntry(int dayOfWeek, int hour, int minute, double temperature, const std::string& description) {
    if (dayOfWeek < 0 || dayOfWeek >= 7) {
        std::cout << "Invalid day of week: " << dayOfWeek << std::endl;
        return;
    }
    
    if (!isValidTime(hour, minute)) {
        std::cout << "Invalid time: " << hour << ":" << minute << std::endl;
        return;
    }
    
    if (!isValidTemperature(temperature)) {
        std::cout << "Invalid temperature: " << temperature << "°C" << std::endl;
        return;
    }
    
    ScheduleEntry entry(hour, minute, temperature, description);
    weeklySchedule[dayOfWeek].entries.push_back(entry);
    
    // Sort entries by time
    std::sort(weeklySchedule[dayOfWeek].entries.begin(), 
              weeklySchedule[dayOfWeek].entries.end(),
              [](const ScheduleEntry& a, const ScheduleEntry& b) {
                  return (a.hour * 60 + a.minute) < (b.hour * 60 + b.minute);
              });
    
    std::cout << "Added schedule entry: " << getDayName(dayOfWeek) << " " 
              << std::setfill('0') << std::setw(2) << hour << ":" 
              << std::setw(2) << minute << " - " << temperature << "°C" << std::endl;
}

void Schedule::removeEntry(int dayOfWeek, int entryIndex) {
    if (dayOfWeek < 0 || dayOfWeek >= 7) {
        std::cout << "Invalid day of week: " << dayOfWeek << std::endl;
        return;
    }
    
    if (entryIndex < 0 || entryIndex >= weeklySchedule[dayOfWeek].entries.size()) {
        std::cout << "Invalid entry index: " << entryIndex << std::endl;
        return;
    }
    
    auto& entries = weeklySchedule[dayOfWeek].entries;
    auto removed = entries[entryIndex];
    entries.erase(entries.begin() + entryIndex);
    
    std::cout << "Removed schedule entry: " << getDayName(dayOfWeek) << " " 
              << std::setfill('0') << std::setw(2) << removed.hour << ":" 
              << std::setw(2) << removed.minute << " - " << removed.temperature << "°C" << std::endl;
}

void Schedule::clearDay(int dayOfWeek) {
    if (dayOfWeek < 0 || dayOfWeek >= 7) {
        std::cout << "Invalid day of week: " << dayOfWeek << std::endl;
        return;
    }
    
    weeklySchedule[dayOfWeek].entries.clear();
    std::cout << "Cleared schedule for " << getDayName(dayOfWeek) << std::endl;
}

void Schedule::clearAll() {
    for (auto& day : weeklySchedule) {
        day.entries.clear();
    }
    std::cout << "Cleared all schedules" << std::endl;
}

double Schedule::getCurrentTargetTemperature() const {
    if (!isEnabled) {
        return defaultTemperature;
    }
    
    int currentDay = getDayOfWeek();
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    std::tm* tm = std::localtime(&time_t);
    
    int currentHour = tm->tm_hour;
    int currentMinute = tm->tm_min;
    
    return getTargetTemperatureAt(currentDay, currentHour, currentMinute);
}

double Schedule::getTargetTemperatureAt(int dayOfWeek, int hour, int minute) const {
    if (dayOfWeek < 0 || dayOfWeek >= 7) {
        return defaultTemperature;
    }
    
    const auto& entries = weeklySchedule[dayOfWeek].entries;
    if (entries.empty()) {
        return defaultTemperature;
    }
    
    int currentTimeMinutes = hour * 60 + minute;
    
    // Find the most recent entry that applies
    for (auto it = entries.rbegin(); it != entries.rend(); ++it) {
        int entryTimeMinutes = it->hour * 60 + it->minute;
        if (entryTimeMinutes <= currentTimeMinutes) {
            return it->temperature;
        }
    }
    
    // If no entry found, use the last entry from the previous day
    int prevDay = (dayOfWeek + 6) % 7; // Previous day
    const auto& prevEntries = weeklySchedule[prevDay].entries;
    if (!prevEntries.empty()) {
        return prevEntries.back().temperature;
    }
    
    return defaultTemperature;
}

std::string Schedule::getNextScheduleChange() const {
    int currentDay = getDayOfWeek();
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    std::tm* tm = std::localtime(&time_t);
    
    int currentHour = tm->tm_hour;
    int currentMinute = tm->tm_min;
    int currentTimeMinutes = currentHour * 60 + currentMinute;
    
    // Check today's schedule
    const auto& entries = weeklySchedule[currentDay].entries;
    for (const auto& entry : entries) {
        int entryTimeMinutes = entry.hour * 60 + entry.minute;
        if (entryTimeMinutes > currentTimeMinutes) {
            std::stringstream ss;
            ss << getDayName(currentDay) << " " 
               << std::setfill('0') << std::setw(2) << entry.hour << ":" 
               << std::setw(2) << entry.minute << " (" << entry.temperature << "°C)";
            return ss.str();
        }
    }
    
    // Check tomorrow's schedule
    int nextDay = (currentDay + 1) % 7;
    const auto& nextEntries = weeklySchedule[nextDay].entries;
    if (!nextEntries.empty()) {
        const auto& nextEntry = nextEntries.front();
        std::stringstream ss;
        ss << getDayName(nextDay) << " " 
           << std::setfill('0') << std::setw(2) << nextEntry.hour << ":" 
           << std::setw(2) << nextEntry.minute << " (" << nextEntry.temperature << "°C)";
        return ss.str();
    }
    
    return "No scheduled changes";
}

bool Schedule::isScheduleEnabled() const {
    return isEnabled;
}

void Schedule::enableSchedule(bool enabled) {
    isEnabled = enabled;
    std::cout << "Schedule " << (enabled ? "enabled" : "disabled") << std::endl;
}

void Schedule::setDefaultTemperature(double temp) {
    if (isValidTemperature(temp)) {
        defaultTemperature = temp;
        std::cout << "Default temperature set to " << temp << "°C" << std::endl;
    } else {
        std::cout << "Invalid default temperature: " << temp << "°C" << std::endl;
    }
}

double Schedule::getDefaultTemperature() const {
    return defaultTemperature;
}

std::vector<ScheduleEntry> Schedule::getDaySchedule(int dayOfWeek) const {
    if (dayOfWeek < 0 || dayOfWeek >= 7) {
        return {};
    }
    return weeklySchedule[dayOfWeek].entries;
}

std::string Schedule::getDayName(int dayOfWeek) const {
    if (dayOfWeek < 0 || dayOfWeek >= 7) {
        return "Unknown";
    }
    return weeklySchedule[dayOfWeek].dayName;
}

int Schedule::getDayOfWeek() const {
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    std::tm* tm = std::localtime(&time_t);
    return tm->tm_wday; // 0 = Sunday, 6 = Saturday
}

bool Schedule::isValidTemperature(double temp) const {
    return temp >= MIN_TEMP && temp <= MAX_TEMP;
}

bool Schedule::isValidTime(int hour, int minute) const {
    return hour >= 0 && hour <= 23 && minute >= 0 && minute <= 59;
}

std::string Schedule::validateSchedule() const {
    std::stringstream ss;
    bool hasErrors = false;
    
    for (int day = 0; day < 7; ++day) {
        const auto& entries = weeklySchedule[day].entries;
        for (size_t i = 0; i < entries.size(); ++i) {
            const auto& entry = entries[i];
            
            if (!isValidTemperature(entry.temperature)) {
                ss << "Invalid temperature " << entry.temperature << "°C on " 
                   << getDayName(day) << " at " << entry.hour << ":" << entry.minute << std::endl;
                hasErrors = true;
            }
            
            if (!isValidTime(entry.hour, entry.minute)) {
                ss << "Invalid time " << entry.hour << ":" << entry.minute << " on " 
                   << getDayName(day) << std::endl;
                hasErrors = true;
            }
            
            // Check for duplicate times
            for (size_t j = i + 1; j < entries.size(); ++j) {
                const auto& other = entries[j];
                if (entry.hour == other.hour && entry.minute == other.minute) {
                    ss << "Duplicate time " << entry.hour << ":" << entry.minute << " on " 
                       << getDayName(day) << std::endl;
                    hasErrors = true;
                }
            }
        }
    }
    
    if (!hasErrors) {
        ss << "Schedule validation passed";
    }
    
    return ss.str();
}

void Schedule::initializeDefaultSchedule() {
    // Clear existing schedule
    clearAll();
    
    // Create a typical weekly schedule
    // Weekdays (Monday-Friday)
    for (int day = 1; day <= 5; ++day) {
        addEntry(day, 6, 0, 20.0, "Wake up");
        addEntry(day, 8, 0, 18.0, "Leave for work");
        addEntry(day, 17, 0, 20.0, "Return home");
        addEntry(day, 22, 0, 18.0, "Bedtime");
    }
    
    // Weekend (Saturday-Sunday)
    addEntry(6, 7, 0, 21.0, "Weekend wake up"); // Saturday
    addEntry(6, 23, 0, 19.0, "Weekend bedtime");
    
    addEntry(0, 8, 0, 21.0, "Sunday wake up"); // Sunday
    addEntry(0, 22, 0, 19.0, "Sunday bedtime");
    
    std::cout << "Default schedule initialized" << std::endl;
}

int Schedule::findNextEntry(int dayOfWeek, int currentHour, int currentMinute) const {
    if (dayOfWeek < 0 || dayOfWeek >= 7) {
        return -1;
    }
    
    const auto& entries = weeklySchedule[dayOfWeek].entries;
    int currentTimeMinutes = currentHour * 60 + currentMinute;
    
    for (size_t i = 0; i < entries.size(); ++i) {
        int entryTimeMinutes = entries[i].hour * 60 + entries[i].minute;
        if (entryTimeMinutes > currentTimeMinutes) {
            return static_cast<int>(i);
        }
    }
    
    return -1; // No more entries today
}

std::chrono::system_clock::time_point Schedule::getNextScheduleTime() const {
    // This would calculate the next schedule time
    // For simplicity, return current time + 1 hour
    return std::chrono::system_clock::now() + std::chrono::hours(1);
} 