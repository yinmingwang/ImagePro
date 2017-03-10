#include <iostream>
#include <QtWidgets/QMainWindow>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <QImage>  
#include <QPixmap> 
#include <qDebug>

using namespace cv;
using namespace std;
inline Mat Resize(Mat srcimg, int width, int height){
	Size Newsize = Size(width, height);
	Mat Reimg = Mat(Newsize, CV_32S);
	resize(srcimg, Reimg,Newsize);
	return Reimg;
}
inline Mat Togray(Mat srcimage) {
	Mat grayimage;
	cvtColor(srcimage, grayimage, CV_BGR2GRAY);
	return grayimage;
}
inline Mat Tobinary(Mat srcimage) {
	Mat binaryiamge;
	Mat grayimage;
	cvtColor(srcimage, grayimage, CV_BGR2GRAY);
	//threshold(grayimage, binaryiamge, 150, 255, THRESH_BINARY);
	adaptiveThreshold(grayimage, binaryiamge, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY, 7, 5);
	//CV_ADAPTIVE_THRESH_MEAN_C CV_ADAPTIVE_THRESH_GAUSSIAN_C
	return binaryiamge;
}
inline int otsu(Mat img) {
	int histogram[256] = { 0 };
	double probability[256] = { 0.0 };
	/*����Ҷ�ֱ��ͼ*/
	for (int i = 0; i < img.rows; i++) {
		for (int j = 0; j < img.cols; j++) {
			histogram[img.at<uchar>(i, j)]++;
		}
	}
	/*���ÿ���Ҷ�ֵ���ֵı���*/
	for (int i = 0; i < 256; i++) {
		probability[i] = histogram[i] / ((img.rows*img.cols) + 0.0);
	}
	double averagelevel = 0.0;
	/*����ܵ�ƽ���Ҷ�*/
	for (int i = 0; i < 256; i++) {
		averagelevel += i*probability[i];
	}

	double max = 0.0;
	int maxthreshold = 0;
	for (int i = 0; i < 256; i++) {
		double p1 = 0.0;
		double p2 = 0.0;
		double averageuj = 0.0;
		/*����ǰ���ı�����ƽ���Ҷ�*/
		for (int j = 0; j <= i; j++) {
			p1 += probability[j];  //ǰ������ռͼ�����
			averageuj += j*probability[j];
		}
		double m1 = averageuj / p1;//ǰ����ƽ���Ҷ�
		p2 = 1 - p1; //��������ռͼ�����
		double m2 = (averagelevel - averageuj) / p2;//������ƽ���Ҷ�
		double g = p1*p2*(m1 - m2)*(m1 - m2); //���ڷ���
		//cout << g << endl;
		if (g > max) {
			max = g; //��������ڷ���
			maxthreshold = i; //���ʹ�ü��ڷ���������ֵ
		}
	}
	//cout << maxthreshold << endl;
	return maxthreshold;
}
inline Mat Showhist(Mat srcimage) {
	cvtColor(srcimage, srcimage, CV_BGR2GRAY);
	MatND hist;
	int bins = 256;
	int hist_size[] = { bins };
	float range[] = { 0, 256 };
	const float* ranges[] = { range };
	int channels[] = { 0 };
	//����ֱ��ͼ  
	calcHist(&srcimage, 1, channels, Mat(), // do not use mask    
		hist, 1, hist_size, ranges,
		true, // the histogram is uniform    
		false);

	//����ֱ��ͼͼ��  
	int hist_height = 420;
	//int bins = 256;  
	double max_val;  //ֱ��ͼ�����ֵ  
	int scale = 1;   //ֱ��ͼ�Ŀ��  
	minMaxLoc(hist, 0, &max_val, 0, 0); //����ֱ��ͼ���ֵ  

	Mat hist_img = Mat::zeros(hist_height, bins*scale, CV_8UC3); //����һ��ֱ��ͼͼ�񲢳�ʼ��Ϊ0   
	//��ֱ��ͼͼ����д��ֱ��ͼ����  
	for (int i = 0; i<bins; i++)
	{
		float bin_val = hist.at<float>(i); // ��i�Ҷȼ��ϵ���      
		int intensity = cvRound(bin_val*hist_height / max_val);  //Ҫ���Ƶĸ߶�    
		//����i�Ҷȼ�������  
		rectangle(hist_img, Point(i*scale, hist_height - 1),
			Point((i + 1)*scale - 1, hist_height - intensity),
			CV_RGB(255,255,255));
	}
	return hist_img;
}

inline Mat ScaleImage(Mat srcimg, int h, int w){
	Size dsize = Size(h, w);
	Mat scaleimage = Mat(dsize, CV_32S);
	resize(srcimg, scaleimage, dsize);
	return scaleimage;
}