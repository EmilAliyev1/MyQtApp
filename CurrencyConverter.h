#pragma once

#include <string>
#include <map>

class CurrencyConverter {
public:
    CurrencyConverter(const std::string& base = "USD") : baseCurrency(base) {}

    void setExchangeRate(const std::string& currency, double rate);

    double convertToBase(double amount, const std::string& currency) const;

    double convertToMaxMin(double amount, const std::string &currency);

private:
    std::string baseCurrency;
    std::map<std::string, double> exchangeRates;
};