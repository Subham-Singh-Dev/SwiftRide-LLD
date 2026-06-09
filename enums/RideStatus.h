#pragma once
#include <string>

enum class RideStatus {
    REQUESTED,   // Rider has booked, waiting for driver
    ACCEPTED,    // Driver accepted the ride
    IN_PROGRESS, // Ride is ongoing
    COMPLETED,   // Ride finished successfully
    CANCELLED    // Ride was cancelled
};

inline std::string rideStatusToString(RideStatus status) {
    switch (status) {
        case RideStatus::REQUESTED:   return "REQUESTED";
        case RideStatus::ACCEPTED:    return "ACCEPTED";
        case RideStatus::IN_PROGRESS: return "IN_PROGRESS";
        case RideStatus::COMPLETED:   return "COMPLETED";
        case RideStatus::CANCELLED:   return "CANCELLED";
    }
    return "UNKNOWN";
}