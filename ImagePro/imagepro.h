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
	void flipimage();
	void reverseimage();
	void contrastAndbright();
	void exchangepicture();
	void edgebyLaplacian();
	void edgebySobel_X();
	void edgebySobel_Y();
	void edgebySobel_XY();
	void edgebyScharr();
private:
	//�˵��� ������
	QMenu *fileMenu;
	QMenu *editMenu;
	QMenu *helpMenu;
	QMenu *selectFun;
	QMenu *edgedetectionMenu;
	QMenu *SobelMenu;
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
	QAction *tograyAction;
	QAction *tobinaryAction;
	QAction *showHistogramAction;
	QAction *flipAction;
	QAction *reverseAction;
	QAction *contrastandbrightAction;
	QAction *Sobel_X_Action;
	QAction *Sobel_Y_Action;
	QAction *Sobel_XY_Action;
	//edge detection
	QAction *LaplacianAction;
	QAction *ScharrAction;
	
	//inputimgDialog *inputdia;
	ImagePro *ui;
	QScrollArea *srcscroll;
	QScrollArea *Proscroll;
	//Mat Image;
};

#endif // IMAGEPRO_H
