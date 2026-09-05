cmake_minimum_required(VERSION 3.20)
project(EVChargingStation VERSION 1.0.0 LANGUAGES CXX)

# Set C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Find required packages
find_package(SQLite3 REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(spdlog REQUIRED)
find_package(curl REQUIRED)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/src)

# Source files
set(SOURCES
    src/main.cpp
    src/database/DatabaseManager.cpp
    src/domain/User.cpp
    src/domain/Vehicle.cpp
    src/domain/ChargingPoint.cpp
    src/domain/ChargingSession.cpp
    src/domain/QueueEntry.cpp
    src/domain/VirtualQueueEntry.cpp
    src/domain/Reservation.cpp
    src/domain/PaymentRecord.cpp
    src/domain/Wallet.cpp
    src/repository/UserRepository.cpp
    src/repository/VehicleRepository.cpp
    src/repository/ChargerRepository.cpp
    src/repository/SessionRepository.cpp
    src/repository/QueueRepository.cpp
    src/repository/VirtualQueueRepository.cpp
    src/repository/PaymentRepository.cpp
    src/repository/WalletRepository.cpp
    src/repository/ReservationRepository.cpp
    src/services/UserService.cpp
    src/services/SessionService.cpp
    src/services/QueueManager.cpp
    src/services/VirtualQueueManager.cpp
    src/services/PowerManager.cpp
    src/services/PaymentService.cpp
    src/services/WalletService.cpp
    src/services/NotificationService.cpp
    src/services/ReservationService.cpp
    src/services/AnalyticsService.cpp
    src/services/ChargingController.cpp
    src/external/PaymentGateway.cpp
    src/external/RazorpayAdapter.cpp
    src/external/NotificationProvider.cpp
    src/external/GeoLocationService.cpp
    src/utils/Logger.cpp
    src/utils/Config.cpp
    src/utils/Validators.cpp
    src/utils/TokenGenerator.cpp
    src/utils/ReceiptGenerator.cpp
    src/utils/DateTimeUtils.cpp
)

# Create main executable
add_executable(ev_charging_station ${SOURCES})

# Link libraries
target_link_libraries(ev_charging_station
    PRIVATE
    SQLite::SQLite3
    nlohmann_json::nlohmann_json
    spdlog::spdlog
    CURL::libcurl
)

# Enable testing
enable_testing()

# Add subdirectories for tests
add_subdirectory(tests)

# Print configuration
message(STATUS "EV Charging Station Backend Configuration:")
message(STATUS "  C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "  Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  Binary Directory: ${CMAKE_BINARY_DIR}")
