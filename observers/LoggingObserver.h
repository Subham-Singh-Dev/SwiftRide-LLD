#pragma once
#include <iostream>
#include "RideObserver.h"

class LoggingObserver : public RideObserver {
public:
    void onStatusChange(
        const std::string& rideId,
        RideStatus newStatus) override
    {
        std::cout << "[LOG] " << rideId
                  << " → " << rideStatusToString(newStatus) << "\n";
    }
};