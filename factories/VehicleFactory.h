#pragma once
#include <memory>
#include <string>
#include "../models/Vehicle.h"

class VehicleFactory {
public:
    static std::shared_ptr<Vehicle> createVehicle(
        const std::string& id,
        const std::string& licensePlate,
        VehicleType type,
        int capacity)
    {
        // Base fare rate is determined by vehicle type
        double fareRate = getFareRate(type);
        return std::make_shared<Vehicle>(id, licensePlate, type, capacity, fareRate);
    }

private:
    // One place to control pricing per vehicle type
    static double getFareRate(VehicleType type) {
        switch (type) {
            case VehicleType::BIKE:  return 6.0;
            case VehicleType::AUTO:  return 8.0;
            case VehicleType::SEDAN: return 12.0;
            case VehicleType::SUV:   return 18.0;
        }
        return 10.0;
    }
};