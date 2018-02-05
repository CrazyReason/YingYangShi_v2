#include "stdafx.h"

#include "game.h"

#include "shibie.h"

using namespace cv;
using namespace std;


HWND GameMianHend = NULL;		//��Ϸ������

HWND GameMoNiHend = NULL;		//ģ�����ӿ�
RECT lpRect;					//��Ϸ�����С


HDC hdcSrc, hdcMem;			//����DC

int start_x, start_y, end_x, end_y;		//�ҵ�λ�õ�����


HBITMAP hIMG_Buff;						//����ͼ�����ݶ���


int bit_err = 0;


//ģ����괦��
void momvoerClock()
{
	LPARAM zuobiao = start_y;
	zuobiao = zuobiao << 16;
	zuobiao += start_x;
	SendMessage(GameMoNiHend, WM_LBUTTONDOWN, MK_LBUTTON, zuobiao);
	SendMessage(GameMoNiHend, WM_LBUTTONUP, 0, zuobiao);

	char A[50];
	sprintf_s(A, "A %d,%d \r", start_x, start_y);
	OutputDebugStringA(A);
}

void momvoerClock(int x, int y)
{
	LPARAM zuobiao = y;
	zuobiao = zuobiao << 16;
	zuobiao += x;
	SendMessage(GameMoNiHend, WM_LBUTTONDOWN, MK_LBUTTON, zuobiao);
	SendMessage(GameMoNiHend, WM_LBUTTONUP, 0, zuobiao);

	char A[50];
	sprintf_s(A, "B %d,%d \r", x, y);
	OutputDebugStringA(A);
}

