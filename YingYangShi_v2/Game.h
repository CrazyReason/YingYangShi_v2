#pragma once

#include "stdafx.h"

extern HDC hdcMem, hdcSrc;

extern HBITMAP hIMG_Buff;						//缓冲图像数据对象
extern RECT lpRect;					//游戏界面大小
extern int bit_err;

//游戏状态
enum Game_Status
{
	Ready,					//准备
	Judgment_ends,			//判断结束
	Find_Level,				//关卡
	Challenge,				//挑战
	Mobs,					//小怪
	Boss,					//BOOSS
	Paper_Man				//结束纸人
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