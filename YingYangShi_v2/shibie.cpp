#include "stdafx.h"

#include "shibie.h"

using namespace cv;
using namespace std;

//查找源图像中模板图像所在的位置
///Method: \n 0: SQDIFF \n 1: SQDIFF NORMED \n 2: TM CCORR \n 3: TM CCORR NORMED \n 4: TM COEFF \n 5: TM COEFF NORMED
Point ShiBie(char * img_name, char * templ_name, int match_method)
{
	Mat img; Mat templ; Mat result;

	/// Load image and template  载入图像
	img = imread(img_name);
	templ = imread(templ_name);

	/// Source image to display   创建原图像副本
	Mat img_display;
	img.copyTo(img_display);

	/// Create the result matrix	创建将存储每个模板位置的匹配结果的结果矩阵。 详细观察结果矩阵的大小（与之匹配的所有可能位置）
	int result_cols = img.cols - templ.cols + 1;
	int result_rows = img.rows - templ.rows + 1;
	result.create(result_rows, result_cols, CV_32FC1);

	//matchTemplate  源图像 模板图像 结果矩阵 匹配幅度 
	matchTemplate(img, templ, result, match_method);
	normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());

	/// Localizing the best match with minMaxLoc  使用minMaxLoc定位结果矩阵R中的最小值和最大值 找到最佳匹配
	double minVal; double maxVal; Point minLoc; Point maxLoc;
	Point matchLoc; double matchVal;

	minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());

	/// For SQDIFF and SQDIFF_NORMED, the best matches are lower values. For all the other methods, the higher the better
	//对于SQDIFF和SQDIFF_NORMED，最佳匹配是较低的值。对于所有其他方法，越高越好
	if (match_method == CV_TM_SQDIFF || match_method == CV_TM_SQDIFF_NORMED)
	{
		matchLoc = minLoc;
		matchVal = minVal;
	}
	else
	{
		matchLoc = maxLoc;
		matchVal = maxVal;
	}
	//使用3: TM CCORR NORMED 方法可以判断出是否存在图片 
	if (matchVal != 1 && match_method == 3)
	{
		matchLoc.x = matchLoc.y = 0;
	}
	return matchLoc;
}


uchar ShiBie_Buff[568 * 1005 * 4];

//查找源图像中模板图像所在的位置
///Method: \n 0: SQDIFF \n 1: SQDIFF NORMED \n 2: TM CCORR \n 3: TM CCORR NORMED \n 4: TM COEFF \n 5: TM COEFF NORMED
Point ShiBie1(HBITMAP Img_Bitmap, char * templ_name, int match_method)
{
	Mat img; Mat templ; Mat result;

	/// Load image and template  载入图像
	HBITMAP Templ_Bitmap;
	BITMAP bmp;
	int nChannels;

	GetObject(Img_Bitmap, sizeof(BITMAP), &bmp);

	nChannels = bmp.bmBitsPixel / 8;
	GetBitmapBits(Img_Bitmap, bmp.bmHeight*bmp.bmWidth * nChannels, ShiBie_Buff);

	img.create(cvSize(bmp.bmWidth, bmp.bmHeight), CV_MAKETYPE(CV_8U, 3));

	int j = 0;
	for (size_t i = 0; i < bmp.bmHeight*bmp.bmWidth * 4; i++)
	{
		if ((i + 1) % 4 != 0)
		{
			img.data[j++] = ShiBie_Buff[i];
		}

	}


	templ = imread(templ_name);
	//Templ_Bitmap = (HBITMAP)LoadImage(NULL, templ_name, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	//GetObject(Templ_Bitmap, sizeof(BITMAP), &bmp);

	//templ.create(cvSize(bmp.bmWidth, bmp.bmHeight), CV_MAKETYPE(CV_8U, nChannels));
	//GetBitmapBits(Templ_Bitmap, bmp.bmHeight*bmp.bmWidth * nChannels, templ.data);


	/// Source image to display   创建原图像副本
	Mat img_display;
	img.copyTo(img_display);

	/// Create the result matrix	创建将存储每个模板位置的匹配结果的结果矩阵。 详细观察结果矩阵的大小（与之匹配的所有可能位置）
	int result_cols = img.cols - templ.cols + 1;
	int result_rows = img.rows - templ.rows + 1;
	result.create(result_rows, result_cols, CV_32FC1);

	//matchTemplate  源图像 模板图像 结果矩阵 匹配幅度 
	matchTemplate(img, templ, result, match_method);
	normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());

	/// Localizing the best match with minMaxLoc  使用minMaxLoc定位结果矩阵R中的最小值和最大值 找到最佳匹配
	double minVal; double maxVal; Point minLoc; Point maxLoc;
	Point matchLoc; double matchVal;

	minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());

	/// For SQDIFF and SQDIFF_NORMED, the best matches are lower values. For all the other methods, the higher the better
	//对于SQDIFF和SQDIFF_NORMED，最佳匹配是较低的值。对于所有其他方法，越高越好
	if (match_method == CV_TM_SQDIFF || match_method == CV_TM_SQDIFF_NORMED)
	{
		matchLoc = minLoc;
		matchVal = minVal;
	}
	else
	{
		matchLoc = maxLoc;
		matchVal = maxVal;
	}
	//使用3: TM CCORR NORMED 方法可以判断出是否存在图片 
	if (matchVal != 1 && match_method == 3)
	{
		matchLoc.x = matchLoc.y = 0;
	}
	return matchLoc;
}

