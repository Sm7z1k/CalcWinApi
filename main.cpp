#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include <Math.h>
#include <stdio.h>

#define IDC_BUTTON_ONE		1001
#define IDC_BUTTON_TWO		1002
#define IDC_BUTTON_THREE	1003
#define IDC_BUTTON_FOUR		1004
#define IDC_BUTTON_FIVE		1005
#define IDC_BUTTON_SIX		1006
#define IDC_BUTTON_SEVEN	1007
#define IDC_BUTTON_EIGHT	1008
#define IDC_BUTTON_NINE		1009
#define IDC_BUTTON_TEN		1010
#define IDC_BUTTON_PLUS		1011
#define IDC_BUTTON_MINUS	1012
#define IDC_BUTTON_MULTIPLY 1013
#define IDC_BUTTON_DIVIDED	1014
#define IDC_BUTTON_EQUALS	1015
#define IDC_EDIT_DIGITS		1016
#define IDC_BUTTON_CHANGE	1017
#define IDC_BUTTON_DOT		1018
#define IDC_BUTTON_ZERO		1019
#define IDC_BUTTON_STEP		1020
#define IDC_BUTTON_KOREN	1021
#define IDC_BUTTON_ONEDIVDG	1022
#define IDC_BUTTON_PERCENT	1023
#define IDC_BUTTON_DELONEDG	1024
#define IDC_BUTTON_DELETE	1025

#define DIGITSCOUNT			18
#define DEFAULTWINDOWWEIGHT	515
#define DEFAULTWINDOWHEIGHT	690
#define DEFAULTBUTTONWEIGHT	125
#define DEFAULTBUTTONHEIGHT	75

BOOL operation = false;
BOOL isDot = false;
FLOAT digitA = 0.0f;
FLOAT digitB = 0.0f;
CHAR cOperation;

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

CONST CHAR className[] = "Calc";

INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(wc));

	wc.cbSize = sizeof(wc);
	wc.cbWndExtra = 0;
	wc.cbClsExtra = 0;
	wc.style = 0;

	wc.hIcon = (HICON)LoadImage(hInst, "Icons\\calc.ico", IMAGE_ICON, LR_DEFAULTSIZE, LR_DEFAULTSIZE, LR_LOADFROMFILE);
	wc.hIconSm = (HICON)LoadImage(hInst, "Icons\\calc.ico", IMAGE_ICON, LR_DEFAULTSIZE, LR_DEFAULTSIZE, LR_LOADFROMFILE);;
	wc.hCursor = 0;
	wc.hbrBackground = (HBRUSH)GetStockObject(DKGRAY_BRUSH);

	wc.hInstance = hInst;
	wc.lpfnWndProc = WndProc;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = className;

	if (!RegisterClassEx(&wc)) MessageBox(NULL, "Class registration failed", "Error", MB_OK | MB_ICONERROR);

	HWND hwnd = CreateWindowEx
	(
		NULL,
		className,
		className,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		DEFAULTWINDOWWEIGHT, DEFAULTWINDOWHEIGHT,
		NULL,
		NULL,
		hInst,
		NULL
	);

	if (hwnd == NULL)
	{
		MessageBox(NULL, "Window creation failed", "Error", MB_OK | MB_ICONERROR);
		return 0;
	}
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	MSG msg;
	while (GetMessage(&msg, 0, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

VOID CreateButton(HWND hwnd, INT but, INT x, INT y, CHAR* ch)
{
	HWND hButton = CreateWindowEx
	(
		NULL,
		"Button",
		ch,
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		x, y,
		DEFAULTBUTTONWEIGHT, DEFAULTBUTTONHEIGHT,
		hwnd,
		(HMENU)but,
		GetModuleHandle(NULL),
		NULL
	);
}

VOID DigitButton(HWND hwnd, INT digit, LPARAM dgt)
{
	(CHAR)digit;
	HWND hEdit = GetDlgItem(hwnd, IDC_EDIT_DIGITS);
	CHAR digits[DIGITSCOUNT]{};
	SendMessage(hEdit, WM_GETTEXT, DIGITSCOUNT + 1, (LPARAM)digits);

	int i = 0;
	while (digits[i] != '\0')
	{
		i++;
	}

	if (digits[0] == '0' && digits[1] != '.')
	{		
		SendMessage(hEdit, WM_SETTEXT, 0, dgt);
	}
	else if (digits[0] != '0' && i + 1 < DIGITSCOUNT && operation == false || digits[1] == '.' && operation == false)
	{
		digits[i] = digit;
		SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)digits);
	}
	else if(operation == true)
	{
		digitA = atof(digits);		
		SendMessage(hEdit, WM_SETTEXT, 0, dgt);
		operation = false;
	}
}

