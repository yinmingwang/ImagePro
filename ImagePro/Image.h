#ifndef IMAGE_H
#define IMAGE_H

#include <iostream>
#include <QtWidgets/QMainWindow>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <QImage>  
#include <QPixmap> 
using namespace cv;
using namespace std;

class Image
{
public:
	Image(Mat& mimg);
	Image(QImage& qimg);
	QImage cvMat2QImage(const cv::Mat& mat);
	cv::Mat QImage2cvMat(QImage image);
	~Image();

private:
	Mat matimage;
	QImage qimage;
};

#endif
