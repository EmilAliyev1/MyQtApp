#pragma once

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <qdatetimeedit.h>
#include <QMainWindow>
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <qpieseries.h>
#include <QString>
#include <qtablewidget.h>
#include "User.h"
#include "DataBase.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();
    void showAccountInfo(const std::shared_ptr<User> &user);

private slots:
    void widgetAddBudget();

    void widgetEditCategoryBudget();

    void widgetDeleteCategoryBudget();


    void populateTransactionsTable() const;

    void populateBudgetTable() const;

    void showUserInfoForm() const;
    void showIncomeExpenseWidget();

    void showAnalyticsWidget() const;

    void showBudgetManagerWidget() const;

    void widgetAddTransaction();
    void widgetEditTransaction();
    void widgetDeleteTransaction();

private:
    void setupUi();
    QWidget* createSidebarWidget();
    QWidget* createAccountInfoWidget();
    QWidget* createTransactionWidget();
    QWidget* createAnalyticsWidget();

    QWidget* createBudgetManagerWidget();




    QCalendarWidget* calendar;
    QPushButton* addBudgetButton;
    QPushButton* editBudgetButton;
    QPushButton* deleteBudgetButton;
    QPushButton* addRecurringExpenseButton;




    QString getDateFromDialog(QWidget *parent, const QDate& date, bool &ok);

    QLineEdit* createLineEdit(const QString& placeholder, bool isPasswordField = false);
    void loadStylesheet();

    // Widgets
    QWidget* centralWidget;
    QWidget* sidebar;
    QWidget* accountInfoWidget;
    QWidget* incomeExpenseWidget;
    QWidget* analyticsWidget;
    QWidget* budgetManagerWidget;

    QTableWidget* transactionsTable;
    QTableWidget* budgetTable;
    std::shared_ptr<User> user_main;

    QLabel* accountCompanynameLabel;
    QLabel* accountEmailLabel;
    QLabel* accountBudgetLabel;

    //void notify(QString message);
};

#endif // MAINWINDOW_H
