#pragma once
#include <vector>
#include <memory>
#include "../models/Driver.h"
#include "../models/Location.h"

// The interface every matching algorithm must follow.
// DispatchService depends on THIS, not on any concrete strategy.
class MatchingStrategy {
public:
    virtual ~MatchingStrategy() = default;

    virtual std::shared_ptr<Driver> findBestDriver(
        const std::vector<std::shared_ptr<Driver>>& availableDrivers,
        const Location& pickupLocation) = 0;
};