FLOAT Calculate(FLOAT a, FLOAT b)
{
	FLOAT Result = 0;
	switch (cOperation)
	{
	case '+':
		Result = a + b;		
		break;	
	case '-':
		Result = a - b;		
		break;
	case '*':
		Result = a * b;		
		break;
	case '/':
		Result = a / b;		
		break;	
	}
	return Result;
}

VOID ProverkaNaNull(HWND hwnd)
{
	HWND hEdit = GetDlgItem(hwnd, IDC_EDIT_DIGITS);
	CHAR digits[DIGITSCOUNT]{};
	SendMessage(hEdit, WM_GETTEXT, DIGITSCOUNT + 1, (LPARAM)digits);
	int i = sizeof(digits) / sizeof(digits[0]) - 1;
	while (digits[i] == '0' || digits[i] == '\0')
	{
		digits[i] = '\0';
		i--;
	}
	if(digits[i] == '.') digits[i] = '\0';
	SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)digits);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{	
	switch (uMsg)
	{
	case WM_CREATE:
	{
		HWND hEditDigit = CreateWindowEx
		(
			NULL,
			"Edit",
			"",
			WS_CHILD | WS_VISIBLE | ES_RIGHT,
			0, 70,
			490, 60,
			hwnd,
			(HMENU)IDC_EDIT_DIGITS,
			GetModuleHandle(NULL),
			NULL
		);
		LOGFONT LF = { 60, 20, 0, 0, FW_BOLD, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, DRAFT_QUALITY, 0, "Arial Black" };
		HFONT hFont = CreateFontIndirect(&LF);
		SendMessage(hEditDigit, WM_SETFONT, (WPARAM)hFont, 0);
		SendMessage(hEditDigit, WM_SETTEXT, 0, (LPARAM)"0");

		CreateButton(hwnd, IDC_BUTTON_PERCENT, 125, 200, (CHAR*)"%");
		CreateButton(hwnd, IDC_BUTTON_DELETE, 250, 200, (CHAR*)"C");
		CreateButton(hwnd, IDC_BUTTON_DELONEDG, 375, 200, (CHAR*)"<x#");
		CreateButton(hwnd, IDC_BUTTON_ONEDIVDG, 0, 275, (CHAR*)"1/x");
		CreateButton(hwnd, IDC_BUTTON_STEP, 125, 275, (CHAR*)"x^2");
		CreateButton(hwnd, IDC_BUTTON_KOREN, 250, 275, (CHAR*)"Vx");
		CreateButton(hwnd, IDC_BUTTON_DIVIDED, 375, 275, (CHAR*)"/");

		CreateButton(hwnd, IDC_BUTTON_MULTIPLY, 375, 350, (CHAR*)"x");
		CreateButton(hwnd, IDC_BUTTON_MINUS, 375, 425, (CHAR*)"-");
		CreateButton(hwnd, IDC_BUTTON_PLUS, 375, 500, (CHAR*)"+");
		CreateButton(hwnd, IDC_BUTTON_EQUALS, 375, 575, (CHAR*)"=");

		CreateButton(hwnd, IDC_BUTTON_SEVEN, 0, 350, (CHAR*)"7");
		CreateButton(hwnd, IDC_BUTTON_EIGHT, 125, 350, (CHAR*)"8");
		CreateButton(hwnd, IDC_BUTTON_NINE, 250, 350, (CHAR*)"9");
		CreateButton(hwnd, IDC_BUTTON_FOUR, 0, 425, (CHAR*)"4");
		CreateButton(hwnd, IDC_BUTTON_FIVE, 125, 425, (CHAR*)"5");
		CreateButton(hwnd, IDC_BUTTON_SIX, 250, 425, (CHAR*)"6");
		CreateButton(hwnd, IDC_BUTTON_ONE, 0, 500, (CHAR*)"1");
		CreateButton(hwnd, IDC_BUTTON_TWO, 125, 500, (CHAR*)"2");
		CreateButton(hwnd, IDC_BUTTON_THREE, 250, 500, (CHAR*)"3");
		CreateButton(hwnd, IDC_BUTTON_CHANGE, 0, 575, (CHAR*)"+/-");
		CreateButton(hwnd, IDC_BUTTON_ZERO, 125, 575, (CHAR*)"0");
		CreateButton(hwnd, IDC_BUTTON_DOT, 250, 575, (CHAR*)".");
	}
	break;
	case WM_CTLCOLOREDIT:
	{
		if ((HWND)lParam == GetDlgItem(hwnd, IDC_EDIT_DIGITS))
		{
			SetBkColor((HDC)wParam, RGB(64, 64, 64));
			SetTextColor((HDC)wParam, RGB(255, 255, 255));
		}
	}
	break;	
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case IDC_BUTTON_ONE:
		{
			DigitButton(hwnd, 49, (LPARAM)"1");
		}
		break;

		case IDC_BUTTON_TWO:
		{
			DigitButton(hwnd, 50, (LPARAM)"2");
		}
		break;
		case IDC_BUTTON_THREE:
		{
			DigitButton(hwnd, 51, (LPARAM)"3");
		}
		break;
		case IDC_BUTTON_FOUR:
		{
			DigitButton(hwnd, 52, (LPARAM)"4");
		}
		break;
		case IDC_BUTTON_FIVE:
		{
			DigitButton(hwnd, 53, (LPARAM)"5");
		}
		break;
		case IDC_BUTTON_SIX:
		{
			DigitButton(hwnd, 54, (LPARAM)"6");
		}
		break;
		case IDC_BUTTON_SEVEN:
		{
			DigitButton(hwnd, 55, (LPARAM)"7");
		}
		break;
		case IDC_BUTTON_EIGHT:
		{
			DigitButton(hwnd, 56, (LPARAM)"8");
		}
		break;
		case IDC_BUTTON_NINE:
		{
			DigitButton(hwnd, 57, (LPARAM)"9");
		}
		break;
		case IDC_BUTTON_ZERO:
		{
			DigitButton(hwnd, 48, (LPARAM)"0");
		}
		break;

		case IDC_BUTTON_DELETE:
		{
			HWND hEdit = GetDlgItem(hwnd, IDC_EDIT_DIGITS);
			SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)"0");
			isDot = false;
		}
		break;
		case IDC_BUTTON_DELONEDG:
		{
			HWND hEdit = GetDlgItem(hwnd, IDC_EDIT_DIGITS);
			CHAR digits[DIGITSCOUNT]{};
			SendMessage(hEdit, WM_GETTEXT, DIGITSCOUNT + 1, (LPARAM)digits);

			if(digits[1] == '\0') SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)"0");
			else
			{
				int i = 0;
				while (digits[i] != '\0')
				{
					i++;
				}
				digits[i - 1] = '\0';
				SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)digits);
			}
		}
		break;

		case IDC_BUTTON_PLUS:
		{
			operation = true;
			cOperation = '+';
			isDot = false;
		}
		break;
		case IDC_BUTTON_MINUS:
		{
			operation = true;
			cOperation = '-';
			isDot = false;
		}
		break;
		case IDC_BUTTON_MULTIPLY:
		{
			operation = true;
			cOperation = '*';
			isDot = false;
		}
		break;
		case IDC_BUTTON_DIVIDED:
		{
			operation = true;
			cOperation = '/';
			isDot = false;
		}
		break;
		case IDC_BUTTON_KOREN:
		{
			HWND hEdit = GetDlgItem(hwnd, IDC_EDIT_DIGITS);
			CHAR digits[DIGITSCOUNT]{};
			SendMessage(hEdit, WM_GETTEXT, DIGITSCOUNT + 1, (LPARAM)digits);
			digitA = atof(digits);			
			CHAR rez[DIGITSCOUNT]{};
			digitA = sqrt(digitA);
			sprintf(rez, "%f", digitA);
			SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)rez);
			ProverkaNaNull(hwnd);
		}
		break;
		case IDC_BUTTON_STEP:
		{
			HWND hEdit = GetDlgItem(hwnd, IDC_EDIT_DIGITS);
			CHAR digits[DIGITSCOUNT]{};
			SendMessage(hEdit, WM_GETTEXT, DIGITSCOUNT + 1, (LPARAM)digits);
			digitA = atof(digits);
			CHAR rez[DIGITSCOUNT]{};
			digitA = digitA * digitA;
			sprintf(rez, "%f", digitA);
			SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)rez);
			ProverkaNaNull(hwnd);
		}
		break;
		case IDC_BUTTON_ONEDIVDG:
		{
			HWND hEdit = GetDlgItem(hwnd, IDC_EDIT_DIGITS);
			CHAR digits[DIGITSCOUNT]{};
			SendMessage(hEdit, WM_GETTEXT, DIGITSCOUNT + 1, (LPARAM)digits);
			digitA = atof(digits);
			CHAR rez[DIGITSCOUNT]{};
			digitA = 1 / digitA;
			sprintf(rez, "%f", digitA);
			SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)rez);
			ProverkaNaNull(hwnd);
		}
		break;
		case IDC_BUTTON_PERCENT:
		{
			HWND hEdit = GetDlgItem(hwnd, IDC_EDIT_DIGITS);
			CHAR digits[DIGITSCOUNT]{};
			SendMessage(hEdit, WM_GETTEXT, DIGITSCOUNT + 1, (LPARAM)digits);
			digitA = atof(digits);
			CHAR rez[DIGITSCOUNT]{};
			digitA = digitA / 100;
			sprintf(rez, "%f", digitA);			
			SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)rez);
			ProverkaNaNull(hwnd);
		}
		break;
		case IDC_BUTTON_CHANGE:
		{
			HWND hEdit = GetDlgItem(hwnd, IDC_EDIT_DIGITS);
			CHAR digits[DIGITSCOUNT]{};
			SendMessage(hEdit, WM_GETTEXT, DIGITSCOUNT + 1, (LPARAM)digits);
			digitA = atof(digits);
			CHAR rez[DIGITSCOUNT]{};
			digitA = digitA * -1;
			sprintf(rez, "%f", digitA);
			SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)rez);
			ProverkaNaNull(hwnd);
		}
		break;
		case IDC_BUTTON_DOT:
		{
			if(isDot == false)
			{
				HWND hEdit = GetDlgItem(hwnd, IDC_EDIT_DIGITS);
				CHAR digits[DIGITSCOUNT]{};
				SendMessage(hEdit, WM_GETTEXT, DIGITSCOUNT + 1, (LPARAM)digits);
				int i = 0;
				while (digits[i] != '\0')
				{
					i++;
				}
				digits[i] = '.';
				SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)digits);
				isDot = true;
			}
		}
		break;
		case IDC_BUTTON_EQUALS:
		{
			if(digitA != 0)
			{
				HWND hEdit = GetDlgItem(hwnd, IDC_EDIT_DIGITS);
				CHAR digits[DIGITSCOUNT]{};
				CHAR rez[DIGITSCOUNT]{};
				SendMessage(hEdit, WM_GETTEXT, DIGITSCOUNT + 1, (LPARAM)digits);
				digitB = atof(digits);
				digitA = Calculate(digitA, digitB);
				digitB = 0;
				sprintf(rez, "%f", digitA);				
				digitA = 0;				
				SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)rez);
				ProverkaNaNull(hwnd);
			}
		}
		break;		
		}	
	}
	break;
	case WM_DESTROY: PostQuitMessage(0); break;

	case WM_CLOSE: DestroyWindow(hwnd); break;

	default: return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return NULL;
}