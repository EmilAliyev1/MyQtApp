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
#include <QRegularExpression>

SignWindow::SignWindow(MainWindow* parent)
    : QDialog(parent), mainWindow(parent) {

    setWindowTitle("Company Authentication");
    setFixedSize(550, 500);
    setModal(true);

    DataBase::initializeId();

    connect(this, &QDialog::finished, parent, &QMainWindow::close);

    loadStylesheet();
    setupUI();

    setLoginForm();

    // Center the window on the screen
    QScreen* screen = QGuiApplication::screenAt(this->geometry().center());
    if (!screen) {
        screen = QGuiApplication::primaryScreen();  // Fallback to primary screen
    }
    QRect screenGeometry = screen->geometry();
    int x = (screenGeometry.width() - width()) / 2;
    int y = (screenGeometry.height() - height()) / 2;
    move(x, y - 100);
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

    loginWidget = createLoginWidget();
    signupWidget = createSignupWidget();

    mainLayout->addWidget(loginWidget);
    mainLayout->addWidget(signupWidget);
    signupWidget->hide();
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

    QPushButton* loginButton = new QPushButton("Login");
    connect(loginButton, &QPushButton::clicked, this, &SignWindow::onLoginClicked);
    loginButton->setCursor(Qt::PointingHandCursor);
    loginButton->setObjectName("LoginButton");

    QPushButton* goToSignupButton = new QPushButton("Create an account");
    connect(goToSignupButton, &QPushButton::clicked, this, &SignWindow::showSignupForm);
    goToSignupButton->setCursor(Qt::PointingHandCursor);
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

    QPushButton* signupButton = new QPushButton("Sign Up");
    connect(signupButton, &QPushButton::clicked, this, &SignWindow::onSignupClicked);
    signupButton->setCursor(Qt::PointingHandCursor);
    signupButton->setObjectName("SignupButton");

    QPushButton* goToLoginButton = new QPushButton("Already have an account? Log in");
    connect(goToLoginButton, &QPushButton::clicked, this, &SignWindow::showLoginForm);
    goToLoginButton->setCursor(Qt::PointingHandCursor);
    goToLoginButton->setObjectName("goToLoginButton");

    // Add widgets
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

bool SignWindow::validateSignup(const QString& companyname, const QString& email, const QString& password) {
    // Check if any field is empty
    if (companyname.isEmpty() || email.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Signup Failed", "Please fill in all fields.");
        return false;
    }

    // Simple company name validation (must be at least 3 characters)
    if (companyname.length() < 3) {
        QMessageBox::warning(this, "Signup Failed", "Company name must be at least 3 characters long.");
        return false;
    }

    // Check for exactly one '@' symbol in email
    if (email.count('@') != 1) {
        QMessageBox::warning(this, "Signup Failed", "Invalid email address.\n\nEmail should contain exactly one '@' symbol.");
        return false;
    }

    // Split email into local and domain parts
    QStringList emailParts = email.split('@');
    if (emailParts.size() != 2) {
        QMessageBox::warning(this, "Signup Failed", "Invalid email address format.");
        return false;
    }

    QString localPart = emailParts[0];
    QString domainPart = emailParts[1];

    // Validate the domain part
    if (domainPart != "gmail.com" && domainPart != "outlook.com" && domainPart != "hotmail.com") {
        QMessageBox::warning(this, "Signup Failed", "Invalid email domain.\n\nThe supported email domains are: \ngmail.com, outlook.com, or hotmail.com.");
        return false;
    }

    // Validate the local part (basic rules: non-empty and valid characters)
    QRegularExpression localPartRegex("^[a-zA-Z0-9._%+-]+$"); // Allows letters, numbers, '.', '_', '%', '+', '-'
    QRegularExpressionMatch match = localPartRegex.match(localPart);

    if (localPart.isEmpty() || !match.hasMatch()) {
        QMessageBox::warning(this, "Signup Failed", "Invalid email address.\n\nThe part before '@' contains invalid characters.");
        return false;
    }

    // Simple password validation (must be at least 6 characters)
    if (password.length() < 6) {
        QMessageBox::warning(this, "Signup Failed", "Password must be at least 6 characters long.");
        return false;
    }

    // Check if company name is already taken
    if (companyname.toStdString() == User::findUserByCompanyName(companyname).getCompanyName()) {
        QMessageBox::warning(this, "Signup Failed", "Another user with the exact Company name was found.");
        return false;
    }

    // Check if email is already taken
    if (email.toStdString() == User::findUserByEmail(email).getEmail()) {
        QMessageBox::warning(this, "Signup Failed", "Another user with the exact Email was found.");
        return false;
    }

    // If all checks pass, return true
    return true;
}

void SignWindow::onLoginClicked() {
    QString email = loginEmailEdit->text();
    QString password = loginPasswordEdit->text();



    if (email.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Login Failed", "Please fill in both email and password.");
        return;
    }

    try {
        User user = User::findUserByEmail(email);

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

    User user = User::findUserByEmail(email);


    if (!validateSignup(companyname, email, password)) {
        // If validation fails, return early (no need to proceed)
        return;
    }

    // If all validations pass
    currentCompanyname = companyname;
    currentEmail = email;
    currentPassword = password;

    // Create the new user
    User newUser(companyname.toStdString(), email.toStdString(), password.toStdString());
    DataBase::addUser(newUser);

    // Show success message
    QMessageBox::information(this, "Signup Success", "Account created successfully!");

    // Pass user info to the main window
    auto* mainWindow = dynamic_cast<MainWindow*>(parentWidget());
    if (mainWindow) {
        mainWindow->showAccountInfo(newUser);
    }

    disconnect(this, &QDialog::finished, parentWidget(), &QMainWindow::close);
    this->accept();
}