// ͼƬת���ڴ�������
//	hBitmap		ͼƬ���
//  lpbi		ָ��λͼ��Ϣͷ�ṹ     
BOOL BitmapToBitmap32(HBITMAP   hBitmap1, HBITMAP hBitmap2)
{
	HDC     hDC;
	//��ǰ�ֱ�����ÿ������ռ�ֽ���            
	int     iBits;
	//λͼ��ÿ������ռ�ֽ���            
	WORD     wBitCount;
	//�����ɫ���С��     λͼ�������ֽڴ�С     ��λͼ�ļ���С     ��     д���ļ��ֽ���                
	DWORD     dwPaletteSize = 0, dwBmBitsSize1 = 0, dwBmBitsSize2 = 0, dwDIBSize = 0, dwWritten = 0;
	//λͼ���Խṹ                
	BITMAP     Bitmap;
	//λͼ�ļ�ͷ�ṹ            
	BITMAPFILEHEADER     bmfHdr;
	//ָ��λͼ��Ϣͷ�ṹ                    
	LPBITMAPINFOHEADER     lpbi1, lpbi2;
	//λͼ��Ϣͷ�ṹ                
	BITMAPINFOHEADER     bi;

	//�����ļ��������ڴ�������ɫ����                
	HANDLE     hDib1, hDib2, hOldPal = NULL;

	//����λͼ�ļ�ÿ��������ռ�ֽ���  
	wBitCount = 32;

	GetObject(hBitmap1, sizeof(Bitmap), (LPSTR)&Bitmap);
	bi.biSize = sizeof(BITMAPINFOHEADER);
	bi.biWidth = Bitmap.bmWidth;
	bi.biHeight = Bitmap.bmHeight;
	bi.biPlanes = 1;
	bi.biBitCount = wBitCount;
	bi.biCompression = BI_RGB;
	bi.biSizeImage = Bitmap.bmWidth * 4 * Bitmap.bmHeight;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrImportant = 0;
	bi.biClrUsed = 0;

	// 24λ��ɫ��ȼ����С ��ȴ洢�Ĵ�С ����Ҫ4�ı��� 
	//��ͼƬ��� * λ�� + 31��/32 ��ʾ�����ɫ��Сλ����Ϊ32�ı��� ��һλ��������һ��long��С����Ϊ���������û���������룩������ոպã��򲻻��һ��long��С
	// ����24λ���ش�С��ͼƬ��ʵ��СӦ��Ϊ ���bmWidth*3 Ϊ4����������size = bmWidth*3*bmHeight������size = ((Bitmap.bmWidth *wBitCount + 31) / 32) * 4 * Bitmap.bmHeight
	dwBmBitsSize1 = Bitmap.bmWidth * 4 * Bitmap.bmHeight;



	//Ϊλͼ���ݷ����ڴ�                
	hDib1 = GlobalAlloc(GHND, dwBmBitsSize1 + dwPaletteSize + sizeof(BITMAPINFOHEADER));
	lpbi1 = (LPBITMAPINFOHEADER)GlobalLock(hDib1);
	*lpbi1 = bi;

	//     ��ȡ�õ�ɫ�����µ�����ֵ                
	GetDIBits(hdcMem, hBitmap1, 0, (UINT)Bitmap.bmHeight,
		(LPSTR)lpbi1 + sizeof(BITMAPINFOHEADER),
		(BITMAPINFO *)lpbi1, DIB_RGB_COLORS);


	//��2��ͼ
	GetObject(hBitmap2, sizeof(Bitmap), (LPSTR)&Bitmap);
	bi.biSize = sizeof(BITMAPINFOHEADER);
	bi.biWidth = Bitmap.bmWidth;
	bi.biHeight = Bitmap.bmHeight;
	bi.biPlanes = 1;
	bi.biBitCount = wBitCount;
	bi.biCompression = BI_RGB;
	bi.biSizeImage = Bitmap.bmWidth * 4 * Bitmap.bmHeight;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrImportant = 0;
	bi.biClrUsed = 0;

	dwBmBitsSize2 = Bitmap.bmWidth * 4 * Bitmap.bmHeight;

	//Ϊλͼ���ݷ����ڴ�                
	hDib2 = GlobalAlloc(GHND, dwBmBitsSize2 + dwPaletteSize + sizeof(BITMAPINFOHEADER));
	lpbi2 = (LPBITMAPINFOHEADER)GlobalLock(hDib2);
	*lpbi2 = bi;

	////     ��ȡ�õ�ɫ�����µ�����ֵ                
	GetDIBits(hdcMem, hBitmap2, 0, (UINT)Bitmap.bmHeight,
		(LPSTR)lpbi2 + sizeof(BITMAPINFOHEADER),
		(BITMAPINFO *)lpbi2, DIB_RGB_COLORS);

	//��long���ͽ��бȽ� һ�ο��ԱȽ�һ������ ������24λ ������3���ֽ�
	COLORREF * a, *b;
	int Y = 0;  //Y�ᣬÿһ�ε�Y��Ӧ�ö���һ����

	char *  tmp1, *tmp2;
	tmp1 = (char *)lpbi1 + 40;			//���ļ�ͷ��ʼ
	tmp2 = (char *)lpbi2 + 40;

	//��ΪͼƬ�ڴ�buf�洢�ֽ�������ģ���
	a = (COLORREF *)tmp1;
	b = (COLORREF *)tmp2;


	DWORD i = 0, j = 0, z = 0;
	dwBmBitsSize1 = dwBmBitsSize1 / 4;
	while (i < dwBmBitsSize1)
	{
		if (*a == *b)
		{
			start_x = i % 1005;
			start_y = 568 - i / 1005;
			b++;
			j++;
			if (j > Bitmap.bmHeight)
			{
				Y++;
				j = 0;
				if (Y > 1)
				{
					momvoerClock();
					return true;
				}				
			}		
			//if (j > 3)
			//{
			//	momvoerClock();
			//	return true;
			//}

		}
		else
		{
			if (j < 3)
			{
				j = 0;
				//if(Y == 0 ) b = (COLORREF *)tmp2;
			}
			//else
			//{
			//	end_x = i % 1005;
			//	end_y = 568 - i / 1005;
			//	j = 0;
			//}
		}
		a++;
		i++;
	}


	//���                    
	GlobalUnlock(hDib1);
	GlobalFree(hDib1);
	GlobalUnlock(hDib2);
	GlobalFree(hDib2);
	return  false;

}


