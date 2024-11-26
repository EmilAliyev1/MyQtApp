#include "MainWindow.h"

#include <iostream>
#include <QApplication>
#include <QFile>
#include <QDebug>
#include <QIcon>
#include <QMessageBox>
#include "CurrencyConverter.h"
#include <qinputdialog.h>
#include <QHeaderView>
#include <QChart>
#include <QChartView>
#include <QPieSeries>
#include <QDateEdit>
#include <QGroupBox>
#include <QDialogButtonBox>
#include <QCalendarWidget>

#include "Validate.h"


MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent) {
    setWindowTitle("Monefy");
    resize(1000, 850);

    loadStylesheet();
    setupUi();

    QScreen* screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();  // Get the screen's geometry
    int x = (screenGeometry.width() - width()) / 2;  // Center horizontally
    int y = (screenGeometry.height() - height()) / 2;  // Center vertically
    move(x, y - 100);  // Move the window to the center
}

void MainWindow::loadStylesheet() {
    QFile styleFile("/home/dingo21/CLionProjects/Checking/mainStyle.qss");
    QFile styleFile2("incomeExpenseTrackerStyle.qss");
    if (styleFile.open(QFile::ReadOnly)) {
        QString styleSheet = QLatin1String(styleFile.readAll());
        setStyleSheet(styleSheet);
        styleFile.close();
    }
}

void MainWindow::setupUi() {
    centralWidget = new QWidget(this);
    QHBoxLayout* mainLayout = new QHBoxLayout(centralWidget);

    sidebar = createSidebarWidget();
    accountInfoWidget = createAccountInfoWidget();
    incomeExpenseWidget = createTransactionWidget();
    analyticsWidget = createAnalyticsWidget();
    budgetManagerWidget = createBudgetManagerWidget();

    mainLayout->addWidget(sidebar);
    mainLayout->addWidget(accountInfoWidget);
    mainLayout->addWidget(incomeExpenseWidget);
    mainLayout->addWidget(analyticsWidget);
    mainLayout->addWidget(budgetManagerWidget);

    incomeExpenseWidget->hide();
    analyticsWidget->hide();
    budgetManagerWidget->hide();

    setCentralWidget(centralWidget);
}

QWidget* MainWindow::createSidebarWidget() {
    sidebar = new QWidget(this);
    sidebar->setMinimumWidth(130);
    sidebar->setMaximumWidth(130);
    sidebar->setObjectName("Sidebar");

    QVBoxLayout* sidebarLayout = new QVBoxLayout(sidebar);
    sidebarLayout->setSpacing(25);

    QIcon userIcon("/home/dingo21/CLionProjects/Checking/user.png");

    QPushButton* userInfoButton = new QPushButton("");
    connect(userInfoButton, &QPushButton::clicked, this, &MainWindow::showUserInfoForm);
    userInfoButton->setCursor(Qt::PointingHandCursor);
    userInfoButton->setIcon(userIcon);
    userInfoButton->setIconSize(QSize(45, 45));
    userInfoButton->setObjectName("UserInfoButton");

    QPushButton* incomeExpenseTrackerButton = new QPushButton("Income\nExpense");
    connect(incomeExpenseTrackerButton, &QPushButton::clicked, this, &MainWindow::showIncomeExpenseWidget);
    incomeExpenseTrackerButton->setCursor(Qt::PointingHandCursor);
    incomeExpenseTrackerButton->setObjectName("OtherButton");

    QPushButton* analyticsButton = new QPushButton("Analytics");
    connect(analyticsButton, &QPushButton::clicked, this, &MainWindow::showAnalyticsWidget);
    analyticsButton->setCursor(Qt::PointingHandCursor);
    analyticsButton->setObjectName("OtherButton");

    QPushButton* budgetButton = new QPushButton("Budget");
    connect(budgetButton, &QPushButton::clicked, this, &MainWindow::showBudgetManagerWidget);
    budgetButton->setCursor(Qt::PointingHandCursor);
    budgetButton->setObjectName("OtherButton");

    sidebarLayout->addSpacing(20);
    sidebarLayout->addWidget(userInfoButton, 0, Qt::AlignCenter);
    sidebarLayout->addSpacing(15);
    sidebarLayout->addWidget(incomeExpenseTrackerButton);
    sidebarLayout->addWidget(analyticsButton);
    sidebarLayout->addWidget(budgetButton);
    sidebarLayout->addStretch();
    sidebarLayout->setAlignment(Qt::AlignLeft);

    return sidebar;
}

