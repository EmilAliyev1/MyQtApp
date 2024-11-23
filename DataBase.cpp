#include "DataBase.h"
#include <iostream>
#include <fstream>
#include <vector>

int DataBase::id = 1;

void DataBase::initializeId() {
    if (!users.empty()) {
        id = users.back().getId() + 1; // Use users.back() to access the last element
    } else {
        id = 1;
    }
}

std::vector<User> DataBase::users = GetAllUsers();;

void DataBase::addUser(const User& user) {
    users.push_back(user);
}

bool DataBase::saveUsers() {
    try {
        std::ofstream ofs{ "/home/dingo21/CLionProjects/Checking/Data.bin", std::ios::binary | std::ios::out | std::ios::trunc };
        if (!ofs.is_open())
            throw std::runtime_error("Failed to open file.");

        for (const auto& user : users)
        {
            int id = user.getId();
            std::string companyName = user.getCompanyName();
            std::string email = user.getEmail();
            std::string password = user.getPassword();


            ofs.write(reinterpret_cast<const char*>(&id), sizeof(id));


            size_t companyNameSize = companyName.size();
            ofs.write(reinterpret_cast<const char*>(&companyNameSize), sizeof(companyNameSize));
            ofs.write(companyName.c_str(), companyNameSize);

            size_t emailSize = email.size();
            ofs.write(reinterpret_cast<const char*>(&emailSize), sizeof(emailSize));
            ofs.write(email.c_str(), emailSize);

            size_t passwordSize = password.size();
            ofs.write(reinterpret_cast<const char*>(&passwordSize), sizeof(passwordSize));
            ofs.write(password.c_str(), passwordSize);

            size_t transactionCount = user.getTransactions().size();
            ofs.write(reinterpret_cast<const char*>(&transactionCount), sizeof(transactionCount));

            for (const auto& transaction : user.getTransactions()) {
                size_t categorySize = transaction.getCategory().size();
                size_t notesSize = transaction.getNotes().size();
                size_t currencySize = transaction.getCurrency().size();
                size_t dateSize = transaction.getDate().size();

                double amount = transaction.getAmount();

                ofs.write(reinterpret_cast<const char*>(&amount), sizeof(amount));
                ofs.write(reinterpret_cast<const char*>(&categorySize), sizeof(categorySize));
                ofs.write(transaction.getCategory().c_str(), categorySize);

                ofs.write(reinterpret_cast<const char*>(&notesSize), sizeof(notesSize));
                ofs.write(transaction.getNotes().c_str(), notesSize);

                ofs.write(reinterpret_cast<const char*>(&currencySize), sizeof(currencySize));
                ofs.write(transaction.getCurrency().c_str(), currencySize);

                ofs.write(reinterpret_cast<const char*>(&dateSize), sizeof(dateSize));
                ofs.write(transaction.getDate().c_str(), dateSize);
            }
        }

        ofs.close();
    }
    catch (...) {
        return false;
    }

    return true;
}




std::vector<User> DataBase::GetAllUsers() {
    std::ifstream ifs("/home/dingo21/CLionProjects/Checking/Data.bin", std::ios::binary | std::ios::in);
    if (!ifs.is_open()) {
        std::cerr << "Failed to open file for reading!" << std::endl;
        return {};
    }

    std::vector<User> users;

    while (ifs.peek() != EOF) {
        User user;

        int id;
        if (!ifs.read(reinterpret_cast<char*>(&id), sizeof(id))) break;
        user.setId(id);

        size_t companyNameSize;
        if (!ifs.read(reinterpret_cast<char*>(&companyNameSize), sizeof(companyNameSize))) break;
        std::string companyName(companyNameSize, '\0');
        if (!ifs.read(&companyName[0], companyNameSize)) break;
        user.setCompanyName(companyName);

        size_t emailSize;
        if (!ifs.read(reinterpret_cast<char*>(&emailSize), sizeof(emailSize))) break;
        std::string email(emailSize, '\0');
        if (!ifs.read(&email[0], emailSize)) break;
        user.setEmail(email);

        size_t passwordSize;
        if (!ifs.read(reinterpret_cast<char*>(&passwordSize), sizeof(passwordSize))) break;
        std::string password(passwordSize, '\0');
        if (!ifs.read(&password[0], passwordSize)) break;
        user.setPassword(password);

        size_t transactionCount;
        if (!ifs.read(reinterpret_cast<char*>(&transactionCount), sizeof(transactionCount))) break;

        for (size_t i = 0; i < transactionCount; ++i) {
            double amount;
            size_t categorySize, notesSize, currencySize, dateSize;

            if (!ifs.read(reinterpret_cast<char*>(&amount), sizeof(amount))) break;

            if (!ifs.read(reinterpret_cast<char*>(&categorySize), sizeof(categorySize))) break;
            std::string category(categorySize, '\0');
            if (!ifs.read(&category[0], categorySize)) break;

            if (!ifs.read(reinterpret_cast<char*>(&notesSize), sizeof(notesSize))) break;
            std::string notes(notesSize, '\0');
            if (!ifs.read(&notes[0], notesSize)) break;

            if (!ifs.read(reinterpret_cast<char*>(&currencySize), sizeof(currencySize))) break;
            std::string currency(currencySize, '\0');
            if (!ifs.read(&currency[0], currencySize)) break;

            if (!ifs.read(reinterpret_cast<char*>(&dateSize), sizeof(dateSize))) break;
            std::string date(dateSize, '\0');
            if (!ifs.read(&date[0], dateSize)) break;

            user.addTransaction(amount, category, date, notes, currency);
        }

        users.push_back(user);
    }

    return users;
}