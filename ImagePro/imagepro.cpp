#include "imagepro.h"
#include <QAction>
#include <QMenuBar>
#include <QMessageBox>
#include <QStatusBar>
#include <QToolBar>
#include <QDialog>
#include <QFileDialog>
#include <opencv2/core/core.hpp>
#include<opencv2/opencv.hpp> 
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
int rowsize = 0;
int colsize = 0;
int rotateangle = 0;
QLabel *srclabel;
QLabel *Prolabel;
ImagePro::ImagePro(QWidget *parent)
	: QMainWindow(parent)
{
	setWindowTitle(tr("Eric"));
	setMaximumSize(1000, 1200);
	setMinimumSize(1000, 1200);
	srclabel = new QLabel(this);
	Prolabel = new QLabel(this);
	srcscroll = new QScrollArea(this);
	Proscroll = new QScrollArea(this);
	srcscroll->setGeometry(15, 60, 420, 550);
	Proscroll->setGeometry(500, 60, 420, 550);
	srcscroll->setWidget(srclabel);
	Proscroll->setWidget(Prolabel);
	srcscroll->show();
	createActions();
	createMenus();
	createToolBars();
}

ImagePro::~ImagePro()
{
}
void ImagePro::openFile()
{
	Mat images;
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
		//matimage = image1;
		qDebug() << image1.cols << " " << image1.rows << endl;
		if (image1.cols > 600 || image1.rows > 420) {
			QMessageBox::information(this, tr("��ʾ"), tr("ͼ������Ѿ���СΪ���ʴ�С��ʾ"));
			images = image1;
		}
		else {
			images = image1;
		}
		
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
			srclabel->resize(QSize(image.width(), image.height()));
			srclabel->show();
			Prolabel->clear();
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
		if (!SavePath.empty()) {
			imwrite(SavePath, saveimage);
			QMessageBox::information(this, tr("��ʾ"), tr("����ɹ�"));
		}
		else {
			QMessageBox::information(this, tr("��ʾ"), tr("����ʧ��"));
			close();
		}
		
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
	Prolabel->resize(QSize(image.width(), image.height()));
	Prolabel->alignment();
	Prolabel->show();
}
void ImagePro::rgbTobinary() {
	QImage image = srclabel->pixmap()->toImage();
	Mat image1 = QImage2Mat(image);
	Mat tempimg = Tobinary(image1);
	image = Mat2QImage(tempimg);
	Prolabel->setPixmap(QPixmap::fromImage(image));
	Prolabel->resize(QSize(image.width(), image.height()));
	Prolabel->alignment();
	Prolabel->show();
}
void ImagePro::showhistogram(){
	QImage image = srclabel->pixmap()->toImage();
	Mat image1 = QImage2Mat(image);
	Mat histogramimg = Showhist(image1);
	image = Mat2QImage(histogramimg);
	Prolabel->setPixmap(QPixmap::fromImage(image));
	Prolabel->resize(QSize(image.width(), image.height()));
	Prolabel->show();
	
}
/*QLabel* ImagePro::getProlabel() {
		return Prolabel;
}
QLabel* ImagePro::getsrclabel() {
	
	return srclabel;
}*/
void  scale(int w, void*) {
	
	Mat image1 = QImage2Mat(qimage);
	//imshow("hello", image1);
	//Mat scaleimage;
	if (colsize < 10)
	{
		//QMessageBox::warning(NULL,QObject::tr("�߶�"), QObject::tr("�߶Ȳ���С��10"));
		colsize = 10;
	}
	else if (rowsize < 10) {
		//QMessageBox::warning(NULL, QObject::tr("���"), QObject::tr("��Ȳ���С��10"));
		rowsize = 10;
	}
 	cv::resize(image1, image1, Size(colsize, rowsize), 0, 0, 3);
	QImage  image = Mat2QImage(image1);
	Prolabel->setPixmap(QPixmap::fromImage(image));
	Prolabel->resize(QSize(image.width(), image.height()));
	Prolabel->show();
	//imshow("test", image1);
}
void ImagePro::scaleimg() {
	QImage image = srclabel->pixmap()->toImage();
	//inputimgDialog *inputdia = new inputimgDialog;
	Mat image1 = QImage2Mat(image);
	rowsize = image1.rows/2;
	colsize = image1.cols/2;
	//QString strQ = QString::fromLocal8Bit(str.c_str());
	namedWindow("ScaleBox");
	createTrackbar("rows", "ScaleBox", &rowsize, image1.rows * 2, scale);
	createTrackbar("cols", "ScaleBox", &colsize, image1.cols * 2, scale);
}
void rotate(int, void*) {
	Mat image1 = QImage2Mat(qimage);
	Mat roimage = rotation(image1, rotateangle);
	QImage  image = Mat2QImage(roimage);
	Prolabel->setPixmap(QPixmap::fromImage(image));
	Prolabel->resize(QSize(image.width(), image.height()));
	Prolabel->show();
}
Mat reverseimg(Mat srcimage) {
	
	Mat reverseimages;
	//channel[0] = srcimage;
	
	//merge(channel, 3, reverseimages);
	//imshow("B", channel[0]);
	return srcimage;
}
void ImagePro::rotateimage() {
	QImage image = srclabel->pixmap()->toImage();
	Mat image1 = QImage2Mat(image);
	namedWindow("RotateBox");
	createTrackbar("angle", "RotateBox", &rotateangle, 360, rotate);
}
void ImagePro::flipimage() {
	QImage image = srclabel->pixmap()->toImage();
	Mat image1 = QImage2Mat(image);
	Mat tempimg = FlipImages(image1);
	image = Mat2QImage(tempimg);
	Prolabel->setPixmap(QPixmap::fromImage(image));
	Prolabel->resize(QSize(image.width(), image.height()));
	Prolabel->alignment();
	Prolabel->show();
}
void ImagePro::reverseimage() {
	QImage image = srclabel->pixmap()->toImage();
	Mat image1 = QImage2Mat(image);
	vector<Mat> channel;
	split(image1, channel);
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < channel[i].rows; j++) {
			for (int k = 0; k < channel[i].cols; k++) {
				channel[i].at<uchar>(j, k) = 255 - channel[i].at<uchar>(j, k);
			}
		}
	}
	Mat tempimg;
	merge(channel,tempimg);
	image = Mat2QImage(tempimg);
	Prolabel->setPixmap(QPixmap::fromImage(image));
	Prolabel->resize(QSize(image.width(), image.height()));
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
	connect(scaleAction, &QAction::triggered, this, &ImagePro::scaleimg);
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
	//rotate picture
	rotateAction = new QAction(tr("��ת"), this);
	rotateAction->setStatusTip(tr("��ת��ǰͼƬ"));
	connect(rotateAction, &QAction::triggered, this, &ImagePro::rotateimage);
	//flip picture
	flipAction = new QAction(tr("����"), this);
	flipAction->setStatusTip(tr("���ɾ���"));
	connect(flipAction, &QAction::triggered, this, &ImagePro::flipimage);
	//reverse color
	reverseAction = new QAction(tr("��ɫ"), this);
	reverseAction->setStatusTip(tr("���ɷ�ɫͼ��"));
	connect(reverseAction, &QAction::triggered, this, &ImagePro::reverseimage);
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
	selectFun->addAction(rotateAction);
	selectFun->addAction(flipAction);
	selectFun->addAction(reverseAction);
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