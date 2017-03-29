#include "imagepro.h"
#include <QAction>
#include <QMenuBar>
#include <QMessageBox>
#include <QStatusBar>
#include <QToolBar>
#include <QDialog>
#include <QFileDialog>
#include <opencv2/core/core.hpp>
#include<opencv2/opencv.hpp> 
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <QFileDialog>
#include <qDebug>
#include <iostream>
#include <stdio.h>
#include <QImage>  
#include <QPixmap>  
#include <QLabel> 
#include <QPainter>
#include <QTextCodec>
#include "PicChange.h"
#include "ImgFunc.h"
using namespace std;
using namespace cv;
#pragma execution_character_set("utf-8")
QImage qimage;
Mat matimage;
int rowsize = 0;
int colsize = 0;
int contrastvalue = 0;
int brightvalue = 0;
int rotateangle = 0;
int BoxValue = 2;
int MeanValue = 2;
int GaussianValue = 2;
int MedianValue = 2;
int BilateralValue = 2;
Mat dooleimage;
Point prevPt(-1, -1);
bool cameraisopen = false;//�ж�����ͷ�Ƿ��
bool isback = false;
QLabel *srclabel;
QLabel *Prolabel;
Mat frame;
ImagePro::ImagePro(QWidget *parent)
	: QMainWindow(parent)
{
	setWindowTitle(tr("Eric"));
	setMaximumSize(1350, 700);
	setMinimumSize(1350, 700);
	srclabel = new QLabel(this);
	Prolabel = new QLabel(this);
	srcscroll = new QScrollArea(this);
	Proscroll = new QScrollArea(this);
	srcscroll->setGeometry(10, 60, 650, 600);
	Proscroll->setGeometry(670, 60, 650, 600);
	srcscroll->setWidget(srclabel);
	Proscroll->setWidget(Prolabel);
	srcscroll->show();
	createActions();
	createMenus();
	createToolBars();
}

