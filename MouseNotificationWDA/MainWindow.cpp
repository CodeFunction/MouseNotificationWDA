/*
* Copyright 2019 Ryan Briggs
* You may use, copy, modify, or distribute this file however you want.
*/
#include "stdafx.h"
#include "MainWindow.h"
#include <thread>

HWND mwHwnd;									// Need global to update noti
HINSTANCE hInst;                                // current applcation instance handle
WCHAR titleText[MAX_LOADSTRING];                // The title bar text
WCHAR windowClassName[MAX_LOADSTRING];          // the main window class name
HMENU hmenu;									// shortcut menu handle

PAINTSTRUCT ps;									// Painting
HDC hDC;

bool allowRemoveNoti = false;					// Don't allow until mouse first travels off screen

MainWindow::MainWindow()
{
	nid.cbSize = -1; // Get rid of warning
}

MainWindow::MainWindow(Registry r) 
{
	registry = r;
	nid.cbSize = -1;
}

HWND MainWindow::getWindowHandle()
{
	return mwHwnd;
}

LPWSTR MainWindow::getTitle()
{
	return titleText;
}

LPWSTR MainWindow::getWindowClassObject()
{
	return windowClassName;
}

UINT MainWindow::getWindowClassName()
{
	return windowClassName[MAX_LOADSTRING];
}

HMENU MainWindow::getMenuHandle()
{
	return hmenu;
}

void MainWindow::setMenuHandle(HMENU mHmenu)
{
	hmenu = mHmenu;
}

NOTIFYICONDATA MainWindow::getNotiIconData()
{
	return nid;
}

BOOL MainWindow::InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance;

	HWND hWnd = CreateWindowW(windowClassName, titleText, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	mwHwnd = hWnd;

	if (!hWnd) return FALSE;

	return TRUE;
}

LRESULT CALLBACK MainWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_COMMAND:
		{
			int wmId = LOWORD(wParam);
			// Parse the menu selections:
			switch (wmId)
			{
				case IDM_ABOUT:
				break;
				case IDM_EXIT:
				{
					DestroyWindow(hWnd);
				}
				break;
				default:
				return DefWindowProc(hWnd, message, wParam, lParam);
			}
		}
		break;
		case WM_PAINT:
		{
			hDC = BeginPaint(hWnd, &ps);
			EndPaint(hWnd, &ps);
		}
		break;
		case WM_DESTROY: 
		{
			PostQuitMessage(0);
		}
		break;
		default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

void MainWindow::showNotiOnScreen()
{
	bool result = false;
	if (allowRemoveNoti) result = Shell_NotifyIcon(NIM_DELETE, &nid);

	nid = {};
	nid.cbSize = sizeof(NOTIFYICONDATA);
	nid.hWnd = getWindowHandle();
	nid.uID = ID_TRAY_ICON_GREEN;
	nid.uVersion = NOTIFYICON_VERSION;
	nid.uCallbackMessage = WM_SYSICON;
	nid.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON2));
	wcscpy_s(nid.szTip, L"Mouse pointer notification");
	nid.uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE;
	Shell_NotifyIcon(NIM_ADD, &nid);
	DestroyIcon(nid.hIcon);
	allowRemoveNoti = true;
}

void MainWindow::showNotiOffScreen()
{
	bool result = false;
	if (allowRemoveNoti) result = Shell_NotifyIcon(NIM_DELETE, &nid);

	if (!result) // If not destroyed, get green again, then destroy
	{
		showNotiOnScreen();
		Shell_NotifyIcon(NIM_DELETE, &nid);
	}

	nid = {};
	nid.cbSize = sizeof(NOTIFYICONDATA);
	nid.hWnd = getWindowHandle();
	nid.uID = ID_TRAY_ICON_RED;
	nid.uVersion = NOTIFYICON_VERSION;
	nid.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON3));
	wcscpy_s(nid.szTip, L"Mouse pointer notification");
	nid.uFlags = NIF_ICON | NIF_TIP | NIF_GUID;
	Shell_NotifyIcon(NIM_ADD, &nid);
	DestroyIcon(nid.hIcon);
	allowRemoveNoti = true;
}