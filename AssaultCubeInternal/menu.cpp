#include "menu.hpp"

#include <time.h>
#include <windows.h>
#include <stdio.h>
#include <sstream>
#include "colors.hpp"
#include <iomanip>

#pragma warning (disable:4244)
#pragma warning (disable:4996)
#pragma warning (disable:4018)

#define MCOLOR_CURRENT		colors::White
#define MCOLOR_FOLDER		colors::Yellow
#define MCOLOR_TEXT			colors::Red
#define MCOLOR_ACTIVE		colors::White
#define MCOLOR_INACTIVE		MAKE_ARGB(255, 153, 153, 153)
#define FORCE_RELOAD if(MENU[Mpos]->typ==MENUFOLDER||MENU[Mpos]->typ==MENUCAT) Mmax=NULL;
BOOL IsKeyPressed(INT vKey_Press, INT vKey_Hold = NULL, BOOL both = FALSE);


void ListMenu::RedrawMenu()
{
	m_maxItems = 0;
}

void ListMenu::SetTitle(std::string str)
{
	m_title = str;
}

void ListMenu::Add(std::string * text)
{
	ListMenuItem newItem;
	newItem.m_menuText = text;
	newItem.m_type = MENUITEMTEXT;
}

void ListMenu::ResetItems()
{
	if (m_menuItems.size())
	{
		auto oldSize = m_menuItems.size();
		m_menuItems.clear();
		m_menuItems.reserve(oldSize);
		return;
	}
	m_menuItems.clear();
}

void ListMenu::Add(std::string caption, std::vector<std::string> options, std::uint32_t *var, std::uint32_t type)
{
	ListMenuItem newItem;
	newItem.m_type = type;
	newItem.m_caption = caption;
	newItem.m_options = options;
	newItem.m_valuePtr = var;
	newItem.m_maxValue = options.size();
	newItem.m_critical = false;

	m_menuItems.push_back(newItem);
	m_maxItems++;
}
void ListMenu::AddRangeCustom(std::string caption, std::vector<std::string> options, float min, float max, float step, int precision, std::uint32_t *var, std::uint32_t type)
{
	std::vector<std::string> generatedOptions = options;

	float currentValue = min;

	while (currentValue <= max)
	{
		std::stringstream stream;
		stream << std::fixed << std::setprecision(precision) << currentValue;
		generatedOptions.push_back(stream.str());
		currentValue = currentValue + step;
	}
	//std::stringstream stream;
	//stream << std::fixed << std::setprecision(precision) << currentValue;
	//generatedOptions.push_back(stream.str());
	Add(caption, generatedOptions, var, type);
}
void ListMenu::AddSubRangeCustom(std::string caption, std::vector<std::string> options, float min, float max, float step, int precision, std::uint32_t *var, std::uint32_t type)
{
	AddRangeCustom("  - " + caption, options, min, max, step, precision, var, type);
}
void ListMenu::AddSubItem(std::string caption, std::vector<std::string> options, std::uint32_t * var, std::uint32_t type)
{
	Add("  - " + caption, options, var, type);
}
void ListMenu::AddSubRange(std::string caption, float min, float max, float step, int precision, std::uint32_t * var, std::uint32_t type)
{
	AddRange("  - " + caption, min,max,step,precision,var,type);
}

void ListMenu::AddRange(std::string caption, float min, float max, float step, int precision, std::uint32_t * var, std::uint32_t type)
{
	std::vector<std::string> generatedOptions;
	
	float currentValue = min;
	
	while (currentValue <= max)
	{
		std::stringstream stream;
		stream  << std::fixed << std::setprecision(precision) << currentValue;
		generatedOptions.push_back(stream.str());
		currentValue = currentValue + step;
	}
	std::stringstream stream;
	stream << std::fixed << std::setprecision(precision) << currentValue;
	generatedOptions.push_back(stream.str());
	Add(caption, generatedOptions, var, type);
}

void ListMenu::AddCritical(std::string caption, std::vector<std::string> options, std::uint32_t * var, int critIdx)
{
	ListMenuItem newItem;
	newItem.m_type = MENUITEM;
	newItem.m_caption = caption;
	newItem.m_options = options;
	newItem.m_valuePtr = var;
	newItem.m_maxValue = options.size();
	newItem.m_critical = true;
	newItem.m_criticalIdx = critIdx;
	m_menuItems.push_back(newItem);
	m_maxItems++;
}

