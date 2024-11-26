//
// Created by dingo21 on 11/25/24.
//

#include "Validate.h"
#include "User.h"
#include <QMessageBox>
#include <QRegularExpression>
#include <QString>


bool Validate::validateLogin(const QString& email, const QString& password, QWidget* signWindow) {
    if (email.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(signWindow, "Login Failed", "Please fill in all fields.");
        return false;
    }

    QRegularExpression localPartRegex("[\\s<>\"'{}()\\[\\]:;,!?`|\\\\]"); // Allows letters, numbers, '.', '_', '%', '+', '-'
    QRegularExpressionMatch matchEmail = localPartRegex.match(email);

    if (matchEmail.hasMatch()) {
        QMessageBox::warning(signWindow, "Login Failed", "Invalid email address.\n\nThe email field contains invalid characters.");
        return false;
    }

    return true;
}

bool Validate::validateSignup(const QString& companyname, const QString& email, const QString& password, QWidget* signWindow) {
    QRegularExpression localPartRegexRestricted("[\\s<>\"'{}()\\[\\]:;,!?`|\\\\]");
    QRegularExpression localPartRegexEmail("^[a-zA-Z0-9._%+-]+$"); // Allows letters, numbers, '.', '_', '%', '+', '-'

    if (companyname.isEmpty() || email.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(signWindow, "Signup Failed", "Please fill in all fields.");
        return false;
    }

    QRegularExpressionMatch matchCompanyName = localPartRegexRestricted.match(companyname);

    if (matchCompanyName.hasMatch()) {
        QMessageBox::warning(signWindow, "Signup Failed", "Invalid company name.\n\nThe company name contains invalid characters.");
        return false;
    }

    if (companyname.length() < 3) {
        QMessageBox::warning(signWindow, "Signup Failed", "Company name must be at least 3 characters long.");
        return false;
    }

    if (email.count('@') != 1) {
        QMessageBox::warning(signWindow, "Signup Failed", "Invalid email address.\n\nEmail should contain exactly one '@' symbol.");
        return false;
    }

    QStringList emailParts = email.split('@');
    if (emailParts.size() != 2) {
        QMessageBox::warning(signWindow, "Signup Failed", "Invalid email address format.");
        return false;
    }

    QString localPart = emailParts[0];
    QString domainPart = emailParts[1];

    if (domainPart != "gmail.com" && domainPart != "outlook.com" && domainPart != "hotmail.com") {
        QMessageBox::warning(signWindow, "Signup Failed", "Invalid email domain.\n\nThe supported email domains are: \ngmail.com, outlook.com, or hotmail.com.");
        return false;
    }

    QRegularExpressionMatch matchEmailValidation = localPartRegexEmail.match(localPart);
    QRegularExpressionMatch matchEmailRestricted = localPartRegexRestricted.match(email);

    if (matchEmailRestricted.hasMatch()) {
        QMessageBox::warning(signWindow, "Signup Failed", "Invalid email address.\n\nThe email contains invalid characters.");
        return false;
    }
    if (localPart.isEmpty() || !matchEmailValidation.hasMatch()) {
        QMessageBox::warning(signWindow, "Signup Failed", "Invalid email address.\n\nThe part before '@' contains invalid characters.");
        return false;
    }

    QRegularExpressionMatch matchPassword = localPartRegexRestricted.match(password);

    if (matchPassword.hasMatch()) {
        QMessageBox::warning(signWindow, "Signup Failed", "Invalid password address.\n\nThe password contains invalid characters.");
        return false;
    }

    if (password.length() < 6) {
        QMessageBox::warning(signWindow, "Signup Failed", "Password must be at least 6 characters long.");
        return false;
    }

    std::shared_ptr<User>  userC = User::findUserByCompanyName(companyname.toStdString());
    if (userC != nullptr && companyname.toStdString() == userC->getCompanyName()) {
        QMessageBox::warning(signWindow, "Signup Failed", "Another user with the exact Company name was found.");
        return false;
    }
    std::shared_ptr<User>  userE = User::findUserByEmail(email.toStdString());
    if (userE != nullptr && email.toStdString() == userE->getEmail()) {
        QMessageBox::warning(signWindow, "Signup Failed", "Another user with the exact Email was found.");
        return false;
    }

    return true;
}

bool Validate::validateText(const QString& text, QWidget* mainWindow) {
    if (text.isEmpty()) {
        QMessageBox::warning(mainWindow, "Transaction Failed", "Please fill in the field.");
        return false;
    }

    QRegularExpression localPartRegex("[\\s<>\"'{}()\\[\\]:;,!?`|\\\\]"); // Allows letters, numbers, '.', '_', '%', '+', '-'
    QRegularExpressionMatch matchText = localPartRegex.match(text);

    if (matchText.hasMatch()) {
        QMessageBox::warning(mainWindow, "Transaction Failed", "Invalid input.\n\nThe field contains invalid characters.");
        return false;
    }


    return true;
}