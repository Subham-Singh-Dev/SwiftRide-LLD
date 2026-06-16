#include "enums/VehicleType.h"
#include "enums/RideStatus.h"
#include "models/Location.h"
#include "models/Vehicle.h"
#include "models/Rider.h"
#include "models/Driver.h"
#include "models/Ride.h"
#include "managers/RideManager.h"
#include "managers/DispatchService.h"
#include "managers/UserManager.h"
#include "factories/UserFactory.h"
#include "factories/VehicleFactory.h"
#include "strategies/NearestDriverStrategy.h"
#include "strategies/BestRatedDriverStrategy.h"
#include "observers/RiderNotificationObserver.h"
#include "observers/DriverNotificationObserver.h"
#include "observers/LoggingObserver.h"
#include "fare/BaseFareCalculator.h"
#include "fare/SurgePricingDecorator.h"
#include "fare/DiscountDecorator.h"
#include <iostream>

int main() {
    UserManager&     um       = UserManager::getInstance();
    DispatchService& dispatch = DispatchService::getInstance();
    RideManager&     rm       = RideManager::getInstance();

    // --- Create and register vehicles ---
    auto v1 = VehicleFactory::createVehicle("V1", "KA-01-1111", VehicleType::SEDAN, 4);
    auto v2 = VehicleFactory::createVehicle("V2", "KA-01-2222", VehicleType::SUV,   6);
    auto v3 = VehicleFactory::createVehicle("V3", "KA-01-3333", VehicleType::AUTO,  3);

    // --- Create and register drivers ---
    auto d1 = UserFactory::createDriver("D1", "Ramesh", "8001", v1, Location(12.97, 77.59, "Brigade Road"));
    auto d2 = UserFactory::createDriver("D2", "Suresh", "8002", v2, Location(12.90, 77.65, "Koramangala"));
    auto d3 = UserFactory::createDriver("D3", "Mahesh", "8003", v3, Location(12.99, 77.61, "Indiranagar"));

    d1->updateRating(4.2);
    d2->updateRating(4.8);
    d3->updateRating(3.9);

    um.registerDriver(d1);
    um.registerDriver(d2);
    um.registerDriver(d3);

    dispatch.registerDriver(d1);
    dispatch.registerDriver(d2);
    dispatch.registerDriver(d3);

    // --- Create and register riders ---
    auto r1 = UserFactory::createRider("R1", "Arjun", "9001");
    auto r2 = UserFactory::createRider("R2", "Priya", "9002");

    um.registerRider(r1);
    um.registerRider(r2);

    // --- Ride 1: Arjun books with Nearest strategy ---
    std::cout << "\n=== Ride 1: Nearest Driver ===\n";
    dispatch.setStrategy(std::make_shared<NearestDriverStrategy>());

    Location pickup1(12.98, 77.60, "MG Road");
    Location drop1(12.93, 77.68, "HSR Layout");

    auto driver1 = dispatch.findDriver(pickup1);
    auto ride1   = rm.createRide(r1, driver1, pickup1, drop1);

    ride1->addObserver(std::make_shared<RiderNotificationObserver>(r1));
    ride1->addObserver(std::make_shared<DriverNotificationObserver>(driver1));
    ride1->addObserver(std::make_shared<LoggingObserver>());

    rm.updateStatus(ride1->getId(), RideStatus::ACCEPTED);
    rm.updateStatus(ride1->getId(), RideStatus::IN_PROGRESS);
    rm.updateStatus(ride1->getId(), RideStatus::COMPLETED);

    // Calculate and save fare (base + surge)
    auto calc1 = std::make_shared<SurgePricingDecorator>(
                    std::make_shared<BaseFareCalculator>(), 1.5);
    ride1->setFare(calc1->calculate(*ride1));

    // --- Ride 2: Priya books with Best Rated strategy ---
    std::cout << "\n=== Ride 2: Best Rated Driver ===\n";
    dispatch.setStrategy(std::make_shared<BestRatedDriverStrategy>());

    Location pickup2(12.95, 77.62, "Jayanagar");
    Location drop2(12.88, 77.70, "Electronic City");

    auto driver2 = dispatch.findDriver(pickup2);
    auto ride2   = rm.createRide(r2, driver2, pickup2, drop2);

    ride2->addObserver(std::make_shared<RiderNotificationObserver>(r2));
    ride2->addObserver(std::make_shared<LoggingObserver>());

    rm.updateStatus(ride2->getId(), RideStatus::ACCEPTED);
    rm.updateStatus(ride2->getId(), RideStatus::IN_PROGRESS);
    rm.updateStatus(ride2->getId(), RideStatus::COMPLETED);

    // Calculate fare (base + discount)
    auto calc2 = std::make_shared<DiscountDecorator>(
                    std::make_shared<BaseFareCalculator>(), 2.0);
    ride2->setFare(calc2->calculate(*ride2));

    // --- Summary ---
    std::cout << "\n=== Ride Summary ===\n";
    for (auto& rider : um.getAllRiders()) {
        std::cout << "\nRider: " << rider->getName() << "\n";
        for (auto& ride : rm.getRidesForRider(rider->getId())) {
            std::cout << "  " << ride->getId()
                      << " | " << rideStatusToString(ride->getStatus())
                      << " | Driver: " << ride->getDriver()->getName()
                      << " | Fare: Rs." << ride->getFare() << "\n";
        }
    }

    return 0;
}