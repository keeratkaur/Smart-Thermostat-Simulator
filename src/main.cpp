#include <iostream>
#include <thread>
#include <chrono>
#include <signal.h>
#include "Thermostat.h"

bool running = true;

void signalHandler(int signum) {
    std::cout << "\nReceived signal " << signum << ". Shutting down gracefully..." << std::endl;
    running = false;
}

void printBanner() {
    std::cout << "===============================================" << std::endl;
    std::cout << "    C++ Smart Thermostat Simulator v1.0.0" << std::endl;
    std::cout << "===============================================" << std::endl;
    std::cout << "Features:" << std::endl;
    std::cout << "  • Temperature and humidity monitoring" << std::endl;
    std::cout << "  • Smart HVAC control (Heat/Cool/Auto)" << std::endl;
    std::cout << "  • Programmable schedules" << std::endl;
    std::cout << "  • Weather integration simulation" << std::endl;
    std::cout << "  • Energy consumption tracking" << std::endl;
    std::cout << "  • User-friendly interface" << std::endl;
    std::cout << "===============================================" << std::endl;
    std::cout << "Press Ctrl+C to exit" << std::endl;
    std::cout << "===============================================" << std::endl;
}

void printUsage() {
    std::cout << "Usage: SmartThermostatSimulator [options]" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  --help, -h          Show this help message" << std::endl;
    std::cout << "  --verbose, -v       Enable verbose output" << std::endl;
    std::cout << "  --demo, -d          Run in demo mode with preset values" << std::endl;
    std::cout << "  --config <file>     Load configuration from file" << std::endl;
}

int main(int argc, char* argv[]) {
    // Set up signal handling
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);

    // Parse command line arguments
    bool verbose = false;
    bool demoMode = false;
    std::string configFile = "";

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--help" || arg == "-h") {
            printUsage();
            return 0;
        } else if (arg == "--verbose" || arg == "-v") {
            verbose = true;
        } else if (arg == "--demo" || arg == "-d") {
            demoMode = true;
        } else if (arg == "--config" && i + 1 < argc) {
            configFile = argv[++i];
        } else {
            std::cerr << "Unknown option: " << arg << std::endl;
            printUsage();
            return 1;
        }
    }

    // Print banner
    printBanner();

    try {
        // Create and initialize thermostat
        Thermostat thermostat;
        
        if (verbose) {
            std::cout << "Initializing thermostat..." << std::endl;
        }
        
        thermostat.initialize();
        
        if (demoMode) {
            std::cout << "Running in demo mode with preset values..." << std::endl;
            // Set demo values
            thermostat.setTargetTemperature(22.0);
            thermostat.setMode(ThermostatMode::AUTO);
        }
        
        if (verbose) {
            std::cout << "Thermostat initialized successfully!" << std::endl;
            std::cout << "Starting main loop..." << std::endl;
        }

        // Main simulation loop
        auto startTime = std::chrono::steady_clock::now();
        int updateCount = 0;
        
        while (running) {
            auto loopStart = std::chrono::steady_clock::now();
            
            // Update thermostat
            thermostat.update();
            
            // Print status every 10 updates (approximately every 10 seconds)
            if (++updateCount % 10 == 0) {
                std::cout << "\n=== System Status ===" << std::endl;
                std::cout << thermostat.getDetailedStatus() << std::endl;
                std::cout << "=====================" << std::endl;
            }
            
            // Calculate loop timing
            auto loopEnd = std::chrono::steady_clock::now();
            auto loopDuration = std::chrono::duration_cast<std::chrono::milliseconds>(loopEnd - loopStart);
            
            // Sleep to maintain 1-second update interval
            if (loopDuration.count() < 1000) {
                std::this_thread::sleep_for(std::chrono::milliseconds(1000 - loopDuration.count()));
            }
        }

        // Shutdown
        if (verbose) {
            std::cout << "Shutting down thermostat..." << std::endl;
        }
        
        thermostat.shutdown();
        
        auto endTime = std::chrono::steady_clock::now();
        auto totalDuration = std::chrono::duration_cast<std::chrono::seconds>(endTime - startTime);
        
        std::cout << "\nSimulation completed successfully!" << std::endl;
        std::cout << "Total runtime: " << totalDuration.count() << " seconds" << std::endl;
        std::cout << "Total updates: " << updateCount << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Unknown error occurred!" << std::endl;
        return 1;
    }

    return 0;
} 