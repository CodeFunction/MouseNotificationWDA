/*
* Copyright 2019 Ryan Briggs
* You may use, copy, modify, or distribute this file however you want.
*/
#pragma once

#include "resource.h"
#include <string>

class Message
{
public:
	Message();
	void displayMsgLPC(LPCWSTR msg);
	void displayMsgString(std::string s);
	void displayMsgChar(char* c);
	void displayMsgByte(CONST BYTE* c);
};