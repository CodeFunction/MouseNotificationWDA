/*
* Copyright 2019 Ryan Briggs
* You may use, copy, modify, or distribute this file however you want.
*/
#include "stdafx.h"
#include "Message.h"
#include "stdio.h"
#include "shellapi.h"

#include <thread>
#include <string>

using String = std::string;
using WString = std::wstring;

Message::Message() {}

void Message::displayMsgLPC(LPCWSTR msg)
{
	MessageBox(NULL, msg, _T("Message Box"), MB_OK | MB_SYSTEMMODAL);
}

void Message::displayMsgString(std::string s)
{
	// Convert from string to LPCWSTR then display
	
	WString ws(s.begin(), s.end());
	LPCWSTR msg = ws.c_str();
	MessageBox(NULL, msg, _T("Message Box"), MB_OK | MB_SYSTEMMODAL);
}

void Message::displayMsgChar(char* c)
{
	// Convert from char* to LPCWSTR then display
	String str(c);
	WString ws(str.begin(), str.end());
	LPCWSTR msg = ws.c_str();
	MessageBox(NULL, msg, _T("Message Box"), MB_OK | MB_SYSTEMMODAL);
}

void Message::displayMsgByte(CONST BYTE* c)
{
	// Convert from char* to LPCWSTR then display
	String s(reinterpret_cast< char const* >(c));
	WString ws(s.begin(), s.end());
	LPCWSTR msg = ws.c_str();
	MessageBox(NULL, msg, _T("Message Box"), MB_OK | MB_SYSTEMMODAL);
}