#ifndef USER_REPOSITORY_H
#define USER_REPOSITORY_H

#include "../domain/User.h"
#include <vector>
#include <memory>
#include <string>

namespace evc::repository {

class UserRepository {
public:
    virtual ~UserRepository() = default;

    // Create
    virtual bool createUser(const evc::domain::User& user) = 0;

    // Read
    virtual std::shared_ptr<evc::domain::User> getUserById(int userId) = 0;
    virtual std::shared_ptr<evc::domain::User> getUserByEmail(const std::string& email) = 0;
    virtual std::shared_ptr<evc::domain::User> getUserByUsername(const std::string& username) = 0;
    virtual std::vector<std::shared_ptr<evc::domain::User>> getAllUsers() = 0;
    virtual std::vector<std::shared_ptr<evc::domain::User>> getUsersByRole(evc::domain::UserRole role) = 0;

    // Update
    virtual bool updateUser(const evc::domain::User& user) = 0;
    virtual bool updateUserPassword(int userId, const std::string& newPasswordHash) = 0;
    virtual bool updateUserRole(int userId, evc::domain::UserRole newRole) = 0;

    // Delete
    virtual bool deleteUser(int userId) = 0;
    virtual bool deactivateUser(int userId) = 0;

    // Check
    virtual bool userExists(int userId) = 0;
    virtual bool emailExists(const std::string& email) = 0;
    virtual bool usernameExists(const std::string& username) = 0;
};

class SQLiteUserRepository : public UserRepository {
public:
    bool createUser(const evc::domain::User& user) override;
    std::shared_ptr<evc::domain::User> getUserById(int userId) override;
    std::shared_ptr<evc::domain::User> getUserByEmail(const std::string& email) override;
    std::shared_ptr<evc::domain::User> getUserByUsername(const std::string& username) override;
    std::vector<std::shared_ptr<evc::domain::User>> getAllUsers() override;
    std::vector<std::shared_ptr<evc::domain::User>> getUsersByRole(evc::domain::UserRole role) override;
    bool updateUser(const evc::domain::User& user) override;
    bool updateUserPassword(int userId, const std::string& newPasswordHash) override;
    bool updateUserRole(int userId, evc::domain::UserRole newRole) override;
    bool deleteUser(int userId) override;
    bool deactivateUser(int userId) override;
    bool userExists(int userId) override;
    bool emailExists(const std::string& email) override;
    bool usernameExists(const std::string& username) override;
};

}

#endif // USER_REPOSITORY_H