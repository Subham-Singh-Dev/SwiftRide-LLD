#pragma once
#include "FareCalculatorDecorator.h"
#include <iostream>

class SurgePricingDecorator : public FareCalculatorDecorator {
public:
    explicit SurgePricingDecorator(
        std::shared_ptr<FareCalculator> inner,
        double multiplier = 1.5)
        : FareCalculatorDecorator(std::move(inner))
        , multiplier_(multiplier) {}

    double calculate(const Ride& ride) const {
        double fare  = calculateInner(ride);
        double surge = fare * multiplier_;

        std::cout << "[Fare] Surge pricing (x" << multiplier_
                  << "): ₹" << surge << "\n";
        return surge;
    }

private:
    double multiplier_;
};