void ListMenu::Render(Render::IRenderer * renderer)
{
	if (m_visible)
	{
		auto x = m_x;
		auto y = m_y;

		renderer->DrawRectangle(x - 1, y - 31, m_itemWidth + 3 + 2, 39, MAKE_ARGB(255, 22, 160, 133), MAKE_ARGB(175, 26, 26, 26));


		if (m_maxItems + 1 > m_maxShowItems)
		{
			renderer->DrawRectangle(x - 1, y + 8, m_itemWidth + 5, m_itemHeight * m_maxShowItems + 20, MAKE_ARGB(255, 22, 160, 133), MAKE_ARGB(175, 26, 26, 26));
		}
		else
		{
			renderer->DrawRectangle(x - 1, y + 8, m_itemWidth + 5, m_itemHeight * m_maxItems + 20, MAKE_ARGB(255, 22, 160, 133), MAKE_ARGB(175, 26, 26, 26));
		}

		renderer->DrawString(x - 1 + (m_itemWidth + 5) / 2, y - 26, colors::White, Render::DRAW_CENTER, m_title.c_str());

		for (int i = m_itemPosition; i < m_maxItems; i++)
		{
			if (m_cursorPosition >(m_itemPosition + (m_maxShowItems - 4)) && m_itemPosition != (m_maxItems - m_maxShowItems))
				m_itemPosition++;

			if ((m_cursorPosition < (m_itemPosition + 3)) && m_itemPosition != 0)
				m_itemPosition--;

			if (i < (m_itemPosition + m_maxShowItems))
			{
				Render::Color drawColor = MCOLOR_CURRENT;

				if (i == m_cursorPosition && m_menuItems[i].m_type != MENUFOLDER)
				{
					renderer->DrawRectangleBackground(x, y + 10, m_itemWidth + 3, m_itemHeight, MAKE_ARGB(255, 70, 70, 70));
				}
				if (m_menuItems[i].m_type == MENUFOLDER)
				{
					drawColor = MCOLOR_FOLDER;

					renderer->DrawRectangleBackground(x + 1, y + 10, m_itemWidth + 3, m_itemHeight, MAKE_ARGB(255, 40, 40, 40));
					renderer->DrawString(x + 8, y + 10, i == m_cursorPosition ? MAKE_ARGB(255, 255, 000, 000) : drawColor, Render::DRAW_LEFT, m_menuItems[i].m_caption.c_str());
					renderer->DrawString(x + m_itemWidth - 28 - 15 + 40, y + 10, i == m_cursorPosition ? MAKE_ARGB(255, 255, 000, 000) : drawColor, Render::DRAW_RIGHT, m_menuItems[i].m_options[(*m_menuItems[i].m_valuePtr)].c_str());

					//g_draw.DrawRect(x + 1, y + 10, (float)m_itemWidth + 3, (float)m_itemHeight, MAKE_ARGB(255, 40, 40, 40));
					/*
					g_draw.DrawF(x + 8, y + 10, i == m_cursorPosition ? MAKE_ARGB(255, 255, 000, 000) : dwDrawColor, g_menuFont,DRAW_LEFT,m_menuItems[i].txt);
					g_draw.DrawF(x + m_itemWidth - 28 - 15 + 40, y + 10, i == m_cursorPosition ? MAKE_ARGB(255, 255, 000, 000) : dwDrawColor,g_menuFont,DRAW_RIGHT, m_menuItems[i].opt[(*m_menuItems[i].var)]);
					*/
				}
				else if (m_menuItems[i].m_type == MENUITEMTEXT)
				{
					renderer->DrawString(x + 20 - 15, y + 10, i == m_cursorPosition ? MAKE_ARGB(255, 255, 000, 000) : drawColor, Render::DRAW_LEFT, (char*) m_menuItems[i].m_menuText->c_str());
					//g_draw.DrawF(x + 20 - 15, y + 10, i == m_cursorPosition ? MAKE_ARGB(255, 255, 000, 000) : dwDrawColor,g_menuFont,DRAW_LEFT, (char*)m_menuItems[i].text->c_str());
				}
				else
				{
					if ((*m_menuItems[i].m_valuePtr) > 0)
						drawColor = MCOLOR_ACTIVE;
					else
						drawColor = MCOLOR_INACTIVE;


					renderer->DrawString(x + 18, y + 10, i == m_cursorPosition ? MAKE_ARGB(255, 255, 000, 000) : drawColor, Render::DRAW_LEFT, m_menuItems[i].m_caption.c_str());
					renderer->DrawStringRaw(x + m_itemWidth - 18 - 15 + 25, y + 10, i == m_cursorPosition ? MAKE_ARGB(255, 255, 000, 000) : drawColor, Render::DRAW_RIGHT, m_menuItems[i].m_options[(*m_menuItems[i].m_valuePtr)].c_str());

					//g_draw.DrawF(x + 18, y + 10, i == m_cursorPosition ? MAKE_ARGB(255, 255, 000, 000) : dwDrawColor, g_menuFont,DRAW_LEFT,m_menuItems[i].txt);
					//g_draw.DrawF(x + m_itemWidth - 18 - 15 + 25, y + 10, i == m_cursorPosition ? MAKE_ARGB(255, 255, 000, 000) : dwDrawColor,g_menuFont,DRAW_RIGHT, m_menuItems[i].opt[(*m_menuItems[i].var)]);
				}

			}
			y += m_itemHeight;
		}
	}
}

