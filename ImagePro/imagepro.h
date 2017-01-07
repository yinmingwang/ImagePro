#ifndef IMAGEPRO_H
#define IMAGEPRO_H

#include <QtWidgets/QMainWindow>
#include "ui_imagepro.h"

class ImagePro : public QMainWindow
{
	Q_OBJECT

public:
	ImagePro(QWidget *parent = 0);
	~ImagePro();

private:
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
	//菜单栏 工具栏
	QMenu * fileMenu;
	QMenu * editMenu;
	QMenu * helpMenu;
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
	//定义help的下拉菜单about aboutQt
	QAction *aboutAction;
	QAction *aboutImageProAction;
	Ui::ImageProClass ui;
};

#endif // IMAGEPRO_H
