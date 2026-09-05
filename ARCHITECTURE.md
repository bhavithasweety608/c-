# EV Charging Station – Complete Feature Set & Architecture

## Project Overview
A comprehensive C++ EV charging management system with vehicle registration, intelligent queue management, automatic slot assignment, real-time notifications, virtual queue support, online payments, and analytics. Built with SQLite backend and web-based interface.

---

## Complete Feature List

### ✅ Core Features (Original)

#### 1. 🚗 Vehicle Registration
- Register EV vehicle details
- Vehicle number, owner name, vehicle type, battery capacity
- Supports 2W, 3W, and 4W vehicles
- Unlimited vehicles per user
- Vehicle profile editing and deletion

#### 2. 🔌 Charger Type Management
- Multiple charger types by vehicle class:
  - 2W Standard Charger
  - 3W Standard Charger
  - 4W Standard Charger
  - 4W Fast Charger
- Charger power specifications (kW)
- Charger availability status
- Maintenance scheduling

#### 3. 🔄 Charger Compatibility
- Automatic compatibility checking
- Prevents incompatible charger assignments
- Suggests alternative chargers if needed
- Compatibility matrix management

#### 4. 🎟️ Automatic Token Generation
- Unique token per charging session
- QR code generation for quick reference
- Token expiry management
- Token history tracking

#### 5. 📋 Smart Charging Queue
- Real-time queue display
- Shows: Token, Vehicle #, Type, Position, Status
- Visual queue representation
- Queue priority sorting

#### 6. 📍 Queue Position
- Live position tracking
- Real-time position updates
- Queue movement notifications
- Estimated wait time display

#### 7. ⏱️ Waiting Time Estimation
- Dynamic calculation based on:
  - Vehicles ahead in queue
  - Charger availability
  - Individual charging duration
  - Current queue load
- Accuracy improves with historical data

#### 8. 🤖 Automatic Slot Assignment
- Intelligent charger selection algorithm
- Considers charger type, availability, location
- Optimizes for minimal wait time
- Fallback options if preferred charger unavailable

#### 9. 🔋 Battery & Energy Management
- Battery capacity tracking
- Current battery percentage input
- Target charge percentage setting
- Energy requirement calculation:
  ```
  Energy Required = Battery Capacity × (Target% − Current%) / 100
  ```

#### 10. ⏳ Charging Time Estimation
- Formula: `Charging Time ≈ Energy Required / Charger Power`
- Accounts for power loss and efficiency
- Updated estimates during charging
- Historical accuracy data

#### 11. 💰 Charging Cost Estimation
- Pre-charging cost calculation
- Formula: `Cost = Energy Required × Rate per kWh`
- Dynamic pricing support
- Off-peak discounts
- Loyalty program integration

#### 12. 🚨 Emergency Priority
- Emergency vehicle override
- Bumps vehicle to front of queue
- Administrative approval option
- Audit trail for emergency requests
- Priority level system (High, Medium, Low)

#### 13. 📅 Slot Reservation
- Reserve charging slot for future date/time
- Select desired charger type
- Availability verification
- Reservation confirmation and cancellation
- Reservation reminders (24h, 1h, 15m before)

#### 14. 🔄 Automatic Next-Vehicle Assignment
- Triggers when vehicle completes charging
- Automatic queue progression
- Compatibility re-verification
- Notification sent to next vehicle
- Slot immediately reassigned

#### 15. 🧾 Digital Charging Receipt
- Auto-generated after charging
- Contains: Vehicle #, Charger Used, Energy, Duration, Cost, Date/Time
- PDF export capability
- Email delivery option
- Digital archival

#### 16. 📚 Charging History
- Complete session history per vehicle
- Filters by date range, charger type, cost
- Export history reports
- Usage patterns analysis
- Mileage tracking (if integrated with vehicle data)

#### 17. 📊 Daily Statistics
- Real-time dashboard showing:
  - Vehicles charged today
  - Vehicles currently waiting
  - Currently charging vehicles
  - Total energy consumed (kWh)
  - Total revenue generated (₹)
  - Available chargers count
  - Peak charging hours
  - Average charging time
  - Cost statistics