///32位位图转8位位图
void Img32toImg8(unsigned long *img32, unsigned char *img8, long leth)
{
	unsigned char *A, *R, *G, *B;
	for (size_t i = 0; i < leth; i++)
	{
		B = (unsigned char *)img32;
		G = ((unsigned char *)img32) + 1;
		R = ((unsigned char *)img32) + 2;
		A = ((unsigned char *)img32) + 3;
		*img8 = (((*R) / 32) << 5) + (((*G) / 32) << 2) + ((*B) / 64);
		img8++;
		img32++;
	}
}

unsigned char user_img[100 * 100 * 4];
Point ShiBie2(HBITMAP Img_Bitmap, char * templ_name, int match_method)
{
	Mat img; Mat templ; Mat result;

	/// Load image and template  载入图像
	HBITMAP Templ_Bitmap;
	BITMAP bmp;
	int nChannels;

	GetObject(Img_Bitmap, sizeof(BITMAP), &bmp);

	nChannels = bmp.bmBitsPixel / 8;
	GetBitmapBits(Img_Bitmap, bmp.bmHeight*bmp.bmWidth * nChannels, ShiBie_Buff);

	img.create(cvSize(bmp.bmWidth, bmp.bmHeight), CV_MAKETYPE(CV_8U, 1));

	Img32toImg8((unsigned long *)ShiBie_Buff, img.data, bmp.bmHeight*bmp.bmWidth);

	//cvConvertScale(ShiBie_Buff, img.data, 255, 0);



	//templ = imread(templ_name);
	Templ_Bitmap = (HBITMAP)LoadImageA(NULL, templ_name, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	GetObject(Templ_Bitmap, sizeof(BITMAP), &bmp);
	GetBitmapBits(Templ_Bitmap, bmp.bmHeight*bmp.bmWidth * nChannels, user_img);

	templ.create(cvSize(bmp.bmWidth, bmp.bmHeight), CV_MAKETYPE(CV_8U, 1));
	Img32toImg8((unsigned long *)user_img, templ.data, bmp.bmHeight*bmp.bmWidth);
	//cvConvertScale(user_img, templ.data, 255, 0);

	//转换为灰度图
	//cvtColor(img, img, CV_BGR2GRAY);
	//cvtColor(templ, templ, CV_BGR2GRAY);

	/// Source image to display   创建原图像副本
	Mat img_display;
	img.copyTo(img_display);

	/// Create the result matrix	创建将存储每个模板位置的匹配结果的结果矩阵。 详细观察结果矩阵的大小（与之匹配的所有可能位置）
	int result_cols = img.cols - templ.cols + 1;
	int result_rows = img.rows - templ.rows + 1;
	result.create(result_rows, result_cols, CV_8UC1);

	//matchTemplate  源图像 模板图像 结果矩阵 匹配幅度 
	matchTemplate(img, templ, result, match_method);
	normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());

	/// Localizing the best match with minMaxLoc  使用minMaxLoc定位结果矩阵R中的最小值和最大值 找到最佳匹配
	double minVal; double maxVal; Point minLoc; Point maxLoc;
	Point matchLoc; double matchVal;

	minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());

	/// For SQDIFF and SQDIFF_NORMED, the best matches are lower values. For all the other methods, the higher the better
	//对于SQDIFF和SQDIFF_NORMED，最佳匹配是较低的值。对于所有其他方法，越高越好
	if (match_method == CV_TM_SQDIFF || match_method == CV_TM_SQDIFF_NORMED)
	{
		matchLoc = minLoc;
		matchVal = minVal;
	}
	else
	{
		matchLoc = maxLoc;
		matchVal = maxVal;
	}
	//使用3: TM CCORR NORMED 方法可以判断出是否存在图片 
	if (matchVal != 1 && match_method == 3)
	{
		matchLoc.x = matchLoc.y = 0;
	}
	return matchLoc;
}

