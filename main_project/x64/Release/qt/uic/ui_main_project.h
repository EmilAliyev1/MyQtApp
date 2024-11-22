/********************************************************************************
** Form generated from reading UI file 'main_project.ui'
**
** Created by: Qt User Interface Compiler version 6.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAIN_PROJECT_H
#define UI_MAIN_PROJECT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_main_projectClass
{
public:
    QWidget *centralWidget;
    QPushButton *pushButton;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *main_projectClass)
    {
        if (main_projectClass->objectName().isEmpty())
            main_projectClass->setObjectName("main_projectClass");
        main_projectClass->resize(600, 400);
        centralWidget = new QWidget(main_projectClass);
        centralWidget->setObjectName("centralWidget");
        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName("pushButton");
        pushButton->setGeometry(QRect(220, 130, 151, 61));
        pushButton->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        main_projectClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(main_projectClass);
        menuBar->setObjectName("menuBar");
        menuBar->setGeometry(QRect(0, 0, 600, 21));
        main_projectClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(main_projectClass);
        mainToolBar->setObjectName("mainToolBar");
        main_projectClass->addToolBar(Qt::ToolBarArea::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(main_projectClass);
        statusBar->setObjectName("statusBar");
        main_projectClass->setStatusBar(statusBar);

        retranslateUi(main_projectClass);
        QObject::connect(pushButton, &QPushButton::clicked, pushButton, qOverload<>(&QPushButton::repaint));

        QMetaObject::connectSlotsByName(main_projectClass);
    } // setupUi

    void retranslateUi(QMainWindow *main_projectClass)
    {
        main_projectClass->setWindowTitle(QCoreApplication::translate("main_projectClass", "main_project", nullptr));
        pushButton->setText(QCoreApplication::translate("main_projectClass", "PushButton", nullptr));
    } // retranslateUi

};

namespace Ui {
    class main_projectClass: public Ui_main_projectClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAIN_PROJECT_H
