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
using namespace std;
using namespace cv;
#pragma execution_character_set("utf-8")
ImagePro::ImagePro(QWidget *parent)
	: QMainWindow(parent)
{
	setWindowTitle(tr("Main Window"));
	setMaximumSize(800, 520);
	setMinimumSize(800, 520);
	srclabel = new QLabel(this);
	srclabel->setGeometry(5, 60, 600, 450);
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
		Mat images = imread(OpenPath);
		if (images.empty()) {
			QMessageBox::information(this,
				tr("打开图像失败"),
				tr("打开图像失败!"));
			return;
		}
		else {
			QImage image = Mat2QImage(images);
			//label = new QLabel(this);
			//label->setGeometry(10, 50, 1000, 800);
			srclabel->setPixmap(QPixmap::fromImage(image));
			srclabel->resize(QSize(image.width(), image.height()));
			srclabel->alignment();
			srclabel->show();
			//ui->scroll->setWidget(label);
		}
	}
}
void ImagePro::saveFile(){
	QString savepath = QFileDialog::getSaveFileName(this,
		tr("Save"),
		"",
		tr("Images (*.png *.bmp *.jpg *.tif *.GIF"));
	string SavePath = string((const char *)savepath.toLocal8Bit());
}
void ImagePro::exitFile(){
	close();
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
	statusBar();
}
void ImagePro::createMenus(){
	//fileMenus
	fileMenu = menuBar()->addMenu(tr("文件"));
	fileMenu->addAction(openAction);
	fileMenu->addAction(saveAction);
	fileMenu->addAction(exitAction);
	editMenu = menuBar()->addMenu(tr("&编辑"));
	editMenu->addAction(copyAction);
	editMenu->addAction(pasteAction);
	//help
	helpMenu = menuBar()->addMenu(tr("&帮助"));
	/*helpMenu->addAction(aboutAction);
	helpMenu->addAction(aboutImageProAction);*/
	//function
	selectFun = menuBar()->addMenu(tr("工具"));
	
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