#include "CurrencyConverter.h"
#include <iostream>

void CurrencyConverter::setExchangeRate(const std::string& currency, double rate) {
    exchangeRates[currency] = rate;
}

double CurrencyConverter::convertToBase(double amount, const std::string& currency) const {
    if (currency == baseCurrency) {
        return amount;
    }
    auto it = exchangeRates.find(currency);
    if (it == exchangeRates.end()) {
        std::cout << ("No exchange rate available for currency: " + currency);
    }
    return amount * it->second;
}

double CurrencyConverter::convertToMaxMin(double amount, const std::string& currency) {
    if (currency == baseCurrency) {
        return amount;
    }
    auto it = exchangeRates.find(currency);
    if (it == exchangeRates.end()) {
        std::cout << ("No exchange rate available for currency: " + currency);
    }
    return amount / it->second;
}