/********************************************************************************
** Form generated from reading UI file 'matlab-gui.ui'
**
** Created by: Qt User Interface Compiler version 5.13.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MATLAB_2D_GUI_H
#define UI_MATLAB_2D_GUI_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MatlabGuiClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MatlabGuiClass)
    {
        if (MatlabGuiClass->objectName().isEmpty())
            MatlabGuiClass->setObjectName(QString::fromUtf8("MatlabGuiClass"));
        MatlabGuiClass->resize(600, 400);
        menuBar = new QMenuBar(MatlabGuiClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        MatlabGuiClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MatlabGuiClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MatlabGuiClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(MatlabGuiClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        MatlabGuiClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(MatlabGuiClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MatlabGuiClass->setStatusBar(statusBar);

        retranslateUi(MatlabGuiClass);

        QMetaObject::connectSlotsByName(MatlabGuiClass);
    } // setupUi

    void retranslateUi(QMainWindow *MatlabGuiClass)
    {
        MatlabGuiClass->setWindowTitle(QCoreApplication::translate("MatlabGuiClass", "MatlabGui", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MatlabGuiClass: public Ui_MatlabGuiClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MATLAB_2D_GUI_H
