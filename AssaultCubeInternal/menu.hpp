#define WIN32_LEAN_AND_MEAN
#ifndef _MENU_H
#define _MENU_H
#include <windows.h>
#include <string>
#include <cstdint>
#include <vector>
#include "render_base.hpp"

enum
{
	MENUFOLDER,
	MENUITEMTEXT,
	MENUITEM,
	MENUCATEGORY
};



class ListMenuItem
{
public:
	std::string				  m_caption;
	std::uint32_t			  m_type;
	std::uint32_t			* m_valuePtr;
	std::uint32_t			  m_maxValue;
	std::string				* m_menuText;
	std::vector<std::string>	  m_options;
	bool m_critical;
	std::uint32_t m_criticalIdx;

};
class ListMenu
{
public:

	ListMenu()
	{
		m_maxShowItems = 40;
		m_itemPosition = 0;
		m_cursorPosition = 0;
		m_maxItems = 0;
		m_itemWidth = 250.0f;
		m_itemHeight = 18.0f;
		m_visible = 1;
		m_x = 100;
		m_y = 180;
		m_criticalWarningShowed = false;
	}
	~ListMenu()
	{
		m_menuItems.clear(); 
	}

	void RedrawMenu();
	void SetTitle(std::string str);

	void Add(std::string caption, std::vector<std::string> options, std::uint32_t *var, std::uint32_t type = MENUITEM);
	void AddSubItem(std::string caption, std::vector<std::string> options, std::uint32_t *var,std::uint32_t type = MENUITEM);
	void AddRange(std::string caption,float min,float max,float step,int precision , std::uint32_t *var, std::uint32_t type = MENUITEM);
	void AddSubRange(std::string caption, float min, float max, float step, int precision, std::uint32_t *var, std::uint32_t type = MENUITEM);
	void AddRangeCustom(std::string caption, std::vector<std::string> options, float min, float max, float step, int precision, std::uint32_t *var, std::uint32_t type = MENUITEM);
	void AddSubRangeCustom(std::string caption, std::vector<std::string> options, float min, float max, float step, int precision, std::uint32_t *var, std::uint32_t type = MENUITEM);
	void AddCritical(std::string caption, std::vector<std::string> options, std::uint32_t * var, int critIdx = 1);
	void Add(std::string * text);
	void ResetItems();
	void Render(Render::IRenderer * renderer);
	void HandleMenuNavigation(void);
	bool InitializationNeeded();
	void Invalidate();

private:
	bool m_criticalWarningShowed;
	std::string	 m_title;
	bool		 m_visible;
	float		m_itemHeight;
	float		m_itemWidth;
	std::int32_t m_cursorPosition;
	std::int32_t m_x;
	std::int32_t m_y;
	std::int32_t m_ypos;
	std::uint32_t m_maxShowItems;
	std::uint32_t m_itemPosition;
	std::uint32_t m_maxItems;
	std::vector<ListMenuItem> m_menuItems;
};

extern ListMenu g_menu;
#endif