#include <QtWidgets/QMainWindow>
#include <QLineEdit>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <qDebug>
#include <iostream>
#include <QLabel>
#include <QPushButton>
#include <QDialog>
#pragma execution_character_set("utf-8")
using namespace cv;
using namespace std;
class  inputimgDialog : public QWidget
{
public:
	inputimgDialog();
	~inputimgDialog();

private:
	QLineEdit *inputwidth;
	QLineEdit *inputheight;
	QVBoxLayout *Vlayout;
	QHBoxLayout *HWlayout;
	QHBoxLayout *HHlayout;
	QVBoxLayout *mainLayout;
	QLabel *width;
	QLabel *height;
	QPushButton *sure;
	QWidget *window;

};