#### 18. 🗄️ SQLite Database
- Persistent storage:
  - User profiles
  - Vehicle records
  - Charger configuration
  - Queue status
  - Charging sessions
  - Reservations
  - Charging history
  - Transactions
  - Payments

#### 19. 🌐 Web-Based Interface
- HTML + CSS + JavaScript frontend
- C++ backend REST API
- SQLite database
- Real-time updates via WebSockets
- Mobile-responsive design

---

### ✨ NEW Features to Add

#### 20. 🌐 Virtual Queue System
- Join queue online without physical presence
- Real-time queue position tracking via web/app
- Live notifications:
  - Queue position update
  - "Your turn is in 15 minutes" alert
  - "Please arrive at station" reminder
  - Turn expiry warning
- Automatic priority loss if user doesn't arrive within time window
- Time window customizable (default: 15-20 minutes)
- Option to rejoin queue if missed
- Location-based check-in capability
- Geo-fencing for arrival confirmation

**Features in Detail:**
```cpp
struct VirtualQueueEntry {
    - queueId: int
    - userId: int
    - vehicleId: int
    - joinedTime: DateTime
    - estimatedTurnTime: DateTime
    - status: ONLINE_WAITING, NOTIFIED_15MIN, NOTIFIED_ARRIVING, CONFIRMED_ARRIVAL, 
              EXPIRED, CHARGING, COMPLETED
    - notificationsSent: vector<NotificationLog>
    - arrivalConfirmedTime: DateTime (nullable)
    - expiryTime: DateTime
    - isPhysicallyPresent: bool
}

// Notification Triggers
- At Queue Join → Welcome message
- 15 mins before → Urgent notification
- 5 mins before → "Please arrive now"
- At Turn Time → "You're up! Report arrival"
- After 20 mins → "Priority expired. Rejoin queue? (End of Queue)"

// Benefits
- Users don't waste time waiting physically
- Better user experience
- Optimized charger utilization
- Reduced station congestion
```

#### 21. 💳 Online Payment Gateway Integration
- Multiple payment options:
  - Credit/Debit Card (Razorpay, PayTM, Stripe)
  - Digital Wallets (Google Pay, Apple Pay, PhonePe)
  - UPI (BHIM, Google Pay, WhatsApp Pay)
  - Net Banking
  - Prepaid wallet/account balance
  - QR code payment
  - EMI options for high-value charges

- Payment features:
  - Pre-payment option before charging
  - Post-payment on completion
  - Automated invoice generation
  - Payment confirmation email/SMS
  - Failed payment retry mechanism
  - Refund processing
  - Payment history and receipts
  - Tax calculation and GST compliance
  - Multiple currency support (future)

**Database Structure:**
```cpp
struct PaymentRecord {
    - paymentId: UUID
    - sessionId: UUID
    - userId: int
    - amount: double
    - currency: string (default: "INR")
    - paymentMethod: CARD, WALLET, UPI, NETBANKING, PREPAID
    - paymentGateway: string (Razorpay, PayTM, etc.)
    - transactionId: string (from gateway)
    - status: PENDING, COMPLETED, FAILED, REFUNDED
    - timestamp: DateTime
    - receiptUrl: string
    - failureReason: string (if failed)
    - retryCount: int
}

struct Wallet {
    - walletId: int
    - userId: int
    - balance: double
    - lastUpdated: DateTime
    - transactions: vector<WalletTransaction>
}

struct WalletTransaction {
    - transactionId: UUID
    - type: CREDIT, DEBIT
    - amount: double
    - reason: string (e.g., "Charging", "Refund", "Promotional")
    - timestamp: DateTime
}
```

- Payment API Endpoints:
  ```
  POST /payment/initiate          → Start payment process
  POST /payment/verify            → Verify payment with gateway
  GET /payment/history            → User payment history
  POST /wallet/add-balance        → Add funds to wallet
  GET /wallet/balance             → Check wallet balance
  POST /payment/refund            → Process refund
  ```

---

## Enhanced System Architecture

### Updated Component Diagram

