#include "User.h"
#include <ctime>

namespace evc::domain {

User::User() : userId(0), role(UserRole::USER), isActive(true) {
    createdAt = time(nullptr);
    updatedAt = time(nullptr);
}

User::User(int id, const std::string& uname, const std::string& em, 
           const std::string& hash, UserRole r)
    : userId(id), username(uname), email(em), passwordHash(hash), role(r), isActive(true) {
    createdAt = time(nullptr);
    updatedAt = time(nullptr);
}

std::string User::getRoleString() const {
    switch (role) {
        case UserRole::ADMIN:
            return "ADMIN";
        case UserRole::USER:
            return "USER";
        case UserRole::TECHNICIAN:
            return "TECHNICIAN";
        case UserRole::OPERATOR:
            return "OPERATOR";
        default:
            return "UNKNOWN";
    }
}

}
