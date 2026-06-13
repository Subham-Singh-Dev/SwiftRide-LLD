#pragma once
#include <string>
#include <memory>
#include <vector>
#include "Rider.h"
#include "Driver.h"
#include "Location.h"
#include "../enums/RideStatus.h"
#include "../observers/RideObserver.h"  // now we include it for real

class Ride {
public:
    Ride(std::string id,
         std::shared_ptr<Rider>  rider,
         std::shared_ptr<Driver> driver,
         Location pickup,
         Location drop)
        : id_(std::move(id))
        , rider_(std::move(rider))
        , driver_(std::move(driver))
        , pickup_(std::move(pickup))
        , drop_(std::move(drop))
        , status_(RideStatus::REQUESTED)
        , fare_(0.0) {}

    const std::string&           getId()     const { return id_; }
    std::shared_ptr<Rider>       getRider()  const { return rider_; }
    std::shared_ptr<Driver>      getDriver() const { return driver_; }
    const Location&              getPickup() const { return pickup_; }
    const Location&              getDrop()   const { return drop_; }
    RideStatus                   getStatus() const { return status_; }
    double                       getFare()   const { return fare_; }

    void setStatus(RideStatus status) { status_ = status; }
    void setFare(double fare)         { fare_ = fare; }

    void addObserver(std::shared_ptr<RideObserver> observer) {
        observers_.push_back(std::move(observer));
    }

    // Now fully implemented — loops through all observers and tells them
    void notifyObservers() {
        for (auto& observer : observers_) {
            observer->onStatusChange(id_, status_);
        }
    }

private:
    std::string             id_;
    std::shared_ptr<Rider>  rider_;
    std::shared_ptr<Driver> driver_;
    Location                pickup_;
    Location                drop_;
    RideStatus              status_;
    double                  fare_;

    std::vector<std::shared_ptr<RideObserver>> observers_;
};