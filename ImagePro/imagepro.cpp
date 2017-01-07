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
using namespace std;
using namespace cv;
ImagePro::ImagePro(QWidget *parent)
	: QMainWindow(parent)
{
	setWindowTitle(tr("Main Window"));
	setMaximumSize(800, 520);
	setMinimumSize(800, 520);
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
	if (openpath == NULL) {
		return;
	}
	string  OpenPath = string((const char *)openpath.toLocal8Bit());
	Mat image = imread(OpenPath);
	imshow("img", image);
	waitKey(0);
}
void ImagePro::saveFile(){
	QString savepath = QFileDialog::getSaveFileName(this,
		tr("Save"),
		"",
		tr("Images (*.png *.bmp *.jpg *.tif *.GIF"));
	string SavePath = string((const char *)savepath.toLocal8Bit());
}
void ImagePro::createActions(){
	//openAction
	openAction = new QAction(QIcon("./Resources/images/open.png"), tr("&Open"), this);
	openAction->setShortcuts(QKeySequence::Open);
	openAction->setStatusTip(tr("Open an existing file"));
	connect(openAction, &QAction::triggered, this, &ImagePro::openFile);
	//saveAction
	saveAction = new QAction(QIcon("./Resources/images/save.png"), tr("&Save"), this);
	saveAction->setShortcut(QKeySequence::Save);
	saveAction->setStatusTip(tr("Save an existing file"));
	connect(saveAction, &QAction::triggered, this, &ImagePro::saveFile);
	//copyAction
	copyAction = new QAction(QIcon("./Resources/images/copy.png"), tr("&Copy"), this);
	copyAction->setShortcut(QKeySequence::Copy);
	copyAction->setStatusTip(tr("cpoy an existing file"));
	//connect(copyAction, &QAction::triggered, this, &ImagePro::saveFile);
	//pasteAction
	pasteAction = new QAction(QIcon("./Resources/images/paste.png"), tr("&Paste"), this);
	pasteAction->setShortcut(QKeySequence::Paste);
	pasteAction->setStatusTip(tr("paste"));
    //exitAction
	exitAction = new QAction(QIcon("./Resources/images/exit.png"), tr("&Exit"), this);
	exitAction->setShortcut(QKeySequence::Close);
	exitAction->setStatusTip(tr("exit windows"));
	statusBar();
}
void ImagePro::createMenus(){
	//fileMenus
	fileMenu = menuBar()->addMenu(tr("&File"));
	fileMenu->addAction(openAction);
	fileMenu->addAction(saveAction);
	//fileMenu->addAction(saveAsAction);
	fileMenu->addAction(exitAction);
	editMenu = menuBar()->addMenu(tr("&Edit"));
	editMenu->addAction(copyAction);
	//editMenu->addAction(cutAction);
	editMenu->addAction(pasteAction);
	helpMenu = menuBar()->addMenu(tr("&Help"));
	/*helpMenu->addAction(aboutAction);
	helpMenu->addAction(aboutImageProAction);*/
	
}
void ImagePro::createToolBars(){
	fileToolBar = addToolBar(tr("&File"));
	fileToolBar->addAction(openAction);
	fileToolBar->addAction(saveAction);
	//fileToolBar->addAction(saveAsAction);
	fileToolBar->addAction(exitAction);
	editToolBar = addToolBar(tr("&Edit"));
	editToolBar->addAction(copyAction);
	//editToolBar->addAction(cutAction);
	editToolBar->addAction(pasteAction);

}