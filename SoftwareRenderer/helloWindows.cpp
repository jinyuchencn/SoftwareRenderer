#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#include <array>

const int WIDTH = 800;
const int HEIGHT = 600;


HBITMAP CreateNewHbitmap(short w, short h, byte bpp= 24, byte**pBits= 0, short nColors= 0)
{
    if(bpp < 16 && !nColors)
    {
        if(bpp==8) nColors= 256;
        else if(bpp==4) nColors= 16;
        else if(bpp==1) nColors= 2;
    }
    byte binfoSz= sizeof(BITMAPINFOHEADER);
    BITMAPINFO* pBi=(BITMAPINFO*)new byte[binfoSz+(bpp < 16?(nColors*sizeof(RGBQUAD)):0)];
    memset( & pBi->bmiHeader, 0, binfoSz);
    pBi->bmiHeader.biSize= binfoSz;
    pBi->bmiHeader.biWidth= w;
    pBi->bmiHeader.biHeight= -h;
    pBi->bmiHeader.biPlanes= 1;
    pBi->bmiHeader.biBitCount= bpp;

    HDC hdc= GetDC(0); //only use Pal_colors if a HPALETTE is selected into hdc.
    HBITMAP hBmp= ::CreateDIBSection(hdc, pBi, DIB_RGB_COLORS,
    (void**)pBits, 0, 0);
    ReleaseDC(0, hdc);
    delete[]pBi;
    return hBmp;
} 

int main() {

    byte *image  = new byte[WIDTH*HEIGHT*3];


	for (int i = 0; i < WIDTH; i++) {
		for (int j = 0; j < HEIGHT; j++) {
			image[(j * WIDTH + i) * 3] = 200;    // b
			image[(j * WIDTH + i) * 3 + 1] = 0;  // g
			image[(j * WIDTH + i) * 3 + 2] = 0;  // r
		}
	}

    byte **p = &image;



    
    HBITMAP hBitmap = (HBITMAP)CreateNewHbitmap(WIDTH,HEIGHT,24,p);

    HDC hDC = GetDC(NULL);
    HDC hBitmapDC = CreateCompatibleDC(hDC);
    HBITMAP hOldBitmap = (HBITMAP)SelectObject(hBitmapDC, hBitmap);

    int changeColor = 0;
    while (true) {
        int b = changeColor%256;
        int g = changeColor%256;
        int r = changeColor%256;

        BitBlt(hDC, 0, 0, 487, 456, hBitmapDC, 0, 0, SRCCOPY);
        for (int i = 0; i < WIDTH; i++) {
            for (int j = 0; j < HEIGHT; j++) {
                image[(j * WIDTH + i) * 3] = b;
                image[(j * WIDTH + i) * 3 + 1] = g;
                image[(j * WIDTH + i) * 3 + 2] = r;
            }
        }
        changeColor++;
    }
    /* Don't forget to release resources that you've acquired */
    SelectObject(hBitmapDC, hOldBitmap);
    DeleteObject(hBitmap);
    DeleteDC(hBitmapDC);

    ReleaseDC(HWND_DESKTOP, hDC);

    return 0;
}