#pragma once

#include <Windows.h>

namespace opengl
{
	bool hook(HMODULE opengl32);
	void unhook();
}