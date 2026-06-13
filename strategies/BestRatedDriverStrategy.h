#pragma once
#include "MatchingStrategy.h"

class BestRatedDriverStrategy : public MatchingStrategy {
public:
    std::shared_ptr<Driver> findBestDriver(
        const std::vector<std::shared_ptr<Driver>>& drivers,
        const Location& pickup) override
    {
        std::shared_ptr<Driver> best = nullptr;
        double highestRating = -1.0;

        for (const auto& driver : drivers) {
            if (!driver->isAvailable()) continue;

            if (driver->getRating() > highestRating) {
                highestRating = driver->getRating();
                best = driver;
            }
        }
        return best;
    }
};