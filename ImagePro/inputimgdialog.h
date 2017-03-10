#ifndef INPUTIMGDIALOG_H
#define INPUTIMGDIALOG_H
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
#include "imagepro.h"
#pragma execution_character_set("utf-8")
using namespace cv;
using namespace std;
class  inputimgDialog : public QWidget
{
	Q_OBJECT

public:
	inputimgDialog();
	~inputimgDialog();
	int getheightvalue();
	int getwidthvalue();
	void done();

private:
	QLineEdit *inputwidth;
	QLineEdit *inputheight;
	QVBoxLayout *Vlayout;
	QHBoxLayout *HWlayout;
	QHBoxLayout *HHlayout;
	QVBoxLayout *mainLayout;
	QLabel *width;
	QLabel *height;
	QWidget *window;
	QPushButton *sure;
	//ImagePro *imagepro;

};
#endif