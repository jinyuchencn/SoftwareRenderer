#include<iostream>
#include <tchar.h>
#include <windows.h>
#include <assert.h>
#include <vector>

#define UNUSED_VAR(x) ((void)(x))

static int g_initialized = 0;

static const int WIDTH = 800;
static const int HEIGHT = 600;

static const char* const WINDOW_CLASS_NAME = "Class";
static const char* const WINDOW_ENTRY_NAME = "Entry";

LRESULT CALLBACK process_message(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

void registerClass() {
	ATOM class_atom;
	WNDCLASS window_class;
	window_class.style = CS_HREDRAW | CS_VREDRAW;
	window_class.lpfnWndProc = process_message;
	window_class.cbClsExtra = 0;
	window_class.cbWndExtra = 0;
	window_class.hInstance = GetModuleHandle(NULL);
	window_class.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	window_class.hCursor = LoadCursor(NULL, IDC_ARROW);
	window_class.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	window_class.lpszMenuName = NULL;
	window_class.lpszClassName = WINDOW_CLASS_NAME;
	class_atom = RegisterClass(&window_class);
	assert(class_atom != 0);
	UNUSED_VAR(class_atom);
}

void initializeWindow() {
	assert(g_initialized == 0);
	registerClass();
	g_initialized = 1;

}

HWND createWindow(const std::string title,int WIDTH,int HEIGHT) {
	DWORD style = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
	RECT rect;
	HWND handle;

	rect.left = 0;
	rect.top = 0;
	rect.right = WIDTH;
	rect.bottom = HEIGHT;

	AdjustWindowRect(&rect, style, 0);

	int newWidth = rect.right - rect.left;
	int newHeight = rect.bottom - rect.top;

	handle = CreateWindow(WINDOW_CLASS_NAME, title.c_str(), style,
		CW_USEDEFAULT, CW_USEDEFAULT, newWidth, newHeight,
		NULL, NULL, GetModuleHandle(NULL), NULL);
	assert(handle != NULL);
	return handle;
}

void createCanvas(HWND handle, int width, int height, std::vector<unsigned char> &image, HDC &memoryDC)
{
	BITMAPINFOHEADER bi_header;
	HBITMAP dib_bitmap;
	HBITMAP old_bitmap;
	HDC window_dc;
	HDC memory_dc;

	//image_t* surface;
	//surface = image_create(width, height, 4, FORMAT_LDR);
	//free(surface->ldr_buffer);
	//surface->ldr_buffer = NULL;

	unsigned char* data = image.data();

	window_dc = GetDC(handle);
	memory_dc = CreateCompatibleDC(window_dc);
	ReleaseDC(handle, window_dc);

	memset(&bi_header, 0, sizeof(BITMAPINFOHEADER));
	bi_header.biSize = sizeof(BITMAPINFOHEADER);
	bi_header.biWidth = width;
	bi_header.biHeight = -height;  /* top-down */
	bi_header.biPlanes = 1;
	bi_header.biBitCount = 32;
	bi_header.biCompression = BI_RGB;
	dib_bitmap = CreateDIBSection(memory_dc, (BITMAPINFO*)&bi_header,
		DIB_RGB_COLORS, (void**)(&data),
		NULL, 0);
	assert(dib_bitmap != NULL);
	old_bitmap = (HBITMAP)SelectObject(memory_dc, dib_bitmap);
	DeleteObject(old_bitmap);

	memoryDC = memory_dc;

}

int main(int argc, char* argv[]){
	initializeWindow();
	HWND handle = createWindow("SoftwareRenderer", WIDTH, HEIGHT);

	std::vector<unsigned char> image(WIDTH/2 * HEIGHT/2 * 4);
	for (int i = 0; i < WIDTH/2; i++) {
		for (int j = 0; j < HEIGHT/2; j++) {
			image[(j * WIDTH/2 + i) * 4] = 200;
			image[(j * WIDTH/2 + i) * 4 + 1] = 0;
			image[(j * WIDTH/2 + i) * 4 + 2] = 0;
			image[(j * WIDTH/2 + i) * 4 + 3] = 0;
		}
	}



	HDC memoryDC;
	createCanvas(handle, WIDTH/2, HEIGHT/2, image, memoryDC);
	SetProp(handle, WINDOW_ENTRY_NAME, handle);
	ShowWindow(handle, SW_SHOW);





	while (true)
	{
		HDC window_dc = GetDC(handle);
		BitBlt(window_dc, 0, 0, WIDTH/2, HEIGHT/2, memoryDC, 0, 0, SRCCOPY);
		ReleaseDC(handle, window_dc);

		UpdateWindow(handle);

		MSG message;
		while (PeekMessage(&message, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
	}

	RemoveProp(handle, WINDOW_ENTRY_NAME);
	DeleteDC(memoryDC);
	DestroyWindow(handle);

	return 0;

}

void createHDCfromImage(HDC &out_memory_dc) {
	BITMAPINFOHEADER bi_header;
	HBITMAP dib_bitmap;
	HBITMAP old_bitmap;

	memset(&bi_header, 0, sizeof(BITMAPINFOHEADER));
	bi_header.biSize = sizeof(BITMAPINFOHEADER);
	bi_header.biWidth = 50;
	bi_header.biHeight = -50;  /* top-down */
	bi_header.biPlanes = 1;
	bi_header.biBitCount = 32;
	bi_header.biCompression = BI_RGB;

	unsigned char image[50*50*4];

	for (int i = 0; i < 50 * 50; i++) {
		image[4 * i] = (unsigned char)30;
		image[4 * i + 1] = (unsigned char)50;
		image[4 * i + 2] = (unsigned char)70;
		image[4 * i + 3] = (unsigned char)200;
	}


	dib_bitmap = CreateDIBSection(out_memory_dc, (BITMAPINFO*)&bi_header,
		DIB_RGB_COLORS, (void**)&image,
		NULL, 0);

	assert(dib_bitmap != NULL);
	old_bitmap = (HBITMAP)SelectObject(out_memory_dc, dib_bitmap);
	DeleteObject(old_bitmap);

}

LRESULT CALLBACK process_message(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	TCHAR greeting[] = _T("Hello, Windows desktop!");

	switch (message)
	{
	case WM_PAINT: {
		hdc = BeginPaint(hWnd, &ps);

		//HDC hdcMem = CreateCompatibleDC(hdc);
		////createHDCfromImage(hdcMem);

		//BITMAPINFOHEADER bi_header;
		//HBITMAP dib_bitmap;
		//HBITMAP old_bitmap;

		//memset(&bi_header, 0, sizeof(BITMAPINFOHEADER));
		//bi_header.biSize = sizeof(BITMAPINFOHEADER);
		//bi_header.biWidth = 50;
		//bi_header.biHeight = -50;  /* top-down */
		//bi_header.biPlanes = 1;
		//bi_header.biBitCount = 32;
		//bi_header.biCompression = BI_RGB;

		//unsigned char image[50 * 50 * 4];

		//for (int i = 0; i < 50 * 50; i++) {
		//	image[4 * i] = (unsigned char)30;
		//	image[4 * i + 1] = (unsigned char)50;
		//	image[4 * i + 2] = (unsigned char)70;
		//	image[4 * i + 3] = (unsigned char)200;
		//}

		//dib_bitmap = CreateDIBSection(hdcMem, (BITMAPINFO*)&bi_header,
		//	DIB_RGB_COLORS, (void**)&image,
		//	NULL, 0);

		//assert(dib_bitmap != NULL);
		//old_bitmap = (HBITMAP)SelectObject(hdcMem, dib_bitmap);
		//DeleteObject(old_bitmap);


		//BitBlt(hdc, 0, 0, 50, 50, hdcMem, 0, 0, SRCCOPY);
		//DeleteDC(hdcMem);

		////TextOut(hdc,
		////	5, 5,
		////	greeting, _tcslen(greeting));



		EndPaint(hWnd, &ps);
		break;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}

	return 0;
}