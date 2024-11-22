#pragma once

#include <string>
#include <map>

class CurrencyConverter {
private:
    std::string baseCurrency;
    std::map<std::string, double> exchangeRates;

public:
    CurrencyConverter(const std::string& base = "USD") : baseCurrency(base) {}

    void setExchangeRate(const std::string& currency, double rate);

    double convertToBase(double amount, const std::string& currency) const;
};