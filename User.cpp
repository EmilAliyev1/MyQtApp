#include "User.h"
#include <iostream>
#include "DataBase.h"

User::User(const std::string& companyName, const std::string& email, const std::string& password)
    : _id(DataBase::id++), _companyName(companyName), _email(email), _password(password) {}

User::User(const int id, const std::string& companyName, const std::string& email, const std::string& password)
    : _id(id), _companyName(companyName), _email(email), _password(password) {}

void User::addTransaction(double amount, const std::string& category, const std::string& date, const std::string& notes, const std::string& currency) {
    _transactions.emplace_back(std::make_shared<Transaction>(amount, category, date, notes, currency));
}

void User::editTransaction(int index, double amount, const std::string& category, const std::string& date, const std::string& notes, const std::string& currency) {
    if (index < 0 || index >= static_cast<int>(_transactions.size())) {
        throw std::out_of_range("Invalid record index.");
    }
    _transactions[index] = std::make_shared<Transaction>(amount, category, date, notes, currency);
}

void User::deleteTransaction(const int index) {
    if (index < 0 || index >= static_cast<int>(_transactions.size())) {
        throw std::out_of_range("Invalid record index.");
    }
    _transactions.erase(_transactions.begin() + index);
}
void User::setExchangeRate(const std::string& currency, double rate) {
    converter.setExchangeRate(currency, rate);
}

void User::addCategoryBudget(const std::string& category, double amount, const std::string& dueDate) {
    categoryBudgets.emplace_back(std::make_shared<Budget>(category, amount, dueDate));
}

void User::editCategoryBudget(int index, const std::string& category, double amount, const std::string& date) {
    if (index < 0 || index >= static_cast<int>(categoryBudgets.size())) {
        throw std::out_of_range("Invalid record index.");
    }
    categoryBudgets[index] = std::make_shared<Budget>(category, amount, date);
}

auto User::deleteCategoryBudget(const int index) -> void {
    if (index < 0 || index >= static_cast<int>(categoryBudgets.size())) {
        throw std::out_of_range("Invalid record index.");
    }
    categoryBudgets.erase(categoryBudgets.begin() + index);
}

double User::getTotalBudget() const {
    double total = 0.0;
    for (const auto& transaction : _transactions) {
        if (transaction) {  // Check if the shared_ptr is not null
            total += converter.convertToBase(transaction->getAmount(), transaction->getCurrency());
        } else {
            std::cerr << "Null transaction encountered. Skipping." << std::endl;
        }
    }
    return total;
}

double User::getTotalCategoryBudget(const std::string& category, const std::string& Date) const {
    double totalSpent = 0.0;
    for (const auto& transaction : _transactions) {
        QDate transactionDate = QDate::fromString(QString::fromStdString(transaction->getDate()), "dd-MM-yyyy");
        QDate budgetDate = QDate::fromString(QString::fromStdString(Date), "dd-MM-yyyy");

        if (transaction && transaction->getCategory() == category && transactionDate <= budgetDate && transactionDate >= QDate::currentDate()) {
            if (transaction->getAmount() < 0) {
                totalSpent += converter.convertToBase(transaction->getAmount(), transaction->getCurrency());
            }
        }
    }
    // Check if spent exceeds the budget
    return -totalSpent;
}

void User::displayUser() const
{
    std::cout << this->_id << " " << this->_companyName << " " << this->_email << '\n';
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

std::vector<std::shared_ptr<Transaction>> User::getTransactions() const
{
    return _transactions;
}

CurrencyConverter User::getConverter() const
{
    return converter;
}

std::vector<std::shared_ptr<Budget>> User::getCategoryBudgets() const {
    return categoryBudgets;
}

bool User::isBudgetExceeded(const std::string& category, const double expense, const std::string& currentTransactionDateStr, bool edit) const {
    if (expense < 0) {
        bool flag;
        QDate currentTransactionDate = QDate::fromString(QString::fromStdString(currentTransactionDateStr), "dd-MM-yyyy");
        for (const auto& budget : categoryBudgets) {
            if (budget && budget->getCategory() == category) {
                double totalSpent = 0.0;
                QDate budgetDate = QDate::fromString(QString::fromStdString(budget->getDate()), "dd-MM-yyyy");
                for (const auto& transaction : _transactions) {
                    QDate transactionDate = QDate::fromString(QString::fromStdString(transaction->getDate()), "dd-MM-yyyy");
                    if (transaction->getAmount() < 0) {
                        if (transaction && transaction->getCategory() == category && (transactionDate <= budgetDate && transactionDate >= QDate::currentDate())) {

                            totalSpent += converter.convertToBase(transaction->getAmount(), transaction->getCurrency());
                        }
                        if (transaction && transaction->getCategory() == category && (currentTransactionDate <= budgetDate && currentTransactionDate >= QDate::currentDate())) {
                            flag = true;
                            if (-expense > budget->getAmount()) return true;
                            if (edit) totalSpent += converter.convertToBase(transaction->getAmount(), transaction->getCurrency());
                        }
                    }
                }

                if (flag) return -(totalSpent + expense) > budget->getAmount();
                if (budget->getCategory() == category && (currentTransactionDate <= budgetDate && currentTransactionDate >= QDate::currentDate())) return -expense > budget->getAmount();
                return -totalSpent > budget->getAmount();
            }
        }
    }
    return false;
}

void User::setId(const int id)
{
    _id = id;
}

void User::setCompanyName(const std::string& companyName)
{
    _companyName = companyName;
}

void User::setEmail(const std::string& email)
{
    _email = email;
}

void User::setPassword(const std::string& password) {
    _password = password;
}


std::shared_ptr<User> User::findUserByEmail(const std::string& email) {
    for (const auto& user : DataBase::users) {
        if (user->getEmail() == email) {
            return user;
        }
    }
    return {};
}

std::shared_ptr<User>  User::findUserByCompanyName(const std::string& companyName) {
    for (const auto& user : DataBase::users) {
        if (user->getCompanyName() == companyName) {
            return user;
        }
    }
    return {};
}

bool User::isCategoryRepeat(const std::string& category) {
    for (const auto& categoryBudget : categoryBudgets) {
        if (categoryBudget && categoryBudget->getCategory() == category) {
            return false;
        }
    }
    return true;
}