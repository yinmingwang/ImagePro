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
	
	//����
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
	//�˵��� ������
	QMenu * fileMenu;
	QMenu * editMenu;
	QMenu * helpMenu;
	QMenu * selectFun;
	QToolBar *fileToolBar;
	QToolBar *editToolBar;
	//������file�������˵�
	QAction *openAction;
	QAction *saveAction;
	QAction *saveAsAction;
	QAction *exitAction; 
	//����edit�������˵�
	QAction *cutAction;
	QAction *copyAction;
	QAction *pasteAction;
	QAction *selectpicAction;
	QAction *showhisAction;
	//����help�������˵�about aboutQt
	QAction *aboutAction;
	QAction *aboutImageProAction;
	//����ѡ��
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
