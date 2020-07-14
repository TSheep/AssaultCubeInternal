#pragma once

#include "color.hpp"
#include <string>
#include "render_base.hpp"
#include "opengl_render.hpp"

namespace Render
{
	class oglrenderer : public IRenderer
	{
	public:
		void DrawRectangleBorder(std::int32_t x, std::int32_t y, std::int32_t width, std::int32_t height, Color borderColor);
		void DrawRectangleBackground(std::int32_t x, std::int32_t y, std::int32_t width, std::int32_t height, Color backgroundColor);
		void DrawString(std::int32_t x, std::int32_t y, Color textColor, std::uint32_t align, const char * format, ...);
		void DrawStringRaw(std::int32_t x, std::int32_t y, Color textColor, std::uint32_t align, std::string str);
	};
}

extern Render::oglrenderer g_render;