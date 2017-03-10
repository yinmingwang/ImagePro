
#include <QtWidgets/QMainWindow>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <qDebug>
#include <QLabel>
#include <QPushButton>
#include <QDialog>
#include <QIntValidator>
#include "inputimgdialog.h"
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <QImage>  
#include <QPixmap> 
#include <QMessageBox>
#include "PicChange.h"
#include "ImgFunc.h"
using namespace cv;
using namespace std;
#pragma execution_character_set("utf-8")
using namespace cv;
using namespace std;
int inputimgDialog::getheightvalue() {
	QString str = inputheight->text();
	int height = str.toInt();
	return height;
}
int inputimgDialog::getwidthvalue() {
	QString str = inputwidth->text();
	int width = str.toInt();
	return width;
}
/*void inputimgDialog::done() {
	//qDebug() << "hello" << endl;
	ImagePro *imagepro = new ImagePro;
	QImage image = imagepro->getsrclabel()->pixmap()->toImage();
	Mat image1 = QImage2Mat(image);
	imshow("test", image1);
	int w = this->getwidthvalue();
	int h = this->getheightvalue();
	Mat scaleimg = ScaleImage(image1, h, w);
	image = Mat2QImage(scaleimg);
	imagepro->getProlabel()->setPixmap(QPixmap::fromImage(image));
	imagepro->getProlabel()->alignment();
	imagepro->getProlabel()->show();
	//QMessageBox::information(this, tr("提示"), tr("保存成功"));
}*/
inputimgDialog::inputimgDialog()
{
	inputwidth = new QLineEdit;
	inputheight = new QLineEdit;
	//QRegExp rexh("^[1-9]\d$|^4[0-1]\d$");
	//QRegExp rexw("^[1-9]\d$|^2\d{2}$");
	inputheight->setValidator(new QIntValidator(10, 420, this));
	inputwidth->setValidator(new QIntValidator(10, 300, this));
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
	//connect(sure, SIGNAL(clicked()), this, SLOT(done()));
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
	window->show();
}

inputimgDialog::~inputimgDialog()
{
}