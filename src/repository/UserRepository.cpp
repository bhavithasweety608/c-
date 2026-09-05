#include "UserRepository.h"
#include "../database/DatabaseManager.h"
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace evc::repository {

bool SQLiteUserRepository::createUser(const evc::domain::User& user) {
    auto db = evc::database::DatabaseManager::getInstance();
    if (!db->isDBConnected()) return false;

    std::string sql = "INSERT INTO users (username, email, password_hash, phone_number, role, first_name, last_name, is_active) "
                      "VALUES (?, ?, ?, ?, ?, ?, ?, ?)";
    std::vector<std::string> params = {
        user.getUsername(),
        user.getEmail(),
        user.getPasswordHash(),
        user.getPhoneNumber(),
        user.getRoleString(),
        user.getFirstName(),
        user.getLastName(),
        user.getIsActive() ? "1" : "0"
    };

    return db->executeUpdateWithParams(sql, params);
}

std::shared_ptr<evc::domain::User> SQLiteUserRepository::getUserById(int userId) {
    auto db = evc::database::DatabaseManager::getInstance();
    if (!db->isDBConnected()) return nullptr;

    std::string sql = "SELECT * FROM users WHERE user_id = " + std::to_string(userId);
    std::vector<std::map<std::string, std::string>> results;
    
    if (!db->executeQuery(sql, results) || results.empty()) {
        return nullptr;
    }

    auto user = std::make_shared<evc::domain::User>();
    const auto& row = results[0];
    
    user->setUserId(userId);
    user->setUsername(row.at("username"));
    user->setEmail(row.at("email"));
    user->setPhoneNumber(row.at("phone_number"));
    user->setFirstName(row.at("first_name"));
    user->setLastName(row.at("last_name"));
    user->setIsActive(row.at("is_active") == "1");

    return user;
}

std::shared_ptr<evc::domain::User> SQLiteUserRepository::getUserByEmail(const std::string& email) {
    auto db = evc::database::DatabaseManager::getInstance();
    if (!db->isDBConnected()) return nullptr;

    std::string sql = "SELECT * FROM users WHERE email = '" + email + "'";
    std::vector<std::map<std::string, std::string>> results;
    
    if (!db->executeQuery(sql, results) || results.empty()) {
        return nullptr;
    }

    auto user = std::make_shared<evc::domain::User>();
    const auto& row = results[0];
    
    user->setUserId(std::stoi(row.at("user_id")));
    user->setUsername(row.at("username"));
    user->setEmail(row.at("email"));
    user->setPhoneNumber(row.at("phone_number"));
    user->setFirstName(row.at("first_name"));
    user->setLastName(row.at("last_name"));
    user->setIsActive(row.at("is_active") == "1");

    return user;
}

std::shared_ptr<evc::domain::User> SQLiteUserRepository::getUserByUsername(const std::string& username) {
    auto db = evc::database::DatabaseManager::getInstance();
    if (!db->isDBConnected()) return nullptr;

    std::string sql = "SELECT * FROM users WHERE username = '" + username + "'";
    std::vector<std::map<std::string, std::string>> results;
    
    if (!db->executeQuery(sql, results) || results.empty()) {
        return nullptr;
    }

    auto user = std::make_shared<evc::domain::User>();
    const auto& row = results[0];
    
    user->setUserId(std::stoi(row.at("user_id")));
    user->setUsername(row.at("username"));
    user->setEmail(row.at("email"));
    user->setPhoneNumber(row.at("phone_number"));
    user->setFirstName(row.at("first_name"));
    user->setLastName(row.at("last_name"));
    user->setIsActive(row.at("is_active") == "1");

    return user;
}

std::vector<std::shared_ptr<evc::domain::User>> SQLiteUserRepository::getAllUsers() {
    auto db = evc::database::DatabaseManager::getInstance();
    std::vector<std::shared_ptr<evc::domain::User>> users;
    
    if (!db->isDBConnected()) return users;

    std::string sql = "SELECT * FROM users";
    std::vector<std::map<std::string, std::string>> results;
    
    if (!db->executeQuery(sql, results)) {
        return users;
    }

    for (const auto& row : results) {
        auto user = std::make_shared<evc::domain::User>();
        user->setUserId(std::stoi(row.at("user_id")));
        user->setUsername(row.at("username"));
        user->setEmail(row.at("email"));
        user->setPhoneNumber(row.at("phone_number"));
        user->setFirstName(row.at("first_name"));
        user->setLastName(row.at("last_name"));
        user->setIsActive(row.at("is_active") == "1");
        users.push_back(user);
    }

    return users;
}

std::vector<std::shared_ptr<evc::domain::User>> SQLiteUserRepository::getUsersByRole(evc::domain::UserRole role) {
    auto db = evc::database::DatabaseManager::getInstance();
    std::vector<std::shared_ptr<evc::domain::User>> users;
    
    if (!db->isDBConnected()) return users;

    // Convert role to string
    std::string roleStr;
    switch (role) {
        case evc::domain::UserRole::ADMIN: roleStr = "ADMIN"; break;
        case evc::domain::UserRole::USER: roleStr = "USER"; break;
        case evc::domain::UserRole::TECHNICIAN: roleStr = "TECHNICIAN"; break;
        case evc::domain::UserRole::OPERATOR: roleStr = "OPERATOR"; break;
    }

    std::string sql = "SELECT * FROM users WHERE role = '" + roleStr + "'";
    std::vector<std::map<std::string, std::string>> results;
    
    if (!db->executeQuery(sql, results)) {
        return users;
    }

    for (const auto& row : results) {
        auto user = std::make_shared<evc::domain::User>();
        user->setUserId(std::stoi(row.at("user_id")));
        user->setUsername(row.at("username"));
        user->setEmail(row.at("email"));
        user->setPhoneNumber(row.at("phone_number"));
        user->setFirstName(row.at("first_name"));
        user->setLastName(row.at("last_name"));
        user->setIsActive(row.at("is_active") == "1");
        users.push_back(user);
    }

    return users;
}

bool SQLiteUserRepository::updateUser(const evc::domain::User& user) {
    auto db = evc::database::DatabaseManager::getInstance();
    if (!db->isDBConnected()) return false;

    std::string sql = "UPDATE users SET username = ?, email = ?, phone_number = ?, first_name = ?, last_name = ?, is_active = ? WHERE user_id = ?";
    std::vector<std::string> params = {
        user.getUsername(),
        user.getEmail(),
        user.getPhoneNumber(),
        user.getFirstName(),
        user.getLastName(),
        user.getIsActive() ? "1" : "0",
        std::to_string(user.getUserId())
    };

    return db->executeUpdateWithParams(sql, params);
}

bool SQLiteUserRepository::updateUserPassword(int userId, const std::string& newPasswordHash) {
    auto db = evc::database::DatabaseManager::getInstance();
    if (!db->isDBConnected()) return false;

    std::string sql = "UPDATE users SET password_hash = ? WHERE user_id = ?";
    std::vector<std::string> params = {newPasswordHash, std::to_string(userId)};

    return db->executeUpdateWithParams(sql, params);
}

bool SQLiteUserRepository::updateUserRole(int userId, evc::domain::UserRole newRole) {
    auto db = evc::database::DatabaseManager::getInstance();
    if (!db->isDBConnected()) return false;

    std::string roleStr;
    switch (newRole) {
        case evc::domain::UserRole::ADMIN: roleStr = "ADMIN"; break;
        case evc::domain::UserRole::USER: roleStr = "USER"; break;
        case evc::domain::UserRole::TECHNICIAN: roleStr = "TECHNICIAN"; break;
        case evc::domain::UserRole::OPERATOR: roleStr = "OPERATOR"; break;
    }

    std::string sql = "UPDATE users SET role = ? WHERE user_id = ?";
    std::vector<std::string> params = {roleStr, std::to_string(userId)};

    return db->executeUpdateWithParams(sql, params);
}

bool SQLiteUserRepository::deleteUser(int userId) {
    auto db = evc::database::DatabaseManager::getInstance();
    if (!db->isDBConnected()) return false;

    std::string sql = "DELETE FROM users WHERE user_id = " + std::to_string(userId);
    return db->executeUpdate(sql);
}

bool SQLiteUserRepository::deactivateUser(int userId) {
    auto db = evc::database::DatabaseManager::getInstance();
    if (!db->isDBConnected()) return false;

    std::string sql = "UPDATE users SET is_active = 0 WHERE user_id = " + std::to_string(userId);
    return db->executeUpdate(sql);
}

bool SQLiteUserRepository::userExists(int userId) {
    auto db = evc::database::DatabaseManager::getInstance();
    if (!db->isDBConnected()) return false;

    std::string sql = "SELECT COUNT(*) as count FROM users WHERE user_id = " + std::to_string(userId);
    std::vector<std::map<std::string, std::string>> results;
    
    if (!db->executeQuery(sql, results)) {
        return false;
    }

    return std::stoi(results[0].at("count")) > 0;
}

bool SQLiteUserRepository::emailExists(const std::string& email) {
    auto db = evc::database::DatabaseManager::getInstance();
    if (!db->isDBConnected()) return false;

    std::string sql = "SELECT COUNT(*) as count FROM users WHERE email = '" + email + "'";
    std::vector<std::map<std::string, std::string>> results;
    
    if (!db->executeQuery(sql, results)) {
        return false;
    }

    return std::stoi(results[0].at("count")) > 0;
}

bool SQLiteUserRepository::usernameExists(const std::string& username) {
    auto db = evc::database::DatabaseManager::getInstance();
    if (!db->isDBConnected()) return false;

    std::string sql = "SELECT COUNT(*) as count FROM users WHERE username = '" + username + "'";
    std::vector<std::map<std::string, std::string>> results;
    
    if (!db->executeQuery(sql, results)) {
        return false;
    }

    return std::stoi(results[0].at("count")) > 0;
}

}