```
┌─────────────────────────────────────────────────────────────────────┐
│                        Frontend Layer                               │
│  ┌──────────────────┬──────────────────┬──────────────────────┐    │
│  │   Web Interface  │   Mobile App     │  Admin Dashboard     │    │
│  │  (HTML/CSS/JS)   │  (React Native)  │  (Real-time stats)   │    │
│  └──────────────────┴──────────────────┴──────────────────────┘    │
└────────────────────────────┬────────────────────────────────────────┘
                             │
┌────────────────────────────▼────────────────────────────────────────┐
│                    API Layer (REST + WebSocket)                     │
│  ┌──────────────────────────────────────────────────────────────┐   │
│  │ Authentication  │ Session  │ Queue  │ Payment  │  Notification │ │
│  │ Endpoints       │ Endpoints│Mgmt   │ Endpoints│   Endpoints   │ │
│  └──────────────────────────────────────────────────────────────┘   │
└────────────────────────────┬────────────────────────────────────────┘
                             │
┌────────────────────────────▼────────────────────────────────────────┐
│                   Application/Service Layer                         │
│  ┌─────────────────────────────────────────────────────────────┐    │
│  │  • SessionManager           • NotificationService           │    │
│  │  • QueueManager             • PaymentService                │    │
│  │  • PowerManager             • WalletService                 │    │
│  │  • ChargingController       • UserService                   │    │
│  │  • VirtualQueueManager      • ReservationService            │    │
│  │  • AnalyticsService         • ReportGenerator               │    │
│  └─────────────────────────────────────────────────────────────┘    │
└────────────────────────────┬────────────────────────────────────────┘
                             │
┌────────────────────────────▼────────────────────────────────────────┐
│                   Domain Layer (Business Logic)                     │
│  ┌─────────────────────────────────────────────────────────────┐    │
│  │ Domain Models:                                              │    │
│  │  • User, Vehicle, ChargingSession, ChargingPoint            │    │
│  │  • QueueEntry, VirtualQueueEntry, Reservation              │    │
│  │  • PaymentRecord, Wallet, Transaction                       │    │
│  │  • StationConfig, Charger, PowerManager                     │    │
│  └─────────────────────────────────────────────────────────────┘    │
└────────────────────────────┬────────────────────────────────────────┘
                             │
┌────────────────────────────▼────────────────────────────────────────┐
│                   Data Access Layer (Repository)                    │
│  ┌─────────────────────────────────────────────────────────────┐    │
│  │  • UserRepository           • QueueRepository               │    │
│  │  • VehicleRepository        • PaymentRepository             │    │
│  │  • ChargerRepository        • WalletRepository              │    │
│  │  • SessionRepository        • ReservationRepository         │    │
│  └─────────────────────────────────────────────────────────────┘    │
└────────────────────────────┬────────────────────────────────────────┘
                             │
┌────────────────────────────▼────────────────────────────────────────┐
│                   Persistence Layer                                 │
│  ┌──────────────────┬──────────────────┬────────────────────────┐   │
│  │   SQLite DB      │   Redis Cache    │  File Storage (PDFs)   │   │
│  └──────────────────┴──────────────────┴────────────────────────┘   │
└─────────────────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────────────────┐
│            External Integrations                                    │
│  ┌──────────────────┬──────────────────┬──────────────────────┐    │
│  │  Payment Gateway │  SMS/Email       │  Geo-location        │    │
│  │  (Razorpay)      │  Service         │  Service             │    │
│  └──────────────────┴──────────────────┴──────────────────────┘    │
└─────────────────────────────────────────────────────────────────────┘
```

---

## Updated Directory Structure