//��RGBת��Ϊ32λBMPͼƬ
BOOL SaveBitmapToFile32(HBITMAP   hBitmap, LPCWSTR szfilename)
{
	HDC     hDC;
	//��ǰ�ֱ�����ÿ������ռ�ֽ���            
	int     iBits;
	//λͼ��ÿ������ռ�ֽ���            
	WORD     wBitCount;
	//�����ɫ���С��     λͼ�������ֽڴ�С     ��λͼ�ļ���С     ��     д���ļ��ֽ���                
	DWORD     dwPaletteSize = 0, dwBmBitsSize = 0, dwDIBSize = 0, dwWritten = 0;
	//λͼ���Խṹ                
	BITMAP     Bitmap;
	//λͼ�ļ�ͷ�ṹ            
	BITMAPFILEHEADER     bmfHdr;
	//λͼ��Ϣͷ�ṹ                
	BITMAPINFOHEADER     bi;
	//ָ��λͼ��Ϣͷ�ṹ                    
	LPBITMAPINFOHEADER     lpbi;
	//�����ļ��������ڴ�������ɫ����                
	HANDLE     fh, hDib, hPal, hOldPal = NULL;

	HBITMAP * P_hBitmap = &hBitmap;

	//����λͼ�ļ�ÿ��������ռ�ֽ���  
	//���㵱ǰϵͳ����ɫλ��1 4 8 24
	hDC = CreateDC(L"DISPLAY", NULL, NULL, NULL);
	iBits = GetDeviceCaps(hDC, BITSPIXEL)*GetDeviceCaps(hDC, PLANES);
	DeleteDC(hDC);
	wBitCount = 32;

	GetObject(hBitmap, sizeof(Bitmap), (LPSTR)&Bitmap);
	bi.biSize = sizeof(BITMAPINFOHEADER);
	bi.biWidth = Bitmap.bmWidth;
	bi.biHeight = Bitmap.bmHeight;
	bi.biPlanes = 1;
	bi.biBitCount = wBitCount;
	bi.biCompression = BI_RGB;
	bi.biSizeImage = Bitmap.bmWidth * 4 * Bitmap.bmHeight;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrImportant = 0;
	bi.biClrUsed = 0;

	dwBmBitsSize = Bitmap.bmWidth * 4 * Bitmap.bmHeight;

	//Ϊλͼ���ݷ����ڴ�                
	hDib = GlobalAlloc(GHND, dwBmBitsSize + dwPaletteSize + sizeof(BITMAPINFOHEADER));
	lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib);
	*lpbi = bi;

	//     �����ɫ��                    
	//hPal = GetStockObject(DEFAULT_PALETTE);
	//if (hPal)
	//{
	//	hDC = ::GetDC(NULL);
	//	hOldPal = ::SelectPalette(hDC, (HPALETTE)hPal, FALSE);
	//	RealizePalette(hDC);
	//}

	////     ��ȡ�õ�ɫ�����µ�����ֵ                
	//GetDIBits(hDC, hBitmap, 0, (UINT)Bitmap.bmHeight,
	//	(LPSTR)lpbi + sizeof(BITMAPINFOHEADER) + dwPaletteSize,
	//	(BITMAPINFO *)lpbi, DIB_RGB_COLORS);

	////�ָ���ɫ��                    
	//if (hOldPal)
	//{
	//	::SelectPalette(hDC, (HPALETTE)hOldPal, TRUE);
	//	RealizePalette(hDC);
	//	::ReleaseDC(NULL, hDC);
	//}

	GetDIBits(hdcMem, hBitmap, 0, (UINT)Bitmap.bmHeight,		//ɨ��߶�
		(LPSTR)lpbi + sizeof(BITMAPINFOHEADER),				//���ݻ���ָ��
		(BITMAPINFO *)lpbi, DIB_RGB_COLORS);				//��������
	//����λͼ�ļ�                    
	fh = CreateFile(szfilename, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);

	if (fh == INVALID_HANDLE_VALUE)         return     FALSE;

	//     ����λͼ�ļ�ͷ                
	bmfHdr.bfType = 0x4D42;     //     "BM"           �����ļ�����     
	dwDIBSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwPaletteSize + dwBmBitsSize;
	bmfHdr.bfSize = dwDIBSize;
	bmfHdr.bfReserved1 = 0;
	bmfHdr.bfReserved2 = 0;
	bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER) + dwPaletteSize;
	//     д��λͼ�ļ�ͷ                
	WriteFile(fh, (LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);
	//     д��λͼ�ļ���������                
	WriteFile(fh, (LPSTR)lpbi, dwBmBitsSize + dwPaletteSize + sizeof(BITMAPINFOHEADER), &dwWritten, NULL);
	//���                    
	GlobalUnlock(hDib);
	GlobalFree(hDib);
	CloseHandle(fh);

	return     TRUE;

}



