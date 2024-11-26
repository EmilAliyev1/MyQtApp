
#pragma once
#include <QDate>


class Budget {
public:
    Budget() = default;
    Budget(const std::string& category, double expense, const std::string& date)
        : category(category), expense(expense), date(date) {}

    std::string getCategory() const { return category; }
    double getAmount() const { return expense; }
    std::string getDate() const { return date; }

    void setCategory(const std::string& category) { this->category = category; }
    void setAmount(double expense) { this->expense = expense; }
    void setDate(const std::string& date) { this->date = date; }
private:
    std::string category{};
    double expense{};
    std::string date{};
};
