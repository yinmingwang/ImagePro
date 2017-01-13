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

private:
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
	//����help�������˵�about aboutQt
	QAction *aboutAction;
	QAction *aboutImageProAction;
	//label
	QLabel *srclabel;
	ImagePro *ui;
};

#endif // IMAGEPRO_H