QWidget* MainWindow::createAccountInfoWidget() {
    accountInfoWidget = new QWidget(this);
    accountInfoWidget->setObjectName("AccountInfoWidget");

    QVBoxLayout* accountInfoLayout = new QVBoxLayout(accountInfoWidget);
    accountInfoLayout->setSpacing(15);

    accountCompanynameLabel = new QLabel("COMPANY NAME");
    accountCompanynameLabel->setObjectName("AccountInfoLabel");

    accountEmailLabel = new QLabel("EMAIL");
    accountEmailLabel->setObjectName("AccountInfoLabel");

    accountInfoLayout->addWidget(accountCompanynameLabel);
    accountInfoLayout->addWidget(accountEmailLabel);
    accountInfoLayout->setAlignment(Qt::AlignCenter);

    return accountInfoWidget;
}

QWidget* MainWindow::createTransactionWidget() {
    incomeExpenseWidget = new QWidget(this);
    incomeExpenseWidget->setObjectName("IncomeExpenseWidget");

    QVBoxLayout* incomeExpenseLayout = new QVBoxLayout(incomeExpenseWidget);
    incomeExpenseLayout->setContentsMargins(20, 20, 20, 20); // Reduce padding
    incomeExpenseLayout->setSpacing(15);                    // Add spacing between elements

    transactionsTable = new QTableWidget(this);
    transactionsTable->setColumnCount(5);
    transactionsTable->setHorizontalHeaderLabels({ "Amount", "Category", "Date", "Notes", "Currency" });

    transactionsTable->setFrameShape(QFrame::Box);
    transactionsTable->setShowGrid(true);

    for (int i = 0; i < transactionsTable->columnCount(); ++i) {
        if (i == 3) {
            transactionsTable->horizontalHeader()->setSectionResizeMode(i, QHeaderView::ResizeToContents);
        } else {
            transactionsTable->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Stretch);
        }
    }

    transactionsTable->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    transactionsTable->setMinimumWidth(800);
    transactionsTable->setMaximumWidth(1200);
    transactionsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    transactionsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    QWidget* tableContainer = new QWidget(this);
    QHBoxLayout* tableLayout = new QHBoxLayout(tableContainer);
    tableLayout->setContentsMargins(0, 0, 0, 0);
    tableLayout->addStretch();
    tableLayout->addWidget(transactionsTable);
    tableLayout->addStretch();
    incomeExpenseLayout->addWidget(tableContainer);

    QLabel* titleLabel = new QLabel("Transactions", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 20px; font-weight: bold; color: #333; margin-bottom: 10px;");
    incomeExpenseLayout->insertWidget(0, titleLabel); // Add label above the table

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(10);                     // Add spacing between buttons
    buttonLayout->setContentsMargins(0, 0, 0, 0);

    QPushButton* addButton = new QPushButton("Add Transaction");
    connect(addButton, &QPushButton::clicked, this, &MainWindow::widgetAddTransaction);
    addButton->setCursor(Qt::PointingHandCursor);

    QPushButton* editButton = new QPushButton("Edit Transaction");
    connect(editButton, &QPushButton::clicked, this, &MainWindow::widgetEditTransaction);
    editButton->setCursor(Qt::PointingHandCursor);

    QPushButton* deleteButton = new QPushButton("Delete Transaction");
    connect(deleteButton, &QPushButton::clicked, this, &MainWindow::widgetDeleteTransaction);
    deleteButton->setCursor(Qt::PointingHandCursor);

    addButton->setObjectName("TransactionButton");
    editButton->setObjectName("TransactionButton");
    deleteButton->setObjectName("TransactionButton");

    buttonLayout->addStretch();
    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(editButton);
    buttonLayout->addWidget(deleteButton);
    buttonLayout->addStretch();

    incomeExpenseLayout->addLayout(buttonLayout);

    return incomeExpenseWidget;
}

