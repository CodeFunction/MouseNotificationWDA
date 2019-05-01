/*
* Copyright 2019 Ryan Briggs
* You may use, copy, modify, or distribute this file however you want.
*/
#pragma once

#include "stdio.h"
#include "shellapi.h"

#include "resource.h"
#include "Registry.h"
#include <string>

class MainWindow
{
private:
	NOTIFYICONDATA nid;
	Registry registry;

public:
	#define MAX_LOADSTRING			100
	#define WM_MYMESSAGE			10078
	#define ID_TRAY_ICON_GREEN  1000
	#define ID_TRAY_ICON_RED    1001
	#define ID_TRAY_EXIT			1002
	#define WM_SYSICON				(WM_USER + 1)

	MainWindow();
	MainWindow(Registry r);
	HWND getWindowHandle();
	BOOL InitInstance(HINSTANCE hInstance, int nCmdShow);
	LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	void showNotiOnScreen();
	void showNotiOffScreen();
	LPWSTR getTitle();
	LPWSTR getWindowClassObject();
	UINT getWindowClassName();
	HMENU getMenuHandle();
	void setMenuHandle(HMENU mHmenu);
	NOTIFYICONDATA getNotiIconData();
};