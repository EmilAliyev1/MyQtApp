#pragma once

#ifndef SIGNWINDOW_H
#define SIGNWINDOW_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include <QString>
#include <QMessageBox>
#include <QCloseEvent>
#include "MainWindow.h"

class MainWindow;

class SignWindow : public QDialog {
    Q_OBJECT

public:
    SignWindow(MainWindow* parent = nullptr);
    ~SignWindow();

private slots:
    void onLoginClicked();
    void onSignupClicked();
    void showSignupForm();
    void showLoginForm();

private:
    void setLoginForm();
    void setSignupForm();

    bool validateLogin(const QString &email, const QString &password);

    bool validateSignup(const QString &companyname, const QString &email, const QString &password);

    //User* findUser(const QString& companyName);
    void loadStylesheet();
    void setupUI();

    QWidget* createLoginWidget();
    QWidget* createSignupWidget();
    QLineEdit* createLineEdit(const QString& placeholder, bool isPasswordField = false);

    QWidget* loginWidget;
    QWidget* signupWidget;

    QLineEdit* loginEmailEdit;
    QLineEdit* loginPasswordEdit;
    QLineEdit* signupCompanynameEdit;
    QLineEdit* signupEmailEdit;
    QLineEdit* signupPasswordEdit;

    QString currentCompanyname;
    QString currentEmail;
    QString currentPassword;

    MainWindow* mainWindow;
};

#endif // SIGNWINDOW_H
