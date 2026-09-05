-- EV Charging Station Database Schema
-- SQLite3 Database

-- ============ USERS & AUTHENTICATION ============

CREATE TABLE IF NOT EXISTS users (
    user_id INTEGER PRIMARY KEY AUTOINCREMENT,
    username TEXT NOT NULL UNIQUE,
    email TEXT NOT NULL UNIQUE,
    password_hash TEXT NOT NULL,
    phone_number TEXT,
    role TEXT DEFAULT 'USER',  -- ADMIN, USER, TECHNICIAN, OPERATOR
    first_name TEXT,
    last_name TEXT,
    is_active BOOLEAN DEFAULT 1,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    CHECK (role IN ('ADMIN', 'USER', 'TECHNICIAN', 'OPERATOR'))
);

CREATE INDEX idx_users_email ON users(email);
CREATE INDEX idx_users_username ON users(username);

-- ============ VEHICLES ============

CREATE TABLE IF NOT EXISTS vehicles (
    vehicle_id INTEGER PRIMARY KEY AUTOINCREMENT,
    user_id INTEGER NOT NULL,
    vehicle_number TEXT NOT NULL UNIQUE,
    vehicle_type TEXT NOT NULL,  -- 2W, 3W, 4W
    vehicle_name TEXT,
    manufacturer TEXT,
    model TEXT,
    battery_capacity_kwh REAL NOT NULL,  -- in kWh
    current_battery_percent REAL DEFAULT 0,
    target_battery_percent REAL DEFAULT 100,
    registration_date TIMESTAMP,
    is_active BOOLEAN DEFAULT 1,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY(user_id) REFERENCES users(user_id),
    CHECK (vehicle_type IN ('2W', '3W', '4W')),
    CHECK (battery_capacity_kwh > 0),
    CHECK (current_battery_percent >= 0 AND current_battery_percent <= 100),
    CHECK (target_battery_percent >= 0 AND target_battery_percent <= 100)
);

CREATE INDEX idx_vehicles_user_id ON vehicles(user_id);
CREATE INDEX idx_vehicles_number ON vehicles(vehicle_number);

-- ============ CHARGING STATIONS & POINTS ============