Point ShiBie3(HBITMAP Img_Bitmap, char * templ_name, int match_method)
{
	Mat img; Mat templ; Mat result;

	/// Load image and template  载入图像
	HBITMAP Templ_Bitmap;
	BITMAP bmp;
	int nChannels;

	GetObject(Img_Bitmap, sizeof(BITMAP), &bmp);

	img.create(cvSize(bmp.bmWidth, bmp.bmHeight), CV_MAKETYPE(CV_8U, 4));

	nChannels = bmp.bmBitsPixel / 8;
	GetBitmapBits(Img_Bitmap, bmp.bmHeight*bmp.bmWidth * nChannels, img.data);




	templ = imread(templ_name);

	//cvConvertScale(user_img, templ.data, 255, 0);

	//转换为灰度图
	cvtColor(img, img, CV_BGR2GRAY);
	cvtColor(templ, templ, CV_BGR2GRAY);

	/// Source image to display   创建原图像副本
	Mat img_display;
	img.copyTo(img_display);

	/// Create the result matrix	创建将存储每个模板位置的匹配结果的结果矩阵。 详细观察结果矩阵的大小（与之匹配的所有可能位置）
	int result_cols = img.cols - templ.cols + 1;
	int result_rows = img.rows - templ.rows + 1;
	result.create(result_rows, result_cols, CV_8UC1);

	//matchTemplate  源图像 模板图像 结果矩阵 匹配幅度 
	matchTemplate(img, templ, result, match_method);
	normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());

	/// Localizing the best match with minMaxLoc  使用minMaxLoc定位结果矩阵R中的最小值和最大值 找到最佳匹配
	double minVal; double maxVal; Point minLoc; Point maxLoc;
	Point matchLoc; double matchVal;

	minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());

	/// For SQDIFF and SQDIFF_NORMED, the best matches are lower values. For all the other methods, the higher the better
	//对于SQDIFF和SQDIFF_NORMED，最佳匹配是较低的值。对于所有其他方法，越高越好
	if (match_method == CV_TM_SQDIFF || match_method == CV_TM_SQDIFF_NORMED)
	{
		matchLoc = minLoc;
		matchVal = minVal;
	}
	else
	{
		matchLoc = maxLoc;
		matchVal = maxVal;
	}
	//使用3: TM CCORR NORMED 方法可以判断出是否存在图片 
	if (matchVal != 1 && match_method == 3)
	{
		matchLoc.x = matchLoc.y = 0;
	}

	//Method: \n 0: SQDIFF \n 1: SQDIFF NORMED \n 2: TM CCORR \n 3: TM CCORR NORMED \n 4: TM COEFF \n 5: TM COEFF NORMED
	return matchLoc;
}

extern Mat Game_img;

