#pragma once

#include <QFile>
#include <fstream>
#include "Transaction.h"
#include "CurrencyConverter.h"
#include "Budget.h"


class User
{
public:
    User() = default;

    User(const std::string &companyName, const std::string &email, const std::string &password);

    User(int id, const std::string &companyName, const std::string &email, const std::string &password);

    void addTransaction(double amount, const std::string& category, const std::string& date, const std::string& notes, const std::string& currency);
    void editTransaction(int index, double amount, const std::string& category, const std::string& date, const std::string& notes, const std::string& currency = "USD");
    void deleteTransaction(int index);
    void setExchangeRate(const std::string& currency, double rate);

    void addCategoryBudget(const std::string& category, double amount, const std::string& dueDate);
    void editCategoryBudget(int index, const std::string &category, double amount, const std::string &date);
    auto deleteCategoryBudget(int index) -> void;

    std::vector<std::shared_ptr<Budget>> getCategoryBudgets() const;
    double getTotalBudget() const;

    double getTotalCategoryBudget(const std::string &category, const std::string& Date) const;

    int getId() const;
    std::string getCompanyName() const;
    std::string getEmail() const;
    std::string getPassword() const;

    void setId(const int id);
    void setCompanyName(const std::string& companyName);
    void setEmail(const std::string& email);
    void setPassword(const std::string& password);

    // void setBudget(const std::string& category, double limit);


    bool isBudgetExceeded(const std::string& category, double expense, const std::string& currentTransactionDateStr, bool edit) const;

    void displayUser() const;
    static void displayTransactions(const std::vector<Transaction> &transactions);

    std::vector<std::shared_ptr<Transaction>> getTransactions() const;

    CurrencyConverter getConverter() const;

    static std::shared_ptr<User> findUserByEmail(const std::string& email);

    static std::shared_ptr<User> findUserByCompanyName(const std::string& companyName);

    bool isCategoryRepeat(const std::string &category);

private:
    int _id{};
    std::string _companyName{};
    std::string _email{};
    std::string _password{};
    std::vector<std::shared_ptr<Transaction>> _transactions{};
    CurrencyConverter converter{};
    std::vector<std::shared_ptr<Budget>> categoryBudgets{};
};
