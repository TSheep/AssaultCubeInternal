#include "ogl_renderer.hpp"
#include "opengl_render.hpp"

#pragma warning(disable: 4244)

Render::oglrenderer g_render;

namespace Render
{
	void oglrenderer::DrawRectangleBorder(std::int32_t x, std::int32_t y, std::int32_t width, std::int32_t height, Color borderColor)
	{
		opengl::draw_rect_outline(x, y, width, height, 1.0f, borderColor.ToUInt32());
	}
	void oglrenderer::DrawRectangleBackground(std::int32_t x, std::int32_t y, std::int32_t width, std::int32_t height, Color backgroundColor)
	{
		opengl::fill_rect(x, y, width, height, backgroundColor.ToUInt32());
	}
	void oglrenderer::DrawString(std::int32_t x, std::int32_t y, Color textColor, std::uint32_t align, const char * format, ...)
	{
		va_list vlist;
		va_start(vlist, format);
		char rbuf[1000];
		vsprintf_s<1000>(rbuf, format, vlist);

		if (align == DRAW_LEFT)
			opengl::draw_text(x, y, textColor.ToUInt32(), rbuf);
		else if (align == DRAW_CENTER)
			opengl::draw_text_center(x, y, textColor.ToUInt32(), rbuf);
		else
			opengl::draw_text_right(x, y, textColor.ToUInt32(), rbuf);

		va_end(vlist);
	}
	void oglrenderer::DrawStringRaw(std::int32_t x, std::int32_t y, Color textColor, std::uint32_t align, std::string str)
	{

		if (align == DRAW_LEFT)
			opengl::draw_text(x, y, textColor.ToUInt32(), str.c_str());
		else if (align == DRAW_CENTER)
			opengl::draw_text_center(x, y, textColor.ToUInt32(), str.c_str());
		else
			opengl::draw_text_right(x, y, textColor.ToUInt32(), str.c_str());

	}
}