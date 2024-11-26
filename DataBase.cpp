#include "DataBase.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <memory>

int DataBase::id = 1;

void DataBase::initializeId() {
    if (!users.empty()) {
        id = users.back()->getId() + 1;
    } else {
        id = 1;
    }
}

std::vector<std::shared_ptr<User>> DataBase::users = GetAllUsers();

void DataBase::addUser(const std::shared_ptr<User>& user) {
    users.push_back(user);
}

bool DataBase::saveUsers() {
    try {
        std::ofstream ofs{"/home/dingo21/CLionProjects/Checking/Data.bin", std::ios::binary | std::ios::out | std::ios::trunc};
        if (!ofs.is_open()) {
            throw std::runtime_error("Failed to open file.");
        }

        for (const auto& user : users) {
            if (!user) {
                std::cerr << "Null user pointer encountered. Skipping." << std::endl;
                continue;
            }

            if (user->getEmail().empty() || user->getCompanyName().empty()) {
                std::cerr << "User ID: " << user->getId() << " has invalid data. Skipping." << std::endl;
                continue;
            }

            // Save user ID
            int id = user->getId();
            ofs.write(reinterpret_cast<const char*>(&id), sizeof(id));

            // Save companyName
            const std::string& companyName = user->getCompanyName();
            size_t companyNameSize = companyName.size();
            ofs.write(reinterpret_cast<const char*>(&companyNameSize), sizeof(companyNameSize));
            ofs.write(companyName.c_str(), companyNameSize);

            // Save email
            const std::string& email = user->getEmail();
            size_t emailSize = email.size();
            ofs.write(reinterpret_cast<const char*>(&emailSize), sizeof(emailSize));
            ofs.write(email.c_str(), emailSize);

            // Save password
            const std::string& password = user->getPassword();
            size_t passwordSize = password.size();
            ofs.write(reinterpret_cast<const char*>(&passwordSize), sizeof(passwordSize));
            ofs.write(password.c_str(), passwordSize);

            // Save transactions
            const auto& transactions = user->getTransactions();
            size_t transactionCount = transactions.size();
            ofs.write(reinterpret_cast<const char*>(&transactionCount), sizeof(transactionCount));

            for (const auto& transaction : transactions) {
                double amount = transaction->getAmount();
                ofs.write(reinterpret_cast<const char*>(&amount), sizeof(amount));

                const std::string& category = transaction->getCategory();
                size_t categorySize = category.size();
                ofs.write(reinterpret_cast<const char*>(&categorySize), sizeof(categorySize));
                ofs.write(category.c_str(), categorySize);

                const std::string& notes = transaction->getNotes();
                size_t notesSize = notes.size();
                ofs.write(reinterpret_cast<const char*>(&notesSize), sizeof(notesSize));
                ofs.write(notes.c_str(), notesSize);

                const std::string& currency = transaction->getCurrency();
                size_t currencySize = currency.size();
                ofs.write(reinterpret_cast<const char*>(&currencySize), sizeof(currencySize));
                ofs.write(currency.c_str(), currencySize);

                const std::string& date = transaction->getDate();
                size_t dateSize = date.size();
                ofs.write(reinterpret_cast<const char*>(&dateSize), sizeof(dateSize));
                ofs.write(date.c_str(), dateSize);
            }

            const auto& categoryBugets = user->getCategoryBudgets();
            size_t categoryBugetCount = categoryBugets.size();
            ofs.write(reinterpret_cast<const char*>(&categoryBugetCount), sizeof(categoryBugetCount));

            for (const auto& categoryBudget : categoryBugets) {
                const std::string& category = categoryBudget->getCategory();
                size_t categorySize = category.size();
                ofs.write(reinterpret_cast<const char*>(&categorySize), sizeof(categorySize));
                ofs.write(category.c_str(), categorySize);

                double amount = categoryBudget->getAmount();
                ofs.write(reinterpret_cast<const char*>(&amount), sizeof(amount));

                const std::string& date = categoryBudget->getDate();
                size_t dateSize = date.size();
                ofs.write(reinterpret_cast<const char*>(&dateSize), sizeof(dateSize));
                ofs.write(date.c_str(), dateSize);
            }
        }

        ofs.close();
    } catch (...) {
        std::cerr << "Unknown error occurred during saveUsers." << std::endl;
        return false;
    }

    return true;
}

std::vector<std::shared_ptr<User>> DataBase::GetAllUsers() {
    std::ifstream ifs("/home/dingo21/CLionProjects/Checking/Data.bin", std::ios::binary | std::ios::in);
    if (!ifs.is_open()) {
        std::cerr << "Failed to open file for reading!" << std::endl;
        return {};
    }

    std::vector<std::shared_ptr<User>> users;

    while (ifs.peek() != EOF) {
        auto user = std::make_shared<User>();
        int id;
        if (!ifs.read(reinterpret_cast<char*>(&id), sizeof(id))) break;
        user->setId(id);

        size_t companyNameSize;
        if (!ifs.read(reinterpret_cast<char*>(&companyNameSize), sizeof(companyNameSize))) break;
        std::string companyName(companyNameSize, '\0');
        if (!ifs.read(&companyName[0], companyNameSize)) break;
        user->setCompanyName(companyName.c_str());

        size_t emailSize;
        if (!ifs.read(reinterpret_cast<char*>(&emailSize), sizeof(emailSize))) break;
        std::string email(emailSize, '\0');
        if (!ifs.read(&email[0], emailSize)) break;
        user->setEmail(email.c_str());

        size_t passwordSize;
        if (!ifs.read(reinterpret_cast<char*>(&passwordSize), sizeof(passwordSize))) break;
        std::string password(passwordSize, '\0');
        if (!ifs.read(&password[0], passwordSize)) break;
        user->setPassword(password.c_str());

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

            user->addTransaction(amount, category, date, notes, currency);
        }

        size_t categoryBudgetCount;
        if (!ifs.read(reinterpret_cast<char*>(&categoryBudgetCount), sizeof(categoryBudgetCount))) break;

        for (size_t i = 0; i < categoryBudgetCount; ++i) {
            double amount;
            size_t categorySize, dateSize;

            if (!ifs.read(reinterpret_cast<char*>(&categorySize), sizeof(categorySize))) break;
            std::string category(categorySize, '\0');
            if (!ifs.read(&category[0], categorySize)) break;

            if (!ifs.read(reinterpret_cast<char*>(&amount), sizeof(amount))) break;

            if (!ifs.read(reinterpret_cast<char*>(&dateSize), sizeof(dateSize))) break;
            std::string date(dateSize, '\0');
            if (!ifs.read(&date[0], dateSize)) break;

            user->addCategoryBudget(category, amount, date);
        }

        users.push_back(user);
    }

    return users;
}