```
ev-charging-station/
├── src/
│   ├── api/
│   │   ├── handlers/
│   │   │   ├── AuthHandler.h/cpp
│   │   │   ├── SessionHandler.h/cpp
│   │   │   ├── QueueHandler.h/cpp
│   │   │   ├── VirtualQueueHandler.h/cpp      [NEW]
│   │   │   ├── PaymentHandler.h/cpp           [NEW]
│   │   │   ├── WalletHandler.h/cpp            [NEW]
│   │   │   ├── NotificationHandler.h/cpp
│   │   │   ├── ReservationHandler.h/cpp
│   │   │   └── AdminHandler.h/cpp
│   │   ├── middleware/
│   │   │   ├── AuthMiddleware.h/cpp
│   │   │   ├── RequestValidator.h/cpp
│   │   │   └── ErrorHandler.h/cpp
│   │   └── routes.cpp
│   │
│   ├── services/
│   │   ├── SessionService.h/cpp
│   │   ├── QueueManager.h/cpp
│   │   ├── VirtualQueueManager.h/cpp          [NEW]
│   │   ├── PowerManager.h/cpp
│   │   ├── PaymentService.h/cpp               [NEW]
│   │   ├── WalletService.h/cpp                [NEW]
│   │   ├── UserService.h/cpp
│   │   ├── NotificationService.h/cpp
│   │   ├── ReservationService.h/cpp
│   │   ├── AnalyticsService.h/cpp
│   │   └── ChargingController.h/cpp
│   │
│   ├── domain/
│   │   ├── User.h
│   │   ├── Vehicle.h
│   │   ├── ChargingSession.h
│   │   ├── ChargingPoint.h
│   │   ├── QueueEntry.h
│   │   ├── VirtualQueueEntry.h                [NEW]
│   │   ├── Reservation.h
│   │   ├── PaymentRecord.h                    [NEW]
│   │   ├── Wallet.h                           [NEW]
│   │   ├── Transaction.h
│   │   └── StationConfig.h
│   │
│   ├── repository/
│   │   ├── UserRepository.h/cpp
│   │   ├── VehicleRepository.h/cpp
│   │   ├── SessionRepository.h/cpp
│   │   ├── QueueRepository.h/cpp
│   │   ├── VirtualQueueRepository.h/cpp       [NEW]
│   │   ├── PaymentRepository.h/cpp            [NEW]
│   │   ├── WalletRepository.h/cpp             [NEW]
│   │   ├── ReservationRepository.h/cpp
│   │   ├── ChargerRepository.h/cpp
│   │   └── IRepository.h (base interface)
│   │
│   ├── external/
│   │   ├── PaymentGateway.h/cpp               [NEW]
│   │   ├── RazorpayAdapter.h/cpp              [NEW]
│   │   ├── NotificationProvider.h/cpp
│   │   ├── SMSService.h/cpp
│   │   ├── EmailService.h/cpp
│   │   └── GeoLocationService.h/cpp           [NEW]
│   │
│   ├── hardware/
│   │   ├── ChargingPointDriver.h/cpp
│   │   ├── PowerMeterDriver.h/cpp
│   │   └── OCPPConnector.h/cpp
│   │
│   ├── utils/
│   │   ├── Logger.h/cpp
│   │   ├── Config.h/cpp
│   │   ├── Validators.h/cpp
│   │   ├── TokenGenerator.h/cpp
│   │   ├── ReceiptGenerator.h/cpp
│   │   ├── QRCodeGenerator.h/cpp
│   │   └── DateTimeUtils.h/cpp
│   │
│   ├── database/
│   │   ├── DatabaseManager.h/cpp
│   │   ├── schema.sql
│   │   └── migrations/
│   │       ├── 001_initial_schema.sql
│   │       ├── 002_add_virtual_queue.sql      [NEW]
│   │       ├── 003_add_payment_tables.sql     [NEW]
│   │       └── 004_add_wallet_system.sql      [NEW]
│   │
│   └── main.cpp
│
├── frontend/                                  [NEW]
│   ├── public/
│   ├── src/
│   │   ├── components/
│   │   │   ├── Queue/
│   │   │   ├── VirtualQueue/                  [NEW]
│   │   │   ├── Payment/                       [NEW]
│   │   │   ├── Wallet/                        [NEW]
│   │   │   ├── Dashboard/
│   │   │   ├── Reservation/
│   │   │   └── History/
│   │   ├── pages/
│   │   ├── services/
│   │   └── App.js
│   ├── package.json
│   └── index.html
│
├── tests/
│   ├── unit/
│   │   ├── services/
│   │   ├── domain/
│   │   └── utils/
│   ├── integration/
│   └── CMakeLists.txt
│
├── docs/
│   ├── API.md
│   ├── DATABASE_SCHEMA.md
│   ├── DEPLOYMENT.md
│   ├── PAYMENT_INTEGRATION.md                 [NEW]
│   └── VIRTUAL_QUEUE.md                       [NEW]
│
├── scripts/
│   ├── setup.sh
│   ├── seed_data.sql
│   └── payment_setup.sh                       [NEW]
│
├── config/
│   ├── app.config.json
│   ├── database.config.json
│   └── payment.config.json                    [NEW]
│
├── CMakeLists.txt
├── conanfile.txt
├── Dockerfile
├── docker-compose.yml
├── .env.example
├── README.md
└── .github/
    └── workflows/
        ├── build.yml
        ├── test.yml
        └── deploy.yml
```

