//g++ lab2.cpp -o lab2 -lgdi32

#include "pch.h"
#include <iostream>
#include "windows.h"
#include <tchar.h>
#include <string>
#include <cstdio>
#include <math.h>


BOOL RegClass(WNDPROC, LPCTSTR, UINT);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

HINSTANCE hInstance;

char szMainClass[] = "MainClass";
char szMainTitle[] = "Cross-Zero";

int WINAPI WinMain(HINSTANCE hInst,
					HINSTANCE hPrevInstance,
					LPSTR lpCmdLine,
					int nCmdShow)
{
	MSG msg;
	HWND hwnd;
	hInstance = hInst;

	if (!RegClass(WndProc, szMainClass, COLOR_WINDOW)) { return FALSE; }

	hwnd = CreateWindow(szMainClass,
							szMainTitle,
							WS_OVERLAPPEDWINDOW | WS_VISIBLE,
							CW_USEDEFAULT,
							CW_USEDEFAULT,
							320,
							335,
							0,
							0,
							hInstance,
							NULL);

	if (!hwnd) { return FALSE; }

	while (GetMessage(&msg, 0, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

BOOL RegClass(WNDPROC Proc, LPCTSTR szName, UINT brBackground)
{
	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.lpfnWndProc = Proc;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(brBackground + 1);
	wc.lpszMenuName = (LPCTSTR)NULL;
	wc.lpszClassName = szName;
	return (RegisterClass(&wc) != 0);
}

BOOL DrawLine(HDC hdc, int x0, int y0, int x, int y) {
	MoveToEx(hdc, x0, y0, NULL);
	return LineTo(hdc, x, y);
}

BOOL DrawCross(HDC hdc, int squareNum) {
	MoveToEx(hdc, 0, 0, NULL);
	LineTo(hdc, 100, 100);
	MoveToEx(hdc, 0, 100, NULL);
	LineTo(hdc, 100, 0);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;

	int xMouse, yMouse;
	int width, height;

	RECT mainRect;

	GetWindowRect(hwnd, &mainRect);
	width = mainRect.right - mainRect.left;
	height = mainRect.bottom - mainRect.top;

	switch (msg)
	{
		case WM_LBUTTONDOWN:
		{
			xMouse = LOWORD(lParam);
			yMouse = HIWORD(lParam);

			hdc = GetDC(hwnd);
			DrawCross(hdc, 1);

			break;
		}

		case WM_RBUTTONDOWN:
		{
			xMouse = LOWORD(lParam);
			yMouse = HIWORD(lParam);
			
			hdc = GetDC(hwnd);
			
			Ellipse(hdc, 100, 100, 200, 200);
			
			break;
		}

		case WM_PAINT:
		{
			
			hdc = BeginPaint(hwnd, &ps);

			DrawLine(hdc, 100, 0, 100, 300);
			DrawLine(hdc, 200, 0, 200, 300);
			DrawLine(hdc, 0, 100, 300, 100);
			DrawLine(hdc, 0, 200, 300, 200);


			EndPaint(hwnd, &ps);

			break;
		}

		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}
