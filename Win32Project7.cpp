#include "stdafx.h"
#include "Win32Project7.h"
using namespace std;
// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szClass[] = "MyWindow";					// The title bar text
TCHAR szWindow[] = "MyWindow";			// the main window class name
// Forward declarations of functions included in this code module:
ATOM MyRegisterClass(HINSTANCE hInstance, LPCSTR, WNDPROC);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
void InitChessboard(BOOL color);
void Load_Game(char);
void Save_Game(char);
BOOL CALLBACK New_Game_Proc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK AboutDlgProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK Load_Game_Proc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK Save_Game_Proc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK Color_Settings_Proc(HWND, UINT, WPARAM, LPARAM);
void Save_Settings(HWND[]);
void LOAD_Settings(HWND[]);
void Paint_squares(HWND[], HWND);
UINT* Recieve_settings();

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	MSG msg;
	HWND hWnd;
	hLib = LoadLibraryEx("resources.dll", 0, LOAD_LIBRARY_AS_DATAFILE);
	MyRegisterClass(hInstance, szClass, WndProc);

	hWnd = CreateWindow(szClass, szWindow, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		CW_USEDEFAULT, CW_USEDEFAULT, CELL * 8 + 20, CELL * 8 + 80, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{
		return FALSE;
	}

	if (!hLib)
	{
		MessageBox(hWnd, "Could not open resources.dll!", "ERROR", MB_OK|MB_ICONERROR);
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}


ATOM MyRegisterClass(HINSTANCE hInstance, LPCSTR szClassname, WNDPROC Proc)
{
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = Proc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szClassname;
	wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

	return RegisterClassEx(&wcex);
}

BOOL CreateMenuItem(HMENU hMenu, char* str, UINT uItem, UINT wID, HMENU hSubMenu, BOOL fByPosition, UINT fType)
{
	MENUITEMINFO mii;
	mii.cbSize = sizeof(MENUITEMINFO);
	mii.fMask = MIIM_STATE | MIIM_TYPE | MIIM_SUBMENU | MIIM_ID;
	mii.fType = fType;
	mii.fState = MFS_ENABLED;
	mii.dwTypeData = str;
	mii.cch = sizeof(str);
	mii.wID = wID;
	mii.hSubMenu = hSubMenu;
	return InsertMenuItem(hMenu, uItem, fByPosition, &mii);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	static RECT r;
	static HBITMAP hBmp;
	static BITMAP bm;
	static HDC hdc, hmemdc;
	HBRUSH hBr,hBr2,hOldBr;
	HPEN hPn, hOldPn;
	static FOCUS* p;
	pos* queue;
	char szBuff[150];
	char szBuff2[100];
	static bool b,b1 = 0;
	static HMENU hMainMenu,hGame,hSettings;
	static UINT a;
	static HWND hStatus;
	UINT* c = new UINT[9];
	switch (message)
	{
	case WM_CREATE:
		hMainMenu = CreateMenu();
		hGame = CreatePopupMenu();
		hSettings = CreatePopupMenu();
		LoadString(hLib, IDS_SAVE_GAME, szBuff, 149);
		CreateMenuItem(hGame, szBuff, 0, IDS_SAVE_GAME, NULL, FALSE, MFT_STRING);
		LoadString(hLib, IDS_LOAD_GAME, szBuff, 149);
		CreateMenuItem(hGame, szBuff, 0, IDS_LOAD_GAME, NULL, FALSE, MFT_STRING);
		LoadString(hLib, IDS_NEW_GAME_AI, szBuff, 149);
		CreateMenuItem(hGame, szBuff, 0, IDS_NEW_GAME_AI, NULL, FALSE, MFT_STRING);
		LoadString(hLib, IDS_NEW_GAME_H, szBuff, 149);
		CreateMenuItem(hGame, szBuff, 0, IDS_NEW_GAME_H, NULL, FALSE, MFT_STRING);

		LoadString(hLib, IDS_COLOR_SETTINGS, szBuff, 149);
		CreateMenuItem(hSettings, szBuff, 0, IDS_COLOR_SETTINGS, NULL, FALSE, MFT_STRING);
		
		LoadString(hLib, IDS_GAME, szBuff, 149);
		CreateMenuItem(hMainMenu, szBuff, 0, IDS_SETTINGS, hGame, FALSE, MFT_STRING);
		LoadString(hLib, IDS_SETTINGS, szBuff, 149);
		CreateMenuItem(hMainMenu, szBuff, 0, IDS_SETTINGS, hSettings, FALSE, MFT_STRING);
		LoadString(hLib, IDS_ABOUT, szBuff, 149);
		CreateMenuItem(hMainMenu, szBuff, 0, IDS_ABOUT, NULL, FALSE, MFT_STRING);

		SetMenu(hWnd,hMainMenu);
		DrawMenuBar(hWnd);
		LoadString(hLib, IDS_WELCOME, szBuff, 149);
		hStatus = CreateStatusWindow(WS_CHILD | WS_VISIBLE, szBuff, hWnd, ID_STATUS);
		hmemdc = CreateCompatibleDC(hdc);
		InitChessboard(WHITE);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDS_SAVE_GAME:
			DialogBox(hLib, MAKEINTRESOURCE(IDD_SAVE_GAME), hWnd, Save_Game_Proc);
			break;
		case IDS_LOAD_GAME:
			DialogBox(hLib, MAKEINTRESOURCE(IDD_LOAD_GAME), hWnd, Load_Game_Proc);
			break;
		case IDS_ABOUT:
			DialogBox(hLib, MAKEINTRESOURCE(IDD_ABOUT), hWnd, AboutDlgProc);
			break;
		case IDS_NEW_GAME_AI:
			LoadString(hLib, IDS_SAVE_QUESTION, szBuff, 99);
			LoadString(hLib, IDS_MESSAGE, szBuff2, 99);
			a=MessageBox(hWnd, szBuff, szBuff2,MB_YESNOCANCEL|MB_ICONQUESTION);
			switch (a)
			{
			case IDYES:
				DialogBox(hLib, MAKEINTRESOURCE(IDD_SAVE_GAME), hWnd, Save_Game_Proc);
				DialogBox(hLib, MAKEINTRESOURCE(IDD_NEW_GAME), hWnd, New_Game_Proc);
				break;
			case IDNO:
				DialogBox(hLib, MAKEINTRESOURCE(IDD_NEW_GAME), hWnd, New_Game_Proc);
				break;
			case IDCANCEL:
				break;
			}
			break;
		case IDS_COLOR_SETTINGS:
			DialogBox(hLib, MAKEINTRESOURCE(IDD_COLOR_SETTINGS), hWnd, Color_Settings_Proc);
			break;
		case IDS_NEW_GAME_H:
			LoadString(hLib, IDS_SAVE_QUESTION, szBuff, 99);
			LoadString(hLib, IDS_MESSAGE, szBuff2, 99);
			a = MessageBox(hWnd, szBuff, szBuff2, MB_YESNOCANCEL | MB_ICONQUESTION);
			switch (a)
			{
			case IDYES:
				DialogBox(hLib, MAKEINTRESOURCE(IDD_SAVE_GAME), hWnd, Save_Game_Proc);
				DialogBox(hLib, MAKEINTRESOURCE(IDD_NEW_GAME), hWnd, New_Game_Proc);
				break;
			case IDNO:
				DialogBox(hLib, MAKEINTRESOURCE(IDD_NEW_GAME), hWnd, New_Game_Proc);
				break;
			case IDCANCEL:
				break;
			}
			break;
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		c = Recieve_settings();
		hBr = CreateSolidBrush(RGB(c[0], c[1], c[2]));
		hBr2 = CreateSolidBrush(RGB(c[3], c[4], c[5]));
		hOldBr = (HBRUSH)SelectObject(hdc, hBr);
		for (int x = 0; x < 8; ++x)
			for (int y = 0; y < 8; ++y)
			{
				if ((x+y)%2==0)
					SelectObject(hdc, hBr);
				else
					SelectObject(hdc, hBr2);
				Rectangle(hdc, CELL*x, CELL*y, CELL*(x + 1), CELL*(y + 1));
			}

		for (int i = 0; i < 32; ++i)
			if (figures[i]!=NULL)
			{
				hBmp = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(figures[i]->getBMP()), IMAGE_BITMAP, 0, 0, LR_COPYFROMRESOURCE);
				SelectObject(hmemdc, hBmp);
				GetObject(hBmp, sizeof(BITMAP), (LPSTR)&bm);
				BitBlt(hdc, CELL * (figures[i]->getPosition().x-1), CELL * (8 - figures[i]->getPosition().y),
					bm.bmWidth, bm.bmHeight, hmemdc, 0, 0, SRCCOPY);
			}
		SelectObject(hdc, hOldBr);
		EndPaint(hWnd, &ps);
		break;
	case WM_LBUTTONDOWN:
		hdc=GetDC(hWnd);
		for (int x = 0; x < 8; ++x)
			for (int y = 0; y < 8; ++y)
			if (LOWORD(lParam) < CELL*(x + 1) && LOWORD(lParam) > CELL*x && HIWORD(lParam) < CELL*(y + 1) && HIWORD(lParam) > CELL*y)
			{
				SetROP2(hdc, R2_XORPEN);
				c = Recieve_settings();
				hPn = CreatePen(PS_DASH, 6, RGB(c[6], c[7], c[8]));
				hOldPn = (HPEN)SelectObject(hdc, hPn);
				hBr = CreateSolidBrush(RGB(0, 0, 0));
				hOldBr = (HBRUSH)SelectObject(hdc, hBr);
				if (!p)
				{
					p = new FOCUS();
					p->lx=-1;
					p->ly = -1;
					p->pFocusedU = NULL;
				}
				if (b) //if figure has been selected. 
				{
					if (p->pFocusedU->generate_steps() != NULL)
					{
						queue = p->pFocusedU->generate_steps();
						for (int i = _msize(queue) / sizeof(queue); i > 0; --i)
						{
							Rectangle(hdc, CELL*(queue[i - 1].x - 1), CELL*(9 - queue[i - 1].y), CELL*(queue[i - 1].x), CELL*(8 - (queue[i - 1].y)));
							if (x + 1 == queue[i - 1].x && (8 - y) == queue[i - 1].y)
							{
								p->pFocusedU->go2(x + 1, 8 - y);
								if (course == WHITE)
									LoadString(hLib, IDS_WHITE_COURSE, szBuff, 149);
								else
									LoadString(hLib, IDS_BLACK_COURSE, szBuff, 149);
								SendMessage(hStatus, SB_SETTEXT, 0, (LONG)szBuff);
								InvalidateRect(hWnd, NULL, TRUE);
								b1 = 1;
								break;
							}
						}
					}
					b = 0;
				}
				if (b1==0)
				p->pFocusedU = chessboard[x][7 - y];
				else
				{
					p->pFocusedU = NULL;
					b1 = 0;
				}
				
				if (p->pFocusedU!=NULL) //lighting for selected unit
				{
					if (p->pFocusedU->getColor() == course)
					{
						b = 1;
						queue = p->pFocusedU->generate_steps();
						if (queue != NULL)
						{
							for (int i = _msize(queue) / sizeof(pos); i > 0; --i)
							{
								Rectangle(hdc, CELL*(queue[i - 1].x - 1), CELL*(9 - queue[i - 1].y), CELL*(queue[i - 1].x), CELL*(8 - (queue[i - 1].y)));
							}
						}
					}					
				}	
				Rectangle(hdc, CELL*p->lx, CELL*p->ly, CELL*(p->lx + 1), CELL*(p->ly + 1));
				p->lx = x;
				p->ly = y;
				Rectangle(hdc, CELL*x, CELL*y, CELL*(x + 1), CELL*(y + 1));
				SelectObject(hdc, hOldBr);
				SelectObject(hdc, hOldPn);
				break;
			}
			ReleaseDC(hWnd, hdc);
		break;
	case WM_DESTROY:
		DeleteDC(hmemdc);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

void InitChessboard(BOOL color)
{
	for (int i = 0; i < 32; ++i)
		delete figures[i];

	for (int x = 0; x < 8; ++x)
	for (int y = 0; y < 8; ++y)
	{
		chessboard[x][y] = NULL;
	}
	course = WHITE;
	Reset_counter();

	int i;
	for (i = 0; i < 8; ++i)
	{
		chessboard[i][1] = new PAWN(color, i + 1, 2);
		figures[i] = chessboard[i][1];
	}
	chessboard[0][0] = new ROOK(color, 1, 1);		figures[i] = chessboard[0][0]; if (color == WHITE)ID_W_ROOK1 = i; else ID_B_ROOK1 = i;
	chessboard[1][0] = new HORSE(color, 2, 1);	++i; figures[i] = chessboard[1][0];
	chessboard[2][0] = new OFFICER(color, 3, 1); ++i; figures[i] = chessboard[2][0];
	chessboard[3][0] = new QUEEN(color, 4, 1);	++i; figures[i] = chessboard[3][0];
	chessboard[4][0] = new KING(color, 5, 1);	++i; figures[i] = chessboard[4][0]; if (color == WHITE) ID_WHITE_KING = i; else ID_BLACK_KING = i;
	chessboard[5][0] = new OFFICER(color, 6, 1); ++i; figures[i] = chessboard[5][0];
	chessboard[6][0] = new HORSE(color, 7, 1);	++i; figures[i] = chessboard[6][0];
	chessboard[7][0] = new ROOK(color, 8, 1);	++i; figures[i] = chessboard[7][0]; if (color == WHITE)ID_W_ROOK2 = i; else ID_B_ROOK2 = i;
	for (int j = 0; j < 8; ++j)
	{
		++i;
		chessboard[j][6] = new PAWN(!color, j + 1, 7);
		figures[i] = chessboard[j][6];
	}
	chessboard[0][7] = new ROOK(!color, 1, 8);	++i; figures[i] = chessboard[0][7];	if (color == WHITE) ID_B_ROOK1 = i; else ID_W_ROOK1 = i;
	chessboard[1][7] = new HORSE(!color, 2, 8);	++i; figures[i] = chessboard[1][7];
	chessboard[2][7] = new OFFICER(!color, 3, 8); ++i; figures[i] = chessboard[2][7];
	chessboard[3][7] = new QUEEN(!color, 4, 8);	++i; figures[i] = chessboard[3][7];
	chessboard[4][7] = new KING(!color, 5, 8);	++i; figures[i] = chessboard[4][7]; if (color == WHITE) ID_BLACK_KING = i; else ID_WHITE_KING = i;
	chessboard[5][7] = new OFFICER(!color, 6, 8); ++i; figures[i] = chessboard[5][7];
	chessboard[6][7] = new HORSE(!color, 7, 8);	++i; figures[i] = chessboard[6][7];
	chessboard[7][7] = new ROOK(!color, 8, 8);	++i; figures[i] = chessboard[7][7]; if(color == WHITE) ID_B_ROOK2 = i; else ID_W_ROOK2 = i;
	InvalidateRect(NULL, NULL, TRUE);
}

void Save_Game(char file[])
{
	fstream f;
	f.open(file, ios_base::out);
	for (int i = 0; i < 32; ++i)
	if (figures[i]!=NULL)
	{
		f << figures[i]->getPosition().x;
		f << figures[i]->getPosition().y;
	}
	else
	{
		f << "nn";
	}
	f << B_ROOK1cstlng << B_ROOK2cstlng << W_ROOK1cstlng << W_ROOK2cstlng << W_SHAH << B_SHAH << course;
	f.close();
}

void Load_Game(char file[])
{
	fstream f;
	char szBuff[72];
	LPSTR s=0;
	f.open(file, ios_base::in);
	f.getline(szBuff, 72);
	UINT x,y,j=0;
	B_ROOK1cstlng = B_ROOK2cstlng = W_ROOK1cstlng = W_ROOK2cstlng = 0;
	InitChessboard(WHITE);
	for (int i = 0; i < 64; i+=2)
	{
		x = szBuff[i] - '0';
		y = szBuff[i+1] - '0';		
		if (x != '62')
			figures[j]->go2(x, y);
		else
		{
			chessboard[figures[j]->getPosition().x-1][figures[j]->getPosition().y-1] = NULL;
			delete figures[j];
		}
		++j;
	}
	B_ROOK1cstlng = szBuff[64]; 
	B_ROOK2cstlng = szBuff[65];
	W_ROOK1cstlng = szBuff[66]; 
	W_ROOK2cstlng = szBuff[67]; 
	W_SHAH = szBuff[68]; 
	B_SHAH = szBuff[69];
	course = szBuff[70];
	f.close();
}

BOOL CALLBACK AboutDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {
	case WM_INITDIALOG:
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam)) 
		{
		case IDC_DONATE:
		case IDOK:
		case IDCANCEL:
			EndDialog(hDlg, 0);
			return TRUE;
		}
		break;
	}
	return FALSE;
}

