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
	Mat grayimage = cv::Mat::zeros(srcimage.size(), CV_8UC1);
	vector<Mat> channel;
	split(srcimage, channel);
	for (int i = 0; i < srcimage.rows; i++) {
		for (int j = 0; j < srcimage.cols; j++) {
			grayimage.at<uchar>(i, j) = 0.11*channel[0].at<uchar>(i, j) + 0.59*channel[1].at<uchar>(i, j) +
				0.3*channel[2].at<uchar>(i, j);
		}
	}
	return grayimage;
}
inline Mat Tobinary(Mat srcimage) {
	Mat binaryiamge;
	Mat grayimage;
	cvtColor(srcimage, grayimage, CV_BGR2GRAY);
	adaptiveThreshold(grayimage, binaryiamge, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY, 7, 5);
	return binaryiamge;
}
inline int otsu(Mat img) {
	int histogram[256] = { 0 };
	double probability[256] = { 0.0 };
	for (int i = 0; i < img.rows; i++) {
		for (int j = 0; j < img.cols; j++) {
			histogram[img.at<uchar>(i, j)]++;
		}
	}
	for (int i = 0; i < 256; i++) {
		probability[i] = histogram[i] / ((img.rows*img.cols) + 0.0);
	}
	double averagelevel = 0.0;
	for (int i = 0; i < 256; i++) {
		averagelevel += i*probability[i];
	}

	double max = 0.0;
	int maxthreshold = 0;
	for (int i = 0; i < 256; i++) {
		double p1 = 0.0; 
		double p2 = 0.0;
		double averageuj = 0.0;
		for (int j = 0; j <= i; j++) {
			p1 += probability[j];  //前景点数占图像比例
			averageuj += j*probability[j];
		}
		double m1 = averageuj / p1;//前景的平均灰度
		p2 = 1 - p1; //背景点数占图像比例
		double m2 = (averagelevel - averageuj) / p2;//背景的平均灰度
		double g = p1*p2*(m1 - m2)*(m1 - m2); //间内方差
		if (g > max) {
			max = g; //求出最大间内方差
			maxthreshold = i; //求出使得间内方差最大的阈值
		}
	}
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
	/*vector<Mat> channel;
	vector<Mat> fchannel;
	Mat tempimage;
	Mat flipimage;
	tempimage.create(srcimage.size(), srcimage.type());
	split(srcimage, channel);
	split(tempimage, fchannel);
	//qDebug() << srcimage.channels() << endl;
	for (int k = 0; k < 3; k++) {
		for (int i = 0; i < srcimage.cols; i++) {
			int u = srcimage.cols - i - 1;
			for (int j = 0; j < srcimage.rows; j++) {
				fchannel[k].at<uchar>(j, i) = channel[k].at<uchar>(j, u);
			}
		}
	}
	merge(fchannel, flipimage);
	return flipimage;
*/
	
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