QWidget* MainWindow::createAnalyticsWidget() {
    analyticsWidget = new QWidget(this);
    analyticsWidget->setObjectName("AnalyticsWidget");

    QVBoxLayout* analyticsLayout = new QVBoxLayout(analyticsWidget);
    analyticsLayout->setSpacing(15);
    analyticsLayout->setContentsMargins(20, 20, 20, 20);

    QGroupBox* dateSelectionGroup = new QGroupBox("Date Range Selection");
    dateSelectionGroup->setObjectName("DateSelectionGroup");
    QHBoxLayout* dateSelectionLayout = new QHBoxLayout(dateSelectionGroup);

    QLabel* startDateLabel = new QLabel("Start Date:");
    startDateLabel->setObjectName("DateLabel");

    QDateEdit* startDateEdit = new QDateEdit(QDate::currentDate().addMonths(-1));
    startDateEdit->setDisplayFormat("dd-MM-yyyy");
    startDateEdit->setObjectName("DateEdit");
    startDateEdit->setFixedWidth(115);
    startDateEdit->setCalendarPopup(true);

    QLabel* endDateLabel = new QLabel("End Date:");
    endDateLabel->setObjectName("DateLabel");

    QDateEdit* endDateEdit = new QDateEdit(QDate::currentDate());
    endDateEdit->setDisplayFormat("dd-MM-yyyy");
    endDateEdit->setObjectName("DateEdit");
    endDateEdit->setFixedWidth(115);
    endDateEdit->setCalendarPopup(true);

    dateSelectionLayout->addWidget(startDateLabel);
    dateSelectionLayout->addWidget(startDateEdit);
    dateSelectionLayout->addSpacing(20);
    dateSelectionLayout->addWidget(endDateLabel);
    dateSelectionLayout->addWidget(endDateEdit);

    analyticsLayout->addWidget(dateSelectionGroup);

    QGroupBox* chartGroup = new QGroupBox("Expenses by Category");
    chartGroup->setObjectName("ChartGroup");
    QVBoxLayout* chartLayout = new QVBoxLayout(chartGroup);

    QPieSeries* pieSeries = new QPieSeries();

    QChart* chart = new QChart();
    chart->addSeries(pieSeries);
    chart->setTitle("Expenses Breakdown");
    chart->setAnimationOptions(QChart::AllAnimations);

    QChartView* chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setStyleSheet(
        "border: 1px solid #ddd; border-radius: 8px; padding: 8px; background-color: #fff;");

    QGroupBox* summaryGroup = new QGroupBox("Summary");
    summaryGroup->setObjectName("SummaryGroup");
    QVBoxLayout* summaryLayout = new QVBoxLayout(summaryGroup);

    QPushButton* totalIncomeButton = new QPushButton("Total Income: ");
    totalIncomeButton->setObjectName("totalIncomeB");
    connect(totalIncomeButton, &QPushButton::clicked, this, [=]() {
        QDate startDate = startDateEdit->date();
        QDate endDate = endDateEdit->date();

        if (startDate <= endDate) {
            double totalIncome = 0;
            QMap<QString, double> categoryTotals;

            for (const auto& transaction : user_main->getTransactions()) {
                QDate transactionDate = QDate::fromString(QString::fromStdString(transaction->getDate()), "dd-MM-yyyy");

                if (transactionDate >= startDate && transactionDate <= endDate) {
                    double amount = transaction->getAmount();
                    QString category = QString::fromStdString(transaction->getCategory());
                    if (amount > 0) {
                        categoryTotals[category] += user_main->getConverter().convertToBase(amount, transaction->getCurrency());
                        totalIncome += user_main->getConverter().convertToBase(amount, transaction->getCurrency());
                    }
                }
            }

            totalIncomeButton->setText(QString("Total Income: $%1").arg(totalIncome, 0, 'f', 2));

            QList<QColor> colorPalette = {
                QColor("#FF5733"), QColor("#33FF57"), QColor("#3357FF"),
                QColor("#FFC300"), QColor("#C70039"), QColor("#900C3F"),
                QColor("#581845"), QColor("#2ECC71"), QColor("#E67E22"),
                QColor("#3498DB"), QColor("#8E44AD"), QColor("#16A085")
            };

            pieSeries->clear();

            int colorIndex = 0;
            for (auto it = categoryTotals.begin(); it != categoryTotals.end(); ++it) {
                pieSeries->append(it.key(), it.value());

                QPieSlice* lastSlice = pieSeries->slices().last();

                lastSlice->setLabel(QString("%1: $%2").arg(it.key()).arg(it.value(), 0, 'f', 2));

                lastSlice->setLabelVisible(true);
                lastSlice->setLabelPosition(QPieSlice::LabelOutside);
                lastSlice->setLabelFont(QFont("Arial", 10, QFont::Normal));

                lastSlice->setBrush(colorPalette[colorIndex]);

                colorIndex = (colorIndex + 1) % colorPalette.size();
            }
        }
        else pieSeries->clear();
    });
    QPushButton* totalExpensesButton = new QPushButton("Total Expenses: ");
    totalExpensesButton->setObjectName("totalExpensesB");
    connect(totalExpensesButton, &QPushButton::clicked, this, [=]() {
        QDate startDate = startDateEdit->date();
        QDate endDate = endDateEdit->date();

        if (startDate <= endDate) {
            double totalExpenses = 0;
            QMap<QString, double> categoryTotals;

            for (const auto& transaction : user_main->getTransactions()) {
                QDate transactionDate = QDate::fromString(QString::fromStdString(transaction->getDate()), "dd-MM-yyyy");

                if (transactionDate >= startDate && transactionDate <= endDate) {
                    double amount = transaction->getAmount();
                    QString category = QString::fromStdString(transaction->getCategory());
                    if (amount < 0) {
                        categoryTotals[category] += user_main->getConverter().convertToBase(amount, transaction->getCurrency());
                        totalExpenses += -user_main->getConverter().convertToBase(amount, transaction->getCurrency());
                    }
                }
            }

            totalExpensesButton->setText(QString("Total Expenses: $%1").arg(totalExpenses, 0, 'f', 2));

            QList<QColor> colorPalette = {
                QColor("#FF5733"), QColor("#33FF57"), QColor("#3357FF"),
                QColor("#FFC300"), QColor("#C70039"), QColor("#900C3F"),
                QColor("#581845"), QColor("#2ECC71"), QColor("#E67E22"),
                QColor("#3498DB"), QColor("#8E44AD"), QColor("#16A085")
            };

            pieSeries->clear();

            int colorIndex = 0;
            for (auto it = categoryTotals.begin(); it != categoryTotals.end(); ++it) {
                pieSeries->append(it.key(), it.value());

                QPieSlice* lastSlice = pieSeries->slices().last();

                // Set the label format for each slice
                lastSlice->setLabel(QString("%1: $%2").arg(it.key()).arg(it.value(), 0, 'f', 2));

                // Enable callouts for labels (this prevents overlap)
                lastSlice->setLabelVisible(true);       // Ensure the label is visible
                lastSlice->setLabelPosition(QPieSlice::LabelOutside);  // Label outside the pie slice
                lastSlice->setLabelFont(QFont("Arial", 10, QFont::Normal)); // Set font for the label

                // Set the brush (color) for the slice
                lastSlice->setBrush(colorPalette[colorIndex]);

                // Rotate through the color palette
                colorIndex = (colorIndex + 1) % colorPalette.size();
            }
        }
        else pieSeries->clear();
    });

    summaryLayout->addWidget(totalIncomeButton);
    summaryLayout->addWidget(totalExpensesButton);
    analyticsLayout->addWidget(summaryGroup);

    chartLayout->addWidget(chartView);

    analyticsLayout->addWidget(chartGroup);

    QPushButton* updateButton = new QPushButton("Update Analytics");
    updateButton->setObjectName("updateButton");
    connect(updateButton, &QPushButton::clicked, this, [=]() {
        QDate startDate = startDateEdit->date();
        QDate endDate = endDateEdit->date();

        if (startDate <= endDate) {
            double totalIncome = 0;
            double totalExpenses = 0;
            QMap<QString, double> categoryTotals;

            for (const auto& transaction : user_main->getTransactions()) {
                QDate transactionDate = QDate::fromString(QString::fromStdString(transaction->getDate()), "dd-MM-yyyy");

                if (transactionDate >= startDate && transactionDate <= endDate) {
                    double amount = transaction->getAmount();
                    QString category = QString::fromStdString(transaction->getCategory());
                    categoryTotals[category] += user_main->getConverter().convertToBase(amount, transaction->getCurrency());
                    if (amount > 0) {
                        totalIncome += user_main->getConverter().convertToBase(amount, transaction->getCurrency());
                    }
                    else {
                        totalExpenses += -user_main->getConverter().convertToBase(amount, transaction->getCurrency());
                    }
                }
            }

            totalIncomeButton->setText(QString("Total Income: $%1").arg(totalIncome, 0, 'f', 2));
            totalExpensesButton->setText(QString("Total Expenses: $%1").arg(totalExpenses, 0, 'f', 2));

            QList<QColor> colorPalette = {
                QColor("#FF5733"), QColor("#33FF57"), QColor("#3357FF"),
                QColor("#FFC300"), QColor("#C70039"), QColor("#900C3F"),
                QColor("#581845"), QColor("#2ECC71"), QColor("#E67E22"),
                QColor("#3498DB"), QColor("#8E44AD"), QColor("#16A085")
            };

            pieSeries->clear();

            int colorIndex = 0;
            for (auto it = categoryTotals.begin(); it != categoryTotals.end(); ++it) {
                pieSeries->append(it.key(), it.value());

                QPieSlice* lastSlice = pieSeries->slices().last();

                lastSlice->setLabel(QString("%1: $%2").arg(it.key()).arg(it.value(), 0, 'f', 2));

                lastSlice->setLabelVisible(true);
                lastSlice->setLabelPosition(QPieSlice::LabelOutside);
                lastSlice->setLabelFont(QFont("Arial", 10, QFont::Normal));

                lastSlice->setBrush(colorPalette[colorIndex]);

                colorIndex = (colorIndex + 1) % colorPalette.size();
            }
        }
        else pieSeries->clear();
    });

    analyticsLayout->addWidget(updateButton);

    return analyticsWidget;
}

