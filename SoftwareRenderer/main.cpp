

#include <cmath>
#include<iostream>
#include <tchar.h>
#include <vector>
#include <initializer_list>
#include "platform/Windows.h"
#include "tools/Math.h"
#include "rasterizer/Ray.h"


#define TRACY_ENABLE

bool g_window_should_close = false;
int g_initialized = 0;
const char* const WINDOW_CLASS_NAME = "Class";
const char* const WINDOW_ENTRY_NAME = "Entry";

static const int WIDTH = 400;
static const int HEIGHT = 225;
static const double AspectRatio = 16.0 / 9.0;

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

// bitmap pixels use BGRA

void setPixel(byte* source,int index,std::vector<unsigned char> &color )
{
	source[index] = color[2];                    // b
	source[index + 1] = color[1];				 // g
	source[index + 2] = color[0];				 // r
	source[index + 3] = color[3];				 // a
	
}



double hitSphere(const Point3& center,double radius,const Ray& r)
{
	Vec3 OC = r.origin() - center;
	auto a = dot(r.direction() ,r.direction());

	// use half b to simplify
	auto half_b = dot(OC,r.direction());
	auto c = dot(OC, OC) - radius * radius;
	auto delta = half_b * half_b - a * c;
	if(delta < 0 ){
	 	return -1.0;
	}
	// sphere is in front of camera, so just get the 
	else{
		return (- half_b - sqrt(delta))/( a);
	}
}

Color rayColor(const Ray& r)
{
	auto t = hitSphere(Point3(0,0,-1),0.5,r);
	if(t>0.0){
		Vec3 N = unit_vector(r.at(t)-Vec3(0,0,-1));
		return 0.5 * Color(N.x()+1,N.y()+1,N.z()+1);
	}
	Vec3 unitDirection = unit_vector(r.direction());
	t = 0.5 * (unitDirection.y() + 1.0);
	return (1.0-t)*Color(1.0,1.0,1.0) + t*Color(0.5,0.7,1.0);
}

int main(int argc, char* argv[]){
	// window & image
	initializeWindow();	
	HWND handle = createWindow("SoftwareRenderer", WIDTH, HEIGHT);
	std::vector<byte> image = std::vector<byte>(WIDTH * HEIGHT * 4, 0);
	byte *image_p = image.data();
    byte **p = &image_p;
	HDC memoryDC;
	createCanvas(handle, WIDTH, HEIGHT, p, memoryDC);
	SetProp(handle, WINDOW_ENTRY_NAME, handle);
	ShowWindow(handle, SW_SHOW);
	
	// camera
	double viewPortHeight = 2.0;
	double viewPortWidth = viewPortHeight * AspectRatio;
	auto focalLength = 1.0;
	auto origin = Point3(0,0,0);
	auto horizontal = Vec3(viewPortWidth,0,0);
	auto vertical = Vec3(0,viewPortHeight,0);
	auto lowerLeftCorner = origin - horizontal/2 -vertical/2-Vec3(0,0,focalLength) ;

	while (!g_window_should_close)
	{
		// update buffer
		// scan: left to right, down to up (lowerLeftCorner == (0,0))
		// bmp: left to right, up to down  (upperLeftCorner == (0,0))
        for (int j = 0; j <HEIGHT ; j++) {
			for (int i = 0; i < WIDTH; i++) {
				auto u = double(i) /(WIDTH-1);
				auto v = double(j) /(HEIGHT-1);
				Ray r(origin,lowerLeftCorner + u*horizontal +v*vertical-origin);
				Color pixelColor = rayColor(r);
				auto rgbaColor = toRGBAColor(pixelColor);
				// reverse Y
				setPixel(image_p, ((HEIGHT-j-1)*WIDTH + i)*4, rgbaColor);
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
	}

	RemoveProp(handle, WINDOW_ENTRY_NAME);
	DeleteDC(memoryDC);
	DestroyWindow(handle);
	return 0;
}

