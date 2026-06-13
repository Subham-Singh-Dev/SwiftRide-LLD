#pragma once
#include <iostream>
#include "FareCalculator.h"

class BaseFareCalculator : public FareCalculator {
public:
    double calculate(const Ride& ride) const override {
        double distance = ride.getPickup().distanceTo(ride.getDrop());
        double rate     = ride.getDriver()->getVehicle()->getBaseFareRate();
        double fare     = distance * rate;

        std::cout << "[Fare] Base fare: ₹" << fare
                  << " (" << distance << " units × ₹" << rate << "/unit)\n";
        return fare;
    }
};