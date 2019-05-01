/*
* Copyright 2019 Ryan Briggs
* You may use, copy, modify, or distribute this file however you want.
*/
#pragma once

#include "resource.h"
#include "Message.h"

class Registry
{
public:
	Registry();
	Registry(Message m);
	void runWithWinStartup();
	void removeStartupAndExit();
};