cv::Point ShiBie(HBITMAP Img_Bitmap, char * templ_name, int match_method)
{
	Mat templ; Mat result;


	/// Create the result matrix	创建将存储每个模板位置的匹配结果的结果矩阵。 详细观察结果矩阵的大小（与之匹配的所有可能位置）
	int result_cols = Game_img.cols - templ.cols + 1;
	int result_rows = Game_img.rows - templ.rows + 1;
	result.create(result_rows, result_cols, CV_8UC1);

	//matchTemplate  源图像 模板图像 结果矩阵 匹配幅度 
	matchTemplate(Game_img, templ, result, match_method);
	normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());

	/// Localizing the best match with minMaxLoc  使用minMaxLoc定位结果矩阵R中的最小值和最大值 找到最佳匹配
	double minVal; double maxVal; Point minLoc; Point maxLoc;
	Point matchLoc; double matchVal;

	minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());

	/// For SQDIFF and SQDIFF_NORMED, the best matches are lower values. For all the other methods, the higher the better
	//对于SQDIFF和SQDIFF_NORMED，最佳匹配是较低的值。对于所有其他方法，越高越好
	if (match_method == CV_TM_SQDIFF || match_method == CV_TM_SQDIFF_NORMED)
	{
		matchLoc = minLoc;
		matchVal = minVal;
	}
	else
	{
		matchLoc = maxLoc;
		matchVal = maxVal;
	}
	//使用3: TM CCORR NORMED 方法可以判断出是否存在图片 
	if (matchVal != 1 && match_method == 3)
	{
		matchLoc.x = matchLoc.y = 0;
	}

	//Method: \n 0: SQDIFF \n 1: SQDIFF NORMED \n 2: TM CCORR \n 3: TM CCORR NORMED \n 4: TM COEFF \n 5: TM COEFF NORMED
	return matchLoc;
}

int  PiPei_typemethod;        //匹配类型
double PiPei_Val;		//匹配结果的阀值
Point ShiBie(Mat templ, int match_method)
{
    Mat result;

	/// Create the result matrix	创建将存储每个模板位置的匹配结果的结果矩阵。 详细观察结果矩阵的大小（与之匹配的所有可能位置）
	int result_cols = Game_img.cols - templ.cols + 1;
	int result_rows = Game_img.rows - templ.rows + 1;
	result.create(result_rows, result_cols, CV_8UC1);

	//matchTemplate  源图像 模板图像 结果矩阵 匹配幅度 
	matchTemplate(Game_img, templ, result, match_method);
	normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());

	/// Localizing the best match with minMaxLoc  使用minMaxLoc定位结果矩阵R中的最小值和最大值 找到最佳匹配
	double minVal; double maxVal; Point minLoc; Point maxLoc;
	Point matchLoc; double matchVal;

	minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());

	/// For SQDIFF and SQDIFF_NORMED, the best matches are lower values. For all the other methods, the higher the better
	//对于SQDIFF和SQDIFF_NORMED，最佳匹配是较低的值。对于所有其他方法，越高越好
	if (match_method == CV_TM_SQDIFF || match_method == CV_TM_SQDIFF_NORMED)
	{
		matchLoc = minLoc;
		matchVal = minVal;
	}
	else
	{
		matchLoc = maxLoc;
		matchVal = maxVal;
	}
	//使用3: TM CCORR NORMED 方法可以判断出是否存在图片 
	if (matchVal != 1 && match_method == 3)
	{
		matchLoc.x = matchLoc.y = 0;
	}
	PiPei_typemethod = match_method;
	PiPei_Val = matchVal;
	//Method: \n 0: SQDIFF \n 1: SQDIFF NORMED \n 2: TM CCORR \n 3: TM CCORR NORMED \n 4: TM COEFF \n 5: TM COEFF NORMED
	return matchLoc;
}

double ShiBie_LunKuo(vector<vector<Point>> tmp)
{
	Mat bk_img;
	vector<vector<Point>> bk_vector;
	vector<Vec4i> bk_hierarch;
	Canny(Game_img, bk_img, 20, 50, 3);		//进行轮廓化

	findContours(bk_img, bk_vector, bk_hierarch, CV_RETR_TREE, CV_CHAIN_APPROX_NONE);//提取轮廓元素

	return  matchShapes(bk_vector[0], tmp[0], CV_CONTOURS_MATCH_I3, 1.0);//进行轮廓匹配
}