BOOL CALLBACK Load_Game_Proc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static HWND hStatic1, hCB;
	static LOGFONT lf;
	HFONT hf;
	WIN32_FIND_DATA FindFileData;
	HANDLE hndl;
	char szBuff[100] = { '0/' };

	switch (uMsg) 
	{
	case WM_INITDIALOG:
		hStatic1 = GetDlgItem(hDlg, IDC_LOAD_GAME);
		hCB = GetDlgItem(hDlg, IDC_COMBO);
		hndl = FindFirstFile(".\\*.sg", &FindFileData);
		if (hndl != INVALID_HANDLE_VALUE)
		{
			do{
				SendMessage(hCB, CB_ADDSTRING, NULL, (LPARAM)FindFileData.cFileName);
			} while (FindNextFile(hndl, &FindFileData) != 0);
			FindClose(hndl);
		}
		lf.lfHeight = 18;
		lstrcpy((LPSTR)&lf.lfFaceName, "Verdana");
		hf = CreateFontIndirect(&lf);
		SendMessage(hStatic1, WM_SETFONT, (WPARAM)hf, TRUE);
		return TRUE;
	case WM_CTLCOLORSTATIC:
		if ((HWND)lParam == hStatic1) 
		{
			SetTextColor((HDC)wParam, RGB(0, 100, 180));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (DWORD)GetSysColorBrush(COLOR_3DFACE);
		}
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) 
		{
		case IDLOAD:
			SendMessage(hCB, CB_GETLBTEXT, 0, (LPARAM)szBuff);
			Load_Game(szBuff);
			EndDialog(hDlg, 0);
			return TRUE;
		case IDC_DELETE:
			SendMessage(hCB, CB_GETLBTEXT, 0, (LPARAM)szBuff);
			remove(szBuff);
			break;
		case IDCANCEL:
			EndDialog(hDlg, 0);
			return TRUE;
		}
		break;
	}
	return FALSE;
}

