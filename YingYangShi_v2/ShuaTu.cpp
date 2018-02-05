
#include "stdafx.h"
#include <iostream>
#include <fstream> 
#include "game.h"
#include "shibie.h"

/////无限刷图 不影响其他程序体验
//// 1. 选择关卡 （坐标为500，250）
//// 2. 点击探索
//// 3. 寻找小怪
//// 4. 当无小怪时，寻找BOSS 或小怪
//// 5. 寻找结束纸人
//// 6. 挑战到 1

using namespace std;
using namespace cv;



cv::Mat DiTuWai_mat;
cv::Mat DiTuZhong_mat;
cv::Mat TanSuo_mat;
cv::Mat Find_Level_mat;
cv::Mat Mobs_mat;
cv::Mat Judgment_ends_mat;

cv::Mat DiTuWai_LunKuo_mat;
cv::Mat DiTuZhong_LunKuo_mat;
cv::Mat Judgment_ends_LunKuo_mat;

vector<Vec4i> hierarch1, hierarch2, hierarch3;

vector<vector<Point>> DiTuWai_vector, DiTuZhong_vector,Judgment_ends_LunKuo_vector;


char* DiTuWai_name = "image//DiTuWai.png";
char* DiTuZhong_name = "image//DiTuZhong.png";
char* TanSuo_name = "image//tansuo.png";
char* Find_Level_name = "image//Find_Level.png";
char* Mobs_name = "image//Mobs.bmp";
char* Judgment_ends_name = "image//Judgment_ends.png";

cv::Point Addr;
cv::Mat Game_img;
int shutu_status = Find_Level;
/////因为opencv 判断子图片位置 如果图片不存在 会导致经常性出现误判的情况 故增加判断图片位置的合法性
int Is_DiTuWai()
{
	Addr = ShiBie(DiTuWai_mat, 0);
	if (Addr.x > 30 && Addr.x < 60 && Addr.y > 520 && Addr.y < 560)
	{
		return 1;
	}
	return 0;
}
int Is_Find_Level()
{
	Addr = ShiBie(Find_Level_mat, 5);
	if (Addr.x > 820 && Addr.y > 50)
	{
		return 1;
	}
	return 0;
}

int Is_TanSuo()
{
	Addr = ShiBie(TanSuo_mat, 0);
	if (Addr.x > 690 && Addr.x < 750 && Addr.y > 400 && Addr.y < 420)
	{
		return 1;
	}
	return 0;
}

int Is_DiTuZhong()
{
	Addr = ShiBie(DiTuZhong_mat, 0);
	//只有这个位置才是觉醒图标点
	if (Addr.x > 740 && Addr.x < 820 && Addr.y > 480 && Addr.y < 520)
	{
		return 1;
	}
	return 0;
}

extern int  PiPei_typemethod;        //匹配类型
extern double PiPei_Val;		//匹配结果的阀值
int Is_JiSha()
{
	///这里使用最可靠的方法 像素匹配 防止出现误判
	//HBITMAP ZiTu = (HBITMAP)LoadImage(NULL, L"image//Judgment_ends.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	//return BitmapToBitmap32(hIMG_Buff, ZiTu);

	Addr = ShiBie(Judgment_ends_mat, 0);
	ofstream outFile("out.txt", ios::app);  //从末尾写入  
	outFile << "结束判断类型：" << PiPei_typemethod << "  " << PiPei_Val << "坐标点 " << Addr;
	outFile << endl;
	outFile.close();

	//只有这个位置才是结束图标点 434 345
	if (Addr.x == 434 && Addr.y == 345)
	{
		return 1;
	}
	//if (Addr.x > 420 && Addr.x < 440 && Addr.y > 330 && Addr.y < 350)
	//{
	//	return 1;
	//}
	return 0;
}



//轮廓匹配
bool Is_LunKuo_JiSha()
{
	double xx = ShiBie_LunKuo(Judgment_ends_LunKuo_vector);

	if (xx > 1)//进行轮廓匹配 结果令人很绝望 跟模板匹配结果差不多 需要完整的图像轮廓
	{
		return true;
	}
	return false;
}

/// 判断当前位置在哪 并矫正外挂的错误状态
/// 注意：必须保证状态判断绝对可靠
void Game_Status()
{
	HBITMAP ZiTu = NULL;


	//通过图片判断现在所处的位置
	// 1 外面地图上
	if (Is_DiTuWai())
	{
		shutu_status = Find_Level;		//进入选择关卡模式
	}
	// 2 副本里面中
	if (Is_DiTuZhong())
	{
		shutu_status = Mobs;			//进入选择小怪模式
	}
	
	// 3 战斗结束后界面
	if (Is_JiSha())
	{
		shutu_status = Judgment_ends;
	}
}

extern HWND hwndEDIT;


