// yinyangshi.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <string>
#include <stdio.h> 
#include "yinyangshi.h"

#include "game.h"

#define MAX_LOADSTRING 100

// ȫ�ֱ���: 
HINSTANCE hInst;                                // ��ǰʵ��
WCHAR szTitle[MAX_LOADSTRING];                  // �������ı�
WCHAR szWindowClass[MAX_LOADSTRING];            // ����������

HWND hWnd;		//�����ھ��				

// �˴���ģ���а����ĺ�����ǰ������: 
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: �ڴ˷��ô��롣

    // ��ʼ��ȫ���ַ���
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_YINYANGSHI, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // ִ��Ӧ�ó����ʼ��: 
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_YINYANGSHI));

    MSG msg;

    // ����Ϣѭ��: 
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  ����: MyRegisterClass()
//
//  Ŀ��: ע�ᴰ���ࡣ
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_YINYANGSHI));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_YINYANGSHI);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   ����: InitInstance(HINSTANCE, int)
//
//   Ŀ��: ����ʵ�����������������
//
//   ע��: 
//
//        �ڴ˺����У�������ȫ�ֱ����б���ʵ�������
//        ��������ʾ�����򴰿ڡ�
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����

   hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);


   return TRUE;
}
HWND hwndEDIT;
void SAVE_chuli();

//
//  ����: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  Ŀ��:    ���������ڵ���Ϣ��
//
//  WM_COMMAND  - ����Ӧ�ó���˵�
//  WM_PAINT    - ����������
//  WM_DESTROY  - �����˳���Ϣ������
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static int   cxChar, cyChar;

	//HBITMAP ZiTu1 = (HBITMAP)LoadImage(NULL, L"image//tmp.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	//HBITMAP ZiTu2 = (HBITMAP)LoadImage(NULL, L"image//Mobs.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	cxChar = sizeof(BITMAPINFOHEADER);

	switch (message)
	{
	case WM_CREATE:
		cxChar = LOWORD(GetDialogBaseUnits());
		cyChar = HIWORD(GetDialogBaseUnits());
		hwndEDIT = CreateWindow(TEXT("Edit"),
			TEXT("��ʼ�һ�"),
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | WS_BORDER | ES_MULTILINE,
			cxChar, cyChar * 10,
			30 * cxChar, 7 * cyChar / 4,
			hWnd, NULL,
			((LPCREATESTRUCT)lParam)->hInstance, NULL);

			//��ʼ����Ϸ�ⲿ�ӿ�1
			if (Game_Init() != true)
			{
				SendMessage(hWnd, WM_DESTROY, 0, 0);
			}

		break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // �����˵�ѡ��: 
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
			case ID_32771:
				MessageBoxW(NULL, L"̽����ʼ", NULL, MB_OK);

				Game_Start();
				//Thread_YuHun();			
				//BitmapToBitmap32(ZiTu1, ZiTu2);

				break;
			case ID_32772:
				MessageBoxW(NULL, L"������ѿ�ʼ", NULL, MB_OK);
				//Thread_YuHun();
				break;
			case ID_32773:
				SAVE_chuli();
				break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: �ڴ˴����ʹ�� hdc ���κλ�ͼ����...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
		Game_Exit();
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// �����ڡ������Ϣ�������
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}



void SAVE_chuli()
{
	static int Save_type = 0;
	char str[100];
	char * split = ",";
	char * p;
	char *next_token = nullptr;
	int x, y, nWidth, nHeight;

	if (Save_type == 0)
	{
		Save_Bmp();
		MessageBoxW(NULL, L"����Ҫ���뱣�������ֵ���ٱ���һ��", NULL, MB_OK);
		Save_type = 1;
	}
	else
	{
		GetWindowTextA(hwndEDIT, str, 100);
		if ((p = strtok_s(str, split, &next_token)) != NULL)
		{
			sscanf_s(p, "%d", &x);
		}
		if ((p = strtok_s(NULL, split, &next_token)) != NULL)
		{
			sscanf_s(p, "%d", &y);
		}
		if ((p = strtok_s(NULL, split, &next_token)) != NULL)
		{
			sscanf_s(p, "%d", &nWidth);
		}
		if ((p = strtok_s(NULL, split, &next_token)) != NULL)
		{
			sscanf_s(p, "%d", &nHeight);
		}
		else
		{
			MessageBoxW(NULL, L"��д��С���겻�ԣ�����", NULL, MB_OK);
		}

		Save_Bmp(x, y, nWidth, nHeight, L"image//save2.bmp");
		MessageBoxW(NULL, L"����ɹ�", NULL, MB_OK);

		Save_type = 0;
	}
}