BOOL CALLBACK Save_Game_Proc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static HWND hStatic1,hEdit;
	static LOGFONT lf;
	HFONT hf;
	char szBuff[100] = {'0/'};

	switch (uMsg) {
	case WM_INITDIALOG:
		hStatic1 = GetDlgItem(hDlg, IDC_SAVE_GAME);
		hEdit = GetDlgItem(hDlg, IDC_EDIT);
		SendMessage(hEdit, EM_LIMITTEXT, (WPARAM)99, NULL);
		lf.lfHeight = 18;
		lstrcpy((LPSTR)&lf.lfFaceName, "Verdana");
		hf = CreateFontIndirect(&lf);
		SendMessage(hStatic1, WM_SETFONT, (WPARAM)hf, TRUE);
		return TRUE;
	case WM_CTLCOLORSTATIC:
		if ((HWND)lParam == hStatic1) {
			SetTextColor((HDC)wParam, RGB(0, 100, 180));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (DWORD)GetSysColorBrush(COLOR_3DFACE);
		}
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDSAVE:
			SendMessage(hEdit, EM_GETLINE, (WPARAM)0, (LPARAM)(LPCSTR)szBuff);
			strcat_s(szBuff, ".sg");
			Save_Game(szBuff);
			EndDialog(hDlg, 0);
			return TRUE;
		case IDCANCEL:
			EndDialog(hDlg, 0);
			return TRUE;
		}
		break;
	}
	return FALSE;
}

