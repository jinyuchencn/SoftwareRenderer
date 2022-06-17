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

void createCanvas(HWND handle, int width, int height, byte** image, HDC &memoryDC)
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
		DIB_RGB_COLORS, (void**)(image),
		NULL, 0);
	assert(dib_bitmap != NULL);
	old_bitmap = (HBITMAP)SelectObject(memory_dc, dib_bitmap);
	DeleteObject(old_bitmap);

	memoryDC = memory_dc;

}

int main(int argc, char* argv[]){
	initializeWindow();
	HWND handle = createWindow("SoftwareRenderer", WIDTH, HEIGHT);

    byte *image  = new byte[WIDTH*HEIGHT*4];


	for (int i = 0; i < WIDTH; i++) {
		for (int j = 0; j < HEIGHT; j++) {
			image[(j * WIDTH + i) * 4] = 200;    // b
			image[(j * WIDTH + i) * 4 + 1] = 100;  // g
			image[(j * WIDTH + i) * 4 + 2] = 0;  // r
            image[(j * WIDTH + i) * 4 + 3] = 255;  // a
		}
	}

    byte **p = &image;



	HDC memoryDC;
	createCanvas(handle, WIDTH, HEIGHT, p, memoryDC);
	SetProp(handle, WINDOW_ENTRY_NAME, handle);
	ShowWindow(handle, SW_SHOW);



    int changeColor = 0;

	while (true)
	{
        int r = (changeColor/10) %255;
        int g = (changeColor/10 +255/3) %255;
        int b = (changeColor/10+255/3 *2) %255;
		HDC window_dc = GetDC(handle);
		BitBlt(window_dc, 0, 0, WIDTH, HEIGHT, memoryDC, 0, 0, SRCCOPY);

        for (int i = 0; i < WIDTH; i++) {
            for (int j = 0; j < HEIGHT; j++) {
                image[(j * WIDTH + i) * 4] = r;
                image[(j * WIDTH + i) * 4 + 1] = g;
                image[(j * WIDTH + i) * 4 + 2] = b;
                image[(j * WIDTH + i) * 4 + 3] = 255;  // a
            }
        }
        
        ReleaseDC(handle, window_dc);

		UpdateWindow(handle);

		MSG message;
		while (PeekMessage(&message, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&message);
			DispatchMessage(&message);
		}

        changeColor++;
	}

	RemoveProp(handle, WINDOW_ENTRY_NAME);
	DeleteDC(memoryDC);
	DestroyWindow(handle);

	return 0;

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