---

## Enhanced Database Schema

### New Tables for Virtual Queue & Payment

```sql
-- Virtual Queue Table
CREATE TABLE virtual_queue (
    queue_id INTEGER PRIMARY KEY AUTOINCREMENT,
    user_id INTEGER NOT NULL,
    vehicle_id INTEGER NOT NULL,
    joined_time TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    estimated_turn_time TIMESTAMP,
    status TEXT DEFAULT 'ONLINE_WAITING',
    arrival_confirmed_time TIMESTAMP,
    expiry_time TIMESTAMP,
    is_physically_present BOOLEAN DEFAULT 0,
    FOREIGN KEY(user_id) REFERENCES users(user_id),
    FOREIGN KEY(vehicle_id) REFERENCES vehicles(vehicle_id)
);

-- Notification Log
CREATE TABLE notification_log (
    notification_id INTEGER PRIMARY KEY AUTOINCREMENT,
    queue_id INTEGER NOT NULL,
    notification_type TEXT,
    message TEXT,
    sent_time TIMESTAMP,
    read_time TIMESTAMP,
    channel TEXT, -- SMS, EMAIL, PUSH
    FOREIGN KEY(queue_id) REFERENCES virtual_queue(queue_id)
);

-- Payment Records Table
CREATE TABLE payment_records (
    payment_id TEXT PRIMARY KEY,
    session_id TEXT NOT NULL,
    user_id INTEGER NOT NULL,
    amount REAL NOT NULL,
    currency TEXT DEFAULT 'INR',
    payment_method TEXT,
    payment_gateway TEXT,
    transaction_id TEXT UNIQUE,
    status TEXT DEFAULT 'PENDING',
    timestamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    receipt_url TEXT,
    failure_reason TEXT,
    retry_count INTEGER DEFAULT 0,
    FOREIGN KEY(user_id) REFERENCES users(user_id),
    FOREIGN KEY(session_id) REFERENCES charging_sessions(session_id)
);

-- Wallet Table
CREATE TABLE wallet (
    wallet_id INTEGER PRIMARY KEY AUTOINCREMENT,
    user_id INTEGER NOT NULL UNIQUE,
    balance REAL DEFAULT 0,
    last_updated TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY(user_id) REFERENCES users(user_id)
);

-- Wallet Transactions Table
CREATE TABLE wallet_transactions (
    transaction_id TEXT PRIMARY KEY,
    wallet_id INTEGER NOT NULL,
    type TEXT, -- CREDIT, DEBIT
    amount REAL NOT NULL,
    reason TEXT,
    timestamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY(wallet_id) REFERENCES wallet(wallet_id)
);

-- Refunds Table
CREATE TABLE refunds (
    refund_id TEXT PRIMARY KEY,
    payment_id TEXT NOT NULL,
    amount REAL NOT NULL,
    reason TEXT,
    status TEXT DEFAULT 'PENDING',
    initiated_at TIMESTAMP,
    completed_at TIMESTAMP,
    FOREIGN KEY(payment_id) REFERENCES payment_records(payment_id)
);

-- Payment Methods Table
CREATE TABLE payment_methods (
    method_id INTEGER PRIMARY KEY AUTOINCREMENT,
    user_id INTEGER NOT NULL,
    type TEXT, -- CARD, UPI, WALLET
    details_encrypted TEXT,
    is_default BOOLEAN,
    created_at TIMESTAMP,
    FOREIGN KEY(user_id) REFERENCES users(user_id)
);
```