QWidget* MainWindow::createBudgetManagerWidget() {
    budgetManagerWidget = new QWidget(this);
    budgetManagerWidget->setObjectName("BudgetManagerWidget");

    QVBoxLayout* budgetManagerLayout = new QVBoxLayout(budgetManagerWidget);
    budgetManagerLayout->setContentsMargins(20, 20, 20, 20); // Reduce padding
    budgetManagerLayout->setSpacing(15);

    QLabel* titleLabel = new QLabel("Budgets", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 20px; font-weight: bold; color: #333; margin-bottom: 10px;");

    budgetManagerLayout->addWidget(titleLabel);

    budgetTable = new QTableWidget(this);
    budgetTable->setColumnCount(3);
    budgetTable->setHorizontalHeaderLabels({"Category", "Budget Limit", "Due Date"});

    transactionsTable->setStyleSheet("QTableWidget { border: 1px solid #ccc; }");
    transactionsTable->setFrameShape(QFrame::Box);
    transactionsTable->setShowGrid(true);

    budgetTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    budgetManagerLayout->addWidget(budgetTable);

    // Buttons
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    addBudgetButton = new QPushButton("Add Budget", this);
    addBudgetButton->setCursor(Qt::PointingHandCursor);
    editBudgetButton = new QPushButton("Edit Budget", this);
    editBudgetButton->setCursor(Qt::PointingHandCursor);
    deleteBudgetButton = new QPushButton("Delete Budget", this);
    deleteBudgetButton->setCursor(Qt::PointingHandCursor);

    addBudgetButton->setObjectName("BudgetButton");
    editBudgetButton->setObjectName("BudgetButton");
    deleteBudgetButton->setObjectName("BudgetButton");

    buttonLayout->addStretch();
    buttonLayout->addWidget(addBudgetButton);
    buttonLayout->addWidget(editBudgetButton);
    buttonLayout->addWidget(deleteBudgetButton);
    buttonLayout->addStretch();
    budgetManagerLayout->addLayout(buttonLayout);

    connect(addBudgetButton, &QPushButton::clicked, this, &MainWindow::widgetAddBudget);
    connect(editBudgetButton, &QPushButton::clicked, this, &MainWindow::widgetEditCategoryBudget);
    connect(deleteBudgetButton, &QPushButton::clicked, this, &MainWindow::widgetDeleteCategoryBudget);

    return budgetManagerWidget;
}

QString MainWindow::getDateFromDialog(QWidget* parent, const QDate& date, bool &ok) {
    QDialog dialog(parent);
    dialog.setWindowTitle("Add Transaction");

    QVBoxLayout* layout = new QVBoxLayout(&dialog);

    QLabel* label = new QLabel("Select a Date (DD-MM-YYYY):");
    layout->addWidget(label);

    QDateEdit* dateEdit = new QDateEdit(date);
    dateEdit->setCalendarPopup(true);
    dateEdit->setDisplayFormat("dd-MM-yyyy");
    layout->addWidget(dateEdit);

    QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    layout->addWidget(buttonBox);

    connect(buttonBox, &QDialogButtonBox::accepted, [&]() {
        if (dateEdit->date().isValid()) {
            dialog.accept();
        } else {
            QMessageBox::warning(&dialog, "Invalid Date", "Please select a valid date.");
        }
    });
    connect(buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {
        ok = true;
        return dateEdit->date().toString("dd-MM-yyyy");
    } else {
        return "";
    }
}

void MainWindow::widgetAddBudget() {
    bool ok;

    QString dueDate = getDateFromDialog(this, QDate::currentDate(), ok);
    if (!ok || dueDate.isEmpty()) return;
    if (QDate::fromString(dueDate, "dd-MM-yyyy") < QDate::currentDate()) {
        QMessageBox::warning(this, "Date selection failed", "Invalid date state.\n\nThe date must be more than the current date.");
        return;
    }


    QString category = QInputDialog::getText(this, "Add Recurring Expense", "Category:", QLineEdit::Normal, "", &ok);
    if (!ok || !Validate::validateText(category, this)) return;
    if (!user_main->isCategoryRepeat(category.toStdString())) {
        QMessageBox::warning(this, "Category selection failed", "Invalid category.\n\nYou can not create budgets with same categories.");
        return;
    }

    double minAmount = user_main->getTotalCategoryBudget(category.toStdString(), dueDate.toStdString());
    if (minAmount == 0) minAmount++;
    double amount = QInputDialog::getDouble(this, "Add Recurring Expense", "Amount: (min: 0$; max: 100000$)", minAmount, minAmount, 1000000, 2, &ok);
    if (!ok) return;

    int row = budgetTable->rowCount();
    budgetTable->insertRow(row);
    budgetTable->setItem(row, 0, new QTableWidgetItem(category));
    budgetTable->setItem(row, 1, new QTableWidgetItem(QString::number(amount)));
    budgetTable->setItem(row, 2, new QTableWidgetItem(dueDate));

    user_main->addCategoryBudget(category.toStdString(), amount, dueDate.toStdString());

    for (auto& user : DataBase::users) {
        if (user && user->getId() == user_main->getId()) {
            user = user_main;
        }
    }

    DataBase::saveUsers();
}

void MainWindow::widgetEditCategoryBudget() {
    int row = budgetTable->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Edit Transaction", "Please select a Transaction to edit.");
        return;
    }
    bool ok;

    QTableWidgetItem* dueDateItem = budgetTable->item(row, 2);

    QString dueDateTextPrevious = dueDateItem->text();
    QDate dueDatePrevious = QDate::fromString(dueDateTextPrevious, "dd-MM-yyyy");
    QString dueDate = getDateFromDialog(this, dueDatePrevious, ok);
    if (!ok || dueDate.isEmpty()) return;
    if (QDate::fromString(dueDate, "dd-MM-yyyy") < QDate::currentDate()) {
        QMessageBox::warning(this, "Date selection failed", "Invalid date state.\n\nThe date must be more than the current date.");
        return;
    }

    QString category = QInputDialog::getText(this, "Edit Transaction", "Category:", QLineEdit::Normal, budgetTable->item(row, 0)->text(), &ok);
    if (!ok || !Validate::validateText(category, this)) return;

    double minAmount = user_main->getTotalCategoryBudget(category.toStdString(), dueDate.toStdString());
    double amount = QInputDialog::getDouble(
        this,
        "Add Transaction",
        "Amount: (min: 0$; max: 100000$)",
        budgetTable->item(row, 1)->text().toDouble(),
        minAmount,
        100000,
        2,
        &ok
    );
    if (!ok) return;


    budgetTable->item(row, 0)->setText(category);
    budgetTable->item(row, 1)->setText(QString::number(amount));
    budgetTable->item(row, 2)->setText(dueDate);

    user_main->editCategoryBudget(row, category.toStdString(), amount, dueDate.toStdString());

    for (auto& user : DataBase::users) {
        if (user && user->getId() == user_main->getId()) {
            user = user_main;
        }
    }

    DataBase::saveUsers();
}

void MainWindow::widgetDeleteCategoryBudget() {
    int selectedRow = budgetTable->currentRow();

    if (selectedRow < 0) {
        QMessageBox::warning(this, "Delete Transaction", "Please select a Transaction to delete.");
        return;
    }


    QMessageBox::StandardButton reply = QMessageBox::question(
        this,
        "Delete Category Budget",
        "Are you sure you want to delete the selected Category Budget?",
        QMessageBox::Yes | QMessageBox::No
    );

    if (reply == QMessageBox::Yes) {
        try {
            user_main->deleteCategoryBudget(selectedRow);
            budgetTable->removeRow(selectedRow);

            QMessageBox::information(this, "Delete Category Budget", "Category Budget deleted successfully.");
        }
        catch (const std::out_of_range& e) {
            QMessageBox::critical(this, "Error", "Failed to delete the Category Budget: " + QString(e.what()));
        }
    }

    for (auto& user : DataBase::users) {
        if (user && user->getId() == user_main->getId()) {
            user = user_main;
        }
    }

    DataBase::saveUsers();
}

void MainWindow::widgetAddTransaction() {
    bool ok;

    QString currency = QInputDialog::getItem(
        this,
        "Add Transaction",
        "Select Currency:",
        {"USD", "EUR", "JPY", "GBP"},
        0,
        false,
        &ok
    );
    if (!ok || currency.isEmpty()) return;

    QString date = getDateFromDialog(this, QDate::currentDate(), ok);
    if (!ok || date.isEmpty()) return;

    double minValue = user_main->getConverter().convertToMaxMin(-100000, currency.toStdString());
    double maxValue = user_main->getConverter().convertToMaxMin(100000, currency.toStdString());

    double amount = QInputDialog::getDouble(
        this,
        "Add Transaction",
        "Amount: (min: -100000$; max: 100000$)",
        0,
        minValue,
        maxValue,
        2,
        &ok
    );
    if (!ok) return;

    QString category = QInputDialog::getText(this, "Add Transaction", "Category:", QLineEdit::Normal, "", &ok);
    if (!ok || !Validate::validateText(category, this)) return;
    if (user_main->isBudgetExceeded(category.toStdString(), user_main->getConverter().convertToBase(amount, currency.toStdString()), date.toStdString(), false) && QDate::fromString(date, "dd-MM-yyyy") >= QDate::currentDate()) {
        QMessageBox::warning(this, "Category input failed", "Exceeded expenses of this category.\n\nYou exceeded the dedicated amount of money set within budget.");
        return;
    }

    QString notes = QInputDialog::getText(this, "Add Transaction", "Notes:", QLineEdit::Normal, "", &ok);
    if (!ok) return;


    int row = transactionsTable->rowCount();
    transactionsTable->insertRow(row);
    transactionsTable->setItem(row, 0, new QTableWidgetItem(QString::number(amount)));
    transactionsTable->setItem(row, 1, new QTableWidgetItem(category));
    transactionsTable->setItem(row, 2, new QTableWidgetItem(date));
    transactionsTable->setItem(row, 3, new QTableWidgetItem(notes));
    transactionsTable->setItem(row, 4, new QTableWidgetItem(currency));


    user_main->addTransaction(amount, category.toStdString(), date.toStdString(), notes.toStdString(), currency.toStdString());

    for (auto& user : DataBase::users) {
        if (user && user->getId() == user_main->getId()) {
            user = user_main;
        }
    }

    DataBase::saveUsers();
}

void MainWindow::widgetEditTransaction() {
    int row = transactionsTable->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Edit Transaction", "Please select a Transaction to edit.");
        return;
    }

    CurrencyConverter currencyBB;
    bool ok;
    QString currency = QInputDialog::getItem(
        this,
        "Add Transaction",
        "Select Currency:",
        {"USD", "EUR", "JPY", "GBP"},
        0,
        false,
        &ok
    );
    if (!ok || currency.isEmpty()) return;

    QTableWidgetItem* dateItem = transactionsTable->item(row, 2);
    QString dateTextPrevious = dateItem->text();
    QDate datePrevious = QDate::fromString(dateTextPrevious, "dd-MM-yyyy");
    QString date = getDateFromDialog(this, datePrevious, ok);
    if (!ok || date.isEmpty()) return;

    double minValue = user_main->getConverter().convertToMaxMin(-100000, currency.toStdString());
    double maxValue = user_main->getConverter().convertToMaxMin(100000, currency.toStdString());

    double previousAmount = transactionsTable->item(row, 0)->text().toDouble();
    double amount = QInputDialog::getDouble(
        this,
        "Add Transaction",
        "Amount: (min: -100000$; max: 100000$)",
        transactionsTable->item(row, 0)->text().toDouble(),
        minValue,
        maxValue,
        2,
        &ok
    );
    if (!ok) return;

    QString category = QInputDialog::getText(this, "Edit Transaction", "Category:", QLineEdit::Normal, transactionsTable->item(row, 1)->text(), &ok);
    if (!ok || !Validate::validateText(category, this)) return;
    if (amount - previousAmount < 0) {
        if (user_main->isBudgetExceeded(category.toStdString(), user_main->getConverter().convertToBase(amount - previousAmount, currency.toStdString()), date.toStdString(), true) && QDate::fromString(date, "dd-MM-yyyy") >= QDate::currentDate()) {
            QMessageBox::warning(this, "Category input failed", "Exceeded expenses of this category.\n\nYou exceeded the dedicated amount of money set within budget.");
            return;
        }
    }
    else {
        if (user_main->isBudgetExceeded(category.toStdString(), user_main->getConverter().convertToBase(previousAmount - amount, currency.toStdString()), date.toStdString(), true) && QDate::fromString(date, "dd-MM-yyyy") >= QDate::currentDate()) {
            QMessageBox::warning(this, "Category input failed", "Exceeded expenses of this category.\n\nYou exceeded the dedicated amount of money set within budget.");
            return;
        }
    }

    QString notes = QInputDialog::getText(this, "Edit Transaction", "Notes:", QLineEdit::Normal, transactionsTable->item(row, 3)->text(), &ok);
    if (!ok) return;


    transactionsTable->item(row, 0)->setText(QString::number(amount));
    transactionsTable->item(row, 1)->setText(category);
    transactionsTable->item(row, 2)->setText(date);
    transactionsTable->item(row, 3)->setText(notes);
    transactionsTable->item(row, 4)->setText(currency);

    user_main->editTransaction(row, amount, category.toStdString(), date.toStdString(), notes.toStdString(), currency.toStdString());

    for (auto& user : DataBase::users) {
        if (user && user->getId() == user_main->getId()) {
            user = user_main;
        }
    }

    DataBase::saveUsers();
}

