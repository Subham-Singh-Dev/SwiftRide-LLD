#pragma once
#include "MatchingStrategy.h"
#include <limits>

class NearestDriverStrategy : public MatchingStrategy {
public:
    std::shared_ptr<Driver> findBestDriver(
        const std::vector<std::shared_ptr<Driver>>& drivers,
        const Location& pickup) override
    {
        std::shared_ptr<Driver> best = nullptr;
        double minDistance = std::numeric_limits<double>::max();

        for (const auto& driver : drivers) {
            if (!driver->isAvailable()) continue;

            double dist = driver->getLocation().distanceTo(pickup);
            if (dist < minDistance) {
                minDistance = dist;
                best = driver;
            }
        }
        return best; // nullptr if no driver found
    }
};