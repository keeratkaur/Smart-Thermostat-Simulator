#!/usr/bin/env python3
"""
C++ Smart Thermostat Simulator Project Validator
This script validates the project structure and checks for basic syntax issues.
"""

import os
import re
import sys
from pathlib import Path

def check_file_exists(filepath):
    """Check if a file exists and return its size."""
    if os.path.exists(filepath):
        size = os.path.getsize(filepath)
        return True, size
    return False, 0

def validate_cpp_syntax(content, filename):
    """Basic C++ syntax validation."""
    issues = []
    
    # Check for basic C++ patterns
    if not re.search(r'#include\s+["<][^>"]*[">]', content):
        issues.append("No #include statements found")
    
    # Check for class definitions
    if not re.search(r'class\s+\w+', content):
        issues.append("No class definitions found")
    
    # Check for function definitions
    if not re.search(r'\w+\s+\w+\s*\([^)]*\)\s*{', content):
        issues.append("No function definitions found")
    
    # Check for semicolons (basic syntax)
    if content.count(';') < 5:
        issues.append("Very few semicolons - possible syntax issues")
    
    return issues

def main():
    print("===============================================")
    print("    C++ Smart Thermostat Simulator Validator")
    print("===============================================")
    
    # Project structure
    required_files = [
        "CMakeLists.txt",
        "README.md",
        "build.sh",
        "build.bat",
        "include/Thermostat.h",
        "include/TemperatureSensor.h",
        "include/HVACSystem.h",
        "include/Schedule.h",
        "include/WeatherSimulator.h",
        "include/EnergyMonitor.h",
        "include/UserInterface.h",
        "src/main.cpp",
        "src/Thermostat.cpp",
        "src/TemperatureSensor.cpp",
        "src/HVACSystem.cpp",
        "src/Schedule.cpp",
        "src/WeatherSimulator.cpp",
        "src/EnergyMonitor.cpp",
        "src/UserInterface.cpp"
    ]
    
    print("\nChecking project structure...")
    all_files_exist = True
    
    for filepath in required_files:
        exists, size = check_file_exists(filepath)
        status = "✓" if exists else "✗"
        size_str = f"({size} bytes)" if exists else "(missing)"
        print(f"  {status} {filepath} {size_str}")
        
        if not exists:
            all_files_exist = False
    
    if all_files_exist:
        print("\n✓ All required files present!")
    else:
        print("\n✗ Some required files are missing!")
        return False
    
    # Check C++ syntax in source files
    print("\nValidating C++ syntax...")
    source_files = [f for f in required_files if f.startswith("src/") or f.startswith("include/")]
    
    syntax_issues = []
    for filepath in source_files:
        try:
            with open(filepath, 'r', encoding='utf-8') as f:
                content = f.read()
            
            issues = validate_cpp_syntax(content, filepath)
            if issues:
                syntax_issues.append((filepath, issues))
                print(f"  ⚠ {filepath}: {', '.join(issues)}")
            else:
                print(f"  ✓ {filepath}: Syntax looks good")
                
        except Exception as e:
            print(f"  ✗ {filepath}: Error reading file - {e}")
            syntax_issues.append((filepath, [f"File read error: {e}"]))
    
    # Check for specific C++ features
    print("\nChecking for C++17 features...")
    cpp17_features = {
        "std::optional": "C++17 optional type",
        "std::variant": "C++17 variant type",
        "std::string_view": "C++17 string_view",
        "auto": "C++11 auto keyword",
        "std::unique_ptr": "Smart pointers",
        "std::chrono": "Time utilities",
        "enum class": "Scoped enumerations"
    }
    
    feature_found = {}
    for feature, description in cpp17_features.items():
        feature_found[feature] = False
    
    for filepath in source_files:
        try:
            with open(filepath, 'r', encoding='utf-8') as f:
                content = f.read()
            
            for feature in cpp17_features:
                if feature in content:
                    feature_found[feature] = True
                    
        except Exception:
            continue
    
    for feature, description in cpp17_features.items():
        status = "✓" if feature_found[feature] else "✗"
        print(f"  {status} {description}")
    
    # Summary
    print("\n===============================================")
    print("Validation Summary")
    print("===============================================")
    
    if all_files_exist and not syntax_issues:
        print("✓ Project structure is valid")
        print("✓ All required files are present")
        print("✓ C++ syntax appears correct")
        print("\nThe C++ Smart Thermostat Simulator project is ready to build!")
        print("\nTo build the project:")
        print("  - Install CMake 3.16+ and a C++17 compiler")
        print("  - Run: ./build.sh (Linux/macOS) or build.bat (Windows)")
        print("  - Or manually: mkdir build && cd build && cmake .. && make")
        return True
    else:
        print("✗ Project has issues that need to be resolved")
        if syntax_issues:
            print("  - C++ syntax issues found")
        if not all_files_exist:
            print("  - Missing required files")
        return False

if __name__ == "__main__":
    success = main()
    sys.exit(0 if success else 1) 