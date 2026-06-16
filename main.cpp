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
#include <string>

// --- Helper: print a section divider ---
void printSection(const std::string& title) {
    std::cout << "\n";
    std::cout << "========================================\n";
    std::cout << "  " << title << "\n";
    std::cout << "========================================\n";
}

// --- Helper: run a ride end to end ---
void runRide(
    RideManager& rm,
    std::shared_ptr<Rider>  rider,
    std::shared_ptr<Driver> driver,
    Location pickup,
    Location drop,
    std::shared_ptr<FareCalculator> fareCalc)
{
    auto ride = rm.createRide(rider, driver, pickup, drop);

    // Attach observers
    ride->addObserver(std::make_shared<RiderNotificationObserver>(rider));
    ride->addObserver(std::make_shared<DriverNotificationObserver>(driver));
    ride->addObserver(std::make_shared<LoggingObserver>());

    // Progress through full lifecycle
    rm.updateStatus(ride->getId(), RideStatus::ACCEPTED);
    rm.updateStatus(ride->getId(), RideStatus::IN_PROGRESS);
    rm.updateStatus(ride->getId(), RideStatus::COMPLETED);

    // Calculate and save fare
    double fare = fareCalc->calculate(*ride);
    ride->setFare(fare);
    std::cout << "Final Fare: Rs." << fare << "\n";
}

int main() {
    UserManager&     um       = UserManager::getInstance();
    DispatchService& dispatch = DispatchService::getInstance();
    RideManager&     rm       = RideManager::getInstance();

    printSection("SETUP: Registering Drivers & Riders");

    // --- Vehicles ---
    auto v1 = VehicleFactory::createVehicle("V1", "KA-01-1111", VehicleType::SEDAN, 4);
    auto v2 = VehicleFactory::createVehicle("V2", "KA-01-2222", VehicleType::SUV,   6);
    auto v3 = VehicleFactory::createVehicle("V3", "KA-01-3333", VehicleType::AUTO,  3);
    auto v4 = VehicleFactory::createVehicle("V4", "KA-01-4444", VehicleType::BIKE,  1);

    // --- Real Bangalore coordinates (1 degree lat ≈ 111 km) ---
    // Distances will now be realistic
    auto d1 = UserFactory::createDriver("D1", "Ramesh", "8001", v1,
                Location(12.9716, 77.5946, "MG Road"));         // MG Road
    auto d2 = UserFactory::createDriver("D2", "Suresh", "8002", v2,
                Location(12.9352, 77.6245, "Koramangala"));      // Koramangala
    auto d3 = UserFactory::createDriver("D3", "Mahesh", "8003", v3,
                Location(12.9784, 77.6408, "Indiranagar"));      // Indiranagar
    auto d4 = UserFactory::createDriver("D4", "Dinesh", "8004", v4,
                Location(12.9121, 77.6446, "HSR Layout"));       // HSR Layout

    d1->updateRating(4.2);
    d2->updateRating(4.8);
    d3->updateRating(3.9);
    d4->updateRating(4.5);

    um.registerDriver(d1);
    um.registerDriver(d2);
    um.registerDriver(d3);
    um.registerDriver(d4);

    dispatch.registerDriver(d1);
    dispatch.registerDriver(d2);
    dispatch.registerDriver(d3);
    dispatch.registerDriver(d4);

    // --- Riders ---
    auto r1 = UserFactory::createRider("R1", "Arjun", "9001");
    auto r2 = UserFactory::createRider("R2", "Priya", "9002");
    auto r3 = UserFactory::createRider("R3", "Karan", "9003");

    um.registerRider(r1);
    um.registerRider(r2);
    um.registerRider(r3);

    // -------------------------------------------------------
    printSection("RIDE 1: Arjun | Nearest Driver | Base Fare");
    // -------------------------------------------------------
    dispatch.setStrategy(std::make_shared<NearestDriverStrategy>());

    // Arjun is near MG Road — d1 (Ramesh) should be matched
    Location pickup1(12.9720, 77.5950, "MG Road");
    Location drop1(12.9352, 77.6245, "Koramangala");  // ~5.5 km

    auto driver1 = dispatch.findDriver(pickup1);
    auto fareCalc1 = std::make_shared<BaseFareCalculator>();
    runRide(rm, r1, driver1, pickup1, drop1, fareCalc1);

    // -------------------------------------------------------
    printSection("RIDE 2: Priya | Best Rated Driver | Surge x1.5");
    // -------------------------------------------------------
    dispatch.setStrategy(std::make_shared<BestRatedDriverStrategy>());

    // Priya is in Indiranagar — d2 (Suresh, 4.8 rating) should be matched
    Location pickup2(12.9784, 77.6408, "Indiranagar");
    Location drop2(12.9121, 77.6446, "HSR Layout");   // ~7.5 km

    auto driver2   = dispatch.findDriver(pickup2);
    auto fareCalc2 = std::make_shared<SurgePricingDecorator>(
                        std::make_shared<BaseFareCalculator>(), 1.5);
    runRide(rm, r2, driver2, pickup2, drop2, fareCalc2);

    // -------------------------------------------------------
    printSection("RIDE 3: Karan | Nearest Driver | Surge x2 + Discount Rs.20");
    // -------------------------------------------------------
    dispatch.setStrategy(std::make_shared<NearestDriverStrategy>());

    // Karan is near HSR Layout
    Location pickup3(12.9100, 77.6500, "HSR Layout");
    Location drop3(12.8400, 77.6800, "Electronic City");  // ~9 km

    auto driver3 = dispatch.findDriver(pickup3);

    auto surgePart = std::make_shared<SurgePricingDecorator>(
                        std::make_shared<BaseFareCalculator>(), 2.0);
    auto fareCalc3 = std::make_shared<DiscountDecorator>(
                        std::static_pointer_cast<FareCalculator>(surgePart), 20.0);
    runRide(rm, r3, driver3, pickup3, drop3, fareCalc3);

    // -------------------------------------------------------
    printSection("FINAL SUMMARY");
    // -------------------------------------------------------
    for (auto& rider : um.getAllRiders()) {
        std::cout << "\nRider: " << rider->getName() << "\n";
        auto rides = rm.getRidesForRider(rider->getId());
        if (rides.empty()) {
            std::cout << "  No rides yet.\n";
            continue;
        }
        for (auto& ride : rides) {
            std::cout << "  " << ride->getId()
                      << " | " << rideStatusToString(ride->getStatus())
                      << " | Driver: "  << ride->getDriver()->getName()
                      << " | Vehicle: " << vehicleTypeToString(
                                             ride->getDriver()->getVehicle()->getType())
                      << " | Fare: Rs." << ride->getFare() << "\n";
        }
    }

    std::cout << "\n========================================\n";
    std::cout << "  SwiftRide simulation complete!\n";
    std::cout << "========================================\n";

    return 0;
}