#pragma once

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QString>
#include <qtablewidget.h>
#include "User.h"
#include "DataBase.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();
    void showAccountInfo(const User& user);

protected:
    void closeEvent(QCloseEvent* event) override;

private slots:
    void populateRecordsTable();

    void showUserInfoForm() ;
    void showIncomeExpenseWidget();

    void addRecord();
    void editRecord();
    void deleteRecord();

private:
    void setupUi();
    QWidget* createSidebarWidget();
    QWidget* createAccountInfoWidget();
    QWidget* createIncomeExpenseWidget();

    QPushButton* createButton(const QString& text, void (MainWindow::* slot)());
    QLineEdit* createLineEdit(const QString& placeholder, bool isPasswordField = false);
    void loadStylesheet();

    // Widgets
    QWidget* centralWidget;
    QWidget* sidebar;
    QWidget* accountInfoWidget;
    QWidget* incomeExpenseWidget;

    QTableWidget* recordsTable;
    User user_main;

    // Labels for account information
    QLabel* accountCompanynameLabel;
    QLabel* accountEmailLabel;

};

#endif // MAINWINDOW_H
