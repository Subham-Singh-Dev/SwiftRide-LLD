#pragma once
#include <string>
#include "../enums/VehicleType.h"

class Vehicle {
public:
    Vehicle(std::string id, std::string licensePlate,
            VehicleType type, int capacity, double baseFareRate)
        : id_(std::move(id))
        , licensePlate_(std::move(licensePlate))
        , type_(type)
        , capacity_(capacity)
        , baseFareRate_(baseFareRate) {}

    // Getters — data is set once at construction, never mutated
    const std::string& getId()           const { return id_; }
    const std::string& getLicensePlate() const { return licensePlate_; }
    VehicleType        getType()         const { return type_; }
    int                getCapacity()     const { return capacity_; }
    double             getBaseFareRate() const { return baseFareRate_; }

private:
    std::string id_;
    std::string licensePlate_;
    VehicleType type_;
    int         capacity_;
    double      baseFareRate_; // Base fare per km (₹)
};