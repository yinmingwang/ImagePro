#ifndef IMAGEPRO_H
#define IMAGEPRO_H

#include <QtWidgets/QMainWindow>
#include "ui_imagepro.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <QLabel>
#include <QScrollArea>
#include <QPushButton>
#include "inputimgdialog.h"
using namespace cv;
class ImagePro : public QMainWindow
{
	Q_OBJECT
public:
	ImagePro(QWidget *parent = 0);
	~ImagePro();
	//void scale(int, void*);
	QLabel* getsrclabel();
    QLabel* getProlabel();
	
	//函数
	void openFile();
	void saveFile();
	void copyFile();
	void pasteFile();
	void cutFile();
	void exitFile();
	void createActions();
	void createMenus();
	void createToolBars();
	//void scale(int, int);
	void rgbTogray();
	void rgbTobinary();
	void scaleBox();
	void showhistogram();
	void scaleimg();
	void rotateimage();
private:
	//菜单栏 工具栏
	QMenu * fileMenu;
	QMenu * editMenu;
	QMenu * helpMenu;
	QMenu * selectFun;
	QToolBar *fileToolBar;
	QToolBar *editToolBar;
	//定义了file的下拉菜单
	QAction *openAction;
	QAction *saveAction;
	QAction *saveAsAction;
	QAction *exitAction; 
	//定义edit的下拉菜单
	QAction *cutAction;
	QAction *copyAction;
	QAction *pasteAction;
	QAction *selectpicAction;
	QAction *showhisAction;
	//定义help的下拉菜单about aboutQt
	QAction *aboutAction;
	QAction *aboutImageProAction;
	//功能选择
	QAction *scaleAction;
	QAction *rotateAction;
	//QAction *rotateAction;
	QAction *tograyAction;
	QAction *tobinaryAction;
	QAction *showHistogramAction;
	//label
	
	//inputimgDialog *inputdia;
	ImagePro *ui;
	QScrollArea *srcscroll;
	QScrollArea *Proscroll;
	//Mat Image;
};

#endif // IMAGEPRO_H
