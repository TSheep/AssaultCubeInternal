#pragma once

#include <Windows.h>

struct globals
{
	HWND game_window;
	int	 window_width;
	int  window_height;
	HMODULE module;
};

extern globals g_globals;