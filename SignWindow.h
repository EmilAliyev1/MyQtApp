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
    explicit SignWindow(MainWindow* parent = nullptr);
    ~SignWindow();

private slots:
    void onLoginClicked();
    void onSignupClicked();
    void showSignupForm();
    void showLoginForm();

private:
    void closeEvent(QCloseEvent* event);
    void setLoginForm();
    void setSignupForm();
    //User* findUser(const QString& companyName);
    void loadStylesheet();
    void setupUI();

    QWidget* createLoginWidget();
    QWidget* createSignupWidget();
    QLineEdit* createLineEdit(const QString& placeholder, bool isPasswordField = false);
    QPushButton* createButton(const QString& text, void (SignWindow::* slot)());

    // UI Elements
    QWidget* loginWidget;
    QWidget* signupWidget;

    QLineEdit* loginEmailEdit;
    QLineEdit* loginPasswordEdit;
    QLineEdit* signupCompanynameEdit;
    QLineEdit* signupEmailEdit;
    QLineEdit* signupPasswordEdit;

    // Account Information
    QString currentCompanyname;
    QString currentEmail;
    QString currentPassword;

    // Pointer to Main Window
    MainWindow* mainWindow;
};

#endif // SIGNWINDOW_H
