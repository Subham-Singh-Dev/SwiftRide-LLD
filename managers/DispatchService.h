#pragma once
#include <vector>
#include <memory>
#include <stdexcept>
#include <iostream>
#include "../models/Driver.h"
#include "../models/Location.h"
#include "../strategies/MatchingStrategy.h"
#include "../strategies/NearestDriverStrategy.h"

class DispatchService {
public:

    // --- Singleton ---
    static DispatchService& getInstance() {
        static DispatchService instance;
        return instance;
    }

    DispatchService(const DispatchService&)            = delete;
    DispatchService& operator=(const DispatchService&) = delete;

    // --- Driver registry ---
    void registerDriver(std::shared_ptr<Driver> driver) {
        drivers_.push_back(driver);
        std::cout << "[DispatchService] Driver registered: "
                  << driver->getName() << "\n";
    }

    // --- Swap strategy at any time ---
    void setStrategy(std::shared_ptr<MatchingStrategy> strategy) {
        strategy_ = std::move(strategy);
        std::cout << "[DispatchService] Matching strategy updated.\n";
    }

    // --- Find the best driver for a pickup ---
    std::shared_ptr<Driver> findDriver(const Location& pickup) {
        if (!strategy_) {
            throw std::runtime_error("No matching strategy set.");
        }

        auto driver = strategy_->findBestDriver(drivers_, pickup);

        if (!driver) {
            throw std::runtime_error("No available drivers found.");
        }

        std::cout << "[DispatchService] Matched driver: "
                  << driver->getName()
                  << " (rating: " << driver->getRating() << ")\n";
        return driver;
    }

private:
    DispatchService()
        : strategy_(std::make_shared<NearestDriverStrategy>()) {}
        // Default strategy is Nearest — sensible out of the box

    std::vector<std::shared_ptr<Driver>>  drivers_;
    std::shared_ptr<MatchingStrategy>     strategy_;
};