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

#include "Validate.h"

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

    QScreen* screen = QGuiApplication::screenAt(this->geometry().center());
    if (!screen) {
        screen = QGuiApplication::primaryScreen();
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

void SignWindow::onLoginClicked() {
    QString email = loginEmailEdit->text();
    QString password = loginPasswordEdit->text();

    if (!Validate::validateLogin(email, password, this)) {
        return;
    }

    foreach(std::shared_ptr<User> user, DataBase::users) {
        user->displayUser();
    }

    try {
        std::shared_ptr<User> user = User::findUserByEmail(email.toStdString());

        if (user && user->getPassword() == password.toStdString()) {
            currentCompanyname = QString::fromStdString(user->getCompanyName());
            currentEmail = QString::fromStdString(user->getEmail());
            currentPassword = QString::fromStdString(user->getPassword());

            user->setExchangeRate("EUR", 1.1);
            user->setExchangeRate("JPY", 0.0075);
            user->setExchangeRate("GBP", 1.25);

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

    foreach(std::shared_ptr<User> user, DataBase::users) {
        user->displayUser();
    }

    if (!Validate::validateSignup(companyname, email, password, this)) {
        return;
    }

    currentCompanyname = companyname;
    currentEmail = email;
    currentPassword = password;

    std::shared_ptr<User> newUser = std::make_shared<User>(
        companyname.toStdString(),
        email.toStdString(),
        password.toStdString()
    );

    newUser->setExchangeRate("EUR", 1.1);
    newUser->setExchangeRate("JPY", 0.0075);
    newUser->setExchangeRate("GBP", 1.25);

    DataBase::addUser(newUser);
    DataBase::saveUsers();

    QMessageBox::information(this, "Signup Success", "Account created successfully!");

    auto* mainWindow = dynamic_cast<MainWindow*>(parentWidget());
    if (mainWindow) {
        mainWindow->showAccountInfo(newUser);
    }
    disconnect(this, &QDialog::finished, parentWidget(), &QMainWindow::close);
    this->accept();
}

