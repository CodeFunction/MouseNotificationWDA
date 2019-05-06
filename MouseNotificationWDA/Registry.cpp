/*
* Copyright 2019 Ryan Briggs
* You may use, copy, modify, or distribute this file however you want.
*/
#include "stdafx.h"
#include "Registry.h"
#include "stdio.h"
#include "shellapi.h"
#include "MouseNotificationWDA.h"

#include <thread>
#include <string>

using String = std::string;
using WString = std::wstring;

Message message;
const LPCWSTR regPath = L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run";
const String regPathS = "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run";

WString createFullPathWithFile(String filename);
String getCurrentPathOfApplication();
String addFilenameToPathOfApplication(String filename);
WString stringToWString(String s);
String wstringToString(WString ws);
bool validatePath(String s);

Registry::Registry() {}

Registry::Registry(Message m)
{
	message = m;
}

void Registry::runWithWinStartup()
{
	HKEY hKey;
	const String filename = appNameS + ".exe";
	WString runFile = createFullPathWithFile(filename);

	if (!validatePath(wstringToString(runFile))) return;

	// Add to registry for run on Windows startup
	LONG lnRes = RegOpenKeyExW(HKEY_CURRENT_USER,
		regPath,
		0, KEY_WRITE,
		&hKey);
	if (ERROR_SUCCESS == lnRes)
	{
		lnRes = RegSetValueExW(hKey,
			appNameWs,
			0,
			REG_SZ,
			// Problem: If written to registry in Asian characters then there's a unicode problem
			// https://stackoverflow.com/questions/16697799/regsetvalueex-function-writing-gibberish
			// Fix: use RegSetValueExW instead of RegSetValueEx, use wstring instead of const char*, etc.
			(const BYTE*)runFile.c_str(),
			(runFile.length() + 1) * sizeof(wchar_t));
	}

	RegCloseKey(hKey);
}

WString createFullPathWithFile(String filename)
{
	String s = getCurrentPathOfApplication();
	s += addFilenameToPathOfApplication(filename);
	WString ws = stringToWString(s);
	return ws;
}

bool validatePath(String s)
{
	String fixSystemProb = "\\WINDOWS\\system32\\"; // After two reboots this is the incorrect path it has
	if (s.find(fixSystemProb) != std::string::npos) return false;
	return true;
}

String getCurrentPathOfApplication()
{
	// Dynamically get path of application's current location
	char path[MAX_PATH];
	GetCurrentDirectoryA(MAX_PATH, path);
	return path;
}

String addFilenameToPathOfApplication(String filename)
{
	String s = "\\";
	s = s + filename;
	return s;
}

WString stringToWString(String s)
{
	WString ws(s.begin(), s.end());
	return ws;
}

String wstringToString(WString ws)
{
	String s(ws.begin(), ws.end());
	return s;
}

void Registry::removeStartupAndExit()
{
	HKEY hKey;

	// Open
	LONG lnRes = RegOpenKeyExW(HKEY_CURRENT_USER, regPath, 0, KEY_ALL_ACCESS, &hKey);

	if (ERROR_SUCCESS == lnRes)
	{
		// Delete value that auto starts the application on Windows startup
		lnRes = RegDeleteValue(hKey, appNameWs);

		if (ERROR_SUCCESS != lnRes)
		{
			String s = "Failed: Please remove regedit HKEY_LOCAL_MACHINE\\" + regPathS + "\\";
			s += appNameS;
			message.displayMsgString(s);
		}
	}

	RegCloseKey(hKey);
}