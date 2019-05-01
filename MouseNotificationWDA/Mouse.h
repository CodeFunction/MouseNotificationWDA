/*
* Copyright 2019 Ryan Briggs
* You may use, copy, modify, or distribute this file however you want.
*/
#pragma once

#include "resource.h"
#include "MainWindow.h"

class Mouse
{
public:
	Mouse();
	Mouse(MainWindow h);
	void continualMousePos();
};