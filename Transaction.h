#pragma once
#include <string>
#include "User.h"

class Transaction
{
public:
    Transaction() = default;
    Transaction(double amount, const std::string& category, const std::string& date, const std::string& notes, const std::string& currency = "USD")
        : amount(amount), category(category), date(date), notes(notes), currency(currency) {}

    double getAmount() const { return amount; }
    std::string getCategory() const { return category; }
    std::string getDate() const { return date; }
    std::string getNotes() const { return notes; }
    std::string getCurrency() const { return currency; }

    void setAmount(double newAmount) {
        amount = newAmount;
    }
    void setCategory(const std::string& newCategory) {
        category = newCategory;
    }
    void setDate(const std::string& newDate) {
        date = newDate;
    }
    void setNotes(const std::string& newNotes) {
        notes = newNotes;
    }
    void setCurrency(const std::string& newCurrency) {
        currency = newCurrency;
    }
private:
    double amount{};
    std::string category{};
    std::string date{};
    std::string notes{};
    std::string currency{};
};