ImagePro::~ImagePro()
{
}
void ImagePro::openFile()
{
	Mat images;
	QString openpath = QFileDialog::getOpenFileName(this,
		tr("Select"),
		"",
		tr("Images (*.png *.bmp *.jpg *.tif *.GIF)"));
	if (openpath.isEmpty()) {
		QMessageBox::warning(this, tr("����"), tr("��ǰ·��Ϊ��"));
	}
	else{
		string  OpenPath = string((const char *)openpath.toLocal8Bit());
		Mat image1 = imread(OpenPath);
		//matimage = image1;
		//qDebug() << image1.cols << " " << image1.rows << endl;
		if (image1.cols > 600 || image1.rows > 420) {
			QMessageBox::information(this, tr("��ʾ"), tr("ͼ������Ѿ���СΪ���ʴ�С��ʾ"));
			images = image1;
		}
		else {
			images = image1;
		}
		
		if (images.empty()) {
			QMessageBox::warning(this, tr("����"), tr("��ͼ��ʧ��!"));
		}
		else {
			QImage image = Mat2QImage(images);
			qimage = image;
			srclabel->setPixmap(QPixmap::fromImage(image));
			srclabel->resize(QSize(image.width(), image.height()));
			srclabel->show();
			Prolabel->clear();
		}
	}
}
 void ImagePro::saveFile(){
	//if (srclabel->text == "") {
	//QMessageBox::information(this, tr("����"), tr("û���κ�ͼ����Ա���"));
	//}
	 if (Prolabel->pixmap() == NULL) {
		 QMessageBox::warning(this, tr("����"), tr("��ǰû��ͼ����Ա���"));
	 }
	else {
		QImage image = Prolabel->pixmap()->toImage();
		QMessageBox::information(this, tr("��ʾ"), tr("ͼ�����޸ģ�ȷ�����棿"));
		Mat saveimage = QImage2Mat(image);
		//imshow("test",testimage);
		QString savepath = QFileDialog::getSaveFileName(this,
			tr("Save"),
			"",
			tr("Images (*.png *.bmp *.jpg *.tif *.GIF"));
		string SavePath = string((const char *)savepath.toLocal8Bit());
		//imshow("show", matimage);
		if (!SavePath.empty()) {
			imwrite(SavePath, saveimage);
			QMessageBox::information(this, tr("��ʾ"), tr("����ɹ�"));
		}
		else {
			QMessageBox::information(this, tr("��ʾ"), tr("����ʧ��"));
		}
	}
}
void ImagePro::exitFile() {
	close();
}
void ImagePro::exchangepicture() {
	if (Prolabel->pixmap() == NULL) {
		QMessageBox::warning(this, tr("����"), tr("��ǰû��ͼ������л�"));
	}
	else {
		QImage image = Prolabel->pixmap()->toImage();
		srclabel->clear();
		srclabel->setPixmap(QPixmap::fromImage(image));
		srclabel->resize(QSize(image.width(), image.height()));
		srclabel->show();
		qimage = srclabel->pixmap()->toImage();
		Prolabel->clear();
	}
}
void ImagePro::rgbTogray() {
	if (srclabel->pixmap() == NULL) {
		QMessageBox::warning(this, tr("����"), tr("��ǰû��ͼ����Դ���"));
	}
	else {
		QImage image = srclabel->pixmap()->toImage();
		Mat image1 = QImage2Mat(image);
		Mat tempimg = Togray(image1);
		image = Mat2QImage(tempimg);
		Prolabel->setPixmap(QPixmap::fromImage(image));
		Prolabel->resize(QSize(image.width(), image.height()));
		Prolabel->alignment();
		Prolabel->show();
	}
}
void ImagePro::rgbTobinary() {
	if (srclabel->pixmap() == NULL) {
		QMessageBox::warning(this, tr("����"), tr("��ǰû��ͼ����Դ���"));
	} 
	else {
		QImage image = srclabel->pixmap()->toImage();
		Mat image1 = QImage2Mat(image);
		Mat tempimg = Tobinary(image1);
		image = Mat2QImage(tempimg);
		Prolabel->setPixmap(QPixmap::fromImage(image));
		Prolabel->resize(QSize(image.width(), image.height()));
		Prolabel->alignment();
		Prolabel->show();
	}
}
void ImagePro::showhistogram(){
	if (srclabel->pixmap() == NULL) {
		QMessageBox::warning(this, tr("����"), tr("��ǰû��ͼ����Դ���"));
	}
	else {
		QImage image = srclabel->pixmap()->toImage();
		Mat image1 = QImage2Mat(image);
		Mat histogramimg = Showhist(image1);
		image = Mat2QImage(histogramimg);
		Prolabel->setPixmap(QPixmap::fromImage(image));
		Prolabel->resize(QSize(image.width(), image.height()));
		Prolabel->show();
	}
}
void  scale(int w, void*) {
	
	Mat image1 = QImage2Mat(qimage);
	//imshow("hello", image1);
	//Mat scaleimage;
	if (colsize < 10)
	{
		//QMessageBox::warning(NULL,QObject::tr("�߶�"), QObject::tr("�߶Ȳ���С��10"));
		colsize = 10;
	}
	else if (rowsize < 10) {
		//QMessageBox::warning(NULL, QObject::tr("���"), QObject::tr("��Ȳ���С��10"));
		rowsize = 10;
	}
 	cv::resize(image1, image1, Size(colsize, rowsize), 0, 0, 3);
	QImage  image = Mat2QImage(image1);
	Prolabel->setPixmap(QPixmap::fromImage(image));
	Prolabel->resize(QSize(image.width(), image.height()));
	Prolabel->show();
	//imshow("test", image1);
}
void ImagePro::scaleimg() {
	if (srclabel->pixmap() == NULL) {
		QMessageBox::warning(this, tr("����"), tr("��ǰû��ͼ����Դ���"));
	}
	else {
		QImage image = srclabel->pixmap()->toImage();
		//inputimgDialog *inputdia = new inputimgDialog;
		Mat image1 = QImage2Mat(image);
		rowsize = image1.rows / 2;
		colsize = image1.cols / 2;
		//QString strQ = QString::fromLocal8Bit(str.c_str());
		namedWindow("ScaleBox", 1);
		createTrackbar("rows", "ScaleBox", &rowsize, image1.rows * 2, scale);
		createTrackbar("cols", "ScaleBox", &colsize, image1.cols * 2, scale);
	}
}
void rotate(int, void*) {
	Mat image1 = QImage2Mat(qimage);
	Mat roimage = rotation(image1, rotateangle);
	QImage  image = Mat2QImage(roimage);
	Prolabel->setPixmap(QPixmap::fromImage(image));
	Prolabel->resize(QSize(image.width(), image.height()));
	Prolabel->show();
}
void ImagePro::rotateimage() {
	if (srclabel->pixmap() == NULL) {
		QMessageBox::warning(this, tr("����"), tr("��ǰû��ͼ����Դ���"));
	}
	else {
		QImage image = srclabel->pixmap()->toImage();
		Mat image1 = QImage2Mat(image);
		namedWindow("RotateBox", 1);
		createTrackbar("angle", "RotateBox", &rotateangle, 360, rotate);
	}
}
void ImagePro::flipimage() {
	if (srclabel->pixmap() == NULL) {
		QMessageBox::warning(this, tr("����"), tr("��ǰû��ͼ����Դ���"));
	}
	else {
		QImage image = srclabel->pixmap()->toImage();
		Mat image1 = QImage2Mat(image);
		Mat tempimg = FlipImages(image1);
		image = Mat2QImage(tempimg);
		Prolabel->setPixmap(QPixmap::fromImage(image));
		Prolabel->resize(QSize(image.width(), image.height()));
		Prolabel->alignment();
		Prolabel->show();
	}
}
void ImagePro::reverseimage() {
	if (srclabel->pixmap() == NULL) {
		QMessageBox::warning(this, tr("����"), tr("��ǰû��ͼ����Դ���"));
	}
	else {
		QImage image = srclabel->pixmap()->toImage();
		Mat image1 = QImage2Mat(image);
		vector<Mat> channel;
		split(image1, channel);
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < channel[i].rows; j++) {
				for (int k = 0; k < channel[i].cols; k++) {
					channel[i].at<uchar>(j, k) = 255 - channel[i].at<uchar>(j, k);
				}
			}
		}
		Mat tempimg;
		merge(channel, tempimg);
		image = Mat2QImage(tempimg);
		Prolabel->setPixmap(QPixmap::fromImage(image));
		Prolabel->resize(QSize(image.width(), image.height()));
		Prolabel->alignment();
		Prolabel->show();
	}
}
void ModifiyContrastAndBright(int, void*) {
	Mat image1 = QImage2Mat(qimage);
	Mat CAB = ChangeContrastAndBright(image1, contrastvalue, brightvalue);
	QImage  image = Mat2QImage(CAB);
	Prolabel->setPixmap(QPixmap::fromImage(image));
	Prolabel->resize(QSize(image.width(), image.height()));
	Prolabel->show();
}
void ImagePro::contrastAndbright() {
	if (srclabel->pixmap() == NULL) {
		QMessageBox::warning(this, tr("����"), tr("��ǰû��ͼ����Դ���"));
	}
	else {
		contrastvalue = 80;
		brightvalue = 80;
		namedWindow("ContrastAndBrightBox", 1);
		createTrackbar("ContrastValue", "ContrastAndBrightBox", &contrastvalue, 300, ModifiyContrastAndBright);
		createTrackbar("BrightValue", "ContrastAndBrightBox", &brightvalue, 300, ModifiyContrastAndBright);
	}
}
void ImagePro::edgebyLaplacian() {
	if (srclabel->pixmap() == NULL) {
		QMessageBox::warning(this, tr("����"), tr("��ǰû��ͼ����Դ���"));
	}
	else {
		QImage image = srclabel->pixmap()->toImage();
		Mat image1 = QImage2Mat(image);
		Mat laplacianimage = EdgeDetection_Laplacian(image1);
		image = Mat2QImage(laplacianimage);
		Prolabel->setPixmap(QPixmap::fromImage(image));
		Prolabel->resize(QSize(image.width(), image.height()));
		Prolabel->show();
	}
}
void ImagePro::edgebySobel_X() {
	if (srclabel->pixmap() == NULL) {
		QMessageBox::warning(this, tr("����"), tr("��ǰû��ͼ����Դ���"));
	}
	else {
		QImage image = srclabel->pixmap()->toImage();
		Mat image1 = QImage2Mat(image);
		Mat sobel_x = EdgeDetection_Sobel_X(image1);
		image = Mat2QImage(sobel_x);
		Prolabel->setPixmap(QPixmap::fromImage(image));
		Prolabel->resize(QSize(image.width(), image.height()));
		Prolabel->show();
	}
}
void ImagePro::edgebySobel_Y() {
	if (srclabel->pixmap() == NULL) {
		QMessageBox::warning(this, tr("����"), tr("��ǰû��ͼ����Դ���"));
	}
	else {
		QImage image = srclabel->pixmap()->toImage();
		Mat image1 = QImage2Mat(image);
		Mat sobel_y = EdgeDetection_Sobel_Y(image1);
		image = Mat2QImage(sobel_y);
		Prolabel->setPixmap(QPixmap::fromImage(image));
		Prolabel->resize(QSize(image.width(), image.height()));
		Prolabel->show();
	}
}
void ImagePro::edgebySobel_XY() {
	if (srclabel->pixmap() == NULL) {
		QMessageBox::warning(this, tr("����"), tr("��ǰû��ͼ����Դ���"));
	}
	else {
		QImage image = srclabel->pixmap()->toImage();
		Mat image1 = QImage2Mat(image);
		Mat sobelimage = EdgeDetection_Sobel(image1);
		image = Mat2QImage(sobelimage);
		Prolabel->setPixmap(QPixmap::fromImage(image));
		Prolabel->resize(QSize(image.width(), image.height()));
		Prolabel->show();
	}
}
void ImagePro::edgebyScharr_X() {
	if (srclabel->pixmap() == NULL) {
		QMessageBox::warning(this, tr("����"), tr("��ǰû��ͼ����Դ���"));
	}
	else {
		QImage image = srclabel->pixmap()->toImage();
		Mat image1 = QImage2Mat(image);
		Mat scharr_x = EdgeDetecction_Scharr_X(image1);
		image = Mat2QImage(scharr_x);
		Prolabel->setPixmap(QPixmap::fromImage(image));
		Prolabel->resize(QSize(image.width(), image.height()));
		Prolabel->show();
	}
}
void ImagePro::edgebyScharr_Y() {
	if (srclabel->pixmap() == NULL) {
		QMessageBox::warning(this, tr("����"), tr("��ǰû��ͼ����Դ���"));
	}
	else {
		QImage image = srclabel->pixmap()->toImage();
		Mat image1 = QImage2Mat(image);
		Mat scharr_y = EdgeDetecction_Scharr_Y(image1);
		image = Mat2QImage(scharr_y);
		Prolabel->setPixmap(QPixmap::fromImage(image));
		Prolabel->resize(QSize(image.width(), image.height()));
		Prolabel->show();
	}
}
void ImagePro::edgebyScharr_XY() {
	if (srclabel->pixmap() == NULL) {
		QMessageBox::warning(this, tr("����"), tr("��ǰû��ͼ����Դ���"));
	}
	else {
		QImage image = srclabel->pixmap()->toImage();
		Mat image1 = QImage2Mat(image);
		Mat scharr_xy = EdgeDetecction_Scharr(image1);
		image = Mat2QImage(scharr_xy);
		Prolabel->setPixmap(QPixmap::fromImage(image));
		Prolabel->resize(QSize(image.width(), image.height()));
		Prolabel->show();
	}
	
}
//filter function
//box
void Trackbar_BoxFilter(int, void*) {
	Mat image1 = QImage2Mat(qimage);
	Mat BoxFilterIamge = Fun_Box_Filter(image1, BoxValue);
	QImage  image = Mat2QImage(BoxFilterIamge);
	Prolabel->setPixmap(QPixmap::fromImage(image));
	Prolabel->resize(QSize(image.width(), image.height()));
	Prolabel->show();
}
void ImagePro::Box_Filter() {
	if (srclabel->pixmap() == NULL) {
		QMessageBox::warning(this, tr("����"), tr("��ǰû��ͼ����Դ���"));
	} 
	else {
		namedWindow("Box Filter", 1);
		createTrackbar("Value", "Box Filter", &BoxValue, 50, Trackbar_BoxFilter);
	}
}
//Mean
void Trackbar_MeanFilter(int, void*) {
	Mat image1 = QImage2Mat(qimage);
	Mat MeanFilterImage = Fun_Mean_Filter(image1, MeanValue);
	QImage  image = Mat2QImage(MeanFilterImage);
	Prolabel->setPixmap(QPixmap::fromImage(image));
	Prolabel->resize(QSize(image.width(), image.height()));
	Prolabel->show();
}
void ImagePro::Mean_Filter() {
	if (srclabel->pixmap() == NULL) {
		QMessageBox::warning(this, tr("����"), tr("��ǰû��ͼ����Դ���"));
	}
	else {
		namedWindow("Mean Filter", 1);
		createTrackbar("Value", "Mean Filter", &MeanValue, 50, Trackbar_MeanFilter);
	}
	
}

