/*
* Copyright 2019 Ryan Briggs
* You may use, copy, modify, or distribute this file however you want.
*/

// MouseNotificationWDA.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "MouseNotificationWDA.h"
#include "Registry.h"
#include "Mouse.h"
#include "MainWindow.h"
#include "shellapi.h"

#include <thread>
#include <string>

Message m;
Registry registry = Registry(m);
MainWindow mw = MainWindow(registry);
Mouse mouse = Mouse(mw);

ATOM MyRegisterClass(HINSTANCE hInstance);
bool isOneApplicationInstance();
LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
void popupMenu(HWND hwnd);
void popupMenuClick(HWND hwnd);
POINT getMenuPosition();
void quitApplication();

MouseNotificationWDA::MouseNotificationWDA() {}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

	if (!isOneApplicationInstance()) return FALSE;

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, mw.getTitle(), MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MOUSENOTIFICATIONWDA, mw.getWindowClassObject(), MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!mw.InitInstance (hInstance, nCmdShow)) return FALSE;

	registry.runWithWinStartup();
	mouse.continualMousePos();
	mw.showNotiOnScreen();

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MOUSENOTIFICATIONWDA));
    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}

bool isOneApplicationInstance()
{
	HANDLE handle = CreateEventW(NULL, FALSE, FALSE, appNameWs);

	if (handle == NULL) return false;

	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		CloseHandle(handle);
		handle = NULL;
		return false;
	}
	
	return true;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style          = CS_DBLCLKS;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
	wcex.lpfnWndProc    = WindowProcedure;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APP));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MOUSENOTIFICATIONWDA);
    wcex.lpszClassName  = mw.getWindowClassObject();
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APP));

    return RegisterClassExW(&wcex);
}

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	if (message == mw.getWindowClassName() && IsWindowVisible(hwnd)) return 0;

	switch (message)
	{
		case WM_CREATE:
		{
			popupMenu(hwnd);
		}
		break;
		case WM_SYSICON:
		{
			if (wParam == ID_TRAY_ICON_GREEN) SetForegroundWindow(hwnd);
			if (lParam == WM_RBUTTONDOWN) popupMenuClick(hwnd);
		}
		break;
		default:
		break;
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}

void popupMenu(HWND hwnd)
{
	ShowWindow(hwnd, SW_HIDE);
	mw.setMenuHandle(CreatePopupMenu());
	AppendMenu(mw.getMenuHandle(), MF_STRING, ID_TRAY_EXIT, TEXT("Clean Exit"));
}

void popupMenuClick(HWND hwnd)
{
	SetForegroundWindow(hwnd);

	POINT point = getMenuPosition();
	UINT flags = TPM_RETURNCMD | TPM_NONOTIFY;
	UINT clicked = TrackPopupMenu(mw.getMenuHandle(), flags, point.x, point.y, 0, hwnd, NULL);

	switch (clicked)
	{
		case (ID_TRAY_EXIT) :
		{
			quitApplication();
		}
		break;
		default:
		break;
	}
}

POINT getMenuPosition()
{
	POINT point;
	GetCursorPos(&point);
	return point;
}

void quitApplication()
{
	Shell_NotifyIcon(NIM_DELETE, &mw.getNotiIconData());
	registry.removeStartupAndExit();
	PostQuitMessage(0);
}
