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
			QMessageBox::information(this, tr("提示"), tr("图像过大，已经缩小为合适大小显示"));
		}
		else if (image1.cols < 420 || image1.rows < 300) {
			QMessageBox::information(this, tr("提示"), tr("图像过小，已经放大为合适大小显示"));
		}
		Mat images = Resize(image1, 300, 420);
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
		imwrite(SavePath, saveimage);
		QMessageBox::information(this, tr("提示"), tr("保存成功"));
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
	selectpicAction = new QAction(tr("选择图像"), this);
	selectpicAction->setStatusTip(tr("选择要处理的图片"));
	//scale
	scaleAction = new QAction(QIcon("./Resources/images/scale.png"),tr("&缩放"), this);
	scaleAction->setShortcut(QKeySequence::ZoomIn);
	scaleAction->setStatusTip(tr("对当前图像进行缩放"));
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
	statusBar();
}
void ImagePro::createMenus(){
	//fileMenus
	fileMenu = menuBar()->addMenu(tr("&文件"));
	fileMenu->addAction(openAction);
	fileMenu->addAction(saveAction);
	fileMenu->addAction(exitAction);
	editMenu = menuBar()->addMenu(tr("&编辑"));
	editMenu->addAction(copyAction);
	editMenu->addAction(pasteAction);
	editMenu->addAction(selectpicAction);
	//function
	selectFun = menuBar()->addMenu(tr("&工具"));
	selectFun->addAction(scaleAction);
	selectFun->addAction(tograyAction);
	selectFun->addAction(tobinaryAction);
	selectFun->addAction(showhisAction);
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