//Gaussian
void Trackbar_GaussianFilter(int, void*) {
	Mat image1 = QImage2Mat(qimage);
	Mat GaussianFilterImage = Fun_Gaussian_Filter(image1, GaussianValue);
	QImage  image = Mat2QImage(GaussianFilterImage);
	Prolabel->setPixmap(QPixmap::fromImage(image));
	Prolabel->resize(QSize(image.width(), image.height()));
	Prolabel->show();
}
void ImagePro::Gaussian_Filter() {
	if (srclabel->pixmap() == NULL) {
		QMessageBox::warning(this, tr("����"), tr("��ǰû��ͼ����Դ���"));
	}
	else {
		namedWindow("Gaussian Filter", 1);
		createTrackbar("Value", "Gaussian Filter", &GaussianValue, 50, Trackbar_GaussianFilter);
	}	
}
//Median
void Trackbar_MedianFilter(int, void*) {
	Mat image1 = QImage2Mat(qimage);
	Mat MedianFilterImage = Fun_Median_Filter(image1,MedianValue);
	QImage  image = Mat2QImage(MedianFilterImage);
	Prolabel->setPixmap(QPixmap::fromImage(image));
	Prolabel->resize(QSize(image.width(), image.height()));
	Prolabel->show();
}
void ImagePro::Median_Filter() {
	if (srclabel->pixmap() == NULL) {
		QMessageBox::warning(this, tr("����"), tr("��ǰû��ͼ����Դ���"));
	}
	else {
		namedWindow("Median Filter", 1);
		createTrackbar("Value", "Median Filter", &MedianValue, 50, Trackbar_MedianFilter);
	}
}
	
