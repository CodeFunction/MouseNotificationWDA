/*
* Copyright 2019 Ryan Briggs
* You may use, copy, modify, or distribute this file however you want.
*/
#include "stdafx.h"
#include "Mouse.h"
#include "stdio.h"
#include "shellapi.h"

#include <thread>
#include <string>

MainWindow mainWindow;

void findPos();

Mouse::Mouse() {}

Mouse::Mouse(MainWindow mw)
{
	mainWindow = mw;
}

void Mouse::continualMousePos()
{
	std::thread t1(findPos);
	t1.detach();
}

void findPos()
{
	// Here we get the mouse poition and show if green if one main screen or red if off main screen.
	// This is part of a loop so we want to do least amount of work possible.
	POINT p;
	long x;
	long y;
	bool change = false;
	bool last = false;

	// Get main display res
	RECT desktop;
	HWND hDesktop = GetDesktopWindow();
	GetWindowRect(hDesktop, &desktop);
	long resWidth = desktop.right;
	std::wstring dW = std::to_wstring(resWidth);

	while (true) // Forever loop
	{
		if (GetCursorPos(&p))
		{
			// Mouse positions
			x = p.x;
			y = p.y;

			// If mouse if to left of (0) or to right of (resWidth) then its on other display (red)
			change = x < 0 || x > resWidth;

			if (change != last)
			{
				last = change;
				if (change)
				{
					mainWindow.showNotiOffScreen(); // red
				}
				else
				{
					mainWindow.showNotiOnScreen(); // green
				}
			}
		}

		Sleep(1000);
	}

}