//��ͼ����
bool Dodge_Handling(HWND hWnd, long nx, long ny, long nWidth, long nHeight, HBITMAP pRGBBuf)
{
	bool is_ok;
	HBITMAP hBitmap = CreateCompatibleBitmap(hdcSrc, nWidth, nHeight);			//������DC���ݵļ���λͼ
	HGDIOBJ hOldbmp = SelectObject(hdcMem, hBitmap);							//ѡ�����DC�У����滻ԭ�ж���
///	BitBlt(hdcMem, 0, 0, nWidth, nHeight, hdcSrc, nx, ny, SRCCOPY);				//��ָ����Դ�豸���������е����ؽ���λ�飨bit_block��ת�����Դ��͵�Ŀ���豸������
	if (BitBlt(hdcMem, 0, 0, lpRect.right - lpRect.left, lpRect.bottom - lpRect.top, hdcSrc, 0, 0, SRCCOPY) == false)				//��ָ����Դ�豸���������е����ؽ���λ�飨bit_block��ת�����Դ��͵�Ŀ���豸������
	{
		bit_err++;
	}

	is_ok = BitmapToBitmap32(hBitmap, pRGBBuf);

	DeleteObject(hBitmap);

	return is_ok;
}

//��ͼ����
bool Dodge_Handling(HBITMAP pRGBBuf)
{
	bool is_ok;
	HBITMAP hBitmap = CreateCompatibleBitmap(hdcSrc, lpRect.right - lpRect.left, lpRect.bottom - lpRect.top);			//������DC���ݵļ���λͼ
	HGDIOBJ hOldbmp = SelectObject(hdcMem, hBitmap);							//ѡ�����DC�У����滻ԭ�ж���
	//BitBlt(hdcMem, 0, 0, lpRect.right - lpRect.left, lpRect.bottom - lpRect.top, hdcSrc, 0, 0, SRCCOPY);				//��ָ����Դ�豸���������е����ؽ���λ�飨bit_block��ת�����Դ��͵�Ŀ���豸������
	if (BitBlt(hdcMem, 0, 0, lpRect.right - lpRect.left, lpRect.bottom - lpRect.top, hdcSrc, 0, 0, SRCCOPY) == false)				//��ָ����Դ�豸���������е����ؽ���λ�飨bit_block��ת�����Դ��͵�Ŀ���豸������
	{
		bit_err++;
	}

	is_ok = BitmapToBitmap32(hBitmap, pRGBBuf);

	DeleteObject(hBitmap);

	return is_ok;
}

//��������ͼƬ
void Save_Bmp()
{
	HBITMAP hBitmap = CreateCompatibleBitmap(hdcSrc, lpRect.right - lpRect.left, lpRect.bottom - lpRect.top);			//������DC���ݵļ���λͼ

	HGDIOBJ hOldbmp = SelectObject(hdcMem, hBitmap);							//ѡ�����DC�У����滻ԭ�ж���																				//����ͼ��DC��
	//BitBlt(hdcMem, 0, 0, lpRect.right - lpRect.left, lpRect.bottom - lpRect.top, hdcSrc, 0, 0, SRCCOPY);				//��ָ����Դ�豸���������е����ؽ���λ�飨bit_block��ת�����Դ��͵�Ŀ���豸������
	if (BitBlt(hdcMem, 0, 0, lpRect.right - lpRect.left, lpRect.bottom - lpRect.top, hdcSrc, 0, 0, SRCCOPY) == false)				//��ָ����Դ�豸���������е����ؽ���λ�飨bit_block��ת�����Դ��͵�Ŀ���豸������
	{
		bit_err++;
	}

	SaveBitmapToFile32(hBitmap, L"image//tmp.bmp");

	DeleteObject(hBitmap);
}

