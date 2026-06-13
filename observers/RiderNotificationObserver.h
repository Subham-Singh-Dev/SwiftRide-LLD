#pragma once
#include <memory>
#include <iostream>
#include "RideObserver.h"
#include "../models/Rider.h"

class RiderNotificationObserver : public RideObserver {
public:
    explicit RiderNotificationObserver(std::shared_ptr<Rider> rider)
        : rider_(std::move(rider)) {}

    void onStatusChange(
        const std::string& rideId,
        RideStatus newStatus) override
    {
        std::cout << "[Rider Notification] Hey " << rider_->getName()
                  << "! Your ride " << rideId
                  << " is now " << rideStatusToString(newStatus) << ".\n";
    }

private:
    std::shared_ptr<Rider> rider_;
};