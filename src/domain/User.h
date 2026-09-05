#ifndef USER_H
#define USER_H

#include <string>
#include <ctime>

namespace evc::domain {

enum class UserRole {
    ADMIN,
    USER,
    TECHNICIAN,
    OPERATOR
};

class User {
private:
    int userId;
    std::string username;
    std::string email;
    std::string passwordHash;
    std::string phoneNumber;
    UserRole role;
    std::string firstName;
    std::string lastName;
    bool isActive;
    time_t createdAt;
    time_t updatedAt;

public:
    // Constructor
    User();
    User(int id, const std::string& uname, const std::string& em, 
         const std::string& hash, UserRole r);

    // Getters
    int getUserId() const { return userId; }
    const std::string& getUsername() const { return username; }
    const std::string& getEmail() const { return email; }
    const std::string& getPasswordHash() const { return passwordHash; }
    const std::string& getPhoneNumber() const { return phoneNumber; }
    UserRole getRole() const { return role; }
    const std::string& getFirstName() const { return firstName; }
    const std::string& getLastName() const { return lastName; }
    bool getIsActive() const { return isActive; }
    time_t getCreatedAt() const { return createdAt; }
    time_t getUpdatedAt() const { return updatedAt; }

    // Setters
    void setUserId(int id) { userId = id; }
    void setUsername(const std::string& uname) { username = uname; }
    void setEmail(const std::string& em) { email = em; }
    void setPasswordHash(const std::string& hash) { passwordHash = hash; }
    void setPhoneNumber(const std::string& phone) { phoneNumber = phone; }
    void setRole(UserRole r) { role = r; }
    void setFirstName(const std::string& first) { firstName = first; }
    void setLastName(const std::string& last) { lastName = last; }
    void setIsActive(bool active) { isActive = active; }
    void setUpdatedAt(time_t time) { updatedAt = time; }

    // Utility
    std::string getFullName() const { return firstName + " " + lastName; }
    std::string getRoleString() const;
};

}

#endif // USER_H