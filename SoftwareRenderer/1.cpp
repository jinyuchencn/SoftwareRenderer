#include "stdafx.h"

//全局变量声明
HINSTANCE hInst;
HBITMAP hbmp;
HDC mdc;

//全局函数的声明
ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void MyPaint(HDC hdc);
//****Winmain函数，程序入口点函数************************************** int
//APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR
//lpCmdLine, int nCmdShow) { MSG msg;
MyRegisterClass(hInstance);
if (!InitInstance(hInstance, nCmdShow)) {
  return FALSE;
} //消息循环 while (GetMessage(&msg, NULL, 0, 0)) { TranslateMessage(&msg);
DispatchMessage(&msg);
}
return msg.wParam;
} //****﹚设计一个窗口类，类似填空题，使用窗口结构体*************************
  //ATOM MyRegisterClass(HINSTANCE hInstance) { WNDCLASSEX wcex;
wcex.cbSize = sizeof(WNDCLASSEX);
wcex.style = CS_HREDRAW | CS_VREDRAW;
wcex.lpfnWndProc = (WNDPROC)WndProc;
wcex.cbClsExtra = 0;
wcex.cbWndExtra = 0;
wcex.hInstance = hInstance;
wcex.hIcon = NULL;
wcex.hCursor = NULL;
wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
wcex.lpszMenuName = NULL;
wcex.lpszClassName = "canvas";
wcex.hIconSm = NULL;
return RegisterClassEx(&wcex);
} //****初始化函数*************************************
  //// 1.建立与窗口DC兼容的内存DC // 2.从文件加载位图并存至内存DC中 BOOL
  //InitInstance(HINSTANCE hInstance, int nCmdShow) { HWND hWnd;
HDC hdc;
hInst = hInstance;
hWnd = CreateWindow("canvas", "绘图窗口", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0,
                    CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);
if (!hWnd) {
  return FALSE;
}
MoveWindow(hWnd, 10, 10, 800, 600, true);
ShowWindow(hWnd, nCmdShow);
UpdateWindow(hWnd);
hdc = GetDC(hWnd);
mdc = CreateCompatibleDC(hdc);
hbmp =
    (HBITMAP)LoadImage(NULL, "bg.bmp", IMAGE_BITMAP, 800, 600, LR_LOADFROMFILE);
SelectObject(mdc, hbmp);
MyPaint(hdc);
ReleaseDC(hWnd, hdc);
return TRUE;
} //****自定义绘图函数********************************* void MyPaint(HDC hdc) {
  //BitBlt(hdc,0,0,800,600,mdc,0,0,SRCCOPY);
//采用BitBlt函数贴图 } //****消息处理函数**********************************
//LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM
//lParam) { PAINTSTRUCT ps;
HDC hdc;
switch (message) {
case WM_PAINT: //窗口重绘消息 hdc = BeginPaint(hWnd, &ps);
  MyPaint(hdc);
  EndPaint(hWnd, &ps);
  break;
case WM_DESTROY: //窗口结束消息 DeleteDC(mdc);
  DeleteObject(hbmp);
  PostQuitMessage(0);
  break;
default: //其他消息 return DefWindowProc(hWnd, message, wParam, lParam);
}
return 0;
}