---

## Virtual Queue Workflow

```
┌─────────────────────────────────────────────────────────────┐
│  Customer Opens App/Website                                 │
└────────────────────────┬────────────────────────────────────┘
                         │
                         ▼
        ┌────────────────────────────────┐
        │ Select Vehicle & Charger Type  │
        └────────────┬───────────────────┘
                     │
                     ▼
        ┌────────────────────────────────┐
        │ Join Virtual Queue (Online)    │
        │ - No physical presence needed  │
        └────────────┬───────────────────┘
                     │
                     ▼
        ┌──────────────────────────────────────┐
        │ System Assigns Position & Token       │
        │ Displays: Position, Wait Time, Cost  │
        └────────────┬────────────────────────┘
                     │
                     ▼
        ┌──────────────────────────────────────┐
        │ User Views Queue in Real-time         │
        │ - Live position tracking             │
        │ - Estimated turn time (updates)      │
        └────────────┬────────────────────────┘
                     │
              ┌──────┴──────┐
              │             │
              ▼             ▼
    ┌─────────────────┐ ┌──────────────────────┐
    │ 15 Min Alert    │ │ Can Cancel/Rejoin    │
    │ "Prepare to     │ │ Queue                │
    │  arrive"        │ │                      │
    └────────┬────────┘ └──────────────────────┘
             │
             ▼
    ┌──────────────────────────────────┐
    │ 5 Min Alert                      │
    │ "Please arrive at station now"   │
    └────────┬─────────────────────────┘
             │
             ▼
    ┌──────────────────────────────────┐
    │ Check-in at Station              │
    │ - Geo-location confirmation      │
    │ - Manual check-in button         │
    └────────┬─────────────────────────┘
             │
        ┌────┴─────┐
        │           │
        ▼           ▼
    ┌─────┐     ┌──────────────────┐
    │YES  │     │NO (Within 20min) │
    └──┬──┘     └────────┬─────────┘
       │                 │
       │                 ▼
       │        ┌──────────────────┐
       │        │ Priority Expires  │
       │        │ Move to End of Q  │
       │        │ Allow Rejoin      │
       │        └──────────────────┘
       │
       ▼
    ┌──────────────────────────────────┐
    │ Automatic Charger Assignment      │
    │ - Compatible charger selected    │
    │ - Session starts                 │
    └────────┬─────────────────────────┘
             │
             ▼
    ┌──────────────────────────────────┐
    │ Charging in Progress              │
    │ - Real-time status updates       │
    │ - Energy & cost tracking         │
    └────────┬─────────────────────────┘
             │
             ▼
    ┌──────────────────────────────────┐
    │ Charging Complete                 │
    │ - Session ends                   │
    └────────┬─────────────────────────┘
             │
             ▼
    ┌──────────────────────────────────┐
    │ Payment (Pre/Post-Charging)       │
    │ - Select payment method          │
    │ - Process transaction            │
    └────────┬─────────────────────────┘
             │
             ▼
    ┌──────────────────────────────────┐
    │ Digital Receipt Generated         │
    │ - Email/Download PDF             │
    │ - Save to history                │
    └──────────────────────────────────┘
```

---

## Online Payment Integration

### Payment Flow

```
Frontend                          Backend                      Payment Gateway
   │                                 │                              │
   │─ POST /payment/initiate ───────▶│                              │
   │                                 │                              │
   │                                 │─ Validate Amount ───────────▶│
   │                                 │◀─ Order ID + Token ──────────│
   │◀─ Payment Page + Order ID ──────│                              │
   │                                 │                              │
   │─ User Enters Payment Details ──▶│                              │
   │   (Card/UPI/Wallet)             │                              │
   │                                 │─ Process Payment ───────────▶│
   │                                 │                              │
   │                                 │◀─ Success/Failure ──────────│
   │                                 │                              │
   │◀─ Payment Confirmation ────────▶│ Create Transaction Record   │
   │                                 │                              │
   │                                 │ Send Receipt Email          │
   │                                 │ Update Session Status       │
```

