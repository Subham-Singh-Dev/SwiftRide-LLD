#pragma once
#include <string>
#include <vector>
#include "User.h"

class Rider : public User {
public:
    Rider(std::string id, std::string name, std::string phone)
        : User(std::move(id), std::move(name), std::move(phone)) {}

    std::string getRole() const override { return "Rider"; }

    void addRideToHistory(const std::string& rideId) {
        rideHistory_.push_back(rideId);
    }

    const std::vector<std::string>& getRideHistory() const {
        return rideHistory_;
    }

private:
    // Stores ride IDs — actual Ride objects live in RideManager (Phase 3)
    std::vector<std::string> rideHistory_;
};