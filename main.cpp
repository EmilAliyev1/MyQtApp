#include <QApplication>
#include "MainWindow.h"
#include "SignWindow.h"
#include "User.h"
#include <iostream>

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);



    MainWindow mainWindow;
    SignWindow* signWindow = new SignWindow(&mainWindow);

    // Connect the windows to ensure the app quits properly
    QObject::connect(&mainWindow, &QMainWindow::destroyed, &app, &QApplication::quit);
    QObject::connect(signWindow, &QDialog::destroyed, &app, &QApplication::quit);

    mainWindow.show();
    signWindow->show();

    return app.exec();
}