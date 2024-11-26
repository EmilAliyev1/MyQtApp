#pragma once

#include <vector>
#include <memory>
#include "User.h"

class DataBase {
public:
    static int id; // Auto-increment ID for users
    static void initializeId();
    static std::vector<std::shared_ptr<User>> users;

    static void addUser(const std::shared_ptr<User>& user);
    static bool saveUsers();
    static std::vector<std::shared_ptr<User>> GetAllUsers();
};