#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include <Math.h>
#include <stdio.h>
#include "options.h"

BOOL operation = false;
INT operationX2 = 0;
BOOL isDot = false;
BOOL isMinus = false;
FLOAT digitA = 0.0f;
FLOAT digitB = 0.0f;
CHAR cOperation;
CHAR cOperationX2;
CONST CHAR className[] = "Calc";
LPCSTR symbols[] =
{
	"%", "CE", "C", "<x|", "1/x", "x^2", "Vx", "/" ,
	"7", "8", "9", "x","4","5","6","-", "1",
	"2", "3", "+", "+/-", "0", ".", "=",
};

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
VOID CreateButton(HWND hwnd, INT but, INT x, INT y, CHAR* ch);
VOID DigitButton(HWND hwnd, INT digit, LPARAM dgt);
FLOAT Calculate(FLOAT a, FLOAT b);
VOID ProverkaNaNull(HWND hwnd);
VOID OperationButtons(CHAR ch);
VOID Equals(HWND hwnd);

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
	wc.hbrBackground = 0;

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
		WS_OVERLAPPEDWINDOW^WS_THICKFRAME^WS_MAXIMIZEBOX,
		CW_USEDEFAULT, CW_USEDEFAULT,
		(DEFAULTBUTTONWEIGHT * 4) + (BUTTONINTERVAL * 3) + 16, (DEFAULTBUTTONHEIGHT * 6) + (BUTTONINTERVAL * 5) + 240,
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

VOID CreateButton(HWND hwnd)
{
	INT k = 0;
	INT but = 1001;
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			but += k;
			HWND hButton = CreateWindowEx
			(
				NULL,
				"Button",
				(LPCSTR)symbols[k],
				WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW,
				(0 + j * (DEFAULTBUTTONWEIGHT + BUTTONINTERVAL)), (200 + i * (DEFAULTBUTTONHEIGHT + BUTTONINTERVAL)),
				DEFAULTBUTTONWEIGHT, DEFAULTBUTTONHEIGHT,
				hwnd,
				(HMENU)but,
				GetModuleHandle(NULL),
				NULL
			);
			but = 1001;
			k++;
		}

	}

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
	else if (operation == true)
	{
		digitA = atof(digits);
		SendMessage(hEdit, WM_SETTEXT, 0, dgt);
		operation = false;
	}
	else if (strcmp(digits, SMILEERROR) == true)
	{
		SendMessage(hEdit, WM_SETTEXT, 0, dgt);
	}

	if (operationX2 == 1)
	{
		cOperationX2 = cOperation;
	}
}

FLOAT Calculate(FLOAT a, FLOAT b, CHAR ch)
{
	FLOAT Result = 0;
	switch (ch)
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

VOID OperationButtons(CHAR ch, HWND hwnd)
{
	operation = true;
	cOperation = ch;
	isDot = false;
	operationX2++;
	if (operationX2 == 2)
	{
		HWND hEdit = GetDlgItem(hwnd, IDC_EDIT_DIGITS);
		if (digitA != 0)
		{
			CHAR digits[DIGITSCOUNT]{};
			CHAR rez[DIGITSCOUNT]{};
			SendMessage(hEdit, WM_GETTEXT, DIGITSCOUNT + 1, (LPARAM)digits);
			digitB = atof(digits);
			digitA = Calculate(digitA, digitB, cOperationX2);			
			sprintf(rez, "%g", digitA);			
			digitA = 0;
			SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)rez);			
		}
		operationX2 = 1;
	}
}