//���沿������ͼƬ
void Save_Bmp(long nx, long ny, long nWidth, long nHeight, LPCWSTR szfilename)
{
	HBITMAP hBitmap = CreateCompatibleBitmap(hdcSrc, nWidth, nHeight);			//������DC���ݵļ���λͼ
	HGDIOBJ hOldbmp = SelectObject(hdcMem, hBitmap);							//ѡ�����DC�У����滻ԭ�ж���,��HGDIOBJ��hdcMem DC��
	//BitBlt(hdcMem, 0, 0, nWidth, nHeight, hdcSrc, nx, ny, SRCCOPY);				//��ָ����Դ�豸���������е����ؽ���λ�飨bit_block��ת�����Դ��͵�Ŀ���豸������
	if (BitBlt(hdcMem, 0, 0, lpRect.right - lpRect.left, lpRect.bottom - lpRect.top, hdcSrc, 0, 0, SRCCOPY) == false)				//��ָ����Դ�豸���������е����ؽ���λ�飨bit_block��ת�����Դ��͵�Ŀ���豸������
	{
		bit_err++;
	}

//	HBITMAP hBitmap = (HBITMAP)LoadImage(NULL, L"image//tmp.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	SaveBitmapToFile32(hBitmap, szfilename);
	DeleteObject(hBitmap);
}


////Ѱ�ҹ��� ����ɱ
//void ShaGuai(char * imgtemp)
//{
//	cv::Point Addr;
//
//	//ֻѰ��3��
//	//for (int i = 0; i < 3; i++)
//	//{
//		Save_Bmp();		//����ԴͼƬ ��Ϊ����
//		Addr = ShiBie("image//tmp.bmp", imgtemp, 0);
//
//		//�ж��Ƿ�Ϸ�
//		if (Addr.y > 150 )
//		{
//			momvoerClock(Addr.x, Addr.y);	//����
//		}
//		
//		Sleep(200);
////	}
//}



//DWORD WINAPI FindThreadProc()
//{
//	HBITMAP ZiTu = NULL;
//	int Status = Challenge;
//
//	int xiaoguai_num = 0;
//	int xiaoguai_time = 0;
//
//	while (true)
//	{
//		switch (Status)
//		{
//		case Find_Level:
//			ZiTu = (HBITMAP)LoadImage(NULL, L"image//Find_Level.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
//			break;
//		case Challenge:
//			ZiTu = (HBITMAP)LoadImage(NULL, L"image//tansuo.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
//			xiaoguai_num = 0;	//ÿ��̽��һ����6ֻС��
//			xiaoguai_time = 0;
//			break;
//		case Mobs:
//			//Ѱ��С�� ������С��
//			ShaGuai("image//Mobs.bmp");
//			//�鿴�Ƿ����
//			ZiTu = (HBITMAP)LoadImage(NULL, L"image//Judgment_ends.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
//			xiaoguai_time++;
//			//��60s ��û���ҵ�һֻ�� ǿ��λ��һ�ξ���
//			if (xiaoguai_time > 20)
//			{
//				momvoerClock(900, 450);
//			}
//			break;
//		case Boss:
//			ZiTu = (HBITMAP)LoadImage(NULL, L"image//Boss.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
//			break;
//		default:
//			break;
//		}
//
//		if (Dodge_Handling(GameMianHend, 0, 0, lpRect.right - lpRect.left, lpRect.bottom - lpRect.top, ZiTu))
//		{
//			if (Status == Mobs)
//			{
//				xiaoguai_num++;
//				xiaoguai_time = 0;
//				if (xiaoguai_num == 5)
//				{
//					Status = Boss;
//				}
//			}
//			else
//			{
//				Status++;
//			}
//			
//		}
//		Sleep(3000);
//	}
//
//}

