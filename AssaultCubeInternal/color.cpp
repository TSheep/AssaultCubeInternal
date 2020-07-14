#include "color.hpp"

Render::Color::Color(std::uint32_t color)
{
	m_color = color;
}

Render::Color::Color(std::uint8_t alpha, std::uint32_t red, std::uint8_t green, std::uint8_t blue)
{
	m_color = (std::uint32_t) ((((alpha) & 0xff) << 24) | (((red) & 0xff) << 16) | (((green) & 0xff) << 8) | ((blue) & 0xff));
}

std::uint32_t Render::Color::ToUInt32()
{
	return m_color;
}
