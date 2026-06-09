#include "enums/VehicleType.h"
#include "enums/RideStatus.h"
#include "models/Location.h"
#include "models/Vehicle.h"
#include "models/Rider.h"
#include "models/Driver.h"
#include "models/Ride.h"
#include <iostream>

// Temporary stub — RideObserver will be built in Phase 5
class RideObserver {};
void Ride::notifyObservers() {}

int main() {
    // 1. Create a vehicle
    auto vehicle = std::make_shared<Vehicle>("V1", "KA-01-1234", VehicleType::SEDAN, 4, 12.0);

    // 2. Create a rider
    auto rider = std::make_shared<Rider>("R1", "Arjun", "9999900000");

    // 3. Create a driver at some location
    Location driverLoc(12.97, 77.59, "Brigade Road");
    auto driver = std::make_shared<Driver>("D1", "Ramesh", "8888800000", vehicle, driverLoc);

    // 4. Create a ride between two locations
    Location pickup(12.97, 77.59, "Brigade Road");
    Location drop(12.93, 77.68, "Koramangala");
    auto ride = std::make_shared<Ride>("RIDE-001", rider, driver, pickup, drop);

    // 5. Print everything to verify
    std::cout << "Rider:  " << rider->getName()  << "\n";
    std::cout << "Driver: " << driver->getName() << "\n";
    std::cout << "Status: " << rideStatusToString(ride->getStatus()) << "\n";

    return 0;
}