#pragma once
#include <memory>
#include <iostream>
#include "RideObserver.h"
#include "../models/Driver.h"

class DriverNotificationObserver : public RideObserver {
public:
    explicit DriverNotificationObserver(std::shared_ptr<Driver> driver)
        : driver_(std::move(driver)) {}

    void onStatusChange(
        const std::string& rideId,
        RideStatus newStatus) override
    {
        std::cout << "[Driver Notification] Hey " << driver_->getName()
                  << "! Ride " << rideId
                  << " status updated to " << rideStatusToString(newStatus) << ".\n";
    }

private:
    std::shared_ptr<Driver> driver_;
};