#pragma once
#include "../models/Ride.h"

//Interface every fare calculator must follow
class FareCalculator {
public:
    virtual ~FareCalculator() = default;

    // Calculate fare based on the ride details
    virtual double calculate(const Ride& ride) const = 0;
};