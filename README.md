# SwiftRide 🚗 — Ride Sharing Service (LLD Project)

A fully in-memory ride-sharing backend built in C++ demonstrating
SOLID principles and design patterns. No databases, no frameworks —
pure Object Oriented Design.

---

## 🏗️ Project Architecture

### Design Patterns Used

| Pattern | Where Used |
|---------|-----------|
| Strategy | Driver Matching Algorithms (Nearest / Best Rated) |
| Factory | Creating User and Vehicle objects |
| Singleton | UserManager, RideManager, DispatchService |
| Observer | Notifications on ride status changes |
| Decorator | Fare Calculation (Surge Pricing, Discounts) |

---

## 🧱 SOLID Principles Applied

- **S — Single Responsibility:** Each class has one job.
  Example: `RideManager` only manages rides, `DispatchService`
  only handles driver matching.

- **O — Open/Closed:** System is open for extension, closed for
  modification. Example: Adding a new matching algorithm only
  requires a new class implementing `MatchingStrategy` — no
  existing code changes needed.

- **L — Liskov Substitution:** `Rider` and `Driver` can be used
  wherever `User` is expected without breaking behavior.

- **I — Interface Segregation:** Interfaces are kept small and
  focused. Example: `RideObserver` only has `onStatusChange()`,
  nothing extra.

- **D — Dependency Inversion:** High-level classes depend on
  abstractions, not concrete classes. Example: `DispatchService`
  depends on `MatchingStrategy` interface, not on
  `NearestDriverStrategy` directly.

---

## 📐 UML Class Diagram (High Level)
```
┌──────────┐         ┌──────────┐        ┌─────────────┐
│  <<abs>> │         │  Rider   │        │   Driver    │
│   User   │◄────────│──────────│        │─────────────│
│──────────│         │-rideHist │        │-vehicle     │
│-id       │         │+bookRide │        │-rating      │
│-name     │         └──────────┘        │-isAvailable │
│-phone    │                             │-location    │
└──────────┘                             └─────────────┘
                                                │
                                         ┌──────────────┐
                                         │   Vehicle    │
                                         │──────────────│
                                         │-type (enum)  │
                                         │-capacity     │
                                         │-baseFareRate │
                                         └──────────────┘

┌─────────────────────────────────────────────────────┐
│                       Ride                          │
│─────────────────────────────────────────────────────│
│-rider, -driver, -pickup, -drop, -status, -fare      │
│-observers: List<RideObserver>                       │
│+addObserver() +setStatus() +notifyObservers()       │
└─────────────────────────────────────────────────────┘
       │ notifies
       ▼
┌────────────────┐  ┌──────────────────────┐  ┌───────────────┐
│ <<interface>>  │  │RiderNotification     │  │LoggingObserver│
│ RideObserver   │◄─│Observer              │  │               │
│+onStatusChange │  └──────────────────────┘  └───────────────┘
└────────────────┘

FARE CALCULATION (Decorator Pattern):
BaseFareCalculator
      ↑
SurgePricingDecorator (wraps BaseFare)
      ↑
DiscountDecorator (wraps SurgePricing)

MATCHING STRATEGY (Strategy Pattern):
<<interface>> MatchingStrategy
      ↑                ↑
NearestDriver    BestRatedDriver
Strategy         Strategy

SINGLETONS:
UserManager | RideManager | DispatchService
```

---

## 📁 Planned Folder Structure
```
SwiftRide/
├── models/
│   ├── Location.h
│   ├── User.h
│   ├── Rider.h
│   ├── Driver.h
│   ├── Vehicle.h
│   └── Ride.h
├── enums/
│   ├── RideStatus.h
│   └── VehicleType.h
├── strategies/
│   ├── MatchingStrategy.h
│   ├── NearestDriverStrategy.h
│   └── BestRatedDriverStrategy.h
├── observers/
│   ├── RideObserver.h
│   ├── RiderNotificationObserver.h
│   ├── DriverNotificationObserver.h
│   └── LoggingObserver.h
├── fare/
│   ├── FareCalculator.h
│   ├── BaseFareCalculator.h
│   ├── FareCalculatorDecorator.h
│   ├── SurgePricingDecorator.h
│   └── DiscountDecorator.h
├── managers/
│   ├── UserManager.h
│   ├── RideManager.h
│   └── DispatchService.h
├── factories/
│   ├── UserFactory.h
│   └── VehicleFactory.h
└── main.cpp
```

---

## 📊 Project Progress

- [x] Phase 1 — Architecture Planning & UML Design
- [ ] Phase 2 — Core Models
- [ ] Phase 3 — Ride Lifecycle
- [ ] Phase 4 — Driver Matching
- [ ] Phase 5 — Notifications
- [ ] Phase 6 — Fare Calculation
- [ ] Phase 7 — Main Simulation
- [ ] Phase 8 — Final UML Diagrams + README Polish
