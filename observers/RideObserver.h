#pragma once
#include <string>
#include "../enums/RideStatus.h"

// The interface every observer must implement.
// Any class that wants to "listen" to ride changes implements this.
class RideObserver {
public:
    virtual ~RideObserver() = default;

    virtual void onStatusChange(
        const std::string& rideId,
        RideStatus newStatus) = 0;
};