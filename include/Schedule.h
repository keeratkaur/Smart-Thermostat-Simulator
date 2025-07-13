#pragma once

#include <vector>
#include <string>
#include <chrono>

struct ScheduleEntry {
    int hour;
    int minute;
    double temperature;
    std::string description;
    
    ScheduleEntry(int h, int m, double temp, const std::string& desc = "")
        : hour(h), minute(m), temperature(temp), description(desc) {}
};

struct DaySchedule {
    std::string dayName;
    std::vector<ScheduleEntry> entries;
    
    DaySchedule(const std::string& name) : dayName(name) {}
};

class Schedule {
private:
    std::vector<DaySchedule> weeklySchedule;
    bool isEnabled;
    double defaultTemperature;
    std::chrono::system_clock::time_point lastScheduleCheck;
    
    // Schedule validation
    const double MIN_TEMP = 10.0;
    const double MAX_TEMP = 30.0;

public:
    Schedule();
    ~Schedule() = default;

    // Core functionality
    void initialize();
    void loadDefaultSchedule();
    void saveSchedule();
    
    // Schedule management
    void addEntry(int dayOfWeek, int hour, int minute, double temperature, const std::string& description = "");
    void removeEntry(int dayOfWeek, int entryIndex);
    void clearDay(int dayOfWeek);
    void clearAll();
    
    // Schedule queries
    double getCurrentTargetTemperature() const;
    double getTargetTemperatureAt(int dayOfWeek, int hour, int minute) const;
    std::string getNextScheduleChange() const;
    
    // Schedule status
    bool isScheduleEnabled() const;
    void enableSchedule(bool enabled);
    void setDefaultTemperature(double temp);
    double getDefaultTemperature() const;
    
    // Schedule information
    std::vector<ScheduleEntry> getDaySchedule(int dayOfWeek) const;
    std::string getDayName(int dayOfWeek) const;
    int getDayOfWeek() const;
    
    // Schedule validation
    bool isValidTemperature(double temp) const;
    bool isValidTime(int hour, int minute) const;
    std::string validateSchedule() const;

private:
    void initializeDefaultSchedule();
    int findNextEntry(int dayOfWeek, int currentHour, int currentMinute) const;
    std::chrono::system_clock::time_point getNextScheduleTime() const;
}; 