
#include <QtWidgets/QMainWindow>
#include <QLineEdit>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <qDebug>
#include <iostream>
#include <QLabel>
#include <QPushButton>
#include <QDialog>
#include "inputimgdialog.h"
#pragma execution_character_set("utf-8")
using namespace cv;
using namespace std;
inputimgDialog::inputimgDialog()
{
	inputwidth = new QLineEdit;
	inputheight = new QLineEdit;
	Vlayout = new QVBoxLayout;
	HWlayout = new QHBoxLayout;
	HHlayout = new QHBoxLayout;
	mainLayout = new QVBoxLayout;
	width = new QLabel;
	height = new QLabel;
	sure = new QPushButton;
	sure->setText(QObject::tr("确定"));
	sure->setFixedSize(50, 30);
	//sure->setStyleSheet("QPushButton{" "border-top-left-radius:11px;" "}");
	window = new QWidget;
	width->setText(QObject::tr("宽度"));
	width->setBuddy(inputwidth);
	height->setText(QObject::tr("高度"));
	height->setBuddy(inputheight);
	HWlayout->addWidget(width);
	HWlayout->addWidget(inputwidth);
	HHlayout->addWidget(height);
	HHlayout->addWidget(inputheight);
	mainLayout->addLayout(HWlayout);
	mainLayout->addLayout(HHlayout);
	mainLayout->addWidget(sure);
	window->setLayout(mainLayout);
	//window->show();
}

inputimgDialog::~inputimgDialog()
{
}