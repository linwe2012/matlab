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
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QSlider>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MatlabGuiClass
{
public:
    QWidget *centralWidget;
    QSlider *contrast;
    QGraphicsView *view1;
    QGraphicsView *view2;
    QSlider *brightness;
    QLabel *label;
    QLabel *label_2;
    QSlider *rotate;
    QLabel *label_3;
    QSlider *width;
    QSlider *height;
    QLabel *label_4;
    QLabel *label_5;
    QCheckBox *face;
    QCheckBox *equalize;
    QCheckBox *gray;
    QCheckBox *bin;
    QStatusBar *statusBar;
    QMenuBar *menuBar;
    QMenu *menu;

    void setupUi(QMainWindow *MatlabGuiClass)
    {
        if (MatlabGuiClass->objectName().isEmpty())
            MatlabGuiClass->setObjectName(QString::fromUtf8("MatlabGuiClass"));
        MatlabGuiClass->resize(1004, 669);
        centralWidget = new QWidget(MatlabGuiClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        contrast = new QSlider(centralWidget);
        contrast->setObjectName(QString::fromUtf8("contrast"));
        contrast->setGeometry(QRect(570, 420, 160, 22));
        contrast->setMaximum(100);
        contrast->setSingleStep(1);
        contrast->setPageStep(10);
        contrast->setValue(50);
        contrast->setOrientation(Qt::Horizontal);
        view1 = new QGraphicsView(centralWidget);
        view1->setObjectName(QString::fromUtf8("view1"));
        view1->setGeometry(QRect(20, 50, 421, 321));
        view2 = new QGraphicsView(centralWidget);
        view2->setObjectName(QString::fromUtf8("view2"));
        view2->setGeometry(QRect(480, 50, 421, 321));
        brightness = new QSlider(centralWidget);
        brightness->setObjectName(QString::fromUtf8("brightness"));
        brightness->setGeometry(QRect(570, 450, 160, 22));
        brightness->setMinimum(-127);
        brightness->setMaximum(127);
        brightness->setOrientation(Qt::Horizontal);
        label = new QLabel(centralWidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(750, 420, 61, 21));
        QFont font;
        font.setFamily(QString::fromUtf8("\351\273\221\344\275\223"));
        font.setPointSize(10);
        label->setFont(font);
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(750, 450, 61, 21));
        label_2->setFont(font);
        rotate = new QSlider(centralWidget);
        rotate->setObjectName(QString::fromUtf8("rotate"));
        rotate->setGeometry(QRect(570, 500, 160, 22));
        rotate->setMinimum(-180);
        rotate->setMaximum(180);
        rotate->setOrientation(Qt::Horizontal);
        label_3 = new QLabel(centralWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(750, 500, 61, 21));
        label_3->setFont(font);
        width = new QSlider(centralWidget);
        width->setObjectName(QString::fromUtf8("width"));
        width->setGeometry(QRect(570, 530, 160, 22));
        width->setMinimum(1);
        width->setMaximum(4);
        width->setValue(1);
        width->setOrientation(Qt::Horizontal);
        height = new QSlider(centralWidget);
        height->setObjectName(QString::fromUtf8("height"));
        height->setGeometry(QRect(570, 560, 160, 22));
        height->setMinimum(1);
        height->setMaximum(4);
        height->setValue(1);
        height->setOrientation(Qt::Horizontal);
        label_4 = new QLabel(centralWidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(750, 530, 61, 21));
        label_4->setFont(font);
        label_5 = new QLabel(centralWidget);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(750, 560, 61, 21));
        label_5->setFont(font);
        face = new QCheckBox(centralWidget);
        face->setObjectName(QString::fromUtf8("face"));
        face->setGeometry(QRect(850, 420, 101, 21));
        face->setFont(font);
        equalize = new QCheckBox(centralWidget);
        equalize->setObjectName(QString::fromUtf8("equalize"));
        equalize->setGeometry(QRect(850, 450, 121, 21));
        equalize->setFont(font);
        gray = new QCheckBox(centralWidget);
        gray->setObjectName(QString::fromUtf8("gray"));
        gray->setGeometry(QRect(850, 480, 121, 21));
        gray->setFont(font);
        gray->setAutoExclusive(false);
        bin = new QCheckBox(centralWidget);
        bin->setObjectName(QString::fromUtf8("bin"));
        bin->setEnabled(true);
        bin->setGeometry(QRect(850, 510, 121, 21));
        bin->setFont(font);
        bin->setMouseTracking(true);
        bin->setAutoExclusive(false);
        MatlabGuiClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(MatlabGuiClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MatlabGuiClass->setStatusBar(statusBar);
        menuBar = new QMenuBar(MatlabGuiClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1004, 26));
        menu = new QMenu(menuBar);
        menu->setObjectName(QString::fromUtf8("menu"));
        MatlabGuiClass->setMenuBar(menuBar);

        menuBar->addAction(menu->menuAction());

        retranslateUi(MatlabGuiClass);

        QMetaObject::connectSlotsByName(MatlabGuiClass);
    } // setupUi

    void retranslateUi(QMainWindow *MatlabGuiClass)
    {
        MatlabGuiClass->setWindowTitle(QCoreApplication::translate("MatlabGuiClass", "MatlabGui", nullptr));
        label->setText(QCoreApplication::translate("MatlabGuiClass", "\345\257\271\346\257\224\345\272\246", nullptr));
        label_2->setText(QCoreApplication::translate("MatlabGuiClass", "\344\272\256\345\272\246", nullptr));
        label_3->setText(QCoreApplication::translate("MatlabGuiClass", "\346\227\213\350\275\254", nullptr));
        label_4->setText(QCoreApplication::translate("MatlabGuiClass", "\345\256\275\345\272\246", nullptr));
        label_5->setText(QCoreApplication::translate("MatlabGuiClass", "\351\253\230\345\272\246", nullptr));
        face->setText(QCoreApplication::translate("MatlabGuiClass", "\344\272\272\350\204\270\346\216\242\346\265\213", nullptr));
        equalize->setText(QCoreApplication::translate("MatlabGuiClass", "\347\233\264\346\226\271\345\233\276\345\235\207\350\241\241", nullptr));
        gray->setText(QCoreApplication::translate("MatlabGuiClass", "\347\201\260\345\272\246", nullptr));
        bin->setText(QCoreApplication::translate("MatlabGuiClass", "\344\272\214\345\200\274\345\214\226", nullptr));
        menu->setTitle(QCoreApplication::translate("MatlabGuiClass", "\346\265\213\350\257\225", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MatlabGuiClass: public Ui_MatlabGuiClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MATLAB_2D_GUI_H
