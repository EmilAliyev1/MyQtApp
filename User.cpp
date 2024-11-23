#include "User.h"

#include <iostream>
#include <ostream>

#include "DataBase.h"

User::User(std::string companyName, std::string email, std::string password)
    : _id(DataBase::id++), _companyName(companyName), _email(email), _password(password) {}

User::User(int id, std::string companyName, std::string email, std::string password)
    : _id(id), _companyName(companyName), _email(email), _password(password) {}

void User::addTransaction(double amount, const std::string& category, const std::string& date, const std::string& notes, const std::string& currency) {
    _transactions.emplace_back(amount, category, date, notes, currency);
}

void User::editTransaction(int index, double amount, const std::string& category, const std::string& date, const std::string& notes, const std::string& currency) {
    if (index < 0 || index >= static_cast<int>(_transactions.size())) {
        throw std::out_of_range("Invalid record index.");
    }
    _transactions[index] = Transaction(amount, category, date, notes, currency);
}

void User::deleteTransaction(int index) {
    if (index < 0 || index >= static_cast<int>(_transactions.size())) {
        throw std::out_of_range("Invalid record index.");
    }
    _transactions.erase(_transactions.begin() + index);
}
void User::setExchangeRate(const std::string& currency, double rate) {
    converter.setExchangeRate(currency, rate);
}

double User::getTotalInBaseCurrency() const {
    double total = 0.0;
    for (const auto& transaction : _transactions) {
        total += converter.convertToBase(transaction.getAmount(), transaction.getCurrency());
    }
    return total;
}

void User::displayUser() const
{
    std::cout << this->_id << " " << this->_companyName << " " << this->_email << '\n';
}

void User::displayTransactions(const std::vector<Transaction> &transactions) const //delete must
{
    for (const auto& element : transactions) {
        std::cout << element.getAmount() << " ";
        std::cout << element.getCategory() << " ";
        std::cout << element.getDate() << " ";
        std::cout << element.getNotes() << " ";
        std::cout << element.getCurrency() << " ";
        std::cout << '\n';
    }
}

int User::getId() const
{
    return _id;
}

std::string User::getCompanyName() const
{
    return _companyName;
}

std::string User::getEmail() const
{
    return _email;
}

std::string User::getPassword() const {
    return _password;
}

std::vector<Transaction> User::getTransactions() const
{
    
    return _transactions;
}

void User::setId(int id)
{
    _id = id;
}

void User::setCompanyName(std::string companyName)
{
    _companyName = companyName;
}

void User::setEmail(std::string email)
{
    _email = email;
}

void User::setPassword(std::string password) {
    _password = password;
}

bool User::readFromBinary(std::ifstream& in) {
    if (!in.good())
        return false;

    in.read(reinterpret_cast<char*>(&_id), sizeof(_id));
    if (!in) return false;

    size_t companyNameSize = 0;
    in.read(reinterpret_cast<char*>(&companyNameSize), sizeof(companyNameSize));
    if (!in) return false;

    _companyName.resize(companyNameSize);
    in.read(&_companyName[0], companyNameSize);
    if (!in) return false;


    size_t emailSize = 0;
    in.read(reinterpret_cast<char*>(&emailSize), sizeof(emailSize));
    if (!in) return false;

    _email.resize(emailSize);
    in.read(&_email[0], emailSize);
    if (!in) return false;

    size_t passwordSize = 0;
    in.read(reinterpret_cast<char*>(&passwordSize), sizeof(passwordSize));
    if (!in) return false;

    _password.resize(passwordSize);
    in.read(&_password[0], passwordSize);
    if (!in) return false;

    return true;
}

User User::findUserByEmail(const QString &email) {
    for (const auto& user : DataBase::users) {
        if (user.getEmail() == email.toStdString()) {
            return user;
        }
    }
    return {};
}

User User::findUserByCompanyName(const QString &companyName) {
    for (const auto& user : DataBase::users) {
        if (user.getCompanyName() == companyName.toStdString()) {
            return user;
        }
    }
    return {};
}