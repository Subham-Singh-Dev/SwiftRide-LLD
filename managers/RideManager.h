#pragma once 
#include <string>
#include <memory>
#include <unordered_map>
#include <vector>
#include <stdexcept>
#include <iostream>
#include "../models/Ride.h"
#include "../models/Rider.h"
#include "../models/Driver.h"
#include "../models/Location.h"
#include "../enums/RideStatus.h"

class RideManager {
public:

    // --- Singleton: get the one global instance ---
    static RideManager& getInstance() {
        static RideManager instance; // created once, lives forever
        return instance;
    }

    // Delete copy and assignment — no one should be able to
    // duplicate or reassign the singleton
    RideManager(const RideManager&)            = delete;
    RideManager& operator=(const RideManager&) = delete;

    // --- Create a new ride ---
    std::shared_ptr<Ride> createRide(
        std::shared_ptr<Rider>  rider,
        std::shared_ptr<Driver> driver,
        Location pickup,
        Location drop)
    {
        std::string rideId = "RIDE-" + std::to_string(nextId_++);

        auto ride = std::make_shared<Ride>(rideId, rider, driver, pickup, drop);

        // Store it
        allRides_[rideId] = ride;

        // Also track it under this rider's history
        ridesByRider_[rider->getId()].push_back(rideId);

        // Mark driver as unavailable
        driver->setAvailable(false);

        std::cout << "[RideManager] Ride created: " << rideId
                  << " | " << rider->getName()
                  << " → " << driver->getName() << "\n";

        return ride;
    }

    // --- Update ride status with validation ---
    void updateStatus(const std::string& rideId, RideStatus newStatus) {
        auto ride = getRideById(rideId);

        // Enforce valid transitions
        validateTransition(ride->getStatus(), newStatus);

        ride->setStatus(newStatus);

        // If ride is done, free up the driver
        if (newStatus == RideStatus::COMPLETED ||
            newStatus == RideStatus::CANCELLED) {
            ride->getDriver()->setAvailable(true);
        }

        std::cout << "[RideManager] " << rideId
                  << " status → " << rideStatusToString(newStatus) << "\n";
        ride->notifyObservers(); // tell all observers about the change
    }

    // --- Lookup ---
    std::shared_ptr<Ride> getRideById(const std::string& rideId) {
        auto it = allRides_.find(rideId);
        if (it == allRides_.end()) {
            throw std::runtime_error("Ride not found: " + rideId);
        }
        return it->second;
    }

    std::vector<std::shared_ptr<Ride>> getRidesForRider(const std::string& riderId) {
        std::vector<std::shared_ptr<Ride>> result;
        auto it = ridesByRider_.find(riderId);
        if (it == ridesByRider_.end()) return result;

        for (const auto& id : it->second) {
            result.push_back(allRides_[id]);
        }
        return result;
    }

private:
    RideManager() = default; // private: no one can call new RideManager()

    // Validate that a status transition makes sense
    void validateTransition(RideStatus current, RideStatus next) {
        // Allowed flow:
        // REQUESTED → ACCEPTED → IN_PROGRESS → COMPLETED
        //           ↘ CANCELLED (from REQUESTED or ACCEPTED only)
        using RS = RideStatus;

        if (current == RS::COMPLETED || current == RS::CANCELLED) {
            throw std::runtime_error("Cannot change status of a finished ride.");
        }
        if (current == RS::REQUESTED  && next == RS::IN_PROGRESS) {
            throw std::runtime_error("Cannot skip ACCEPTED status.");
        }
        if (current == RS::IN_PROGRESS && next == RS::ACCEPTED) {
            throw std::runtime_error("Cannot go backwards in status.");
        }
    }

    int nextId_ = 1;
    std::unordered_map<std::string, std::shared_ptr<Ride>> allRides_;
    std::unordered_map<std::string, std::vector<std::string>> ridesByRider_;
};