/********************************************************************************
** Form generated from reading UI file 'imagepro.ui'
**
** Created by: Qt User Interface Compiler version 5.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_IMAGEPRO_H
#define UI_IMAGEPRO_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ImageProClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *ImageProClass)
    {
        if (ImageProClass->objectName().isEmpty())
            ImageProClass->setObjectName(QStringLiteral("ImageProClass"));
        ImageProClass->resize(600, 400);
        menuBar = new QMenuBar(ImageProClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        ImageProClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(ImageProClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        ImageProClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(ImageProClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        ImageProClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(ImageProClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        ImageProClass->setStatusBar(statusBar);

        retranslateUi(ImageProClass);

        QMetaObject::connectSlotsByName(ImageProClass);
    } // setupUi

    void retranslateUi(QMainWindow *ImageProClass)
    {
        ImageProClass->setWindowTitle(QApplication::translate("ImageProClass", "ImagePro", 0));
    } // retranslateUi

};

namespace Ui {
    class ImageProClass: public Ui_ImageProClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_IMAGEPRO_H
