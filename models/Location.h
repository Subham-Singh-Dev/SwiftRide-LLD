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
        // 1 degree latitude ≈ 111 km
        // 1 degree longitude ≈ 111 km * cos(latitude)
        const double KM_PER_DEG = 111.0;
        double dx = (latitude  - other.latitude)  * KM_PER_DEG;
        double dy = (longitude - other.longitude) * KM_PER_DEG;
        return std::sqrt(dx * dx + dy * dy); // result now in km
}
};