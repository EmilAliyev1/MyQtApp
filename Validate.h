#pragma once
#include <QString>
#include <QWidget>


class Validate {
public:
    static bool validateLogin(const QString& email, const QString& password, QWidget* signWindow);

    static bool validateSignup(const QString &companyname, const QString &email, const QString &password, QWidget* signWindow);

    static bool validateText(const QString &text, QWidget *mainWindow);
};