//Bilateral
void Trackbar_BilateralFilter(int, void*) {
	Mat image1 = QImage2Mat(qimage);
	Mat BilateralFilterImage = Fun_Bilateral_Filter(image1, BilateralValue);
	QImage  image = Mat2QImage(BilateralFilterImage);
	Prolabel->setPixmap(QPixmap::fromImage(image));
	Prolabel->resize(QSize(image.width(), image.height()));
	Prolabel->show();
}

void ImagePro::Bilateral_Filter() {
	if (srclabel->pixmap() == NULL) {
		QMessageBox::warning(this, tr("����"), tr("��ǰû��ͼ����Դ���"));
	}
	else {
		namedWindow("Bilateral Filter", 1);
		createTrackbar("Value", "Bilateral Filter", &BilateralValue, 50, Trackbar_BilateralFilter);
	}
}

void ImagePro::opencamera() {
	VideoCapture capture(0);
	if (!capture.isOpened())
	{
		QMessageBox::warning(this, tr("����"), tr("����ͷû�д�"));
	}
	else {
		CascadeClassifier cascade, nestedCascade;
		bool stop = false;
		cameraisopen = true;
		cascade.load("F:\\win8.1_softwareinstall\\opencv\\sources\\data\\haarcascades\\haarcascade_frontalface_alt.xml");
		nestedCascade.load("F:\\win8.1_softwareinstall\\opencv\\sources\\data\\haarcascades\\haarcascade_eye.xml");
		while (!stop)
		{
			capture >> frame;
			detectAndDraw(frame, cascade, nestedCascade, 2, 0);
			//imshow("camera", frame);
			if (waitKey(30) >= 0) {
				stop = true;
			}
		}
		if (stop) {
			capture.release();
		}
	}
}
void ImagePro::takephoto() {
	if (cameraisopen) {
		if (!frame.empty())
		{
			//imshow("test", frame);
			Mat phtotoimage = Fun_Bilateral_Filter(frame, 10);
			QImage image = Mat2QImage(phtotoimage);
			qimage = image;
			srclabel->setPixmap(QPixmap::fromImage(image));
			srclabel->resize(QSize(image.width(), image.height()));
			srclabel->show();
			Prolabel->clear();
		}
	}
	else{
		QMessageBox::information(this, tr("��ʾ"), tr("���ȴ�����ͷ"));
	}
}
static void onMouse(int event, int x, int y, int flags, void*)
{
	if (event == EVENT_LBUTTONUP || !(flags & EVENT_FLAG_LBUTTON))
		prevPt = Point(-1, -1);
	else if (event == EVENT_LBUTTONDOWN)
		prevPt = Point(x, y);
	else if (event == EVENT_MOUSEMOVE && (flags & EVENT_FLAG_LBUTTON))
	{
		Point pt(x, y);
		if (prevPt.x < 0)
			prevPt = pt;
		line(dooleimage, prevPt, pt, Scalar::all(255), 5, 8, 0);
		prevPt = pt;
		QImage image = Mat2QImage(dooleimage);
		Prolabel->setPixmap(QPixmap::fromImage(image));
		Prolabel->resize(QSize(image.width(), image.height()));
		Prolabel->show();
		//imshow("test", dooleimage);
	}
}

