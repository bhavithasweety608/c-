#include "DatabaseManager.h"
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <fstream>
#include <sstream>
#include <iostream>

namespace evc::database {

DatabaseManager* DatabaseManager::instance = nullptr;

DatabaseManager::DatabaseManager() : db(nullptr), isConnected(false) {}

DatabaseManager* DatabaseManager::getInstance() {
    if (instance == nullptr) {
        instance = new DatabaseManager();
    }
    return instance;
}

void DatabaseManager::destroyInstance() {
    if (instance != nullptr) {
        instance->disconnect();
        delete instance;
        instance = nullptr;
    }
}

bool DatabaseManager::connect(const std::string& path) {
    if (isConnected) {
        auto logger = spdlog::stdout_color_mt("db");
        logger->warn("Database already connected");
        return true;
    }

    dbPath = path;
    int rc = sqlite3_open(path.c_str(), &db);

    if (rc != SQLITE_OK) {
        auto logger = spdlog::stdout_color_mt("db");
        logger->error("Failed to open database: {}", sqlite3_errmsg(db));
        return false;
    }

    isConnected = true;
    auto logger = spdlog::stdout_color_mt("db");
    logger->info("Database connected successfully: {}", path);
    return true;
}

bool DatabaseManager::disconnect() {
    if (!isConnected || db == nullptr) {
        return false;
    }

    int rc = sqlite3_close(db);
    if (rc != SQLITE_OK) {
        auto logger = spdlog::stdout_color_mt("db");
        logger->error("Failed to close database: {}", sqlite3_errmsg(db));
        return false;
    }

    db = nullptr;
    isConnected = false;
    auto logger = spdlog::stdout_color_mt("db");
    logger->info("Database disconnected");
    return true;
}

bool DatabaseManager::executeUpdate(const std::string& sql) {
    if (!isConnected || db == nullptr) {
        auto logger = spdlog::stdout_color_mt("db");
        logger->error("Database not connected");
        return false;
    }

    char* errMsg = nullptr;
    int rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg);

    if (rc != SQLITE_OK) {
        auto logger = spdlog::stdout_color_mt("db");
        logger->error("SQL Error: {}", errMsg);
        sqlite3_free(errMsg);
        return false;
    }

    return true;
}

bool DatabaseManager::executeUpdateWithParams(const std::string& sql, const std::vector<std::string>& params) {
    if (!isConnected || db == nullptr) {
        return false;
    }

    sqlite3_stmt* stmt = nullptr;
    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);

    if (rc != SQLITE_OK) {
        auto logger = spdlog::stdout_color_mt("db");
        logger->error("Failed to prepare statement: {}", sqlite3_errmsg(db));
        return false;
    }

    // Bind parameters
    for (size_t i = 0; i < params.size(); i++) {
        sqlite3_bind_text(stmt, i + 1, params[i].c_str(), -1, SQLITE_STATIC);
    }

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    return rc == SQLITE_DONE;
}

sqlite3_stmt* DatabaseManager::prepareStatement(const std::string& sql) {
    if (!isConnected || db == nullptr) {
        return nullptr;
    }

    sqlite3_stmt* stmt = nullptr;
    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);

    if (rc != SQLITE_OK) {
        auto logger = spdlog::stdout_color_mt("db");
        logger->error("Failed to prepare statement: {}", sqlite3_errmsg(db));
        return nullptr;
    }

    return stmt;
}

bool DatabaseManager::executeQuery(const std::string& sql, std::vector<std::map<std::string, std::string>>& results) {
    if (!isConnected || db == nullptr) {
        return false;
    }

    sqlite3_stmt* stmt = prepareStatement(sql);
    if (stmt == nullptr) {
        return false;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::map<std::string, std::string> row;
        int columnCount = sqlite3_column_count(stmt);

        for (int i = 0; i < columnCount; i++) {
            const char* colName = sqlite3_column_name(stmt, i);
            const char* colValue = reinterpret_cast<const char*>(sqlite3_column_text(stmt, i));
            row[colName] = colValue ? colValue : "";
        }

        results.push_back(row);
    }

    sqlite3_finalize(stmt);
    return true;
}

int DatabaseManager::getLastInsertRowId() const {
    return static_cast<int>(sqlite3_last_insert_rowid(db));
}

int DatabaseManager::getChangesCount() const {
    return sqlite3_changes(db);
}

bool DatabaseManager::beginTransaction() {
    return executeUpdate("BEGIN TRANSACTION;");
}

bool DatabaseManager::commitTransaction() {
    return executeUpdate("COMMIT;");
}

bool DatabaseManager::rollbackTransaction() {
    return executeUpdate("ROLLBACK;");
}

bool DatabaseManager::initializeDatabase() {
    if (!createTables()) {
        auto logger = spdlog::stdout_color_mt("db");
        logger->error("Failed to create tables");
        return false;
    }
    return true;
}

bool DatabaseManager::createTables() {
    // Read schema from file
    std::ifstream schemaFile("database/schema.sql");
    if (!schemaFile.is_open()) {
        auto logger = spdlog::stdout_color_mt("db");
        logger->error("Failed to open schema.sql");
        return false;
    }

    std::stringstream buffer;
    buffer << schemaFile.rdbuf();
    std::string schema = buffer.str();
    schemaFile.close();

    // Split by semicolon and execute each statement
    std::stringstream ss(schema);
    std::string statement;

    while (std::getline(ss, statement, ';')) {
        // Trim whitespace
        statement.erase(0, statement.find_first_not_of(" \n\r\t"));
        statement.erase(statement.find_last_not_of(" \n\r\t") + 1);

        if (!statement.empty() && statement.substr(0, 2) != "--") {
            if (!executeUpdate(statement + ";")) {
                auto logger = spdlog::stdout_color_mt("db");
                logger->error("Failed to execute schema statement: {}", statement);
                return false;
            }
        }
    }

    auto logger = spdlog::stdout_color_mt("db");
    logger->info("Database tables created successfully");
    return true;
}

bool DatabaseManager::dropAllTables() {
    std::vector<std::string> tables = {
        "users", "vehicles", "charging_stations", "charging_points",
        "charging_sessions", "queue_entries", "virtual_queue",
        "notification_log", "reservations", "payment_records",
        "wallet", "wallet_transactions", "refunds", "payment_methods",
        "charging_history", "daily_statistics", "pricing_config", "audit_log"
    };

    for (const auto& table : tables) {
        std::string sql = "DROP TABLE IF EXISTS " + table + ";"
        if (!executeUpdate(sql)) {
            auto logger = spdlog::stdout_color_mt("db");
            logger->error("Failed to drop table: {}", table);
            return false;
        }
    }

    auto logger = spdlog::stdout_color_mt("db");
    logger->info("All tables dropped");
    return true;
}

std::string DatabaseManager::getLastError() const {
    if (db == nullptr) {
        return "Database not connected";
    }
    return std::string(sqlite3_errmsg(db));
}

int DatabaseManager::getLastErrorCode() const {
    if (db == nullptr) {
        return -1;
    }
    return sqlite3_extended_errcode(db);
}

DatabaseManager::~DatabaseManager() {
    if (isConnected) {
        disconnect();
    }
}

}
