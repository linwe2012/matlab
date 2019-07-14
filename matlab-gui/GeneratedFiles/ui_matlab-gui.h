/********************************************************************************
** Form generated from reading UI file 'matlab-gui.ui'
**
** Created by: Qt User Interface Compiler version 5.12.3
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
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
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
    QWidget *verticalLayoutWidget;
    QVBoxLayout *layout;
    QSlider *penSize;
    QLabel *label_6;
    QSlider *penColorR;
    QLabel *label_7;
    QSlider *penColorG;
    QLabel *label_8;
    QSlider *penColorB;
    QLabel *label_9;
    QPushButton *cancel;
    QLabel *color;
    QLabel *RDisp;
    QLabel *GDisp;
    QLabel *BDisp;
    QLabel *sizeDisp;
    QLabel *contrastDisp;
    QLabel *brightDisp;
    QLabel *rotDisp;
    QLabel *widthDisp;
    QLabel *heightDisp;
    QStatusBar *statusBar;
    QMenuBar *menuBar;
    QMenu *menu;

    void setupUi(QMainWindow *MatlabGuiClass)
    {
        if (MatlabGuiClass->objectName().isEmpty())
            MatlabGuiClass->setObjectName(QString::fromUtf8("MatlabGuiClass"));
        MatlabGuiClass->resize(1098, 893);
        centralWidget = new QWidget(MatlabGuiClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        contrast = new QSlider(centralWidget);
        contrast->setObjectName(QString::fromUtf8("contrast"));
        contrast->setGeometry(QRect(560, 550, 160, 22));
        contrast->setMaximum(100);
        contrast->setSingleStep(1);
        contrast->setPageStep(10);
        contrast->setValue(50);
        contrast->setOrientation(Qt::Horizontal);
        view1 = new QGraphicsView(centralWidget);
        view1->setObjectName(QString::fromUtf8("view1"));
        view1->setGeometry(QRect(20, 20, 461, 461));
        view2 = new QGraphicsView(centralWidget);
        view2->setObjectName(QString::fromUtf8("view2"));
        view2->setGeometry(QRect(570, 20, 461, 461));
        brightness = new QSlider(centralWidget);
        brightness->setObjectName(QString::fromUtf8("brightness"));
        brightness->setGeometry(QRect(560, 580, 160, 22));
        brightness->setMinimum(-127);
        brightness->setMaximum(127);
        brightness->setOrientation(Qt::Horizontal);
        label = new QLabel(centralWidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(730, 550, 61, 21));
        QFont font;
        font.setFamily(QString::fromUtf8("\351\273\221\344\275\223"));
        font.setPointSize(10);
        label->setFont(font);
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(740, 580, 61, 21));
        label_2->setFont(font);
        rotate = new QSlider(centralWidget);
        rotate->setObjectName(QString::fromUtf8("rotate"));
        rotate->setGeometry(QRect(560, 630, 160, 22));
        rotate->setMinimum(-180);
        rotate->setMaximum(180);
        rotate->setOrientation(Qt::Horizontal);
        label_3 = new QLabel(centralWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(740, 630, 61, 21));
        label_3->setFont(font);
        width = new QSlider(centralWidget);
        width->setObjectName(QString::fromUtf8("width"));
        width->setGeometry(QRect(560, 660, 160, 22));
        width->setMinimum(1);
        width->setMaximum(8);
        width->setValue(2);
        width->setOrientation(Qt::Horizontal);
        height = new QSlider(centralWidget);
        height->setObjectName(QString::fromUtf8("height"));
        height->setGeometry(QRect(560, 690, 160, 22));
        height->setMinimum(1);
        height->setMaximum(8);
        height->setValue(2);
        height->setOrientation(Qt::Horizontal);
        label_4 = new QLabel(centralWidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(740, 660, 61, 21));
        label_4->setFont(font);
        label_5 = new QLabel(centralWidget);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(740, 690, 61, 21));
        label_5->setFont(font);
        face = new QCheckBox(centralWidget);
        face->setObjectName(QString::fromUtf8("face"));
        face->setGeometry(QRect(890, 550, 101, 21));
        face->setFont(font);
        equalize = new QCheckBox(centralWidget);
        equalize->setObjectName(QString::fromUtf8("equalize"));
        equalize->setGeometry(QRect(890, 580, 121, 21));
        equalize->setFont(font);
        gray = new QCheckBox(centralWidget);
        gray->setObjectName(QString::fromUtf8("gray"));
        gray->setGeometry(QRect(890, 610, 121, 21));
        gray->setFont(font);
        gray->setAutoExclusive(false);
        bin = new QCheckBox(centralWidget);
        bin->setObjectName(QString::fromUtf8("bin"));
        bin->setEnabled(true);
        bin->setGeometry(QRect(890, 640, 121, 21));
        bin->setFont(font);
        bin->setMouseTracking(true);
        bin->setAutoExclusive(false);
        verticalLayoutWidget = new QWidget(centralWidget);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(570, 20, 431, 431));
        layout = new QVBoxLayout(verticalLayoutWidget);
        layout->setSpacing(6);
        layout->setContentsMargins(11, 11, 11, 11);
        layout->setObjectName(QString::fromUtf8("layout"));
        layout->setContentsMargins(0, 0, 0, 0);
        penSize = new QSlider(centralWidget);
        penSize->setObjectName(QString::fromUtf8("penSize"));
        penSize->setGeometry(QRect(50, 550, 160, 22));
        penSize->setMinimum(1);
        penSize->setMaximum(15);
        penSize->setOrientation(Qt::Horizontal);
        label_6 = new QLabel(centralWidget);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(230, 550, 81, 21));
        label_6->setFont(font);
        penColorR = new QSlider(centralWidget);
        penColorR->setObjectName(QString::fromUtf8("penColorR"));
        penColorR->setGeometry(QRect(50, 610, 160, 22));
        penColorR->setMaximum(255);
        penColorR->setValue(255);
        penColorR->setSliderPosition(255);
        penColorR->setOrientation(Qt::Horizontal);
        label_7 = new QLabel(centralWidget);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(230, 610, 101, 21));
        label_7->setFont(font);
        penColorG = new QSlider(centralWidget);
        penColorG->setObjectName(QString::fromUtf8("penColorG"));
        penColorG->setGeometry(QRect(50, 650, 160, 22));
        penColorG->setMaximum(255);
        penColorG->setValue(0);
        penColorG->setOrientation(Qt::Horizontal);
        label_8 = new QLabel(centralWidget);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(230, 650, 91, 21));
        label_8->setFont(font);
        penColorB = new QSlider(centralWidget);
        penColorB->setObjectName(QString::fromUtf8("penColorB"));
        penColorB->setGeometry(QRect(50, 690, 160, 22));
        penColorB->setMaximum(255);
        penColorB->setValue(0);
        penColorB->setOrientation(Qt::Horizontal);
        label_9 = new QLabel(centralWidget);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(230, 690, 91, 21));
        label_9->setFont(font);
        cancel = new QPushButton(centralWidget);
        cancel->setObjectName(QString::fromUtf8("cancel"));
        cancel->setGeometry(QRect(110, 740, 91, 31));
        QFont font1;
        font1.setFamily(QString::fromUtf8("\351\273\221\344\275\223"));
        font1.setPointSize(11);
        cancel->setFont(font1);
        color = new QLabel(centralWidget);
        color->setObjectName(QString::fromUtf8("color"));
        color->setGeometry(QRect(400, 620, 72, 71));
        RDisp = new QLabel(centralWidget);
        RDisp->setObjectName(QString::fromUtf8("RDisp"));
        RDisp->setGeometry(QRect(330, 610, 51, 21));
        RDisp->setFont(font);
        GDisp = new QLabel(centralWidget);
        GDisp->setObjectName(QString::fromUtf8("GDisp"));
        GDisp->setGeometry(QRect(330, 650, 51, 21));
        GDisp->setFont(font);
        BDisp = new QLabel(centralWidget);
        BDisp->setObjectName(QString::fromUtf8("BDisp"));
        BDisp->setGeometry(QRect(330, 690, 51, 21));
        BDisp->setFont(font);
        sizeDisp = new QLabel(centralWidget);
        sizeDisp->setObjectName(QString::fromUtf8("sizeDisp"));
        sizeDisp->setGeometry(QRect(330, 550, 51, 21));
        sizeDisp->setFont(font);
        contrastDisp = new QLabel(centralWidget);
        contrastDisp->setObjectName(QString::fromUtf8("contrastDisp"));
        contrastDisp->setGeometry(QRect(800, 550, 51, 21));
        contrastDisp->setFont(font);
        brightDisp = new QLabel(centralWidget);
        brightDisp->setObjectName(QString::fromUtf8("brightDisp"));
        brightDisp->setGeometry(QRect(800, 580, 51, 21));
        brightDisp->setFont(font);
        rotDisp = new QLabel(centralWidget);
        rotDisp->setObjectName(QString::fromUtf8("rotDisp"));
        rotDisp->setGeometry(QRect(800, 630, 51, 21));
        rotDisp->setFont(font);
        widthDisp = new QLabel(centralWidget);
        widthDisp->setObjectName(QString::fromUtf8("widthDisp"));
        widthDisp->setGeometry(QRect(800, 660, 51, 21));
        widthDisp->setFont(font);
        heightDisp = new QLabel(centralWidget);
        heightDisp->setObjectName(QString::fromUtf8("heightDisp"));
        heightDisp->setGeometry(QRect(800, 690, 51, 21));
        heightDisp->setFont(font);
        MatlabGuiClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(MatlabGuiClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MatlabGuiClass->setStatusBar(statusBar);
        menuBar = new QMenuBar(MatlabGuiClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1098, 26));
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
        label_6->setText(QCoreApplication::translate("MatlabGuiClass", "\347\224\273\347\254\224\347\262\227\347\273\206", nullptr));
        label_7->setText(QCoreApplication::translate("MatlabGuiClass", "\347\224\273\347\254\224\351\242\234\350\211\262-R", nullptr));
        label_8->setText(QCoreApplication::translate("MatlabGuiClass", "\347\224\273\347\254\224\351\242\234\350\211\262-G", nullptr));
        label_9->setText(QCoreApplication::translate("MatlabGuiClass", "\347\224\273\347\254\224\351\242\234\350\211\262-B", nullptr));
        cancel->setText(QCoreApplication::translate("MatlabGuiClass", "\347\273\230\347\224\273\346\222\244\351\224\200", nullptr));
        color->setText(QString());
        RDisp->setText(QString());
        GDisp->setText(QString());
        BDisp->setText(QString());
        sizeDisp->setText(QString());
        contrastDisp->setText(QString());
        brightDisp->setText(QString());
        rotDisp->setText(QString());
        widthDisp->setText(QString());
        heightDisp->setText(QString());
        menu->setTitle(QCoreApplication::translate("MatlabGuiClass", "\346\265\213\350\257\225", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MatlabGuiClass: public Ui_MatlabGuiClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MATLAB_2D_GUI_H