### Supported Payment Methods

```cpp
enum PaymentMethod {
    CREDIT_CARD,          // Visa, Mastercard
    DEBIT_CARD,           // All major banks
    UPI,                  // All UPI providers
    NET_BANKING,          // ICICI, HDFC, SBI, etc.
    DIGITAL_WALLET,       // Google Pay, PhonePe, PayTM
    PREPAID_WALLET,       // Account balance
    QR_CODE_PAYMENT       // Static QR at station
};

struct PaymentGatewayConfig {
    string gateway_name;              // Razorpay, PayTM, Stripe
    string api_key;                   // API credentials
    string api_secret;
    double transaction_fee_percent;   // 2-3% typically
    int timeout_seconds;              // 300
    bool supports_recurring;
    bool supports_refunds;
    int max_refund_days;
};
```

### API Endpoints for Payment

```
POST /api/v1/payment/initiate
  Params: { sessionId, amount, paymentMethod }
  Returns: { orderId, paymentPageUrl, token }

POST /api/v1/payment/verify
  Params: { orderId, paymentId, signature }
  Returns: { status, transactionId, receipt }

GET /api/v1/payment/history
  Query: { userId, limit, offset }
  Returns: [ { paymentId, amount, status, date, receipt } ]

POST /api/v1/payment/refund
  Params: { paymentId, reason }
  Returns: { refundId, status }

POST /api/v1/wallet/add-balance
  Params: { userId, amount, paymentMethod }
  Returns: { walletId, newBalance }

GET /api/v1/wallet/balance
  Query: { userId }
  Returns: { balance, lastUpdated }

GET /api/v1/wallet/transactions
  Query: { userId, limit }
  Returns: [ { transactionId, type, amount, date } ]
```

---

## Complete Feature Summary Table

| Feature | Category | Priority | Status | Complexity |
|---------|----------|----------|--------|-----------|
| Vehicle Registration | Core | High | ✅ | Low |
| Charger Management | Core | High | ✅ | Low |
| Compatibility Check | Core | High | ✅ | Medium |
| Token Generation | Core | High | ✅ | Low |
| Smart Queue | Core | High | ✅ | Medium |
| Automatic Slot Assignment | Core | High | ✅ | High |
| Battery Management | Core | High | ✅ | Medium |
| Cost Estimation | Core | High | ✅ | Medium |
| Emergency Priority | Core | Medium | ✅ | Medium |
| Slot Reservation | Core | High | ✅ | High |
| Digital Receipt | Core | Medium | ✅ | Low |
| Charging History | Core | Medium | ✅ | Low |
| Daily Statistics | Core | Medium | ✅ | Medium |
| **Virtual Queue** | **NEW** | **High** | 🔄 | **High** |
| **Online Payment** | **NEW** | **High** | 🔄 | **High** |
| **Wallet System** | **NEW** | **Medium** | 🔄 | **Medium** |
| **Notifications** | Enhancement | High | ✅ | Medium |
| **Web Interface** | Enhancement | High | ✅ | High |

---

## Technology Stack (Updated)

### Backend (C++)
- **Language**: C++17/20
- **Build**: CMake 3.20+
- **Package Manager**: Conan 2.0+
- **Web Framework**: Beast (Boost.ASIO) or cpp-httplib
- **Database**: SQLite3 + libsqlite3
- **Caching**: Redis (optional, for session storage)
- **Logging**: spdlog
- **Testing**: Google Test (GTest)

### Payment Integration
- **Razorpay SDK** for C++ (or REST API client)
- **nlohmann/json** for JSON processing
- **curl/libcurl** for HTTP requests
- **OpenSSL** for encryption

### Frontend
- **HTML5 + CSS3** (Responsive design)
- **JavaScript/ES6** (Vanilla or React.js)
- **WebSocket** (Real-time updates)
- **Chart.js** or **D3.js** (Analytics)
- **Geolocation API** (Browser native)