void ImagePro::doodlepicture() {
	if (srclabel->pixmap() == NULL) {
		QMessageBox::warning(this, tr("����"), tr("��ǰ����Ϳѻ��ͼ��"));
	}
	else {
		QImage image = srclabel->pixmap()->toImage();
		Mat image1 = QImage2Mat(image);
		dooleimage = image1.clone();
		imshow("src", dooleimage);
		setMouseCallback("src", onMouse, 0);
	}
}
void ImagePro::enhancePicture() {
	if (srclabel->pixmap() == NULL) {
		QMessageBox::warning(this, tr("����"), tr("��ǰû��ͼ����Դ���"));
	}
	else {
		QImage image = srclabel->pixmap()->toImage();
		Mat image1 = QImage2Mat(image);
		Mat enhanceimg = EnhanceImages(image1);
		image = Mat2QImage(enhanceimg);
		Prolabel->setPixmap(QPixmap::fromImage(image));
		Prolabel->resize(QSize(image.width(), image.height()));
		Prolabel->alignment();
		Prolabel->show();
	}
}
void ImagePro::createActions(){
	//openAction
	openAction = new QAction(QIcon("./Resources/images/open.png"), tr("&��"), this);
	openAction->setShortcuts(QKeySequence::Open);
	openAction->setStatusTip(tr("��ͼƬ"));
	connect(openAction, &QAction::triggered, this, &ImagePro::openFile);
	//saveAction
	saveAction = new QAction(QIcon("./Resources/images/save.png"), tr("&����"), this);
	saveAction->setShortcut(QKeySequence::Save);
	saveAction->setStatusTip(tr("���浱ǰͼƬ"));
	connect(saveAction, &QAction::triggered, this, &ImagePro::saveFile);
	//copyAction
	copyAction = new QAction(QIcon("./Resources/images/copy.png"), tr("&����"), this);
	copyAction->setShortcut(QKeySequence::Copy);
	copyAction->setStatusTip(tr("���Ƶ�ǰͼƬ"));
	//connect(copyAction, &QAction::triggered, this, &ImagePro::saveFile);
	//pasteAction
	pasteAction = new QAction(QIcon("./Resources/images/paste.png"), tr("&ճ��"), this);
	pasteAction->setShortcut(QKeySequence::Paste);
	pasteAction->setStatusTip(tr("ճ��"));
    //exitAction
	exitAction = new QAction(QIcon("./Resources/images/exit.png"), tr("&�˳�"), this);
	exitAction->setShortcut(QKeySequence::Close);
	exitAction->setStatusTip(tr("�˳���ǰ����"));
	connect(exitAction, &QAction::triggered, this, &ImagePro::exitFile);
	//select picture
	selectpicAction = new QAction(QIcon("./Resources/images/exchange.png"), tr("�л�ͼ��"), this);
	selectpicAction->setStatusTip(tr("�л������ͼƬ"));
	connect(selectpicAction, &QAction::triggered, this, &ImagePro::exchangepicture);
	//Open Camera
	OpenCameraAction = new  QAction(QIcon("./Resources/images/open_camera.png"),tr("������ͷ"), this);
	connect(OpenCameraAction, &QAction::triggered, this, &ImagePro::opencamera);
	//take photo
	TakePhotoAction = new QAction(QIcon("./Resources/images/takephoto.png"), tr("&����"), this);
	TakePhotoAction->setStatusTip(tr("������Ƭ"));
	connect(TakePhotoAction, &QAction::triggered, this, &ImagePro::takephoto);
	//scale
	scaleAction = new QAction(QIcon("./Resources/images/scale.png"),tr("&����"), this);
	scaleAction->setShortcut(QKeySequence::ZoomIn);
	scaleAction->setStatusTip(tr("�Ե�ǰͼ���������"));
	connect(scaleAction, &QAction::triggered, this, &ImagePro::scaleimg);
	//togary picture
	tograyAction = new QAction(tr("&�Ҷ�ͼ"), this);
	tograyAction->setShortcut(QKeySequence::Quit);
	tograyAction->setStatusTip(tr("��ͼƬת��Ϊ�Ҷ�ͼ��"));
	connect(tograyAction, &QAction::triggered, this, &ImagePro::rgbTogray);
	//tobinary picture
	tobinaryAction = new QAction(tr("��ֵ��"),this);
	tobinaryAction->setStatusTip(tr("��ͼƬת��Ϊ��ֵͼ��"));
	connect(tobinaryAction, &QAction::triggered, this, &ImagePro::rgbTobinary);
	//show histogram
	showhisAction = new QAction(tr("ֱ��ͼ"), this);
	showhisAction->setStatusTip(tr("��ʾ��ǰͼ��ֱ��ͼ"));
	connect(showhisAction, &QAction::triggered, this, &ImagePro::showhistogram);
	//rotate picture
	rotateAction = new QAction(QIcon("./Resources/images/rotate.png"), tr("��ת"), this);
	rotateAction->setStatusTip(tr("��ת��ǰͼƬ"));
	connect(rotateAction, &QAction::triggered, this, &ImagePro::rotateimage);
	//flip picture
	flipAction = new QAction(QIcon("./Resources/images/symmetric.png"),tr("����"), this);
	flipAction->setStatusTip(tr("���ɾ���"));
	connect(flipAction, &QAction::triggered, this, &ImagePro::flipimage);
	//reverse color
	reverseAction = new QAction(QIcon("./Resources/images/inverse.png"),tr("��ɫ"), this);
	reverseAction->setStatusTip(tr("���ɷ�ɫͼ��"));
	connect(reverseAction, &QAction::triggered, this, &ImagePro::reverseimage);
	//modifiy picture
	contrastandbrightAction = new QAction(tr("�ԱȶȺ�����"), this);
	contrastandbrightAction->setStatusTip(tr("�����ԱȶȺ�����"));
	connect(contrastandbrightAction, &QAction::triggered, this, &ImagePro::contrastAndbright);
	//edge detection
	LaplacianAction = new QAction(tr("Laplacian"), this);
	LaplacianAction->setStatusTip(tr("����Laplacian�ı�Ե���"));
	connect(LaplacianAction, &QAction::triggered, this, &ImagePro::edgebyLaplacian);
	//sobel
	Sobel_X_Action = new QAction(tr("X����"), this);
	connect(Sobel_X_Action, &QAction::triggered, this, &ImagePro::edgebySobel_X);
	Sobel_Y_Action = new QAction(tr("Y����"), this);
	connect(Sobel_Y_Action, &QAction::triggered, this, &ImagePro::edgebySobel_Y);
	Sobel_XY_Action = new QAction(tr("XY������"), this);
	connect(Sobel_XY_Action, &QAction::triggered, this, &ImagePro::edgebySobel_XY);
	//scharr
	Scharr_X_Action = new QAction(tr("X����"), this);
	connect(Scharr_X_Action, &QAction::triggered, this, &ImagePro::edgebyScharr_X);
	Scharr_Y_Action = new QAction(tr("Y����"), this);
	connect(Scharr_Y_Action, &QAction::triggered, this, &ImagePro::edgebyScharr_Y);
	Scharr_XY_Action = new QAction(tr("XY������"), this);
	connect(Scharr_XY_Action, &QAction::triggered, this, &ImagePro::edgebyScharr_XY);
	//Filter
	//Box Filter
	Box_FilterAction = new QAction(tr("�����˲�"), this);
	connect(Box_FilterAction, &QAction::triggered, this, &ImagePro::Box_Filter);
	//Mean Filter
	Mean_FilterAction = new QAction(tr("��ֵ�˲�"), this);
	connect(Mean_FilterAction, &QAction::triggered, this, &ImagePro::Mean_Filter);
	//Gaussian Filter
	Gaussian_FilterAction = new QAction(tr("��˹�˲�"), this);
	connect(Gaussian_FilterAction, &QAction::triggered, this, &ImagePro::Gaussian_Filter);
	//Median Filter
	Median_FilterAction = new QAction(tr("��ֵ�˲�"), this);
	connect(Median_FilterAction, &QAction::triggered, this, &ImagePro::Median_Filter);
	//Bilateral Filter
	Bilateral_FilterAction = new QAction(tr("˫���˲�"), this);
	connect(Bilateral_FilterAction, &QAction::triggered, this, &ImagePro::Bilateral_Filter);
	//enhance picture
	enhanceAction = new QAction(QIcon("./Resources/images/enhance.png"), tr("ͼ����ǿ"), this);
	enhanceAction->setStatusTip(tr("ֱ��ͼ���⻯��ǿ��ǰͼƬ"));
	connect(enhanceAction, &QAction::triggered, this, &ImagePro::enhancePicture);
	//inpaint
	doodleAction = new QAction(QIcon("./Resources/images/doodle.png"),tr("Ϳѻ"), this);
	connect(doodleAction, &QAction::triggered, this, &ImagePro::doodlepicture);
	statusBar();
}

