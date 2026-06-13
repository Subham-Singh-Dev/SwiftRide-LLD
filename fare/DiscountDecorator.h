#pragma once
#include "FareCalculatorDecorator.h"
#include <iostream>
#include <algorithm>

class DiscountDecorator : public FareCalculatorDecorator {
public:
    explicit DiscountDecorator(
        std::shared_ptr<FareCalculator> inner,
        double flatDiscount = 20.0)
        : FareCalculatorDecorator(std::move(inner))
        , discount_(flatDiscount) {}

    double calculate(const Ride& ride) const override {
        double fare       = calculateInner(ride);
        double discounted = std::max(0.0, fare - discount_);

        std::cout << "[Fare] Discount (-₹" << discount_
                  << "): ₹" << discounted << "\n";
        return discounted;
    }

private:
    double discount_;
};