#pragma once

#include <cstdint>

namespace Render
{
	class Color
	{
	public:
		Color(std::uint32_t color);
		Color(std::uint8_t alpha, std::uint32_t red, std::uint8_t green, std::uint8_t blue);
		std::uint32_t ToUInt32();
	private:
		std::uint32_t m_color;
	};
}