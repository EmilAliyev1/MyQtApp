#pragma once
#include <vector>
#include "User.h"
#include <vector>


class DataBase
{
private:
public:
    static int id;
    static void initializeId();
    static std::vector<User> users;
    static void addUser(const User& user);
    static bool saveUsers();
    static std::vector<User> GetAllUsers();
};
