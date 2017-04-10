#ifndef IMAGEPRO_H
#define IMAGEPRO_H

#include <QtWidgets/QMainWindow>
#include "ui_imagepro.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <QLabel>
#include <QScrollArea>
#include <QPushButton>
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
	void enhancePicture();
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
	void doodlepicture();
	void opencamera();
	void takephoto();
	void edgebyLaplacian();
	void edgebySobel_X();
	void edgebySobel_Y();
	void edgebySobel_XY();
	void edgebyScharr_X();
	void edgebyScharr_Y();
	void edgebyScharr_XY();
	//Filter
	//box
	void Box_Filter();
	//mean
	void Mean_Filter();
	//Gaussian
	void Gaussian_Filter();
	//median
	void Median_Filter();
	//bilateral
	void Bilateral_Filter();
	//whitening
	void Whitening();
private:
	//菜单栏
	QMenu *fileMenu;
	QMenu *editMenu;
	QMenu *helpMenu;
	QMenu *selectFun;
	QMenu *edgedetectionMenu;
	QMenu *SobelMenu;
	QMenu *ScharrMenu;
	//Filter
	QMenu *FilterMenu;
	QMenu *linearFilterMenu;
	QMenu *nonlinearFilterMenu;
	//toolbar
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
	QAction *OpenCameraAction;
	QAction *TakePhotoAction;
	//定义help的下拉菜单about aboutQt
	QAction *aboutAction;
	QAction *aboutImageProAction;
	//功能选择
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
	QAction *showhisAction;
	QAction *enhanceAction;
	QAction *doodleAction;
	//edge detection
	QAction *LaplacianAction;
	QAction *Scharr_X_Action;
	QAction *Scharr_Y_Action;
	QAction *Scharr_XY_Action;
	//Filter
	//box
	QAction *Box_FilterAction;
	//mean
	QAction *Mean_FilterAction;
	//Gaussian
	QAction *Gaussian_FilterAction;
    //median
	QAction *Median_FilterAction;
	//bilateral
	QAction *Bilateral_FilterAction;
	//inputimgDialog *inputdia;
	//whitening
	QAction *whiteningAction;
	ImagePro *ui;
	QScrollArea *srcscroll;
	QScrollArea *Proscroll;
	//Mat Image;
};

#endif // IMAGEPRO_H
