#pragma once
#include <windows.h>
#include <assert.h>
#include <tchar.h>
#include <string>

#define UNUSED_VAR(x) ((void)(x))

extern bool g_window_should_close;
extern int g_initialized;
extern const char* const WINDOW_CLASS_NAME;
extern const char* const WINDOW_ENTRY_NAME;

LRESULT CALLBACK process_message(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


void registerClass();

void initializeWindow();

HWND createWindow(const std::string title,int WIDTH,int HEIGHT);

void createCanvas(HWND handle, int width, int height, byte** image, HDC &memoryDC);

LRESULT CALLBACK process_message(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
