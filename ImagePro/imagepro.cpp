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
int contrastvalue = 0;
int brightvalue = 0;
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
			QMessageBox::information(this, tr("提示"), tr("图像过大，已经缩小为合适大小显示"));
			images = image1;
		}
		else {
			images = image1;
		}
		
		if (images.empty()) {
			QMessageBox::information(this,
				tr("打开图像失败"),
				tr("打开图像失败!"));
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
		//QMessageBox::information(this, tr("警告"), tr("没有任何图像可以保存"));
	//}
	
    QImage image = Prolabel->pixmap()->toImage();
	if (image.height() == 0 || image.width() == 0) {
		QMessageBox::warning(this, tr("警告"), tr("当前没有图像可以保存"));
		close();
	}
	else {
		QMessageBox::information(this, tr("提示"), tr("图像已修改，确定保存？"));
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
			QMessageBox::information(this, tr("提示"), tr("保存成功"));
		}
		else {
			QMessageBox::information(this, tr("提示"), tr("保存失败"));
			close();
		}
		
	}
	
}
void ImagePro::exitFile(){
	close();
}
void ImagePro::exchangepicture() {
	QImage image = Prolabel->pixmap()->toImage();
	qimage = image;
	srclabel->clear();
	srclabel->setPixmap(QPixmap::fromImage(image));
	srclabel->resize(QSize(image.width(), image.height()));
	srclabel->show();
	Prolabel->clear();

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
		//QMessageBox::warning(NULL,QObject::tr("高度"), QObject::tr("高度不能小于10"));
		colsize = 10;
	}
	else if (rowsize < 10) {
		//QMessageBox::warning(NULL, QObject::tr("宽度"), QObject::tr("宽度不能小于10"));
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
	namedWindow("ScaleBox",1);
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
	namedWindow("RotateBox",1);
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
void ModifiyContrastAndBright(int, void*) {
	Mat image1 = QImage2Mat(qimage);
	/*if (contrastvalue <= 0) {
		contrastvalue = 1;
	}
	if (brightvalue <= 0) {
		brightvalue = 1;
	}*/
	Mat CAB = ChangeContrastAndBright(image1, contrastvalue, brightvalue);
	QImage  image = Mat2QImage(CAB);
	Prolabel->setPixmap(QPixmap::fromImage(image));
	Prolabel->resize(QSize(image.width(), image.height()));
	Prolabel->show();
}
void ImagePro::contrastAndbright() {
	contrastvalue = 80;
	brightvalue = 80;
	namedWindow("ContrastAndBrightBox", 1);
	createTrackbar("ContrastValue", "ContrastAndBrightBox", &contrastvalue, 300, ModifiyContrastAndBright);
	createTrackbar("BrightValue", "ContrastAndBrightBox", &brightvalue, 300, ModifiyContrastAndBright);
}
void ImagePro::edgebyLaplacian() {
	QImage image = srclabel->pixmap()->toImage();
	Mat image1 = QImage2Mat(image);
	Mat laplacianimage = EdgeDetection_Laplacian(image1);
	image = Mat2QImage(laplacianimage);
	Prolabel->setPixmap(QPixmap::fromImage(image));
	Prolabel->resize(QSize(image.width(), image.height()));
	//Prolabel->alignment();
	Prolabel->show();
}
void ImagePro::edgebySobel_X() {
	QImage image = srclabel->pixmap()->toImage();
	Mat image1 = QImage2Mat(image);
	Mat sobel_x = EdgeDetection_Sobel_X(image1);
	image = Mat2QImage(sobel_x);
	Prolabel->setPixmap(QPixmap::fromImage(image));
	Prolabel->resize(QSize(image.width(), image.height()));
	//Prolabel->alignment();
	Prolabel->show();
}
void ImagePro::edgebySobel_Y() {
	QImage image = srclabel->pixmap()->toImage();
	Mat image1 = QImage2Mat(image);
	Mat sobel_y = EdgeDetection_Sobel_Y(image1);
	image = Mat2QImage(sobel_y);
	Prolabel->setPixmap(QPixmap::fromImage(image));
	Prolabel->resize(QSize(image.width(), image.height()));
	//Prolabel->alignment();
	Prolabel->show();
}
void ImagePro::edgebySobel_XY() {
	QImage image = srclabel->pixmap()->toImage();
	Mat image1 = QImage2Mat(image);
	Mat sobelimage = EdgeDetection_Sobel(image1);
	image = Mat2QImage(sobelimage);
	Prolabel->setPixmap(QPixmap::fromImage(image));
	Prolabel->resize(QSize(image.width(), image.height()));
	//Prolabel->alignment();
	Prolabel->show();
}
void ImagePro::createActions(){
	//openAction
	openAction = new QAction(QIcon("./Resources/images/open.png"), tr("&打开"), this);
	openAction->setShortcuts(QKeySequence::Open);
	openAction->setStatusTip(tr("打开图片"));
	connect(openAction, &QAction::triggered, this, &ImagePro::openFile);
	//saveAction
	saveAction = new QAction(QIcon("./Resources/images/save.png"), tr("&保存"), this);
	saveAction->setShortcut(QKeySequence::Save);
	saveAction->setStatusTip(tr("保存当前图片"));
	connect(saveAction, &QAction::triggered, this, &ImagePro::saveFile);
	//copyAction
	copyAction = new QAction(QIcon("./Resources/images/copy.png"), tr("&复制"), this);
	copyAction->setShortcut(QKeySequence::Copy);
	copyAction->setStatusTip(tr("复制当前图片"));
	//connect(copyAction, &QAction::triggered, this, &ImagePro::saveFile);
	//pasteAction
	pasteAction = new QAction(QIcon("./Resources/images/paste.png"), tr("&粘贴"), this);
	pasteAction->setShortcut(QKeySequence::Paste);
	pasteAction->setStatusTip(tr("粘贴"));
    //exitAction
	exitAction = new QAction(QIcon("./Resources/images/exit.png"), tr("&退出"), this);
	exitAction->setShortcut(QKeySequence::Close);
	exitAction->setStatusTip(tr("退出当前窗口"));
	connect(exitAction, &QAction::triggered, this, &ImagePro::exitFile);
	//select picture
	selectpicAction = new QAction(tr("切换图像"), this);
	selectpicAction->setStatusTip(tr("切换处理的图片"));
	connect(selectpicAction, &QAction::triggered, this, &ImagePro::exchangepicture);
	//scale
	scaleAction = new QAction(QIcon("./Resources/images/scale.png"),tr("&缩放"), this);
	scaleAction->setShortcut(QKeySequence::ZoomIn);
	scaleAction->setStatusTip(tr("对当前图像进行缩放"));
	connect(scaleAction, &QAction::triggered, this, &ImagePro::scaleimg);
	//togary picture
	tograyAction = new QAction(tr("&灰度图"), this);
	tograyAction->setShortcut(QKeySequence::Quit);
	tograyAction->setStatusTip(tr("把图片转化为灰度图像"));
	connect(tograyAction, &QAction::triggered, this, &ImagePro::rgbTogray);
	//tobinary picture
	tobinaryAction = new QAction(tr("二值化"),this);
	tobinaryAction->setStatusTip(tr("把图片转化为二值图像"));
	connect(tobinaryAction, &QAction::triggered, this, &ImagePro::rgbTobinary);
	//show histogram
	showhisAction = new QAction(tr("直方图"), this);
	showhisAction->setStatusTip(tr("显示当前图像直方图"));
	connect(showhisAction, &QAction::triggered, this, &ImagePro::showhistogram);
	//rotate picture
	rotateAction = new QAction(tr("旋转"), this);
	rotateAction->setStatusTip(tr("旋转当前图片"));
	connect(rotateAction, &QAction::triggered, this, &ImagePro::rotateimage);
	//flip picture
	flipAction = new QAction(tr("镜像"), this);
	flipAction->setStatusTip(tr("生成镜像"));
	connect(flipAction, &QAction::triggered, this, &ImagePro::flipimage);
	//reverse color
	reverseAction = new QAction(tr("反色"), this);
	reverseAction->setStatusTip(tr("生成反色图像"));
	connect(reverseAction, &QAction::triggered, this, &ImagePro::reverseimage);
	//modifiy picture
	contrastandbrightAction = new QAction(tr("对比度和亮度"), this);
	contrastandbrightAction->setStatusTip(tr("调整对比度和凉度"));
	connect(contrastandbrightAction, &QAction::triggered, this, &ImagePro::contrastAndbright);
	//edge detection
	LaplacianAction = new QAction(tr("Laplacian"), this);
	LaplacianAction->setStatusTip(tr("基于Laplacian的边缘检测"));
	connect(LaplacianAction, &QAction::triggered, this, &ImagePro::edgebyLaplacian);
	Sobel_X_Action = new QAction(tr("X方向"), this);
	connect(Sobel_X_Action, &QAction::triggered, this, &ImagePro::edgebySobel_X);
	Sobel_Y_Action = new QAction(tr("Y方向"), this);
	connect(Sobel_Y_Action, &QAction::triggered, this, &ImagePro::edgebySobel_Y);
	Sobel_XY_Action = new QAction(tr("XY方向混合"), this);
	connect(Sobel_XY_Action, &QAction::triggered, this, &ImagePro::edgebySobel_XY);
	ScharrAction = new QAction(tr("Scharr"), this);
	statusBar();
}

void ImagePro::createMenus(){
	//fileMenus
	fileMenu = menuBar()->addMenu(tr("&文件"));
	fileMenu->addAction(openAction);
	fileMenu->addAction(saveAction);
	fileMenu->addAction(exitAction);
	//edit
	editMenu = menuBar()->addMenu(tr("&编辑"));
	editMenu->addAction(copyAction);
	editMenu->addAction(pasteAction);
	editMenu->addAction(selectpicAction);
	//function
	selectFun = menuBar()->addMenu(tr("&工具"));
	SobelMenu = new QMenu(tr("Sobel"));
	edgedetectionMenu= new QMenu(tr("&边缘检测"));
	edgedetectionMenu->addAction(LaplacianAction);
	SobelMenu->addAction(Sobel_X_Action);
	SobelMenu->addAction(Sobel_Y_Action);
	SobelMenu->addAction(Sobel_XY_Action);
	edgedetectionMenu->addMenu(SobelMenu);
	edgedetectionMenu->addAction(ScharrAction);
	selectFun->addMenu(edgedetectionMenu);
	selectFun->addAction(scaleAction);
	selectFun->addAction(tograyAction);
	selectFun->addAction(tobinaryAction);
	selectFun->addAction(showhisAction);
	selectFun->addAction(rotateAction);
	selectFun->addAction(flipAction);
	selectFun->addAction(reverseAction);
	selectFun->addAction(contrastandbrightAction);
	
	//help
	helpMenu = menuBar()->addMenu(tr("&帮助"));
	/*helpMenu->addAction(aboutAction);
	helpMenu->addAction(aboutImageProAction);*/
	
	
}
void ImagePro::createToolBars(){
	fileToolBar = addToolBar(tr("&文件"));
	fileToolBar->addAction(openAction);
	fileToolBar->addAction(saveAction);
	//fileToolBar->addAction(saveAsAction);
	fileToolBar->addAction(exitAction);
	editToolBar = addToolBar(tr("&编辑"));
	editToolBar->addAction(copyAction);
	//editToolBar->addAction(cutAction);
	editToolBar->addAction(pasteAction);
}