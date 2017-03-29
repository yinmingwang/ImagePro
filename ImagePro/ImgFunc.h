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
	/*计算灰度直方图*/
	for (int i = 0; i < img.rows; i++) {
		for (int j = 0; j < img.cols; j++) {
			histogram[img.at<uchar>(i, j)]++;
		}
	}
	/*求出每个灰度值出现的比例*/
	for (int i = 0; i < 256; i++) {
		probability[i] = histogram[i] / ((img.rows*img.cols) + 0.0);
	}
	double averagelevel = 0.0;
	/*求出总的平均灰度*/
	for (int i = 0; i < 256; i++) {
		averagelevel += i*probability[i];
	}

	double max = 0.0;
	int maxthreshold = 0;
	for (int i = 0; i < 256; i++) {
		double p1 = 0.0;
		double p2 = 0.0;
		double averageuj = 0.0;
		/*计算前景的比例和平均灰度*/
		for (int j = 0; j <= i; j++) {
			p1 += probability[j];  //前景点数占图像比例
			averageuj += j*probability[j];
		}
		double m1 = averageuj / p1;//前景的平均灰度
		p2 = 1 - p1; //背景点数占图像比例
		double m2 = (averagelevel - averageuj) / p2;//背景的平均灰度
		double g = p1*p2*(m1 - m2)*(m1 - m2); //间内方差
		//cout << g << endl;
		if (g > max) {
			max = g; //求出最大间内方差
			maxthreshold = i; //求出使得间内方差最大的阈值
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
	//计算直方图  
	calcHist(&srcimage, 1, channels, Mat(), // do not use mask    
		hist, 1, hist_size, ranges,
		true, // the histogram is uniform    
		false);

	//绘制直方图图像  
	int hist_height = 420;
	//int bins = 256;  
	double max_val;  //直方图的最大值  
	int scale = 1;   //直方图的宽度  
	minMaxLoc(hist, 0, &max_val, 0, 0); //计算直方图最大值  

	Mat hist_img = Mat::zeros(hist_height, bins*scale, CV_8UC3); //创建一个直方图图像并初始化为0   
	//在直方图图像中写入直方图数据  
	for (int i = 0; i<bins; i++)
	{
		float bin_val = hist.at<float>(i); // 第i灰度级上的数      
		int intensity = cvRound(bin_val*hist_height / max_val);  //要绘制的高度    
		//填充第i灰度级的数据  
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

inline Mat rotation(Mat srcImg, double angle) {
	Mat tempImg;
	CV_Assert(!srcImg.empty());
	float radian = (float)(angle / 180.0 * CV_PI);
	//填充图像使其符合旋转要求
	int uniSize = (int)(max(srcImg.cols, srcImg.rows)* 1.414);
	int dx = (int)(uniSize - srcImg.cols) / 2;
	int dy = (int)(uniSize - srcImg.rows) / 2;
	copyMakeBorder(srcImg, tempImg, dy, dy, dx, dx, BORDER_CONSTANT);
	//旋转中心
	Point2f center((float)(tempImg.cols / 2), (float)(tempImg.rows / 2));
	Mat affine_matrix = getRotationMatrix2D(center, angle, 1.0);
	//旋转
	warpAffine(tempImg, tempImg, affine_matrix, tempImg.size());
	//旋转后的图像大小
	float sinVal = fabs(sin(radian));
	float cosVal = fabs(cos(radian));
	Size targetSize((int)(srcImg.cols * cosVal + srcImg.rows * sinVal),
		(int)(srcImg.cols * sinVal + srcImg.rows * cosVal));

	//剪掉四周边框
	int x = (tempImg.cols - targetSize.width) / 2;
	int y = (tempImg.rows - targetSize.height) / 2;
	Rect rect(x, y, targetSize.width, targetSize.height);
	tempImg = Mat(tempImg, rect);
	//imshow("Show", tempImg);
	return tempImg;
}
inline Mat FlipImages(Mat srcimage) {
	Mat flipimage;
	flipimage.create(srcimage.size(), srcimage.type());
	Mat map_x;
	Mat map_y;
	map_x.create(srcimage.size(), CV_32FC1);
	map_y.create(srcimage.size(), CV_32FC1);
	for (int i = 0; i < srcimage.rows; ++i)
	{
		for (int j = 0; j < srcimage.cols; ++j)
		{
			map_x.at<float>(i, j) = (float)(srcimage.cols - j);
			map_y.at<float>(i, j) = (float)i;  
		}
	}
	remap(srcimage, flipimage, map_x, map_y, CV_INTER_LINEAR);
	return flipimage;
}
inline Mat ChangeContrastAndBright(Mat srcimage, int contrastvalue, int brightvalue) {
	Mat cAb = Mat::zeros(srcimage.size(), srcimage.type());
	for (int i = 0; i < srcimage.rows; i++) {
		for (int j = 0; j < srcimage.cols; j++) {
			for (int k = 0; k < 3; k++) {
				cAb.at<Vec3b>(i, j)[k] = saturate_cast<uchar>((contrastvalue*0.01)*(srcimage.at<Vec3b>(i, j)[k]) + brightvalue);
			}
		}
	}
	return cAb;
}
inline Mat EdgeDetection_Laplacian(Mat srcimage) {
	GaussianBlur(srcimage, srcimage, Size(3, 3), 0, 0, BORDER_DEFAULT);
	Mat grayimage;
	Mat Laplacianimage;
	Mat absimage;
	cvtColor(srcimage, grayimage, COLOR_RGB2GRAY);
	Laplacian(grayimage, Laplacianimage, CV_16S, 3, 1, 0, BORDER_DEFAULT);
	convertScaleAbs(Laplacianimage, absimage);
	return absimage;
}
inline Mat EdgeDetection_Sobel_X(Mat srcimage) {
	Mat gradX, grayimage;
	Mat absX;
	GaussianBlur(srcimage, srcimage, Size(3, 3), 0, 0, BORDER_DEFAULT);
	cvtColor(srcimage, grayimage, CV_RGB2GRAY);
	Sobel(grayimage, gradX, CV_16S, 1, 0, 3, 1, 0, BORDER_DEFAULT);
	convertScaleAbs(gradX, absX);
	return absX;
}
inline Mat EdgeDetection_Sobel_Y(Mat srcimage) {
	Mat gradY, absY, grayimage;
	GaussianBlur(srcimage, srcimage, Size(3, 3), 0, 0, BORDER_DEFAULT);
	cvtColor(srcimage, grayimage, CV_RGB2GRAY);
	Sobel(grayimage, gradY, CV_16S, 0, 1, 3, 1, 0, BORDER_DEFAULT);
	convertScaleAbs(gradY, absY);
	return absY;
}
inline Mat EdgeDetection_Sobel(Mat srcimage){
	Mat gradX, gradY, absX, absY, sobelimage, grayimage;
	GaussianBlur(srcimage, srcimage, Size(3, 3), 0, 0, BORDER_DEFAULT);
	cvtColor(srcimage, grayimage, CV_RGB2GRAY);
	Sobel(grayimage, gradX, CV_16S, 1, 0, 3, 1, 0, BORDER_DEFAULT);
	convertScaleAbs(gradX, absX);
	Sobel(grayimage, gradY, CV_16S, 0, 1, 3, 1, 0, BORDER_DEFAULT);
	convertScaleAbs(gradY, absY);
	addWeighted(absX, 0.5, absY, 0.5, 0, sobelimage);
	return sobelimage;
}
inline Mat EdgeDetecction_Scharr_X(Mat srcimage) {
	Mat gradX, grayimage;
	Mat absX;
	GaussianBlur(srcimage, srcimage, Size(3, 3), 0, 0, BORDER_DEFAULT);
	cvtColor(srcimage, grayimage, CV_RGB2GRAY);
	Scharr(grayimage, gradX, CV_16S, 1, 0, 1, 0, BORDER_DEFAULT);
	convertScaleAbs(gradX, absX);
	return absX;
}
inline Mat EdgeDetecction_Scharr_Y(Mat srcimage) {
	Mat gradY, absY, grayimage;
	GaussianBlur(srcimage, srcimage, Size(3, 3), 0, 0, BORDER_DEFAULT);
	cvtColor(srcimage, grayimage, CV_RGB2GRAY);
	Scharr(grayimage, gradY, CV_16S, 0, 1, 1, 0, BORDER_DEFAULT);
	convertScaleAbs(gradY, absY);
	return absY;
}
inline Mat EdgeDetecction_Scharr(Mat srcimage) {
	Mat gradX, gradY, absX, absY, scharrimage, grayimage;
	GaussianBlur(srcimage, srcimage, Size(3, 3), 0, 0, BORDER_DEFAULT);
	cvtColor(srcimage, grayimage, CV_RGB2GRAY);
	Scharr(grayimage, gradX, CV_16S, 1, 0, 1, 0, BORDER_DEFAULT);
	convertScaleAbs(gradX, absX);
	Scharr(grayimage, gradY, CV_16S, 0, 1, 1, 0, BORDER_DEFAULT);
	convertScaleAbs(gradY, absY);
	addWeighted(absX, 0.5, absY, 0.5, 0, scharrimage);
	return scharrimage;
}
//filter
//Box
inline Mat Fun_Box_Filter(Mat srcimage, int image_size) {
	Mat Boximage;
	boxFilter(srcimage, Boximage, -1, Size(image_size+1, image_size+1));
	return Boximage;
}

//Mean
inline Mat Fun_Mean_Filter(Mat srcimage, int image_size) {
	Mat Meamimage;
	blur(srcimage, Meamimage, Size(image_size+1, image_size+1), Point(-1, -1));
	return Meamimage;
}
//Gaussian
inline Mat Fun_Gaussian_Filter(Mat srcimage, int image_size) {
	Mat Gimage;
	GaussianBlur(srcimage, Gimage, Size(image_size*2+1, image_size*2+1), 0, 0);
	return Gimage;
}
//Median
inline Mat Fun_Median_Filter(Mat srcimage, int image_size) {
	Mat Mimage;
	medianBlur(srcimage, Mimage, image_size*2+1);
	return Mimage;
}
//Bilateral
inline Mat Fun_Bilateral_Filter(Mat srcimage, int image_size) {
	Mat Bimage;
	bilateralFilter(srcimage, Bimage, image_size, image_size*2, image_size/2);
	return Bimage;
}
inline Mat EnhanceImages(Mat srcimage) {
	Mat enhanceimage;
	vector<Mat> channel;
	split(srcimage, channel);
	for (int i = 0; i < srcimage.channels(); i++) {
		equalizeHist(channel[i], channel[i]);
	}
	merge(channel, enhanceimage);
	return enhanceimage;
}
//detect
inline void detectAndDraw(Mat& img, CascadeClassifier& cascade,
	CascadeClassifier& nestedCascade,
	double scale, bool tryflip)
{
	int i = 0;
	double t = 0;
	//建立用于存放人脸的向量容器
	vector<Rect> faces, faces2;
	//定义一些颜色，用来标示不同的人脸
	const static Scalar colors[] = {
		CV_RGB(0, 0, 255),
		CV_RGB(0, 128, 255),
		CV_RGB(0, 255, 255),
		CV_RGB(0, 255, 0),
		CV_RGB(255, 128, 0),
		CV_RGB(255, 255, 0),
		CV_RGB(255, 0, 0),
		CV_RGB(255, 0, 255) };
	//建立缩小的图片，加快检测速度
	//nt cvRound (double value) 对一个double型的数进行四舍五入，并返回一个整型数！
	Mat gray, smallImg(cvRound(img.rows / scale), cvRound(img.cols / scale), CV_8UC1);
	//转成灰度图像，Harr特征基于灰度图
	cvtColor(img, gray, CV_BGR2GRAY);
	//改变图像大小，使用双线性差值
	resize(gray, smallImg, smallImg.size(), 0, 0, INTER_LINEAR);
	//变换后的图像进行直方图均值化处理
	equalizeHist(smallImg, smallImg);
	//程序开始和结束插入此函数获取时间，经过计算求得算法执行时间
	t = (double)cvGetTickCount();
	//检测人脸
	//detectMultiScale函数中smallImg表示的是要检测的输入图像为smallImg，faces表示检测到的人脸目标序列，1.1表示
	//每次图像尺寸减小的比例为1.1，2表示每一个目标至少要被检测到3次才算是真的目标(因为周围的像素和不同的窗口大
	//小都可以检测到人脸),CV_HAAR_SCALE_IMAGE表示不是缩放分类器来检测，而是缩放图像，Size(30, 30)为目标的
	//最小最大尺寸
	cascade.detectMultiScale(smallImg, faces,
		1.1, 2, 0
		//|CV_HAAR_FIND_BIGGEST_OBJECT
		//|CV_HAAR_DO_ROUGH_SEARCH
		| CV_HAAR_SCALE_IMAGE
		, Size(30, 30));
	//如果使能，翻转图像继续检测
	if (tryflip)
	{
		flip(smallImg, smallImg, 1);
		cascade.detectMultiScale(smallImg, faces2,
			1.1, 2, 0
			//|CV_HAAR_FIND_BIGGEST_OBJECT
			//|CV_HAAR_DO_ROUGH_SEARCH
			| CV_HAAR_SCALE_IMAGE
			, Size(30, 30));
		for (vector<Rect>::const_iterator r = faces2.begin(); r != faces2.end(); r++)
		{
			faces.push_back(Rect(smallImg.cols - r->x - r->width, r->y, r->width, r->height));
		}
	}
	t = (double)cvGetTickCount() - t;
	//   qDebug( "detection time = %g ms\n", t/((double)cvGetTickFrequency()*1000.) );
	for (vector<Rect>::const_iterator r = faces.begin(); r != faces.end(); r++, i++)
	{
		Mat smallImgROI;
		vector<Rect> nestedObjects;
		Point center;
		Scalar color = colors[i % 8];
		int radius;

		double aspect_ratio = (double)r->width / r->height;
		if (0.75 < aspect_ratio && aspect_ratio < 1.3)
		{
			//标示人脸时在缩小之前的图像上标示，所以这里根据缩放比例换算回去
			center.x = cvRound((r->x + r->width*0.5)*scale);
			center.y = cvRound((r->y + r->height*0.5)*scale);
			radius = cvRound((r->width + r->height)*0.25*scale);
			circle(img, center, radius, color, 3, 8, 0);
		}
		else
			rectangle(img, cvPoint(cvRound(r->x*scale), cvRound(r->y*scale)),
			cvPoint(cvRound((r->x + r->width - 1)*scale), cvRound((r->y + r->height - 1)*scale)),
			color, 3, 8, 0);
		if (nestedCascade.empty())
			continue;
		smallImgROI = smallImg(*r);
		//同样方法检测人眼
		nestedCascade.detectMultiScale(smallImgROI, nestedObjects,
			1.1, 2, 0
			//|CV_HAAR_FIND_BIGGEST_OBJECT
			//|CV_HAAR_DO_ROUGH_SEARCH
			//|CV_HAAR_DO_CANNY_PRUNING
			| CV_HAAR_SCALE_IMAGE
			, Size(30, 30));
		for (vector<Rect>::const_iterator nr = nestedObjects.begin(); nr != nestedObjects.end(); nr++)
		{
			center.x = cvRound((r->x + nr->x + nr->width*0.5)*scale);
			center.y = cvRound((r->y + nr->y + nr->height*0.5)*scale);
			radius = cvRound((nr->width + nr->height)*0.25*scale);
			circle(img, center, radius, color, 3, 8, 0);
		}
	}
	imshow("识别结果", img);
}