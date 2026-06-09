#pragma once
#include <string>
#include <memory>
#include "User.h"
#include "Vehicle.h"
#include "Location.h"

class Driver : public User {
public:
    Driver(std::string id, std::string name, std::string phone,
           std::shared_ptr<Vehicle> vehicle, Location initialLocation)
        : User(std::move(id), std::move(name), std::move(phone))
        , vehicle_(std::move(vehicle))
        , location_(std::move(initialLocation))
        , rating_(5.0)      // All drivers start with a perfect rating
        , isAvailable_(true) {}

    std::string getRole() const override { return "Driver"; }

    // Vehicle
    std::shared_ptr<Vehicle> getVehicle() const { return vehicle_; }

    // Location — mutable, updates as driver moves
    const Location& getLocation() const         { return location_; }
    void setLocation(const Location& loc)       { location_ = loc; }

    // Availability
    bool isAvailable() const                    { return isAvailable_; }
    void setAvailable(bool available)           { isAvailable_ = available; }

    // Rating
    double getRating() const                    { return rating_; }
    void updateRating(double newRating)         { rating_ = newRating; }

private:
    std::shared_ptr<Vehicle> vehicle_;
    Location location_;
    double   rating_;
    bool     isAvailable_;
};