DWORD WINAPI ShuaTu_Guanka()
{
	char * ZiTu = NULL;
	int xiaoguai_num = 0;
	int Time_num = 0;
	BITMAP bmp;
	int nChannels;

	char A[50];

	DiTuWai_mat = cv::imread(DiTuWai_name);
	DiTuZhong_mat = cv::imread(DiTuZhong_name);
	TanSuo_mat = cv::imread(TanSuo_name);
	Find_Level_mat = cv::imread(Find_Level_name);
	Mobs_mat = cv::imread(Mobs_name);
	Judgment_ends_mat = cv::imread(Judgment_ends_name);

	cvtColor(DiTuWai_mat, DiTuWai_mat, CV_BGR2GRAY);
	cvtColor(DiTuZhong_mat, DiTuZhong_mat, CV_BGR2GRAY);
	cvtColor(TanSuo_mat, TanSuo_mat, CV_BGR2GRAY);
	cvtColor(Find_Level_mat, Find_Level_mat, CV_BGR2GRAY);
	cvtColor(Mobs_mat, Mobs_mat, CV_BGR2GRAY);
	cvtColor(Judgment_ends_mat, Judgment_ends_mat, CV_BGR2GRAY);

	Canny(DiTuWai_mat, DiTuWai_LunKuo_mat, 20, 50, 3);
	Canny(DiTuZhong_mat, DiTuZhong_LunKuo_mat, 20, 50, 3);
	Canny(Judgment_ends_mat, Judgment_ends_LunKuo_mat, 20, 50, 3);


	findContours(DiTuWai_LunKuo_mat, DiTuWai_vector, hierarch1, CV_RETR_TREE, CV_CHAIN_APPROX_NONE);//提取轮廓元素
	findContours(DiTuZhong_LunKuo_mat, DiTuZhong_vector, hierarch2, CV_RETR_TREE, CV_CHAIN_APPROX_NONE);//提取轮廓元素
	findContours(Judgment_ends_LunKuo_mat, Judgment_ends_LunKuo_vector, hierarch3, CV_RETR_TREE, CV_CHAIN_APPROX_NONE);//提取轮廓元素


	while (true)
	{
		SelectObject(hdcMem, hIMG_Buff);			//选择对象到DC中（会替换原有对象）				//拷贝图像到DC上
//BitBlt(hdcMem, 0, 0, lpRect.right - lpRect.left, lpRect.bottom - lpRect.top, hdcSrc, 0, 0, SRCCOPY);				//对指定的源设备环境区域中的像素进行位块（bit_block）转换，以传送到目标设备环境。
		if (BitBlt(hdcMem, 0, 0, lpRect.right - lpRect.left, lpRect.bottom - lpRect.top, hdcSrc, 0, 0, SRCCOPY) == false)				//对指定的源设备环境区域中的像素进行位块（bit_block）转换，以传送到目标设备环境。
		{
			bit_err++;
		}

		//z这里直接转换为灰度图 不必每次传输图像文件
		GetObject(hIMG_Buff, sizeof(BITMAP), &bmp);
		Game_img.create(cvSize(bmp.bmWidth, bmp.bmHeight), CV_MAKETYPE(CV_8U, 4));
		nChannels = bmp.bmBitsPixel / 8;
		GetBitmapBits(hIMG_Buff, bmp.bmHeight*bmp.bmWidth * nChannels, Game_img.data);
		cvtColor(Game_img, Game_img, CV_BGR2GRAY);

		switch (shutu_status)
		{
			//选择关卡 
		case Find_Level:
			sprintf_s(A, "状态 :选关\r");
			// 点击关卡
			if (Is_Find_Level())
			{
				momvoerClock(Addr.x, Addr.y);
				break;
			}
			// 点击探索

			if (Is_TanSuo())
			{
				momvoerClock(Addr.x, Addr.y);
				shutu_status = Mobs;
				xiaoguai_num = 0;
			}
			break;
			//寻找小怪
		case Mobs:
			sprintf_s(A, "状态 :找小怪\r");
			ZiTu = "image//Mobs.bmp";
			Addr = ShiBie(Mobs_mat, 5);
			if ((Addr.x == 0 && Addr.y == 0) || Addr.y < 150 || Addr.y > 480)		//判断不合法
			{
				//判断当前所处环境
				if (!Is_DiTuZhong())
				{
					shutu_status = Find_Level;
					break;
				}
				//如果还在地图中
				//判断是否有结束小纸人
				else if (Paper_Man_Set())
				{
					shutu_status = Find_Level;
					break;
				}
				//这一片没有怪物 开始移动一次
				else
				{
					if (xiaoguai_num < 3)
					{
						momvoerClock(800, 450);	//右移动一段距离
					}
					else
					{
						momvoerClock(100, 450);	//左移动一段距离
					}
					xiaoguai_num = (xiaoguai_num > 6) ? 0 : xiaoguai_num++;
				}
			}
			else
			{
				//进入击杀模式
				momvoerClock(Addr.x, Addr.y);
				shutu_status = Judgment_ends;
			}
			break;
		case Judgment_ends:
			sprintf_s(A, "状态 :等打死\r");
			if (Is_JiSha())
			{
				//快速重复点击两次 防止失败 （好像结束摇的的时候点击无效）
				Sleep(500);
				momvoerClock(Addr.x, Addr.y);
				Sleep(200);
				momvoerClock(Addr.x, Addr.y);
				shutu_status = Mobs;
				//Sleep(4000);
			}
			break;
		default:
			break;
		}
		if (++Time_num  > 3)
		{
			Game_Status();////10S矫正一次
			Time_num = 0;
		}
OutputDebugStringA(A);
SetWindowTextA(hwndEDIT,A);
ofstream outFile("out.txt", ios::app);  //从末尾写入  
outFile << A;
outFile << endl;
outFile.close();
		Sleep(2000);
	}


}