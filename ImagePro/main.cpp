#include "imagepro.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	ImagePro w;
	w.show();
	return a.exec();
}