BOOL CALLBACK New_Game_Proc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDR_BLACk:
			User_color = BLACK;
			return TRUE;
		case IDR_WHITE:
			User_color = WHITE;
			return TRUE;
		case IDOK:
			InitChessboard(User_color);
			EndDialog(hDlg, 0);
			return TRUE;;
		case IDCANCEL:
			EndDialog(hDlg, 0);
			return TRUE;
		}
		break;
	}
	return FALSE;
}

BOOL CALLBACK Figure_Select_Proc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static UINT u = 0;
	switch (uMsg)
	{
	case WM_INITDIALOG:
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDR_QUEEn:
			u = 0;
			return TRUE;
		case IDR_ROOK:
			u = 1;
			return TRUE;
		case IDR_HORSE:
			u = 2;
			return TRUE;
		case IDR_OFFICER:
			u = 3;
			return TRUE;
		case IDOK:
			switch (u)
			{
			case 0:
				figures[lastking->getID()] = new QUEEN(lastking->getColor(), lastking->getPosition().x, (lastking->getPosition().y));
				chessboard[lastking->getPosition().x - 1][lastking->getPosition().y - 1] = figures[lastking->getID()];
				break;
			case 1:
				figures[lastking->getID()] = new ROOK(lastking->getColor(), lastking->getPosition().x, (lastking->getPosition().y));
				chessboard[lastking->getPosition().x - 1][lastking->getPosition().y - 1] = figures[lastking->getID()];
				break;
			case 2:
				figures[lastking->getID()] = new HORSE(lastking->getColor(), lastking->getPosition().x, (lastking->getPosition().y));
				chessboard[lastking->getPosition().x - 1][lastking->getPosition().y - 1] = figures[lastking->getID()];
				break;
			case 3:
				figures[lastking->getID()] = new OFFICER(lastking->getColor(), lastking->getPosition().x, (lastking->getPosition().y));
				chessboard[lastking->getPosition().x - 1][lastking->getPosition().y - 1] = figures[lastking->getID()];
				break;
			}
			EndDialog(hDlg, 0);
			return TRUE;;
		}
		break;
	}
	return FALSE;
}

