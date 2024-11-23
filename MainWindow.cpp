#include "MainWindow.h"

#include <iostream>
#include <QApplication>
#include <QFile>
#include <QDebug>
#include <QIcon>
#include <QCloseEvent>
#include <QMessageBox>
#include "CurrencyConverter.h"
#include <qinputdialog.h>
#include <QHeaderView>


MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent) {
    setWindowTitle("Monefy");
    resize(1000, 700);

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
        qDebug() << "Stylesheet loaded successfully:" << styleSheet;
        setStyleSheet(styleSheet);
        styleFile.close();
    }
    else {
        qDebug() << "Failed to load stylesheet:" << styleFile.errorString();
    }
}

void MainWindow::setupUi() {
    centralWidget = new QWidget(this);
    QHBoxLayout* mainLayout = new QHBoxLayout(centralWidget);

    sidebar = createSidebarWidget();
    accountInfoWidget = createAccountInfoWidget();
    incomeExpenseWidget = createTransactionWidget();

    mainLayout->addWidget(sidebar);
    mainLayout->addWidget(accountInfoWidget);
    mainLayout->addWidget(incomeExpenseWidget);

    incomeExpenseWidget->hide();

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

    QPushButton* button = createButton("", &MainWindow::showUserInfoForm);
    button->setIcon(userIcon);
    button->setIconSize(QSize(45, 45));
    button->setObjectName("UserInfoButton");

    QPushButton* button1 = createButton("Income\nExpense", &MainWindow::showIncomeExpenseWidget);
    button1->setObjectName("OtherButton");

    QPushButton* button2 = new QPushButton("Button 2");
    button2->setObjectName("OtherButton");
    button2->setCursor(Qt::PointingHandCursor);

    QPushButton* button3 = new QPushButton("Button 3");
    button3->setObjectName("OtherButton");
    button3->setCursor(Qt::PointingHandCursor);

    sidebarLayout->addSpacing(20);
    sidebarLayout->addWidget(button, 0, Qt::AlignCenter);
    sidebarLayout->addSpacing(15);
    sidebarLayout->addWidget(button1);
    sidebarLayout->addWidget(button2);
    sidebarLayout->addWidget(button3);
    sidebarLayout->addStretch();
    sidebarLayout->setAlignment(Qt::AlignLeft);

    return sidebar;
}

QWidget* MainWindow::createAccountInfoWidget() {
    accountInfoWidget = new QWidget(this);
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
    QWidget* incomeExpenseWidget = new QWidget(this);
    incomeExpenseWidget->setObjectName("IncomeExpenseWidget");

    QVBoxLayout* incomeExpenseLayout = new QVBoxLayout(incomeExpenseWidget);
    incomeExpenseLayout->setContentsMargins(50, 50, 50, 50);


    recordsTable = new QTableWidget(this);
    recordsTable->setColumnCount(5);
    recordsTable->setHorizontalHeaderLabels({ "Amount", "Category", "Date", "Notes", "Currency" });
    recordsTable->setColumnWidth(0, 120);  // Set the width of the "Amount" column to 150 pixels
    recordsTable->setColumnWidth(1, 175);  // Set the width of the "Category" column to 200 pixels
    recordsTable->setColumnWidth(2, 125);  // Set the width of the "Date" column to 120 pixels
    recordsTable->setColumnWidth(3, 175);  // Set the width of the "Notes" column to 250 pixels
    recordsTable->setColumnWidth(4, 100);
    recordsTable->horizontalHeader()->setStretchLastSection(true);
    recordsTable->setObjectName("RecordTable");

    incomeExpenseLayout->addWidget(recordsTable);


    QHBoxLayout* buttonLayout = new QHBoxLayout();

    QPushButton* addButton = createButton("Add Record", &MainWindow::widgetAddTransaction);
    addButton->setObjectName("TransactionButton");
    QPushButton* editButton = createButton("Edit Record", &MainWindow::widgetEditTransaction);
    editButton->setObjectName("TransactionButton");
    QPushButton* deleteButton = createButton("Delete Record", &MainWindow::widgetDeleteTransaction);
    deleteButton->setObjectName("TransactionButton");

    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(editButton);
    buttonLayout->addWidget(deleteButton);

    incomeExpenseLayout->addLayout(buttonLayout);

    return incomeExpenseWidget;
}


void MainWindow::widgetAddTransaction() {
    bool ok;
    double amount = QInputDialog::getDouble(this, "Add Record", "Amount:", 0, -100000, 100000, 2, &ok);
    if (!ok) return;

    QString category = QInputDialog::getText(this, "Add Record", "Category:", QLineEdit::Normal, "", &ok);
    if (!ok || category.isEmpty()) return;

    QString date = QInputDialog::getText(this, "Add Record", "Date (YYYY-MM-DD):", QLineEdit::Normal, "", &ok);
    if (!ok || date.isEmpty()) return;

    QString notes = QInputDialog::getText(this, "Add Record", "Notes:", QLineEdit::Normal, "", &ok);
    if (!ok) return;

    QString currency = QInputDialog::getText(this, "Add Record", "Currency (e.g., USD):", QLineEdit::Normal, "USD", &ok);
    if (!ok || currency.isEmpty()) return;


    int row = recordsTable->rowCount();
    recordsTable->insertRow(row);
    recordsTable->setItem(row, 0, new QTableWidgetItem(QString::number(amount)));
    recordsTable->setItem(row, 1, new QTableWidgetItem(category));
    recordsTable->setItem(row, 2, new QTableWidgetItem(date));
    recordsTable->setItem(row, 3, new QTableWidgetItem(notes));
    recordsTable->setItem(row, 4, new QTableWidgetItem(currency));


    user_main.addTransaction(amount, category.toStdString(), date.toStdString(), notes.toStdString(), currency.toStdString());

    for (auto& user : DataBase::users) {
        if (user.getId() == user_main.getId()) {
            user = user_main;  // Update the user
            return;
        }
    }

    DataBase::saveUsers();
}

