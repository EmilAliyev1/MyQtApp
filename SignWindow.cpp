#include "SignWindow.h"
#include "MainWindow.h"
#include <QLabel>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QFile>
#include <QApplication>
#include "User.h"


SignWindow::SignWindow(MainWindow* parent)
    : QDialog(parent), mainWindow(parent) {

    setWindowTitle("Company Authentication");
    setFixedSize(550, 500);


    connect(this, &QDialog::finished, parent, &QMainWindow::close);

    loadStylesheet();
    setupUI();

    setLoginForm();
}

SignWindow::~SignWindow() = default;

void SignWindow::loadStylesheet() {
    QFile styleFile("/home/dingo21/CLionProjects/Checking/signStyle.qss");
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

void SignWindow::setupUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    // Create login and signup widgets
    loginWidget = createLoginWidget();
    signupWidget = createSignupWidget();

    mainLayout->addWidget(loginWidget);
    mainLayout->addWidget(signupWidget);
    setLayout(mainLayout);
}

QWidget* SignWindow::createLoginWidget() {
    QWidget* widget = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(widget);

    QLabel* title = new QLabel("Login");
    title->setStyleSheet("font-size: 20px; font-weight: bold; color: #333333;");
    title->setAlignment(Qt::AlignCenter);

    QLabel* emailLabel = new QLabel("Email");
    emailLabel->setObjectName("EmailLabel");
    loginEmailEdit = createLineEdit("Enter your email");

    QLabel* passwordLabel = new QLabel("Password");
    passwordLabel->setObjectName("PasswordLabel");
    loginPasswordEdit = createLineEdit("Enter your password", true);

    QPushButton* loginButton = createButton("Login", &SignWindow::onLoginClicked);
    loginButton->setObjectName("LoginButton");
    loginButton->setObjectName("LoginButton");

    QPushButton* goToSignupButton = createButton("Create an account", &SignWindow::showSignupForm);
    goToSignupButton->setObjectName("goToSignupButton");

    layout->addWidget(title);
    layout->addWidget(emailLabel);
    layout->addWidget(loginEmailEdit);
    layout->addWidget(passwordLabel);
    layout->addWidget(loginPasswordEdit);
    layout->addSpacing(90);
    layout->addWidget(loginButton);
    layout->addSpacing(10);
    layout->addWidget(goToSignupButton, 0, Qt::AlignCenter);

    return widget;
}

QWidget* SignWindow::createSignupWidget() {
    QWidget* widget = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(widget);

    QLabel* title = new QLabel("Create an Account");
    title->setStyleSheet("font-size: 20px; font-weight: bold; color: #333333;");
    title->setAlignment(Qt::AlignCenter);

    QLabel* companynameLabel = new QLabel("Company Name");
    companynameLabel->setObjectName("CompanynameLabel");
    signupCompanynameEdit = createLineEdit("Choose a company name");

    QLabel* emailLabel = new QLabel("Email");
    emailLabel->setObjectName("EmailLabel");
    signupEmailEdit = createLineEdit("Choose an email");

    QLabel* passwordLabel = new QLabel("Password");
    passwordLabel->setObjectName("PasswordLabel");
    signupPasswordEdit = createLineEdit("Choose a password", true);

    QPushButton* signupButton = createButton("Sign Up", &SignWindow::onSignupClicked);
    signupButton->setObjectName("SignupButton");

    QPushButton* goToLoginButton = createButton("Already have an account? Log in", &SignWindow::showLoginForm);
    goToLoginButton->setObjectName("goToLoginButton");

    layout->addWidget(title);
    layout->addWidget(companynameLabel);
    layout->addWidget(signupCompanynameEdit);
    layout->addWidget(emailLabel);
    layout->addWidget(signupEmailEdit);
    layout->addWidget(passwordLabel);
    layout->addWidget(signupPasswordEdit);
    layout->addWidget(signupButton);
    layout->addSpacing(10);
    layout->addWidget(goToLoginButton, 0, Qt::AlignCenter);

    return widget;
}

QLineEdit* SignWindow::createLineEdit(const QString& placeholder, bool isPasswordField) {
    QLineEdit* lineEdit = new QLineEdit(this);
    lineEdit->setPlaceholderText(placeholder);
    if (isPasswordField) {
        lineEdit->setEchoMode(QLineEdit::Password);
    }
    return lineEdit;
}

QPushButton* SignWindow::createButton(const QString& text, void (SignWindow::* slot)()) {
    QPushButton* button = new QPushButton(text);
    button->setCursor(Qt::PointingHandCursor);
    connect(button, &QPushButton::clicked, this, slot);
    return button;
}

void SignWindow::showSignupForm() {
    setSignupForm();
}

void SignWindow::showLoginForm() {
    setLoginForm();
}

void SignWindow::setLoginForm() {
    loginWidget->show();
    signupWidget->hide();
}

void SignWindow::setSignupForm() {
    signupWidget->show();
    loginWidget->hide();
}


void SignWindow::onLoginClicked() {
    QString email = loginEmailEdit->text();
    QString password = loginPasswordEdit->text();


    foreach (User user, DataBase::users) {
        user.displayUser();
    }

    if (email.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Login Failed", "Please fill in both email and password.");
        return;
    }

    try {
        User user = User::findUserByEmail(DataBase::users, email.toStdString());

        if (user.getPassword() == password.toStdString()) {
            currentCompanyname = QString::fromStdString(user.getCompanyName());
            currentEmail = QString::fromStdString(user.getEmail());
            currentPassword = QString::fromStdString(user.getPassword());

            auto* mainWindow = dynamic_cast<MainWindow*>(parentWidget());
            if (mainWindow) {
                mainWindow->showAccountInfo(user);
            }

            QMessageBox::information(this, "Login Successful", "Welcome!");

            disconnect(this, &QDialog::finished, parentWidget(), &QMainWindow::close);

            this->accept();
        }
        else {
            QMessageBox::warning(this, "Login Failed", "Invalid password. Please try again.");
        }
    }
    catch (const std::runtime_error& e) {
        QMessageBox::warning(this, "Login Failed", "Account not found. Please check your email or sign up.");
    }
}

void SignWindow::onSignupClicked() {
    QString companyname = signupCompanynameEdit->text();
    QString email = signupEmailEdit->text();
    QString password = signupPasswordEdit->text();


    foreach (User user, DataBase::users) {
        user.displayUser();
    }

    if (companyname.isEmpty() || email.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Signup Failed", "Please fill in all fields.");
    }
    else {

        // Create the new user
        currentCompanyname = companyname;
        currentEmail = email;
        currentPassword = password;
        User newUser(companyname.toStdString(), email.toStdString(), password.toStdString());
        DataBase::addUser(newUser);
        // Save all users to binary file
        
        QMessageBox::information(this, "Signup Success", "Account created successfully!");
        

        // Update the main window with the new user info
        auto* mainWindow = dynamic_cast<MainWindow*>(parentWidget());
        if (mainWindow) {
            mainWindow->showAccountInfo(newUser);
        }

        disconnect(this, &QDialog::finished, parentWidget(), &QMainWindow::close);
        this->accept();
    }
}


void SignWindow::closeEvent(QCloseEvent* event) {
    if (result() == QDialog::Rejected) {
        QWidget* parentWindow = parentWidget();
        parentWindow->close();
        exit(0);
    }
    event->accept();
}
