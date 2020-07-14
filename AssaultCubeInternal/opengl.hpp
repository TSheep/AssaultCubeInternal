#pragma once

#include <Windows.h>

namespace opengl
{
	using OnDraw_t = void(*)();

	bool hook(HMODULE opengl32, HWND wnd, OnDraw_t on_draw);
	void unhook();
}