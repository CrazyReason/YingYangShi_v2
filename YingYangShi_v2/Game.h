#pragma once

#include "stdafx.h"

extern HDC hdcMem, hdcSrc;

extern HBITMAP hIMG_Buff;						//����ͼ�����ݶ���
extern RECT lpRect;					//��Ϸ�����С
extern int bit_err;

//��Ϸ״̬
enum Game_Status
{
	Ready,					//׼��
	Judgment_ends,			//�жϽ���
	Find_Level,				//�ؿ�
	Challenge,				//��ս
	Mobs,					//С��
	Boss,					//BOOSS
	Paper_Man				//����ֽ��
};

int Game_Init(void);

void Game_Exit(void);

void Game_Start(void);

void Save_Bmp();

void Save_Bmp(long nx, long ny, long nWidth, long nHeight, LPCWSTR szfilename);


bool Dodge_Handling( HBITMAP pRGBBuf);

void momvoerClock();

void momvoerClock(int x, int y);

int Paper_Man_Set();
int JiSha(bool Rea, int TIME);

BOOL BitmapToBitmap32(HBITMAP   hBitmap1, HBITMAP hBitmap2);