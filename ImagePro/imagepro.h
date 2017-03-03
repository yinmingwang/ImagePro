#ifndef IMAGEPRO_H
#define IMAGEPRO_H

#include <QtWidgets/QMainWindow>
#include "ui_imagepro.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <QLabel>
using namespace cv;
class ImagePro : public QMainWindow
{
	Q_OBJECT

public:
	ImagePro(QWidget *parent = 0);
	~ImagePro();
signals:
	void size(int, int);
public:
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
	void scale(int, int);
	void rgbTogray();
	void rgbTobinary();
	void scaleBox();
	void showhistogram();
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
	QAction *tograyAction;
	QAction *tobinaryAction;
	QAction *showHistogramAction;
	//label
	QLabel *srclabel;
	QLabel *Prolabel;
	ImagePro *ui;
	//Mat Image;
};

#endif // IMAGEPRO_H
