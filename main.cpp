#include <QApplication>
#include "MainWindow.h"
#include "SignWindow.h"
#include <iostream>

int main(int argc, char* argv[]) {

    QApplication app(argc, argv);

    MainWindow mainWindow;
    mainWindow.show();

    SignWindow* signWindow = new SignWindow(&mainWindow);
    signWindow->exec();

    return app.exec();
}
