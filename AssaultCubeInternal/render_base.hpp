#pragma once

#include "color.hpp"
#include <string>

namespace Render
{
	enum
	{
		DRAW_LEFT,
		DRAW_CENTER,
		DRAW_RIGHT
	};
	class IRenderer
	{
	public:
		virtual void DrawRectangle(std::int32_t x, std::int32_t y, std::int32_t width, std::int32_t height, Color borderColor, Color backgroundColor)
		{
			DrawRectangleBorder(x, y, width, height, borderColor);
			DrawRectangleBackground(x + 1, y + 1, width - 1, height - 1, backgroundColor);
		}
		virtual void DrawRectangleBorder(std::int32_t x, std::int32_t y, std::int32_t width, std::int32_t height, Color borderColor) = 0;
		virtual void DrawRectangleBackground(std::int32_t x, std::int32_t y, std::int32_t width, std::int32_t height, Color backgroundColor) = 0;
		virtual void DrawString(std::int32_t x, std::int32_t y, Color textColor, std::uint32_t align, const char * format, ...) = 0;
		virtual void DrawStringRaw(std::int32_t x, std::int32_t y, Color textColor, std::uint32_t align,std::string str) = 0;
	};
}
