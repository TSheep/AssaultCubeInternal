#pragma once
#pragma comment(lib, "OpenGL32.lib")

#include <Windows.h>
#include <stdio.h>
#include <gl\GL.h>

#include "colors.hpp"


struct vec3 { float x, y, z; };

namespace opengl
{

	void setup();
	void restore();


	void build_font(HDC dc, HWND wnd);
	void draw_text(float x, float y, DWORD color, const char* format, ...);
	void draw_text_center(float x, float y, DWORD color, const char* format, ...);
	void draw_text_right(float x, float y, DWORD color, const char* format, ...);


	void fill_rect(float x, float y, float width, float height, DWORD color);
	void draw_rect_outline(float x, float y, float width, float height, float lineWidth, DWORD color);

	HDC  get_dc();
}