void MainWindow::widgetDeleteTransaction() {
    int selectedRow = transactionsTable->currentRow();

    if (selectedRow < 0) {
        QMessageBox::warning(this, "Delete Transaction", "Please select a Transaction to delete.");
        return;
    }


    QMessageBox::StandardButton reply = QMessageBox::question(
        this,
        "Delete Transaction",
        "Are you sure you want to delete the selected Transaction?",
        QMessageBox::Yes | QMessageBox::No
    );

    if (reply == QMessageBox::Yes) {
        try {
            user_main->deleteTransaction(selectedRow);
            transactionsTable->removeRow(selectedRow);

            QMessageBox::information(this, "Delete Transaction", "Transaction deleted successfully.");
        }
        catch (const std::out_of_range& e) {
            QMessageBox::critical(this, "Error", "Failed to delete the Transaction: " + QString(e.what()));
        }
    }

    for (auto& user : DataBase::users) {
        if (user && user->getId() == user_main->getId()) {
            user = user_main;
        }
    }

    DataBase::saveUsers();
}

void MainWindow::populateTransactionsTable() const{
    transactionsTable->setRowCount(0);

    int row = 0;
    for (const auto& transaction : user_main->getTransactions()) {
        transactionsTable->insertRow(row);

        transactionsTable->setItem(row, 0, new QTableWidgetItem(QString::number(transaction->getAmount())));
        transactionsTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(transaction->getCategory())));
        transactionsTable->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(transaction->getDate())));
        transactionsTable->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(transaction->getNotes())));
        transactionsTable->setItem(row, 4, new QTableWidgetItem(QString::fromStdString(transaction->getCurrency())));

        ++row;
    }
}