CREATE TABLE IF NOT EXISTS charging_stations (
    station_id INTEGER PRIMARY KEY AUTOINCREMENT,
    station_name TEXT NOT NULL,
    location TEXT NOT NULL,
    latitude REAL,
    longitude REAL,
    total_power_capacity_kw REAL NOT NULL,
    available_power_kw REAL NOT NULL,
    operator_name TEXT,
    is_operational BOOLEAN DEFAULT 1,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

CREATE INDEX idx_stations_location ON charging_stations(location);

CREATE TABLE IF NOT EXISTS charging_points (
    point_id INTEGER PRIMARY KEY AUTOINCREMENT,
    station_id INTEGER NOT NULL,
    charger_type TEXT NOT NULL,  -- 2W_STANDARD, 3W_STANDARD, 4W_STANDARD, 4W_FAST
    max_power_kw REAL NOT NULL,
    current_power_kw REAL DEFAULT 0,
    current_status TEXT DEFAULT 'AVAILABLE',  -- AVAILABLE, OCCUPIED, FAULTED, MAINTENANCE
    vehicle_type_supported TEXT NOT NULL,  -- 2W, 3W, 4W
    last_maintenance_date TIMESTAMP,
    creation_date TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    is_active BOOLEAN DEFAULT 1,
    FOREIGN KEY(station_id) REFERENCES charging_stations(station_id),
    CHECK (charger_type IN ('2W_STANDARD', '3W_STANDARD', '4W_STANDARD', '4W_FAST')),
    CHECK (vehicle_type_supported IN ('2W', '3W', '4W')),
    CHECK (current_status IN ('AVAILABLE', 'OCCUPIED', 'FAULTED', 'MAINTENANCE'))
);

CREATE INDEX idx_points_station_id ON charging_points(station_id);
CREATE INDEX idx_points_type ON charging_points(charger_type);

-- ============ CHARGING SESSIONS ============

CREATE TABLE IF NOT EXISTS charging_sessions (
    session_id TEXT PRIMARY KEY,
    vehicle_id INTEGER NOT NULL,
    charging_point_id INTEGER NOT NULL,
    user_id INTEGER NOT NULL,
    start_time TIMESTAMP,
    end_time TIMESTAMP,
    status TEXT DEFAULT 'IDLE',  -- IDLE, CHARGING, PAUSED, COMPLETED, ERROR
    energy_delivered_kwh REAL DEFAULT 0,
    initial_battery_percent REAL NOT NULL,
    target_battery_percent REAL NOT NULL,
    energy_required_kwh REAL NOT NULL,
    estimated_charging_time_minutes INTEGER,
    actual_charging_time_minutes INTEGER,
    total_cost REAL DEFAULT 0,
    charger_efficiency_percent REAL DEFAULT 95,
    is_emergency_session BOOLEAN DEFAULT 0,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY(vehicle_id) REFERENCES vehicles(vehicle_id),
    FOREIGN KEY(charging_point_id) REFERENCES charging_points(point_id),
    FOREIGN KEY(user_id) REFERENCES users(user_id),
    CHECK (status IN ('IDLE', 'CHARGING', 'PAUSED', 'COMPLETED', 'ERROR')),
    CHECK (energy_delivered_kwh >= 0),
    CHECK (initial_battery_percent >= 0 AND initial_battery_percent <= 100)
);

CREATE INDEX idx_sessions_vehicle_id ON charging_sessions(vehicle_id);
CREATE INDEX idx_sessions_user_id ON charging_sessions(user_id);
CREATE INDEX idx_sessions_status ON charging_sessions(status);
CREATE INDEX idx_sessions_start_time ON charging_sessions(start_time);

-- ============ QUEUE MANAGEMENT ============

CREATE TABLE IF NOT EXISTS queue_entries (
    queue_id INTEGER PRIMARY KEY AUTOINCREMENT,
    token_number TEXT NOT NULL UNIQUE,
    user_id INTEGER NOT NULL,
    vehicle_id INTEGER NOT NULL,
    station_id INTEGER NOT NULL,
    desired_charger_type TEXT NOT NULL,
    joined_time TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    queue_position INTEGER,
    status TEXT DEFAULT 'WAITING',  -- WAITING, ASSIGNED, CHARGING, COMPLETED, CANCELLED
    estimated_wait_minutes INTEGER,
    priority_level INTEGER DEFAULT 0,  -- 0=normal, 1=medium, 2=high (emergency)
    is_emergency BOOLEAN DEFAULT 0,
    FOREIGN KEY(user_id) REFERENCES users(user_id),
    FOREIGN KEY(vehicle_id) REFERENCES vehicles(vehicle_id),
    FOREIGN KEY(station_id) REFERENCES charging_stations(station_id),
    CHECK (status IN ('WAITING', 'ASSIGNED', 'CHARGING', 'COMPLETED', 'CANCELLED')),
    CHECK (priority_level IN (0, 1, 2))
);

CREATE INDEX idx_queue_station_id ON queue_entries(station_id);
CREATE INDEX idx_queue_user_id ON queue_entries(user_id);
CREATE INDEX idx_queue_status ON queue_entries(status);
CREATE INDEX idx_queue_token ON queue_entries(token_number);

-- ============ VIRTUAL QUEUE ============

CREATE TABLE IF NOT EXISTS virtual_queue (
    queue_id INTEGER PRIMARY KEY AUTOINCREMENT,
    user_id INTEGER NOT NULL,
    vehicle_id INTEGER NOT NULL,
    station_id INTEGER NOT NULL,
    joined_time TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    estimated_turn_time TIMESTAMP,
    status TEXT DEFAULT 'ONLINE_WAITING',  -- ONLINE_WAITING, NOTIFIED_15MIN, NOTIFIED_ARRIVING, 
                                            -- CONFIRMED_ARRIVAL, EXPIRED, CHARGING, COMPLETED
    arrival_confirmed_time TIMESTAMP,
    expiry_time TIMESTAMP,
    is_physically_present BOOLEAN DEFAULT 0,
    last_position_update TIMESTAMP,
    queue_position INTEGER,
    FOREIGN KEY(user_id) REFERENCES users(user_id),
    FOREIGN KEY(vehicle_id) REFERENCES vehicles(vehicle_id),
    FOREIGN KEY(station_id) REFERENCES charging_stations(station_id),
    CHECK (status IN ('ONLINE_WAITING', 'NOTIFIED_15MIN', 'NOTIFIED_ARRIVING', 
                      'CONFIRMED_ARRIVAL', 'EXPIRED', 'CHARGING', 'COMPLETED'))
);

CREATE INDEX idx_virtual_queue_station ON virtual_queue(station_id);
CREATE INDEX idx_virtual_queue_user ON virtual_queue(user_id);
CREATE INDEX idx_virtual_queue_status ON virtual_queue(status);

-- ============ NOTIFICATIONS ============

CREATE TABLE IF NOT EXISTS notification_log (
    notification_id INTEGER PRIMARY KEY AUTOINCREMENT,
    queue_id INTEGER NOT NULL,
    notification_type TEXT NOT NULL,  -- POSITION_UPDATE, 15MIN_ALERT, 5MIN_ALERT, 
                                       -- ARRIVAL_ALERT, SESSION_COMPLETE, ERROR, PAYMENT_REMINDER
    message TEXT NOT NULL,
    sent_time TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    read_time TIMESTAMP,
    channel TEXT DEFAULT 'PUSH',  -- SMS, EMAIL, PUSH, IN_APP
    recipient_phone TEXT,
    recipient_email TEXT,
    is_sent BOOLEAN DEFAULT 1,
    FOREIGN KEY(queue_id) REFERENCES virtual_queue(queue_id),
    CHECK (channel IN ('SMS', 'EMAIL', 'PUSH', 'IN_APP')),
    CHECK (notification_type IN ('POSITION_UPDATE', '15MIN_ALERT', '5MIN_ALERT', 
                                 'ARRIVAL_ALERT', 'SESSION_COMPLETE', 'ERROR', 'PAYMENT_REMINDER'))
);

CREATE INDEX idx_notification_queue ON notification_log(queue_id);
CREATE INDEX idx_notification_sent_time ON notification_log(sent_time);

-- ============ RESERVATIONS ============

CREATE TABLE IF NOT EXISTS reservations (
    reservation_id TEXT PRIMARY KEY,
    user_id INTEGER NOT NULL,
    vehicle_id INTEGER NOT NULL,
    station_id INTEGER NOT NULL,
    charger_type TEXT NOT NULL,
    reserved_date DATE NOT NULL,
    start_time TIME NOT NULL,
    end_time TIME NOT NULL,
    status TEXT DEFAULT 'CONFIRMED',  -- CONFIRMED, CANCELLED, COMPLETED, EXPIRED
    confirmation_code TEXT UNIQUE,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY(user_id) REFERENCES users(user_id),
    FOREIGN KEY(vehicle_id) REFERENCES vehicles(vehicle_id),
    FOREIGN KEY(station_id) REFERENCES charging_stations(station_id),
    CHECK (status IN ('CONFIRMED', 'CANCELLED', 'COMPLETED', 'EXPIRED'))
);

CREATE INDEX idx_reservations_user ON reservations(user_id);
CREATE INDEX idx_reservations_date ON reservations(reserved_date);

-- ============ PAYMENT & TRANSACTIONS ============

CREATE TABLE IF NOT EXISTS payment_records (
    payment_id TEXT PRIMARY KEY,
    session_id TEXT NOT NULL,
    user_id INTEGER NOT NULL,
    amount REAL NOT NULL,
    currency TEXT DEFAULT 'INR',
    payment_method TEXT NOT NULL,  -- CARD, WALLET, UPI, NETBANKING, PREPAID
    payment_gateway TEXT,  -- Razorpay, PayTM, Stripe
    transaction_id TEXT UNIQUE,
    gateway_response TEXT,
    status TEXT DEFAULT 'PENDING',  -- PENDING, COMPLETED, FAILED, REFUNDED
    initiated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    completed_at TIMESTAMP,
    receipt_url TEXT,
    failure_reason TEXT,
    retry_count INTEGER DEFAULT 0,
    FOREIGN KEY(user_id) REFERENCES users(user_id),
    FOREIGN KEY(session_id) REFERENCES charging_sessions(session_id),
    CHECK (status IN ('PENDING', 'COMPLETED', 'FAILED', 'REFUNDED')),
    CHECK (payment_method IN ('CARD', 'WALLET', 'UPI', 'NETBANKING', 'PREPAID')),
    CHECK (amount > 0)
);

CREATE INDEX idx_payment_user ON payment_records(user_id);
CREATE INDEX idx_payment_session ON payment_records(session_id);
CREATE INDEX idx_payment_status ON payment_records(status);

-- ============ WALLET & PREPAID ============

CREATE TABLE IF NOT EXISTS wallet (
    wallet_id INTEGER PRIMARY KEY AUTOINCREMENT,
    user_id INTEGER NOT NULL UNIQUE,
    balance REAL DEFAULT 0,
    total_credits_added REAL DEFAULT 0,
    total_spent REAL DEFAULT 0,
    last_updated TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY(user_id) REFERENCES users(user_id),
    CHECK (balance >= 0)
);

CREATE INDEX idx_wallet_user ON wallet(user_id);

CREATE TABLE IF NOT EXISTS wallet_transactions (
    transaction_id TEXT PRIMARY KEY,
    wallet_id INTEGER NOT NULL,
    type TEXT NOT NULL,  -- CREDIT, DEBIT
    amount REAL NOT NULL,
    reason TEXT,  -- Charging, Refund, Promotional, Manual
    previous_balance REAL,
    new_balance REAL,
    timestamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY(wallet_id) REFERENCES wallet(wallet_id),
    CHECK (type IN ('CREDIT', 'DEBIT')),
    CHECK (amount > 0)
);

CREATE INDEX idx_wallet_trans_wallet ON wallet_transactions(wallet_id);
CREATE INDEX idx_wallet_trans_time ON wallet_transactions(timestamp);

-- ============ REFUNDS ============

CREATE TABLE IF NOT EXISTS refunds (
    refund_id TEXT PRIMARY KEY,
    payment_id TEXT NOT NULL,
    amount REAL NOT NULL,
    reason TEXT NOT NULL,
    status TEXT DEFAULT 'PENDING',  -- PENDING, APPROVED, REJECTED, COMPLETED
    requested_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    approved_at TIMESTAMP,
    completed_at TIMESTAMP,
    approved_by TEXT,
    notes TEXT,
    FOREIGN KEY(payment_id) REFERENCES payment_records(payment_id),
    CHECK (status IN ('PENDING', 'APPROVED', 'REJECTED', 'COMPLETED')),
    CHECK (amount > 0)
);

CREATE INDEX idx_refund_payment ON refunds(payment_id);
CREATE INDEX idx_refund_status ON refunds(status);

-- ============ PAYMENT METHODS ============

CREATE TABLE IF NOT EXISTS payment_methods (
    method_id INTEGER PRIMARY KEY AUTOINCREMENT,
    user_id INTEGER NOT NULL,
    type TEXT NOT NULL,  -- CARD, UPI, NETBANKING
    method_name TEXT,
    last_four_digits TEXT,
    token TEXT,  -- Tokenized payment details (encrypted)
    is_default BOOLEAN DEFAULT 0,
    is_active BOOLEAN DEFAULT 1,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY(user_id) REFERENCES users(user_id),
    CHECK (type IN ('CARD', 'UPI', 'NETBANKING'))
);

CREATE INDEX idx_payment_methods_user ON payment_methods(user_id);

-- ============ CHARGING HISTORY & ANALYTICS ============

CREATE TABLE IF NOT EXISTS charging_history (
    history_id INTEGER PRIMARY KEY AUTOINCREMENT,
    session_id TEXT NOT NULL,
    vehicle_id INTEGER NOT NULL,
    user_id INTEGER NOT NULL,
    charging_point_id INTEGER NOT NULL,
    station_id INTEGER NOT NULL,
    charger_type TEXT NOT NULL,
    start_time TIMESTAMP NOT NULL,
    end_time TIMESTAMP NOT NULL,
    duration_minutes INTEGER NOT NULL,
    energy_consumed_kwh REAL NOT NULL,
    cost REAL NOT NULL,
    payment_status TEXT DEFAULT 'COMPLETED',
    rating INTEGER,  -- 1-5 stars
    review TEXT,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY(session_id) REFERENCES charging_sessions(session_id),
    FOREIGN KEY(vehicle_id) REFERENCES vehicles(vehicle_id),
    FOREIGN KEY(user_id) REFERENCES users(user_id),
    FOREIGN KEY(charging_point_id) REFERENCES charging_points(point_id),
    FOREIGN KEY(station_id) REFERENCES charging_stations(station_id)
);

CREATE INDEX idx_history_user ON charging_history(user_id);
CREATE INDEX idx_history_vehicle ON charging_history(vehicle_id);
CREATE INDEX idx_history_date ON charging_history(start_time);

-- ============ DAILY STATISTICS ============

CREATE TABLE IF NOT EXISTS daily_statistics (
    stat_id INTEGER PRIMARY KEY AUTOINCREMENT,
    station_id INTEGER NOT NULL,
    stat_date DATE NOT NULL,
    vehicles_charged INTEGER DEFAULT 0,
    vehicles_waiting INTEGER DEFAULT 0,
    currently_charging INTEGER DEFAULT 0,
    total_energy_consumed_kwh REAL DEFAULT 0,
    total_revenue REAL DEFAULT 0,
    available_chargers INTEGER DEFAULT 0,
    peak_hour TIME,
    average_charging_time_minutes REAL DEFAULT 0,
    charger_utilization_percent REAL DEFAULT 0,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY(station_id) REFERENCES charging_stations(station_id),
    UNIQUE(station_id, stat_date)
);

CREATE INDEX idx_stats_station ON daily_statistics(station_id);
CREATE INDEX idx_stats_date ON daily_statistics(stat_date);

-- ============ PRICING & CONFIGURATION ============

CREATE TABLE IF NOT EXISTS pricing_config (
    config_id INTEGER PRIMARY KEY AUTOINCREMENT,
    station_id INTEGER NOT NULL,
    charger_type TEXT NOT NULL,
    rate_per_kwh REAL NOT NULL,
    time_based_rate_per_minute REAL,
    parking_fee_per_minute REAL,
    base_fee REAL DEFAULT 0,
    peak_hours_start TIME,
    peak_hours_end TIME,
    peak_rate_multiplier REAL DEFAULT 1.5,
    discount_percent REAL DEFAULT 0,
    effective_from DATE,
    effective_to DATE,
    is_active BOOLEAN DEFAULT 1,
    FOREIGN KEY(station_id) REFERENCES charging_stations(station_id),
    CHECK (rate_per_kwh >= 0),
    CHECK (base_fee >= 0)
);

CREATE INDEX idx_pricing_station ON pricing_config(station_id);

-- ============ AUDIT LOG ============

CREATE TABLE IF NOT EXISTS audit_log (
    log_id INTEGER PRIMARY KEY AUTOINCREMENT,
    user_id INTEGER,
    action TEXT NOT NULL,
    entity_type TEXT NOT NULL,
    entity_id TEXT NOT NULL,
    old_value TEXT,
    new_value TEXT,
    timestamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    ip_address TEXT,
    FOREIGN KEY(user_id) REFERENCES users(user_id)
);

CREATE INDEX idx_audit_user ON audit_log(user_id);
CREATE INDEX idx_audit_timestamp ON audit_log(timestamp);
CREATE INDEX idx_audit_entity ON audit_log(entity_type, entity_id);
