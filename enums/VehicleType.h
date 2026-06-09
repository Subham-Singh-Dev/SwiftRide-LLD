#pragma once
#include <string>

enum class VehicleType {
    BIKE,
    AUTO,
    SEDAN,
    SUV
};

// Helper: human-readable name for a VehicleType
inline std::string vehicleTypeToString(VehicleType type) {
    switch (type) {
        case VehicleType::BIKE:  return "Bike";
        case VehicleType::AUTO:  return "Auto";
        case VehicleType::SEDAN: return "Sedan";
        case VehicleType::SUV:   return "SUV";
    }
    return "Unknown";
}