BOOL CALLBACK Color_Settings_Proc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static HWND Sliders[9];
	UINT s[9];
	switch (uMsg) 
	{
	case WM_INITDIALOG:
		Sliders[0] = GetDlgItem(hDlg, IDS_GREEN_1);
		Sliders[1] = GetDlgItem(hDlg, IDS_RED_1);
		Sliders[2] = GetDlgItem(hDlg, IDS_BLUE_1);
		Sliders[3] = GetDlgItem(hDlg, IDS_GREEN_2);
		Sliders[4] = GetDlgItem(hDlg, IDS_RED_2);
		Sliders[5] = GetDlgItem(hDlg, IDS_BLUE_2);
		Sliders[6] = GetDlgItem(hDlg, IDS_GREEN_3);
		Sliders[7] = GetDlgItem(hDlg, IDS_RED_3);
		Sliders[8] = GetDlgItem(hDlg, IDS_BLUE_3);
		for (int i = 0; i < 9; ++i)
		{
			SendMessage(Sliders[i], TBM_SETRANGE, TRUE, MAKELONG(0,255));
			SendMessage(Sliders[i], TBM_SETTICFREQ, 32, 0);
		}
		LOAD_Settings(Sliders);
		Paint_squares(Sliders, hDlg);
		return TRUE;
	case WM_HSCROLL:
		Paint_squares(Sliders, hDlg);
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam)) 
		{
		case IDOK:
			Save_Settings(Sliders);
			EndDialog(hDlg, 0);
			return TRUE;;
		case IDCANCEL:
			EndDialog(hDlg, 0);
			return TRUE;
		}
		break;
	}
	return FALSE;
}

