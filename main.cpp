#include "enums/VehicleType.h"
#include "enums/RideStatus.h"
#include "models/Location.h"
#include "models/Vehicle.h"
#include "models/Rider.h"
#include "models/Driver.h"
#include "models/Ride.h"
#include "managers/RideManager.h"
#include "managers/DispatchService.h"
#include "strategies/NearestDriverStrategy.h"
#include "observers/RiderNotificationObserver.h"
#include "observers/DriverNotificationObserver.h"
#include "observers/LoggingObserver.h"
#include "fare/BaseFareCalculator.h"
#include "fare/SurgePricingDecorator.h"
#include "fare/DiscountDecorator.h"
#include <iostream>

int main() {
    // --- Setup ---
    auto v1 = std::make_shared<Vehicle>("V1", "KA-01-1111", VehicleType::SEDAN, 4, 12.0);
    auto d1 = std::make_shared<Driver>("D1", "Ramesh", "8001", v1, Location(12.97, 77.59, "Brigade Road"));

    DispatchService& dispatch = DispatchService::getInstance();
    dispatch.registerDriver(d1);

    auto rider  = std::make_shared<Rider>("R1", "Arjun", "9999900000");
    Location pickup(12.98, 77.60, "MG Road");
    Location drop(12.93, 77.68, "HSR Layout");

    auto driver = dispatch.findDriver(pickup);

    RideManager& rm   = RideManager::getInstance();
    auto ride         = rm.createRide(rider, driver, pickup, drop);

    ride->addObserver(std::make_shared<RiderNotificationObserver>(rider));
    ride->addObserver(std::make_shared<LoggingObserver>());

    rm.updateStatus(ride->getId(), RideStatus::ACCEPTED);
    rm.updateStatus(ride->getId(), RideStatus::IN_PROGRESS);
    rm.updateStatus(ride->getId(), RideStatus::COMPLETED);

    // --- Scenario 1: Base fare only ---
    std::cout << "\n=== Scenario 1: Base Fare Only ===\n";
    auto calc1 = std::make_shared<BaseFareCalculator>();
    double fare1 = calc1->calculate(*ride);
    std::cout << "Final fare: ₹" << fare1 << "\n";

    // --- Scenario 2: Base + Surge ---
    std::cout << "\n=== Scenario 2: Base + Surge (x1.5) ===\n";
    auto calc2 = std::make_shared<SurgePricingDecorator>(
                    std::make_shared<BaseFareCalculator>(), 1.5);
    double fare2 = calc2->calculate(*ride);
    std::cout << "Final fare: ₹" << fare2 << "\n";

    // --- Scenario 3: Base + Surge + Discount ---
    std::cout << "\n=== Scenario 3: Base + Surge (x1.5) + Discount (₹5) ===\n";
    auto surgePricing = std::make_shared<SurgePricingDecorator>(
                            std::make_shared<BaseFareCalculator>(), 1.5);

    auto calc3 = std::make_shared<DiscountDecorator>(
                    std::static_pointer_cast<FareCalculator>(surgePricing),
                    5.0);
    double fare3 = calc3->calculate(*ride);
    std::cout << "Final fare: ₹" << fare3 << "\n";

    // Save final fare to the ride
    ride->setFare(fare3);
    std::cout << "\nFare saved to ride: ₹" << ride->getFare() << "\n";

    return 0;
}