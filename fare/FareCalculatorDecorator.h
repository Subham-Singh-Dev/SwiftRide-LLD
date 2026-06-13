#pragma once
#include <memory>
#include "FareCalculator.h"

// All decorators inherit from this.
// It holds a reference to the calculator it wraps.
class FareCalculatorDecorator : public FareCalculator {
public:
    explicit FareCalculatorDecorator(std::shared_ptr<FareCalculator> inner)
        : inner_(std::move(inner)) {}

protected:
    // Call this to get the result from the wrapped calculator
    double calculateInner(const Ride& ride) const {
        return inner_->calculate(ride);
    }

private:
    std::shared_ptr<FareCalculator> inner_;
};