void ImagePro::createMenus(){
	//fileMenus
	fileMenu = menuBar()->addMenu(tr("&�ļ�"));
	fileMenu->addAction(openAction);
	fileMenu->addAction(saveAction);
	fileMenu->addAction(exitAction);
	//edit
	editMenu = menuBar()->addMenu(tr("&�༭"));
	editMenu->addAction(copyAction);
	editMenu->addAction(pasteAction);
	editMenu->addAction(selectpicAction);
	editMenu->addAction(OpenCameraAction);
	//function
	selectFun = menuBar()->addMenu(tr("&����"));
	SobelMenu = new QMenu(tr("Sobel"));
	ScharrMenu = new QMenu(tr("Scharr"));
	FilterMenu = new QMenu(tr("�˲�"));
	linearFilterMenu = new QMenu(tr("�����˲�"));
	nonlinearFilterMenu = new QMenu(tr("�������˲�"));
	//add filter action
	FilterMenu->addMenu(linearFilterMenu);
	FilterMenu->addMenu(nonlinearFilterMenu);
	linearFilterMenu->addAction(Box_FilterAction);
	linearFilterMenu->addAction(Mean_FilterAction);
	linearFilterMenu->addAction(Gaussian_FilterAction);
	nonlinearFilterMenu->addAction(Median_FilterAction);
	nonlinearFilterMenu->addAction(Bilateral_FilterAction);
	edgedetectionMenu= new QMenu(tr("&��Ե���"));
	edgedetectionMenu->addMenu(SobelMenu);
	edgedetectionMenu->addMenu(ScharrMenu);
	edgedetectionMenu->addAction(LaplacianAction);
	SobelMenu->addAction(Sobel_X_Action);
	SobelMenu->addAction(Sobel_Y_Action);
	SobelMenu->addAction(Sobel_XY_Action);
	ScharrMenu->addAction(Scharr_X_Action);
	ScharrMenu->addAction(Scharr_Y_Action);
	ScharrMenu->addAction(Scharr_XY_Action);
	//selectmenu
	selectFun->addAction(scaleAction);
	selectFun->addAction(rotateAction);
	selectFun->addAction(flipAction);
	selectFun->addAction(reverseAction);
	selectFun->addAction(enhanceAction);
	selectFun->addAction(doodleAction);
	selectFun->addAction(tograyAction);
	selectFun->addAction(tobinaryAction);
	selectFun->addAction(showhisAction);
	selectFun->addMenu(edgedetectionMenu);
	selectFun->addMenu(FilterMenu);
	//help
	helpMenu = menuBar()->addMenu(tr("&����"));
	/*helpMenu->addAction(aboutAction);
	helpMenu->addAction(aboutImageProAction);*/
	
	
}
void ImagePro::createToolBars(){
	fileToolBar = addToolBar(tr("&�ļ�"));
	fileToolBar->addAction(openAction);
	fileToolBar->addAction(saveAction);
	fileToolBar->addAction(exitAction);
	editToolBar = addToolBar(tr("&�༭"));
	editToolBar->addAction(copyAction);
	editToolBar->addAction(pasteAction);
	editToolBar->addAction(selectpicAction);
	editToolBar->addAction(scaleAction);
	editToolBar->addAction(rotateAction);
	editToolBar->addAction(enhanceAction);
	editToolBar->addAction(reverseAction);
	editToolBar->addAction(doodleAction);
	editToolBar->addAction(OpenCameraAction);
	editToolBar->addAction(TakePhotoAction);
}