VOID Equals(HWND hwnd)
{
	HWND hEdit = GetDlgItem(hwnd, IDC_EDIT_DIGITS);
	if (digitA != 0)
	{
		CHAR digits[DIGITSCOUNT]{};		
		CHAR rez[DIGITSCOUNT]{};		
		SendMessage(hEdit, WM_GETTEXT, DIGITSCOUNT + 1, (LPARAM)digits);
		digitB = atof(digits);
		digitA = Calculate(digitA, digitB, cOperation);
		sprintf(rez, "%g", digitA);
		digitA = 0;
		SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)rez);		
	}	
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
	{
		HBRUSH hBrush = CreateSolidBrush(RGB(32, 32, 32));
		SetClassLong(hwnd, GCL_HBRBACKGROUND, (LONG)hBrush);

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

		CreateButton(hwnd);			
	}
	break;
	case WM_CTLCOLOREDIT:
	{
		if ((HWND)lParam == GetDlgItem(hwnd, IDC_EDIT_DIGITS))
		{
			SetBkColor((HDC)wParam, RGB(32, 32, 32));
			SetTextColor((HDC)wParam, RGB(255, 255, 255));
		}
	}
	break;
	case WM_DRAWITEM:
	{
		static HFONT hfontButton = CreateFont(-25, 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET,
			OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY, DEFAULT_PITCH, "Courier New");
		LPDRAWITEMSTRUCT Item = (LPDRAWITEMSTRUCT)lParam;

		SelectObject(Item->hDC, hfontButton);	

		/*HBRUSH hBrushPush = CreateSolidBrush(RGB(20, 20, 20));
		HBRUSH hBrushDef = CreateSolidBrush(RGB(32, 32, 32));
		if (Item->itemState & ODS_SELECTED)
			FillRect(Item->hDC, &Item->rcItem, hBrushPush);
		else
			FillRect(Item->hDC, &Item->rcItem, hBrushDef);*/

		int len = GetWindowTextLength(Item->hwndItem);
		char* buf = new char[len + 1];
		GetWindowTextA(Item->hwndItem, buf, len + 1);
		DrawTextA(Item->hDC, buf, len, &Item->rcItem, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

		return true;
	}
	break;
	case WM_CTLCOLORBTN:
	{
		for (int i = 0; i < 8; i++)
		{
			if ((HWND)lParam == GetDlgItem(hwnd, IDC_BUTTON_PERCENT + i))
			{
				SetBkColor((HDC)wParam, RGB(50, 50, 50));
				SetTextColor((HDC)wParam, RGB(255, 255, 255));
				return (LRESULT)CreateSolidBrush(RGB(50, 50, 50));
			}
		}
		if ((HWND)lParam == GetDlgItem(hwnd, IDC_BUTTON_MULTIPLY) ||
			(HWND)lParam == GetDlgItem(hwnd, IDC_BUTTON_MINUS) ||
			(HWND)lParam == GetDlgItem(hwnd, IDC_BUTTON_PLUS))
		{
			SetBkColor((HDC)wParam, RGB(50, 50, 50));
			SetTextColor((HDC)wParam, RGB(255, 255, 255));
			return (LRESULT)CreateSolidBrush(RGB(50, 50, 50));
		}
		if ((HWND)lParam == GetDlgItem(hwnd, IDC_BUTTON_EQUALS))
		{
			SetBkColor((HDC)wParam, RGB(139, 231, 236));			
			return (LRESULT)CreateSolidBrush(RGB(139, 231, 236));
		}
		for(int i = 0; i < 3; i++)
		{
			if ((HWND)lParam == GetDlgItem(hwnd, IDC_BUTTON_SEVEN + i))
			{
				SetBkColor((HDC)wParam, RGB(59, 59, 59));
				SetTextColor((HDC)wParam, RGB(255, 255, 255));
				return (LRESULT)CreateSolidBrush(RGB(59, 59, 59));
			}
			if ((HWND)lParam == GetDlgItem(hwnd, IDC_BUTTON_FOUR + i))
			{
				SetBkColor((HDC)wParam, RGB(59, 59, 59));
				SetTextColor((HDC)wParam, RGB(255, 255, 255));
				return (LRESULT)CreateSolidBrush(RGB(59, 59, 59));
			}
			if ((HWND)lParam == GetDlgItem(hwnd, IDC_BUTTON_ONE + i))
			{
				SetBkColor((HDC)wParam, RGB(59, 59, 59));
				SetTextColor((HDC)wParam, RGB(255, 255, 255));
				return (LRESULT)CreateSolidBrush(RGB(59, 59, 59));
			}
			if ((HWND)lParam == GetDlgItem(hwnd, IDC_BUTTON_CHANGE + i))
			{
				SetBkColor((HDC)wParam, RGB(59, 59, 59));
				SetTextColor((HDC)wParam, RGB(255, 255, 255));
				return (LRESULT)CreateSolidBrush(RGB(59, 59, 59));
			}
		}
	}
	break;
	case WM_COMMAND:
	{
		if (LOWORD(wParam) == IDC_EDIT_DIGITS && HIWORD(wParam) == EN_SETFOCUS)
			SetFocus(hwnd);

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
		case IDC_BUTTON_CE:
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

			if (digits[1] == '\0') SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)"0");
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
			OperationButtons('+', hwnd);
		}
		break;
		case IDC_BUTTON_MINUS:
		{
			OperationButtons('-', hwnd);
		}
		break;
		case IDC_BUTTON_MULTIPLY:
		{
			OperationButtons('*', hwnd);
		}
		break;
		case IDC_BUTTON_DIVIDED:
		{
			OperationButtons('/', hwnd);
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
			sprintf(rez, "%g", digitA);
			SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)rez);			
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
			sprintf(rez, "%g", digitA);
			SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)rez);			
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
			sprintf(rez, "%g", digitA);
			SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)rez);			
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
			sprintf(rez, "%g", digitA);
			SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)rez);			
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
			sprintf(rez, "%g", digitA);
			SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)rez);	
		}
		break;
		case IDC_BUTTON_DOT:
		{
			if (isDot == false)
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
			Equals(hwnd);
			operationX2 = 0;
		}
		break;
		case IDC_EDIT_DIGITS:
		{
			HWND hEdit = GetDlgItem(hwnd, IDC_EDIT_DIGITS);
			CHAR digits[DIGITSCOUNT]{};
			SendMessage(hEdit, WM_GETTEXT, DIGITSCOUNT + 1, (LPARAM)digits);
			if (sizeof(digits) / sizeof(digits[0]) > DIGITSCOUNT)
				SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)":(");
		}
		break;
		}
	}
	break;
	case WM_KEYDOWN:
	{
		if(GetKeyState(VK_SHIFT) < 0)
		{
			if(wParam == 0x38) SendMessage(hwnd, WM_COMMAND, IDC_BUTTON_MULTIPLY, 0);
		}
		else if(LOWORD(wParam) >= 0x37 && LOWORD(wParam) <= 0x39)
		{
			SendMessage(hwnd, WM_COMMAND, LOWORD(wParam) - 0x37 + IDC_BUTTON_SEVEN, 0);
		}	
		else if (LOWORD(wParam) >= 0x34 && LOWORD(wParam) <= 0x36)
		{
			SendMessage(hwnd, WM_COMMAND, LOWORD(wParam) - 0x34 + IDC_BUTTON_FOUR, 0);
		}
		else if (LOWORD(wParam) >= 0x31 && LOWORD(wParam) <= 0x33)
		{
			SendMessage(hwnd, WM_COMMAND, LOWORD(wParam) - 0x31 + IDC_BUTTON_ONE, 0);
		}
		else if (LOWORD(wParam) == 0x30)
		{
			SendMessage(hwnd, WM_COMMAND, LOWORD(wParam) - 0x30 + IDC_BUTTON_ZERO, 0);
		}
		switch(LOWORD(wParam))
		{
		case VK_OEM_PLUS: SendMessage(hwnd, WM_COMMAND, IDC_BUTTON_PLUS, 0); break;
		case VK_OEM_MINUS: SendMessage(hwnd, WM_COMMAND, IDC_BUTTON_MINUS, 0); break;		
		case VK_MULTIPLY: SendMessage(hwnd, WM_COMMAND, IDC_BUTTON_MULTIPLY, 0); break;
		case VK_OEM_2:
		case VK_DIVIDE: SendMessage(hwnd, WM_COMMAND, IDC_BUTTON_DIVIDED, 0); break;
		case VK_OEM_PERIOD: SendMessage(hwnd, WM_COMMAND, IDC_BUTTON_DOT, 0); break;
		case VK_RETURN: SendMessage(hwnd, WM_COMMAND, IDC_BUTTON_EQUALS, 0); break;
		case VK_ESCAPE: SendMessage(hwnd, WM_COMMAND, IDC_BUTTON_DELETE, 0); break;
		case VK_BACK: SendMessage(hwnd, WM_COMMAND, IDC_BUTTON_DELONEDG, 0); break;
		}
	}
	break;
	case WM_DESTROY: PostQuitMessage(0); break;

	case WM_CLOSE: DestroyWindow(hwnd); break;

	default: return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return NULL;
}