void Save_Settings(HWND Sliders[])
{
	fstream f;
	f.open("settings.st", ios_base::out);
	for (int i = 0; i < 9; ++i)
	{
		f << SendMessage(Sliders[i], TBM_GETPOS, NULL, NULL) << " ";
	}
	f.close();
	HWND hWnd = FindWindow(szClass, szWindow);
	InvalidateRect(hWnd, NULL, TRUE);
}

void LOAD_Settings(HWND Sliders[])
{
	fstream f;
	f.open("settings.st", ios_base::in);
	if (!f.is_open())
	{
		f.close();
		f.open("settings.st", ios_base::out);
		f << "0 0 0 255 255 255 0 0 0 ";
		f.close();
		f.open("settings.st", ios_base::in);
	}

	UINT i=0,k=0;
	char szBuff[40];
	char szBuff2[4] = { '/0' };
	f.getline(szBuff, 39);
	f.close();
	for (int j = 0; j < 9; ++j)
	{
		k = 0;
		while (szBuff[i] != ' ')
		{
			szBuff2[k] = szBuff[i];
			++i;
			++k;
		}
		++i;
		k = atoi(szBuff2);
		SendMessage(Sliders[j], TBM_SETPOS, TRUE, k);
	}
}

UINT* Recieve_settings()
{
	fstream f;
	f.open("settings.st", ios_base::in);
	UINT i = 0, k = 0;
	UINT* n = new UINT[9];
	char szBuff[40];
	char szBuff2[4] = { '/0' };
	f.getline(szBuff, 39);
	for (int j = 0; j < 9; ++j)
	{
		k = 0;
		while (szBuff[i] != ' ')
		{
			szBuff2[k] = szBuff[i];
			++i;
			++k;
		}
		++i;
		n[j]= atoi(szBuff2);
	}
	return n;
}