### Infrastructure
- **Database**: SQLite for dev, PostgreSQL for production
- **Messaging**: Optional RabbitMQ for notifications
- **Cache**: Redis (optional)
- **Container**: Docker & Docker Compose
- **Deployment**: AWS/Heroku or On-premise

### External APIs
- **Razorpay API** (Payments)
- **Twilio/AWS SNS** (SMS)
- **SendGrid/AWS SES** (Email)
- **Google Maps API** (Geolocation)

---

## Development Phases (Updated)

### Phase 1: Foundation & Core Features (Weeks 1-2)
- [x] Project setup and CMake configuration
- [x] Core domain models
- [x] SQLite schema and basic repository layer
- [x] Token generation and queue management
- [x] Unit tests framework

### Phase 2: Queue & Charging Logic (Weeks 3-4)
- [x] SessionManager implementation
- [x] Smart queue algorithm
- [x] Automatic slot assignment
- [x] Charger compatibility logic
- [x] REST API endpoints (basic)

### Phase 3: Virtual Queue System (Week 5) **[NEW]**
- [ ] VirtualQueueManager service
- [ ] Queue position tracking
- [ ] Notification triggers (15min, 5min, arrival)
- [ ] Geo-location check-in
- [ ] Priority expiry logic
- [ ] WebSocket for real-time updates

### Phase 4: Payment Integration (Weeks 6-7) **[NEW]**
- [ ] PaymentService & PaymentGateway adapter
- [ ] Razorpay integration
- [ ] WalletService & wallet management
- [ ] Payment endpoints (initiate, verify, refund)
- [ ] Transaction logging
- [ ] Receipt generation

### Phase 5: Notifications & Analytics (Weeks 8-9)
- [ ] SMS notifications (Twilio)
- [ ] Email notifications (SendGrid)
- [ ] Push notifications (Firebase)
- [ ] AnalyticsService
- [ ] Dashboard statistics
- [ ] Historical reports

### Phase 6: Frontend Development (Weeks 10-11)
- [ ] User dashboard (HTML/CSS/JS)
- [ ] Virtual queue status page
- [ ] Payment checkout page
- [ ] Wallet management UI
- [ ] Charging history view
- [ ] Admin panel

### Phase 7: Testing & Optimization (Week 12)
- [ ] Integration testing
- [ ] Performance testing
- [ ] Security audit
- [ ] Load testing
- [ ] Bug fixes

### Phase 8: Deployment & Documentation (Week 13)
- [ ] Docker containerization
- [ ] Database migrations
- [ ] API documentation
- [ ] User guides
- [ ] Deployment scripts

---

## Presentation Talking Points

**"What makes this project special?"**

> This EV charging management system intelligently handles multiple aspects of the charging ecosystem:
>
> 1. **Smart Queue Management**: Automatically assigns compatible chargers, calculates wait times, and manages vehicle priorities.
>
> 2. **Virtual Queue Innovation**: Users join online and receive alerts when their turn approaches, eliminating wasted physical waiting time.
>
> 3. **Integrated Online Payments**: Supports multiple payment methods (cards, UPI, wallets) with automatic receipt generation and refund processing.
>
> 4. **Real-time Analytics**: Dashboard provides live metrics on charger utilization, revenue, and system health.
>
> 5. **Battery-aware Charging**: Calculates optimal charging time and cost based on battery capacity and energy requirements.
>
> 6. **Scalable Architecture**: Modular design supports multiple charging stations and future feature additions.
>
> The system combines user convenience (virtual queue, online payments), operational efficiency (automatic assignment, load balancing), and comprehensive tracking (receipts, history, analytics).

---

## Next Steps

1. **Set up project structure** with CMake
2. **Design SQLite schema** including new virtual queue & payment tables
3. **Implement domain models** and repository layer
4. **Build virtual queue manager** with notification logic
5. **Integrate Razorpay** payment gateway
6. **Create REST API** endpoints
7. **Develop frontend** dashboard
8. **Write tests** and documentation

Would you like me to help with any specific component? I can create:
- Complete CMakeLists.txt setup
- Domain model classes
- Database schema with migrations
- VirtualQueueManager implementation
- PaymentService integration code
- Frontend starter templates
- API endpoint implementations
