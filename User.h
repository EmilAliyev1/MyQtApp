#pragma once

#include <QString>
#include <QFile>
#include <QDataStream>
#include <fstream>
#include "Transaction.h"
#include "CurrencyConverter.h"

class User
{
private:
    int _id{};
    std::string _companyName{};
    std::string _email{};
    std::string _password{};
    std::vector<Transaction> _transactions{};
    CurrencyConverter converter;
public:
    User() = default;
    User(std::string companyName, std::string email, std::string password);
    User(int id, std::string companyName, std::string email, std::string password);

    void addTransaction(double amount, const std::string& category, const std::string& date, const std::string& notes, const std::string& currency);
    void addTransaction(const Transaction& transaction);
    void editTransaction(int index, double amount, const std::string& category, const std::string& date, const std::string& notes, const std::string& currency = "USD");
    void deleteTransaction(int index);
    void setExchangeRate(const std::string& currency, double rate);

    double getTotalInBaseCurrency() const;

    int getId() const;
    std::string getCompanyName() const;
    std::string getEmail() const;
    std::string getPassword() const;

    void setId(int id);
    void setCompanyName(std::string companyName);
    void setEmail(std::string email);
    void setPassword(std::string password);

    void displayUser() const;
    void displayTransactions(const std::vector<Transaction> &transactions) const;

    std::vector<Transaction> getTransactions() const;

    bool readFromBinary(std::ifstream& in);
    static User findUserByEmail(const std::vector<User>& users, const std::string& email);
};
