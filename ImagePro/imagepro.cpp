#include "imagepro.h"
#include <QAction>
#include <QMenuBar>
#include <QMessageBox>
#include <QStatusBar>
#include <QToolBar>
#include <QDialog>
#include <QFileDialog>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <QFileDialog>
#include <qDebug>
#include <iostream>
#include <stdio.h>
#include <QImage>  
#include <QPixmap>  
#include <QLabel> 
#include <QPainter>
#include <QTextCodec>
#include "PicChange.h"
#include "ImgFunc.h"
using namespace std;
using namespace cv;
#pragma execution_character_set("utf-8")
QImage qimage;
Mat matimage;
ImagePro::ImagePro(QWidget *parent)
	: QMainWindow(parent)
{
	setWindowTitle(tr("Main Window"));
	setMaximumSize(800, 520);
	setMinimumSize(800, 520);
	srclabel = new QLabel(this);
	Prolabel = new QLabel(this);
	srclabel->setGeometry(5, 60, 600, 450);
	Prolabel->setGeometry(450, 60, 600, 450);
	createActions();
	createMenus();
	createToolBars();
}

ImagePro::~ImagePro()
{
}
void ImagePro::openFile()
{
	QString openpath = QFileDialog::getOpenFileName(this,
		tr("Select"),
		"",
		tr("Images (*.png *.bmp *.jpg *.tif *.GIF)"));
	if (openpath.isEmpty()) {
		return;
	}
	else{
		string  OpenPath = string((const char *)openpath.toLocal8Bit());
		Mat image1 = imread(OpenPath);
		matimage = image1;
		if (image1.cols > 420 || image1.rows > 300) {
			QMessageBox::information(this, tr("��ʾ"), tr("ͼ������Ѿ���СΪ���ʴ�С��ʾ"));
		}
		else if (image1.cols < 420 || image1.rows < 300) {
			QMessageBox::information(this, tr("��ʾ"), tr("ͼ���С���Ѿ��Ŵ�Ϊ���ʴ�С��ʾ"));
		}
		Mat images = Resize(image1, 300, 420);
		if (images.empty()) {
			QMessageBox::information(this,
				tr("��ͼ��ʧ��"),
				tr("��ͼ��ʧ��!"));
			return;
		}
		else {
			QImage image = Mat2QImage(images);
			qimage = image;
			srclabel->setPixmap(QPixmap::fromImage(image));
			//srclabel->resize(QSize(500, 300));
			//Prolabel->setPixmap(QPixmap::fromImage(image));
			srclabel->alignment();
			//Prolabel->alignment();
			//Prolabel->show();
			srclabel->show();
			//ui->scroll->setWidget(label);
		}
	}
}
void ImagePro::saveFile(){
	//if (srclabel->text == "") {
		//QMessageBox::information(this, tr("����"), tr("û���κ�ͼ����Ա���"));
	//}
    QImage image = Prolabel->pixmap()->toImage();
	if (image.height() == 0 || image.width() == 0) {
		QMessageBox::warning(this, tr("����"), tr("��ǰû��ͼ����Ա���"));
		close();
	}
	else {
		QMessageBox::information(this, tr("��ʾ"), tr("ͼ�����޸ģ�ȷ�����棿"));
		Mat saveimage = QImage2Mat(image);
		//imshow("test",testimage);
		QString savepath = QFileDialog::getSaveFileName(this,
			tr("Save"),
			"",
			tr("Images (*.png *.bmp *.jpg *.tif *.GIF"));
		string SavePath = string((const char *)savepath.toLocal8Bit());
		//imshow("show", matimage);
		imwrite(SavePath, saveimage);
		QMessageBox::information(this, tr("��ʾ"), tr("����ɹ�"));
	}
	
}
void ImagePro::exitFile(){
	close();
}
void ImagePro::rgbTogray() {
	QImage image = srclabel->pixmap()->toImage();
	Mat image1 = QImage2Mat(image);
	Mat tempimg = Togray(image1);
	image = Mat2QImage(tempimg);
	Prolabel->setPixmap(QPixmap::fromImage(image));
	Prolabel->alignment();
	Prolabel->show();
}
void ImagePro::rgbTobinary() {
	QImage image = srclabel->pixmap()->toImage();
	Mat image1 = QImage2Mat(image);
	Mat tempimg = Tobinary(image1);
	image = Mat2QImage(tempimg);
	Prolabel->setPixmap(QPixmap::fromImage(image));
	Prolabel->alignment();
	Prolabel->show();
}
void ImagePro::showhistogram(){
	QImage image = srclabel->pixmap()->toImage();
	Mat image1 = QImage2Mat(image);
	Mat histogramimg = Showhist(image1);
	Mat images = Resize(histogramimg, 300, 420);
	imshow("his", histogramimg);
	image = Mat2QImage(histogramimg);
	Prolabel->setPixmap(QPixmap::fromImage(image));
	Prolabel->alignment();
	Prolabel->show();
}
void ImagePro::createActions(){
	//openAction
	openAction = new QAction(QIcon("./Resources/images/open.png"), tr("&��"), this);
	openAction->setShortcuts(QKeySequence::Open);
	openAction->setStatusTip(tr("��ͼƬ"));
	connect(openAction, &QAction::triggered, this, &ImagePro::openFile);
	//saveAction
	saveAction = new QAction(QIcon("./Resources/images/save.png"), tr("&����"), this);
	saveAction->setShortcut(QKeySequence::Save);
	saveAction->setStatusTip(tr("���浱ǰͼƬ"));
	connect(saveAction, &QAction::triggered, this, &ImagePro::saveFile);
	//copyAction
	copyAction = new QAction(QIcon("./Resources/images/copy.png"), tr("&����"), this);
	copyAction->setShortcut(QKeySequence::Copy);
	copyAction->setStatusTip(tr("���Ƶ�ǰͼƬ"));
	//connect(copyAction, &QAction::triggered, this, &ImagePro::saveFile);
	//pasteAction
	pasteAction = new QAction(QIcon("./Resources/images/paste.png"), tr("&ճ��"), this);
	pasteAction->setShortcut(QKeySequence::Paste);
	pasteAction->setStatusTip(tr("ճ��"));
    //exitAction
	exitAction = new QAction(QIcon("./Resources/images/exit.png"), tr("&�˳�"), this);
	exitAction->setShortcut(QKeySequence::Close);
	exitAction->setStatusTip(tr("�˳���ǰ����"));
	connect(exitAction, &QAction::triggered, this, &ImagePro::exitFile);
	//select picture
	selectpicAction = new QAction(tr("ѡ��ͼ��"), this);
	selectpicAction->setStatusTip(tr("ѡ��Ҫ�����ͼƬ"));
	//scale
	scaleAction = new QAction(QIcon("./Resources/images/scale.png"),tr("&����"), this);
	scaleAction->setShortcut(QKeySequence::ZoomIn);
	scaleAction->setStatusTip(tr("�Ե�ǰͼ���������"));
	//togary picture
	tograyAction = new QAction(tr("&�Ҷ�ͼ"), this);
	tograyAction->setShortcut(QKeySequence::Quit);
	tograyAction->setStatusTip(tr("��ͼƬת��Ϊ�Ҷ�ͼ��"));
	connect(tograyAction, &QAction::triggered, this, &ImagePro::rgbTogray);
	//tobinary picture
	tobinaryAction = new QAction(tr("��ֵ��"),this);
	tobinaryAction->setStatusTip(tr("��ͼƬת��Ϊ��ֵͼ��"));
	connect(tobinaryAction, &QAction::triggered, this, &ImagePro::rgbTobinary);
	//show histogram
	showhisAction = new QAction(tr("ֱ��ͼ"), this);
	showhisAction->setStatusTip(tr("��ʾ��ǰͼ��ֱ��ͼ"));
	connect(showhisAction, &QAction::triggered, this, &ImagePro::showhistogram);
	statusBar();
}
void ImagePro::createMenus(){
	//fileMenus
	fileMenu = menuBar()->addMenu(tr("&�ļ�"));
	fileMenu->addAction(openAction);
	fileMenu->addAction(saveAction);
	fileMenu->addAction(exitAction);
	editMenu = menuBar()->addMenu(tr("&�༭"));
	editMenu->addAction(copyAction);
	editMenu->addAction(pasteAction);
	editMenu->addAction(selectpicAction);
	//function
	selectFun = menuBar()->addMenu(tr("&����"));
	selectFun->addAction(scaleAction);
	selectFun->addAction(tograyAction);
	selectFun->addAction(tobinaryAction);
	selectFun->addAction(showhisAction);
	//help
	helpMenu = menuBar()->addMenu(tr("&����"));
	/*helpMenu->addAction(aboutAction);
	helpMenu->addAction(aboutImageProAction);*/
	
	
}
void ImagePro::createToolBars(){
	fileToolBar = addToolBar(tr("&�ļ�"));
	fileToolBar->addAction(openAction);
	fileToolBar->addAction(saveAction);
	//fileToolBar->addAction(saveAsAction);
	fileToolBar->addAction(exitAction);
	editToolBar = addToolBar(tr("&�༭"));
	editToolBar->addAction(copyAction);
	//editToolBar->addAction(cutAction);
	editToolBar->addAction(pasteAction);

}