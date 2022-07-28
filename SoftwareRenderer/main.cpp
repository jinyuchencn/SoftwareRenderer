

#include<iostream>
#include <tchar.h>
#include <vector>
#include <initializer_list>

#include "platform/Windows.h"
#include "tools/Math.h"



bool g_window_should_close = false;
int g_initialized = 0;
const char* const WINDOW_CLASS_NAME = "Class";
const char* const WINDOW_ENTRY_NAME = "Entry";

static const int WIDTH = 200;
static const int HEIGHT = 21;



static std::vector<byte> RED = {255,0,0,255};
static std::vector<byte> ORANGE = {255,165,0,255}; 
static std::vector<byte> YELLOE = {255,255,0,255};
static std::vector<byte> GREEN = {0,255,0,255};
static std::vector<byte> BLUEGREEN = {0,127,255,255};
static std::vector<byte> BLUE = {0,0,255,255};
static std::vector<byte> PURPLE = {139,0,255,255};

template<typename T>
void myPrint(const T &t){
	std::cout<<t<<std::endl;
}

template<typename T,typename...Args>
void myPrint(const T &t, const Args&...rest)
{
	std::cout<<t<<" ";
	myPrint(rest...);
}

void setPixel(byte* source,int index,std::vector<byte> &color )
{
	source[index] = color[2];                    // b
	source[index + 1] = color[1];				 // g
	source[index + 2] = color[0];				 // r
	source[index + 3] = color[3];				 // a
}

int main(int argc, char* argv[]){
	initializeWindow();
	
	HWND handle = createWindow("SoftwareRenderer", WIDTH, HEIGHT);

	std::vector<byte> image = std::vector<byte>(WIDTH*HEIGHT*4,0);


	byte *image_p = image.data();
    byte **p = &image_p;



	HDC memoryDC;
	createCanvas(handle, WIDTH, HEIGHT, p, memoryDC);
	SetProp(handle, WINDOW_ENTRY_NAME, handle);
	ShowWindow(handle, SW_SHOW);



    int changeColor = 0;

	int index1 = HEIGHT/7; 
	int index2 = HEIGHT/7 * 2;
	int index3 = HEIGHT/7 * 3;
	int index4 = HEIGHT/7 * 4;
	int index5 = HEIGHT/7 * 5;
	int index6 = HEIGHT/7 * 6;

	while (!g_window_should_close)
	{
		// update buffer
        for (int i = 0; i < WIDTH; i++) {
            for (int j = 0; j < HEIGHT; j++) {

				if(j<=index1) setPixel(image_p,(j * WIDTH + i) * 4,RED);
				else if(j<=index2) setPixel(image_p,(j * WIDTH + i) * 4,ORANGE);
				else if(j<=index3) setPixel(image_p,(j * WIDTH + i) * 4,YELLOE);
				else if(j<=index4) setPixel(image_p,(j * WIDTH + i) * 4,GREEN);
				else if(j<=index5) setPixel(image_p,(j * WIDTH + i) * 4,BLUEGREEN);
				else if(j<=index6) setPixel(image_p,(j * WIDTH + i) * 4,BLUE);
				else  setPixel(image_p,(j * WIDTH + i) * 4,PURPLE);

            }
        }
		HDC window_dc = GetDC(handle);
		BitBlt(window_dc, 0, 0, WIDTH, HEIGHT, memoryDC, 0, 0, SRCCOPY);        
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
	myPrint("test",3);
	

	return 0;

}

