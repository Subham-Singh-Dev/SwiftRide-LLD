#pragma once
#include <string>

// Abstract base class — you can't be "just a User" in this system.
// Every user is either a Rider or a Driver.
class User {
public:
    User(std::string id, std::string name, std::string phone)
        : id_(std::move(id))
        , name_(std::move(name))
        , phone_(std::move(phone)) {}

    virtual ~User() = default;

    const std::string& getId()    const { return id_; }
    const std::string& getName()  const { return name_; }
    const std::string& getPhone() const { return phone_; }

    // Pure virtual: forces Rider and Driver to identify themselves
    virtual std::string getRole() const = 0;

private:
    std::string id_;
    std::string name_;
    std::string phone_;
};