void MainWindow::populateBudgetTable() const{
    budgetTable->setRowCount(0);

    int row = 0;
    for (const auto& categoryBudget : user_main->getCategoryBudgets()) {
        budgetTable->insertRow(row);

        budgetTable->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(categoryBudget->getCategory())));
        budgetTable->setItem(row, 1, new QTableWidgetItem(QString::number(categoryBudget->getAmount())));
        budgetTable->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(categoryBudget->getDate())));

        ++row;
    }
}

QLineEdit* MainWindow::createLineEdit(const QString& placeholder, bool isPasswordField) {
    QLineEdit* lineEdit = new QLineEdit(this);
    lineEdit->setPlaceholderText(placeholder);
    if (isPasswordField) {
        lineEdit->setEchoMode(QLineEdit::Password);
    }
    return lineEdit;
}

MainWindow::~MainWindow() = default;

void MainWindow::showUserInfoForm() const {
    analyticsWidget->hide();
    incomeExpenseWidget->hide();
    budgetManagerWidget->hide();
    accountInfoWidget->show();
}

void MainWindow::showIncomeExpenseWidget() {
    accountInfoWidget->hide();
    analyticsWidget->hide();
    budgetManagerWidget->hide();
    populateTransactionsTable();
    incomeExpenseWidget->show();
}

void MainWindow::showAnalyticsWidget() const {
    accountInfoWidget->hide();
    incomeExpenseWidget->hide();
    budgetManagerWidget->hide();
    analyticsWidget->show();
}

void MainWindow::showBudgetManagerWidget() const {
    accountInfoWidget->hide();
    incomeExpenseWidget->hide();
    analyticsWidget->hide();
    populateBudgetTable();
    budgetManagerWidget->show();
}

void MainWindow::showAccountInfo(const std::shared_ptr<User> &user) {

    accountCompanynameLabel->setText(accountCompanynameLabel->text() + "<font color='red'>" + ":  " + QString::fromStdString(user->getCompanyName()));
    accountEmailLabel->setText(accountEmailLabel->text() + "<font color='red'>" + ":  " + QString::fromStdString(user->getEmail()));

    user_main = user;
}