void Paint_squares(HWND Sliders[], HWND hWnd)
{
	HBRUSH hBr, hBr2, hOldBr;
	HPEN hPn, hOldPn;
	HDC hdc;
	
	hdc = GetDC(hWnd);
	hBr = CreateSolidBrush(RGB(SendMessage(Sliders[0], TBM_GETPOS, NULL, NULL),
		SendMessage(Sliders[1], TBM_GETPOS, NULL, NULL), SendMessage(Sliders[2], TBM_GETPOS, NULL, NULL)));
	hBr2 = CreateSolidBrush(RGB(SendMessage(Sliders[3], TBM_GETPOS, NULL, NULL),
		SendMessage(Sliders[4], TBM_GETPOS, NULL, NULL), SendMessage(Sliders[5], TBM_GETPOS, NULL, NULL)));
	hOldBr = (HBRUSH)SelectObject(hdc, hBr);
	for (int x = 0; x < 3; ++x)
	for (int y = 0; y < 3; ++y)
	{
		if ((x + y) % 2 == 0)
			SelectObject(hdc, hBr);
		else
			SelectObject(hdc, hBr2);
		Rectangle(hdc, CELL*x + 310, CELL*y + 80, CELL*(x + 1) + 310, CELL*(y + 1) + 80);
	}

	SetROP2(hdc, R2_XORPEN);
	hPn = CreatePen(PS_DASH, 6, RGB(SendMessage(Sliders[6], TBM_GETPOS, NULL, NULL),
		SendMessage(Sliders[7], TBM_GETPOS, NULL, NULL), SendMessage(Sliders[8], TBM_GETPOS, NULL, NULL)));
	hOldPn = (HPEN)SelectObject(hdc, hPn);
	hBr = CreateSolidBrush(RGB(0, 0, 0));
	SelectObject(hdc, hBr);
	Rectangle(hdc, CELL + 310, CELL + 80, CELL*2 + 310, CELL*2 + 80);

	SelectObject(hdc, hOldBr);
	SelectObject(hdc, hOldPn);
	ReleaseDC(hWnd, hdc);
}