////��ɱһ�ι���
//Rea �Ƿ���Ҫ׼��
int JiSha(bool Rea ,int TIME)
{
	//cv::Point Addr;
	HBITMAP ZiTu = (HBITMAP)LoadImage(NULL, L"image//Judgment_ends.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		for (int i = 0; i < TIME; i++)
		{
			//Save_Bmp();		//����ԴͼƬ ��Ϊ����
			//Addr = ShiBie("image//tmp.bmp", "image//Judgment_ends.bmp", 0);
	
			////�ж��Ƿ�Ϸ�
			//if (Addr.x >250 && Addr.y > 250)
			//{
			//	momvoerClock(Addr.x, Addr.y);	//����
			//}
			
			if (Dodge_Handling(GameMianHend, 0, 0, lpRect.right - lpRect.left, lpRect.bottom - lpRect.top, ZiTu))
			{
				//�����ظ�������� ��ֹʧ�� ���������ҡ�ĵ�ʱ������Ч��
				Sleep(500);
				momvoerClock();
				Sleep(100);
				momvoerClock();
				return 1;
			}
			Sleep(1000);
		}
		return 0;
}


////��ȡֽ�˽���
int Paper_Man_Set()
{
	cv::Point Addr;
	int obj = 0;

Start:	

	Addr = ShiBie3(hIMG_Buff, "image//Paper_Man.bmp", 0);

	//�ж��Ƿ�Ϸ� ֽ�˴��·�Χ
	if (Addr.y >280 && Addr.y < 450)
	{
		momvoerClock(Addr.x, Addr.y);
		Sleep(300);
		momvoerClock(500, 450);		//��Ҫ�������ε��������� ��2�β��ܵ������..... ��Ȼ���� �ӵ�
		Sleep(800);

		SelectObject(hdcMem, hIMG_Buff);							//ѡ�����DC�У����滻ԭ�ж���																				//����ͼ��DC��
		//BitBlt(hdcMem, 0, 0, lpRect.right - lpRect.left, lpRect.bottom - lpRect.top, hdcSrc, 0, 0, SRCCOPY);				//��ָ����Դ�豸���������е����ؽ���λ�飨bit_block��ת�����Դ��͵�Ŀ���豸������
		if (BitBlt(hdcMem, 0, 0, lpRect.right - lpRect.left, lpRect.bottom - lpRect.top, hdcSrc, 0, 0, SRCCOPY) == false)				//��ָ����Դ�豸���������е����ؽ���λ�飨bit_block��ת�����Դ��͵�Ŀ���豸������
		{
			bit_err++;
		}

		obj = 1;
		goto Start;		//��֪���������� ��������ѭ�� ʧ���˳�
	}
	else
	{
		return obj;
	}
}

//��Ϸ״̬
bool Game_Start_State = true;

//�ж��Ƿ���Ϸ����
void Game_Over()
{
	//�鿴���ѱ�־ ֻ���ڵ�ͼ�ϲŻ���
	cv::Point Addr;


	//Sleep(4000);	//��ʱ4S �ȴ�ҳ����ת
	SelectObject(hdcMem, hIMG_Buff);							//ѡ�����DC�У����滻ԭ�ж���																				//����ͼ��DC��

	//BitBlt(hdcMem, 0, 0, lpRect.right - lpRect.left, lpRect.bottom - lpRect.top, hdcSrc, 0, 0, SRCCOPY);				//��ָ����Դ�豸���������е����ؽ���λ�飨bit_block��ת�����Դ��͵�Ŀ���豸������
	if (BitBlt(hdcMem, 0, 0, lpRect.right - lpRect.left, lpRect.bottom - lpRect.top, hdcSrc, 0, 0, SRCCOPY) == false)				//��ָ����Դ�豸���������е����ؽ���λ�飨bit_block��ת�����Դ��͵�Ŀ���豸������
	{
		bit_err++;
	}

	Addr = ShiBie(hIMG_Buff, "image//juexing.bmp", 0);
	//ֻ�����λ�ò��Ǿ���ͼ���
	if (Addr.x > 40  && Addr.x < 65 && Addr.y > 490 && Addr.y < 520)		//55 505
	{
		Game_Start_State = false;
	}
}


DWORD WINAPI FindThreadProc()
{
	char * ZiTu = NULL;
	int Status = Find_Level;
	cv::Point Addr;

	int xiaoguai_num = 0;


	while (true)
	{
//		Save_Bmp();		//����ԴͼƬ ��Ϊ����

		SelectObject(hdcMem, hIMG_Buff);							//ѡ�����DC�У����滻ԭ�ж���				//����ͼ��DC��

		if (BitBlt(hdcMem, 0, 0, lpRect.right - lpRect.left, lpRect.bottom - lpRect.top, hdcSrc, 0, 0, SRCCOPY) == false)				//��ָ����Դ�豸���������е����ؽ���λ�飨bit_block��ת�����Դ��͵�Ŀ���豸������
		{
			bit_err++;
		}
		switch (Status)
		{
		case Find_Level:
			//ZiTu = "image//Find_Level.bmp";
			//Addr = ShiBie("image//tmp.bmp", ZiTu, 3);
			//if ((Addr.x == 0 && Addr.y == 0) || Addr.x < 850) //�ж��Ƿ�Ϸ�
			//{
			//	Status = Challenge;
			//	break;
			//}
			//momvoerClock(Addr.x, Addr.y);
			momvoerClock(500, 250);	//�����Ļ����
			Sleep(1000);
			momvoerClock(500, 250);	//�����Ļ����
			Status = Challenge;
			Game_Start_State = true;
			Sleep(1000);
			break;
		case Challenge:
			ZiTu = "image//tansuo.bmp";
			Addr = ShiBie(hIMG_Buff, ZiTu, 0);
			if (Addr.x > 700 && Addr.x < 750 && Addr.y > 400 && Addr.y < 420)	//703 412
			{
				momvoerClock(Addr.x, Addr.y);
				Status = Mobs;
				xiaoguai_num = 0;
			}
			Sleep(7000);		//���⼸��BB������Ѱ��
			break;
		case Mobs:		
			if (Game_Start_State == false)
			{
				Status = Find_Level;
				break;
			}

			ZiTu = "image//Mobs.bmp";
			Addr = ShiBie(hIMG_Buff, ZiTu, 5);
			if ((Addr.x == 0 && Addr.y == 0) || Addr.y < 150 || Addr.y > 480)		//�ж��Ƿ�Ϸ�
			{
				//if (xiaoguai_num > 2)		//С�ֱ�ɱ��
				if(Paper_Man_Set())
				{
					//Status = Boss;
					//Paper_Man_Set();
					//Status = Find_Level;
					//break;
				}
				else						//������С��
				{
					if (xiaoguai_num < 3)
					{
						momvoerClock(900, 450);	//���ƶ�һ�ξ���
					}
					else 
					{
						momvoerClock(100, 450);	//���ƶ�һ�ξ���
					}					
					xiaoguai_num = (xiaoguai_num > 6)?0:xiaoguai_num++;
				}
			}
			else
			{
				momvoerClock(Addr.x, Addr.y);								

				//�ȴ���ɱ���
				JiSha(1, 100);
			}
			Sleep(9000);
			Game_Over();		//�ж��Ƿ����
			break;
		default:
			break;
		}				
	}

}

void Game_Status();
DWORD WINAPI ShuaTu_Guanka();

void Game_Start()
{
	static int Is_Start = 1;
	if (Is_Start)
	{
		//CreateThread(0, 0, (LPTHREAD_START_ROUTINE)FindThreadProc, nullptr, 0, 0);	//������Ϸ����߳�
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)ShuaTu_Guanka, nullptr, 0, 0);	//������Ϸ����߳�
		Is_Start = 0;
	}
	else
	{
		MessageBoxW(NULL, L"��Ϸ�ѿ�ʼ", NULL, MB_OK);
	}
	
}

