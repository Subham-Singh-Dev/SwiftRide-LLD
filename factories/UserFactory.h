#pragma once
#include <memory>
#include <string>
#include "../models/Rider.h"
#include "../models/Driver.h"
#include "../models/Vehicle.h"
#include "../models/Location.h"

class UserFactory {
public:
    static std::shared_ptr<Rider> createRider(
        const std::string& id,
        const std::string& name,
        const std::string& phone)
    {
        return std::make_shared<Rider>(id, name, phone);
    }

    static std::shared_ptr<Driver> createDriver(
        const std::string& id,
        const std::string& name,
        const std::string& phone,
        std::shared_ptr<Vehicle> vehicle,
        Location startLocation)
    {
        return std::make_shared<Driver>(id, name, phone,
                                        std::move(vehicle),
                                        std::move(startLocation));
    }
};