//g++ lab2.cpp -o lab2 -lgdi32

#include "pch.h"
#include <iostream>
#include "windows.h"
#include <tchar.h>
#include <string>
#include <cstdio>
#include <math.h>
#include "string.h"

using namespace std;

BOOL RegClass(WNDPROC, LPCTSTR, UINT);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE hInstance;
char szMainClass[] = "MainClass";
char szMainTitle[] = "Cross-Zero";

void DrawLine(HDC, int, int, int, int);
void DrawGameField(HDC);
void DrawCross(HDC, int, int);
void CheckForWinner(HWND);
void PrintWinner(HWND);
void ComputerTurn(HDC, int, int);

int xMouse, yMouse;

char gameField[3][3] = {'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a'};
char symbol;
char winner[64] = "Not yet";


int WINAPI WinMain(HINSTANCE hInst,	HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	MSG msg;
	HWND hwnd;
	hInstance = hInst;

	if (!RegClass(WndProc, szMainClass, COLOR_WINDOW)) { return FALSE; }

	const int w = 308, h = 330;
	int x = (GetSystemMetrics(SM_CXSCREEN) - w) / 2;
	int y = (GetSystemMetrics(SM_CYSCREEN) - h) / 2;

	hwnd = CreateWindow(szMainClass,
							szMainTitle,
							WS_VISIBLE | WS_SYSMENU | WS_MINIMIZEBOX,
							x, y,
							w, h,
							0,
							0,
							hInstance,
							NULL);

	if (!hwnd) { return FALSE; }

	while (GetMessage(&msg, 0, 0, 0))
	{		
		CheckForWinner(hwnd);
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

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{	
	HDC hdc;
	PAINTSTRUCT ps;		

	switch (msg)
	{	
		case WM_LBUTTONDOWN:
		{
			xMouse = LOWORD(lParam);
			yMouse = HIWORD(lParam);

			hdc = GetDC(hwnd);

			if (gameField[xMouse / 100][yMouse / 100] != 'O' 
				&& gameField[xMouse / 100][yMouse / 100] != 'X')
			{
				DrawCross(hdc, xMouse / 100, yMouse / 100);
				symbol = 'X';
				gameField[xMouse / 100][yMouse / 100] = symbol;
			}
			else
			{
				char str[64];
				sprintf(str, "There is already cross/zero \nPlease, try again", xMouse / 100, yMouse / 100);
				MessageBox(hwnd, (LPCTSTR)str, "XY", MB_OK);
			}										
			break;
		}

		case WM_LBUTTONUP:
		{
			/*char str[64];
			sprintf(str, "0: %c, 1: %c, 2: %c, 3: %c, 4: %c, 5: %c, 6: %c, 7: %c, 8: %c", gameField[0][0], gameField[1][0], gameField[2][0],
																							gameField[0][1], gameField[1][1], gameField[2][1],
																							gameField[0][2], gameField[1][2], gameField[2][2]);
			MessageBox(hwnd, (LPCTSTR)str, "GameField", MB_OK);
			*/
			/*sprintf(str, "3: %c, 4: %c, 5: %c", gameField[0][1], gameField[1][1], gameField[2][1]);
			MessageBox(hwnd, (LPCTSTR)str, "RES", MB_OK);
			sprintf(str, "6: %c, 7: %c, 8: %c", gameField[0][2], gameField[1][2], gameField[2][2]);
			MessageBox(hwnd, (LPCTSTR)str, "RES", MB_OK);*/
			xMouse = LOWORD(lParam);
			yMouse = HIWORD(lParam);

			hdc = GetDC(hwnd);
			ComputerTurn(hdc, xMouse, yMouse);

			break;
		}

		case WM_RBUTTONDOWN:
		{
			xMouse = LOWORD(lParam);
			yMouse = HIWORD(lParam);
			
			hdc = GetDC(hwnd);
			
			if (gameField[xMouse / 100][yMouse / 100] != 'O'
				&& gameField[xMouse / 100][yMouse / 100] != 'X')
			{
				Ellipse(hdc, (xMouse / 100) * 100, (yMouse / 100) * 100, (xMouse / 100 + 1) * 100, (yMouse / 100 + 1) * 100);
				symbol = 'O';
				gameField[xMouse / 100][yMouse / 100] = symbol;
			}
			else
			{
				char str[64];
				sprintf(str, "There is already cross/zero \nPlease, try again", xMouse / 100, yMouse / 100);
				MessageBox(hwnd, (LPCTSTR)str, "XY", MB_OK);
			}			
			break;
		}

		case WM_PAINT:
		{			
			hdc = BeginPaint(hwnd, &ps);
			DrawGameField(hdc);			//Отрисовка поля
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

//Отрисовка линии
void DrawLine(HDC hdc, int left, int top, int right, int bottom) 
{
	MoveToEx(hdc, left, top, NULL);
	LineTo(hdc, right, bottom);
}

//Отрисовка игрового поля
void DrawGameField(HDC hdc) 
{
	DrawLine(hdc, 100, 0, 100, 300);
	DrawLine(hdc, 200, 0, 200, 300);
	DrawLine(hdc, 0, 100, 300, 100);
	DrawLine(hdc, 0, 200, 300, 200);
}

//Отрисовка креста
void DrawCross(HDC hdc, int left, int top)
{
	DrawLine(hdc, left * 100, top * 100, (left + 1) * 100, (top + 1) * 100);
	DrawLine(hdc, left * 100, (top + 1) * 100, (left + 1) * 100, top * 100);
}

//Проверка на победителя
void CheckForWinner(HWND hwnd)
{
	if ((gameField[0][0] == symbol && gameField[1][0] == symbol && gameField[2][0] == symbol) ||
		(gameField[0][1] == symbol && gameField[1][1] == symbol && gameField[2][1] == symbol) ||
		(gameField[0][2] == symbol && gameField[1][1] == symbol && gameField[2][2] == symbol) ||
		(gameField[0][0] == symbol && gameField[0][1] == symbol && gameField[0][2] == symbol) ||
		(gameField[1][0] == symbol && gameField[1][1] == symbol && gameField[1][2] == symbol) ||
		(gameField[2][0] == symbol && gameField[2][1] == symbol && gameField[2][2] == symbol) ||
		(gameField[0][0] == symbol && gameField[1][1] == symbol && gameField[2][2] == symbol) ||
		(gameField[2][0] == symbol && gameField[1][1] == symbol && gameField[0][2] == symbol)
		)
	{
		if (symbol == 'X') { strcpy(winner, "User"); }
		else if (symbol == 'O') { strcpy(winner, "Computer"); }
	}
	else if ((gameField[0][0] != 'a' && gameField[1][0] != 'a' && gameField[2][0] != 'a') &&
			 (gameField[0][1] != 'a' && gameField[1][1] != 'a' && gameField[2][1] != 'a') &&
			 (gameField[0][2] != 'a' && gameField[1][1] != 'a' && gameField[2][2] != 'a'))
	{
		strcpy(winner, "Nobody");
	}	
	
	if (strcmp(winner, "User") == 0
		|| strcmp(winner, "Computer") == 0
		|| strcmp(winner, "Nobody") == 0
		)
	{
		char str[64];
		sprintf(str, "Winner: %s", winner);
		MessageBox(hwnd, (LPCTSTR)str, "Winner", MB_OK);
		SendMessage(hwnd, WM_CLOSE, 0, 0);
	}
}

//Ход компьютера
void ComputerTurn(HDC hdc, int _xMouse, int _yMouse)
{

}

BOOL FreeSquare()
{
	
}


