
#include "stdafx.h"
#include <iostream>
#include <fstream> 
#include "game.h"
#include "shibie.h"

/////����ˢͼ ��Ӱ��������������
//// 1. ѡ��ؿ� ������Ϊ500��250��
//// 2. ���̽��
//// 3. Ѱ��С��
//// 4. ����С��ʱ��Ѱ��BOSS ��С��
//// 5. Ѱ�ҽ���ֽ��
//// 6. ��ս�� 1

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
/////��Ϊopencv �ж���ͼƬλ�� ���ͼƬ������ �ᵼ�¾����Գ������е���� �������ж�ͼƬλ�õĺϷ���
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
	//ֻ�����λ�ò��Ǿ���ͼ���
	if (Addr.x > 740 && Addr.x < 820 && Addr.y > 480 && Addr.y < 520)
	{
		return 1;
	}
	return 0;
}

extern int  PiPei_typemethod;        //ƥ������
extern double PiPei_Val;		//ƥ�����ķ�ֵ
int Is_JiSha()
{
	///����ʹ����ɿ��ķ��� ����ƥ�� ��ֹ��������
	//HBITMAP ZiTu = (HBITMAP)LoadImage(NULL, L"image//Judgment_ends.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	//return BitmapToBitmap32(hIMG_Buff, ZiTu);

	Addr = ShiBie(Judgment_ends_mat, 0);
	ofstream outFile("out.txt", ios::app);  //��ĩβд��  
	outFile << "�����ж����ͣ�" << PiPei_typemethod << "  " << PiPei_Val << "����� " << Addr;
	outFile << endl;
	outFile.close();

	//ֻ�����λ�ò��ǽ���ͼ��� 434 345
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



//����ƥ��
bool Is_LunKuo_JiSha()
{
	double xx = ShiBie_LunKuo(Judgment_ends_LunKuo_vector);

	if (xx > 1)//��������ƥ�� ������˺ܾ��� ��ģ��ƥ������� ��Ҫ������ͼ������
	{
		return true;
	}
	return false;
}

/// �жϵ�ǰλ������ ��������ҵĴ���״̬
/// ע�⣺���뱣֤״̬�жϾ��Կɿ�
void Game_Status()
{
	HBITMAP ZiTu = NULL;


	//ͨ��ͼƬ�ж�����������λ��
	// 1 �����ͼ��
	if (Is_DiTuWai())
	{
		shutu_status = Find_Level;		//����ѡ��ؿ�ģʽ
	}
	// 2 ����������
	if (Is_DiTuZhong())
	{
		shutu_status = Mobs;			//����ѡ��С��ģʽ
	}
	
	// 3 ս�����������
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


	findContours(DiTuWai_LunKuo_mat, DiTuWai_vector, hierarch1, CV_RETR_TREE, CV_CHAIN_APPROX_NONE);//��ȡ����Ԫ��
	findContours(DiTuZhong_LunKuo_mat, DiTuZhong_vector, hierarch2, CV_RETR_TREE, CV_CHAIN_APPROX_NONE);//��ȡ����Ԫ��
	findContours(Judgment_ends_LunKuo_mat, Judgment_ends_LunKuo_vector, hierarch3, CV_RETR_TREE, CV_CHAIN_APPROX_NONE);//��ȡ����Ԫ��


	while (true)
	{
		SelectObject(hdcMem, hIMG_Buff);			//ѡ�����DC�У����滻ԭ�ж���				//����ͼ��DC��
//BitBlt(hdcMem, 0, 0, lpRect.right - lpRect.left, lpRect.bottom - lpRect.top, hdcSrc, 0, 0, SRCCOPY);				//��ָ����Դ�豸���������е����ؽ���λ�飨bit_block��ת�����Դ��͵�Ŀ���豸������
		if (BitBlt(hdcMem, 0, 0, lpRect.right - lpRect.left, lpRect.bottom - lpRect.top, hdcSrc, 0, 0, SRCCOPY) == false)				//��ָ����Դ�豸���������е����ؽ���λ�飨bit_block��ת�����Դ��͵�Ŀ���豸������
		{
			bit_err++;
		}

		//z����ֱ��ת��Ϊ�Ҷ�ͼ ����ÿ�δ���ͼ���ļ�
		GetObject(hIMG_Buff, sizeof(BITMAP), &bmp);
		Game_img.create(cvSize(bmp.bmWidth, bmp.bmHeight), CV_MAKETYPE(CV_8U, 4));
		nChannels = bmp.bmBitsPixel / 8;
		GetBitmapBits(hIMG_Buff, bmp.bmHeight*bmp.bmWidth * nChannels, Game_img.data);
		cvtColor(Game_img, Game_img, CV_BGR2GRAY);

		switch (shutu_status)
		{
			//ѡ��ؿ� 
		case Find_Level:
			sprintf_s(A, "״̬ :ѡ��\r");
			// ����ؿ�
			if (Is_Find_Level())
			{
				momvoerClock(Addr.x, Addr.y);
				break;
			}
			// ���̽��

			if (Is_TanSuo())
			{
				momvoerClock(Addr.x, Addr.y);
				shutu_status = Mobs;
				xiaoguai_num = 0;
			}
			break;
			//Ѱ��С��
		case Mobs:
			sprintf_s(A, "״̬ :��С��\r");
			ZiTu = "image//Mobs.bmp";
			Addr = ShiBie(Mobs_mat, 5);
			if ((Addr.x == 0 && Addr.y == 0) || Addr.y < 150 || Addr.y > 480)		//�жϲ��Ϸ�
			{
				//�жϵ�ǰ��������
				if (!Is_DiTuZhong())
				{
					shutu_status = Find_Level;
					break;
				}
				//������ڵ�ͼ��
				//�ж��Ƿ��н���Сֽ��
				else if (Paper_Man_Set())
				{
					shutu_status = Find_Level;
					break;
				}
				//��һƬû�й��� ��ʼ�ƶ�һ��
				else
				{
					if (xiaoguai_num < 3)
					{
						momvoerClock(800, 450);	//���ƶ�һ�ξ���
					}
					else
					{
						momvoerClock(100, 450);	//���ƶ�һ�ξ���
					}
					xiaoguai_num = (xiaoguai_num > 6) ? 0 : xiaoguai_num++;
				}
			}
			else
			{
				//�����ɱģʽ
				momvoerClock(Addr.x, Addr.y);
				shutu_status = Judgment_ends;
			}
			break;
		case Judgment_ends:
			sprintf_s(A, "״̬ :�ȴ���\r");
			if (Is_JiSha())
			{
				//�����ظ�������� ��ֹʧ�� ���������ҡ�ĵ�ʱ������Ч��
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
			Game_Status();////10S����һ��
			Time_num = 0;
		}
OutputDebugStringA(A);
SetWindowTextA(hwndEDIT,A);
ofstream outFile("out.txt", ios::app);  //��ĩβд��  
outFile << A;
outFile << endl;
outFile.close();
		Sleep(2000);
	}


}