#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include <stdexcept>
#include <iostream>
#include "../models/Rider.h"
#include "../models/Driver.h"

class UserManager {
public:

    // --- Singleton ---
    static UserManager& getInstance() {
        static UserManager instance;
        return instance;
    }

    UserManager(const UserManager&)            = delete;
    UserManager& operator=(const UserManager&) = delete;

    // --- Register users ---
    void registerRider(std::shared_ptr<Rider> rider) {
        riders_[rider->getId()] = rider;
        std::cout << "[UserManager] Rider registered: "
                  << rider->getName() << "\n";
    }

    void registerDriver(std::shared_ptr<Driver> driver) {
        drivers_[driver->getId()] = driver;
        std::cout << "[UserManager] Driver registered: "
                  << driver->getName() << "\n";
    }

    // --- Lookup ---
    std::shared_ptr<Rider> getRider(const std::string& id) {
        auto it = riders_.find(id);
        if (it == riders_.end())
            throw std::runtime_error("Rider not found: " + id);
        return it->second;
    }

    std::shared_ptr<Driver> getDriver(const std::string& id) {
        auto it = drivers_.find(id);
        if (it == drivers_.end())
            throw std::runtime_error("Driver not found: " + id);
        return it->second;
    }

    // --- Get all drivers (used by DispatchService) ---
    std::vector<std::shared_ptr<Driver>> getAllDrivers() {
        std::vector<std::shared_ptr<Driver>> result;
        for (auto& pair : drivers_)
            result.push_back(pair.second);
        return result;
    }

    // --- Get all riders ---
    std::vector<std::shared_ptr<Rider>> getAllRiders() {
        std::vector<std::shared_ptr<Rider>> result;
        for (auto& pair : riders_)
            result.push_back(pair.second);
        return result;
    }

private:
    UserManager() = default;

    std::unordered_map<std::string, std::shared_ptr<Rider>>  riders_;
    std::unordered_map<std::string, std::shared_ptr<Driver>> drivers_;
};