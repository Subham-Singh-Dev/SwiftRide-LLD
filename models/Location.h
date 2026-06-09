#pragma once
#include <cmath>
#include <string>

struct Location {
    double latitude;
    double longitude;
    std::string label; // Optional human-readable name, e.g. "MG Road"

    Location(double lat, double lng, std::string lbl = "")
        : latitude(lat), longitude(lng), label(std::move(lbl)) {}

    // Euclidean distance — good enough for our in-memory simulation.
    // Real apps use Haversine (great-circle distance) for accuracy.
    double distanceTo(const Location& other) const {
        double dx = latitude - other.latitude;
        double dy = longitude - other.longitude;
        return std::sqrt(dx * dx + dy * dy);
    }
};