void ListMenu::HandleMenuNavigation(void)
{
	if (GetAsyncKeyState(VK_INSERT) & 1)
		m_visible = !m_visible;

	if (!m_visible)
		return;

	if (GetAsyncKeyState(VK_UP) & 1) {
		do {
			m_cursorPosition--;
			if (m_cursorPosition < 0)  m_cursorPosition = m_maxItems - 1;
		} while (m_menuItems[m_cursorPosition].m_type == MENUITEMTEXT);
	}
	else if (GetAsyncKeyState(VK_DOWN) & 1) {
		do {
			m_cursorPosition++;
			if (m_cursorPosition == m_maxItems) m_cursorPosition = 0;
		} while (m_menuItems[m_cursorPosition].m_type == MENUITEMTEXT);
	}
	else if (m_menuItems[m_cursorPosition].m_valuePtr) {
		int dir = 0;

		if (GetAsyncKeyState(VK_LEFT) & 1 && *m_menuItems[m_cursorPosition].m_valuePtr > 0) dir = -1;
		if (GetAsyncKeyState(VK_RIGHT) & 1 && *m_menuItems[m_cursorPosition].m_valuePtr < (m_menuItems[m_cursorPosition].m_maxValue - 1)) 
			dir = 1;
		if (dir) {
			if (!m_criticalWarningShowed && m_menuItems[m_cursorPosition].m_critical && ((*m_menuItems[m_cursorPosition].m_valuePtr) + dir) >= m_menuItems[m_cursorPosition].m_criticalIdx) {
				char warningMessage[500];
				sprintf(warningMessage, "You are about to enable the Item %s - Option %s (%i)\nThis feature is classified as risky!\n"
										"Do you still wan't to enable it and dismiss future warnings about risky features?",
										m_menuItems[m_cursorPosition].m_caption.c_str(),
										m_menuItems[m_cursorPosition].m_options[((*m_menuItems[m_cursorPosition].m_valuePtr) + dir)].c_str(),
										(*m_menuItems[m_cursorPosition].m_valuePtr) + dir);
				
				/*if (MessageBoxA(0, warningMessage, "Caution", MB_ICONWARNING | MB_YESNO | MB_TOPMOST | MB_SETFOREGROUND | MB_SERVICE_NOTIFICATION) == IDYES)
				{
					m_criticalWarningShowed = true;
				}
				else
				{
					return;
				}*/
			}
			*m_menuItems[m_cursorPosition].m_valuePtr += dir;
			if (m_menuItems[m_cursorPosition].m_type == MENUFOLDER)	m_maxItems = 0;
			if (m_menuItems[m_cursorPosition].m_type == MENUCATEGORY)	m_maxItems = 0;
		}
	}
}

bool ListMenu::InitializationNeeded()
{
	return m_maxItems == 0;
}

void ListMenu::Invalidate()
{
	m_maxItems = 0;
}

BOOL IsKeyPressed(INT vKey_Press, INT vKey_Hold, BOOL both)
{
	if (both == FALSE)
	{
		if (vKey_Hold == NULL)
		{
			if (GetAsyncKeyState(vKey_Press) & 1)
				return TRUE;
		}

		if (vKey_Press == NULL)
		{
			if (GetAsyncKeyState(vKey_Hold))
				return TRUE;
		}

		if (vKey_Hold != NULL && vKey_Press != NULL)
		{
			if ((GetAsyncKeyState(vKey_Hold)) && (GetAsyncKeyState(vKey_Press) & 1))
				return TRUE;
		}
	}
	else
	{
		if ((GetAsyncKeyState(vKey_Hold)) || (GetAsyncKeyState(vKey_Press) & 1))
			return TRUE;
	}
	return FALSE;
}