void MainWindow::widgetEditTransaction() {
    int row = recordsTable->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Edit Record", "Please select a record to edit.");
        return;
    }

    bool ok;
    double amount = QInputDialog::getDouble(this, "Edit Record", "Amount:", recordsTable->item(row, 0)->text().toDouble(), -100000, 100000, 2, &ok);
    if (!ok) return;

    QString category = QInputDialog::getText(this, "Edit Record", "Category:", QLineEdit::Normal, recordsTable->item(row, 1)->text(), &ok);
    if (!ok || category.isEmpty()) return;

    QString date = QInputDialog::getText(this, "Edit Record", "Date (YYYY-MM-DD):", QLineEdit::Normal, recordsTable->item(row, 2)->text(), &ok);
    if (!ok || date.isEmpty()) return;

    QString notes = QInputDialog::getText(this, "Edit Record", "Notes:", QLineEdit::Normal, recordsTable->item(row, 3)->text(), &ok);
    if (!ok) return;

    QString currency = QInputDialog::getText(this, "Edit Record", "Currency (e.g., USD):", QLineEdit::Normal, recordsTable->item(row, 4)->text(), &ok);
    if (!ok || currency.isEmpty()) return;


    recordsTable->item(row, 0)->setText(QString::number(amount));
    recordsTable->item(row, 1)->setText(category);
    recordsTable->item(row, 2)->setText(date);
    recordsTable->item(row, 3)->setText(notes);
    recordsTable->item(row, 4)->setText(currency);

    user_main.editTransaction(row, amount, category.toStdString(), date.toStdString(), notes.toStdString(), currency.toStdString());

    for (auto& user : DataBase::users) {
        if (user.getId() == user_main.getId()) {
            user = user_main;  // Update the user
            return;
        }
    }

    DataBase::saveUsers();
}

void MainWindow::widgetDeleteTransaction() {
    int selectedRow = recordsTable->currentRow();

    if (selectedRow < 0) {
        QMessageBox::warning(this, "Delete Record", "Please select a record to delete.");
        return;
    }


    QMessageBox::StandardButton reply = QMessageBox::question(
        this,
        "Delete Record",
        "Are you sure you want to delete the selected record?",
        QMessageBox::Yes | QMessageBox::No
    );

    if (reply == QMessageBox::Yes) {
        try {
            user_main.deleteTransaction(selectedRow);
            recordsTable->removeRow(selectedRow);

            QMessageBox::information(this, "Delete Record", "Record deleted successfully.");
        }
        catch (const std::out_of_range& e) {
            QMessageBox::critical(this, "Error", "Failed to delete the record: " + QString(e.what()));
        }
    }

    for (auto& user : DataBase::users) {
        if (user.getId() == user_main.getId()) {
            user = user_main;  // Update the user
            return;
        }
    }

    DataBase::saveUsers();
}

void MainWindow::populateRecordsTable() {
    recordsTable->setRowCount(0);

    int row = 0;
    for (const auto& transaction : user_main.getTransactions()) {
        recordsTable->insertRow(row);

        recordsTable->setItem(row, 0, new QTableWidgetItem(QString::number(transaction.getAmount())));
        recordsTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(transaction.getCategory())));
        recordsTable->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(transaction.getDate())));
        recordsTable->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(transaction.getNotes())));
        recordsTable->setItem(row, 4, new QTableWidgetItem(QString::fromStdString(transaction.getCurrency())));

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

QPushButton* MainWindow::createButton(const QString& text, void (MainWindow::* slot)()) {
    QPushButton* button = new QPushButton(text);
    button->setCursor(Qt::PointingHandCursor);
    connect(button, &QPushButton::clicked, this, slot);
    return button;
}

MainWindow::~MainWindow() = default;

void MainWindow::showUserInfoForm() {
    accountInfoWidget->show();
    incomeExpenseWidget->hide();
}

void MainWindow::showIncomeExpenseWidget() {
    accountInfoWidget->hide();
    populateRecordsTable();
    incomeExpenseWidget->show();
}

void MainWindow::showAccountInfo(const User& user) {

    accountCompanynameLabel->setText(accountCompanynameLabel->text() + "<font color='red'>" + ":  " + QString::fromStdString(user.getCompanyName()));
    accountEmailLabel->setText(accountEmailLabel->text() + "<font color='red'>" + ":  " + QString::fromStdString(user.getEmail()));

    user_main = user;
}