//��ʼ����������Դ
int Game_Init()
{
	HWND Main_From = FindWindow(L"Qt5QWindowIcon", nullptr);
	HWND Ci_From = FindWindowEx(Main_From, nullptr, L"Qt5QWindowIcon", L"m_pMainWidgetWindow");
	HWND CiCi_From = FindWindowEx(Ci_From, nullptr, L"Qt5QWindowIcon", L"m_pViewWidgetWindow");
	HWND CiCiCi_From = FindWindowEx(CiCi_From, nullptr, L"BlueStacksApp", L"_ctl.Window");

	if (CiCiCi_From == NULL)
	{
		MessageBoxW(NULL, L" ����ʦδ���У�", NULL, MB_OK);
		return false;
	}

	//��ȡ��Ϸ���ڴ�С
	GetWindowRect(CiCiCi_From, &lpRect);

	GameMianHend = CiCiCi_From;		//������

	GameMoNiHend = CiCi_From;		//Windows�ⲿ�ӿ�

	hdcSrc = GetDC(GameMianHend);													//��ȡ����������Ļ���������DC
	hdcMem = CreateCompatibleDC(hdcSrc);										//��������DC

	hIMG_Buff = CreateCompatibleBitmap(hdcSrc, lpRect.right - lpRect.left, lpRect.bottom - lpRect.top);			//������DC���ݵļ���λͼ


	return  true;
}

//��Ϸ�˳����ͷ���Դ
void Game_Exit()
{
	DeleteObject(hdcMem);

	ReleaseDC(GameMianHend, hdcSrc);  //����Ҫ�ͷž�� ��Ȼ�ᵼ���´ε���ʧ��
	DeleteObject(hdcSrc);
	DeleteObject(hIMG_Buff);
}