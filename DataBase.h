#pragma once
#include <vector>
#include "User.h"
#include <vector>


class DataBase
{
private:
public:
    static int id;
    static std::vector<User> users;
    static void addUser(const User& user);
    static bool saveUsers(std::vector<User>& users);
    static std::vector<User> GetAllUsers();
};
