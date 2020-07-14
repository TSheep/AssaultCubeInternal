#include "opengl_render.hpp"
#include "globals.hpp"

namespace opengl
{
	constexpr int	font_height = 14;
	unsigned int	g_text_base;
	HDC				g_hdc = nullptr;
	HWND			g_window;
	TEXTMETRICA		g_font_metrics;

	struct colconv
	{
		union
		{
			struct
			{
				uint8_t b;
				uint8_t g;
				uint8_t r;
				uint8_t a;
			};
			DWORD color;
		};
	};

	HDC get_dc()
	{
		return g_hdc;
	}
	void setup()
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		glPushMatrix();
		GLint viewport[4];
		glGetIntegerv(GL_VIEWPORT, viewport);
		glViewport(0, 0, viewport[2], viewport[3]);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, viewport[2], viewport[3], 0, -1, 1);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glDisable(GL_DEPTH_TEST);
	}

	void restore()
	{
		glPopMatrix();
		glPopAttrib();
	}

	void fill_rect(float x, float y, float width, float height, DWORD color)
	{
		colconv cv;
		cv.color = color;

		glColor4ub(cv.r, cv.g, cv.b, cv.a);
		glBegin(GL_QUADS);
		glVertex2f(x, y);
		glVertex2f(x + width, y);
		glVertex2f(x + width, y + height);
		glVertex2f(x, y + height);
		glEnd();
	}

	void draw_rect_outline(float x, float y, float width, float height, float lineWidth, DWORD color)
	{
		colconv cv;
		cv.color = color;

		glLineWidth(lineWidth);
		glBegin(GL_LINE_STRIP);
		glColor4ub(cv.r, cv.g, cv.b, cv.a);
		glVertex2f(x - 0.5f, y - 0.5f);
		glVertex2f(x + width + 0.5f, y - 0.5f);
		glVertex2f(x + width + 0.5f, y + height + 0.5f);
		glVertex2f(x - 0.5f, y + height + 0.5f);
		glVertex2f(x - 0.5f, y - 0.5f);
		glEnd();
	}


	void build_font(HDC dc, HWND window)
	{
		g_hdc = dc;
		g_text_base = glGenLists(96);
		HFONT hFont = CreateFontA(-font_height, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, FF_DONTCARE | DEFAULT_PITCH, "Verdana");
		HFONT hOldFont = (HFONT)SelectObject(g_hdc, hFont);

		GetTextMetrics(g_hdc, &g_font_metrics);

		wglUseFontBitmaps(g_hdc, 32, 96, g_text_base);
		SelectObject(g_hdc, hOldFont);
		DeleteObject(hFont);

	}

	void draw_text(float x, float y, DWORD color, const char* format, ...)
	{
		colconv cv;
		cv.color = color;
		y += (g_font_metrics.tmHeight / 1.5f);

		glColor4ub(cv.r, cv.g, cv.b, cv.a);
		glRasterPos2f(x, y);

		char text[100];
		va_list args;

		va_start(args, format);
		vsprintf_s(text, 100, format, args);
		va_end(args);

		glPushAttrib(GL_LIST_BIT);
		glListBase(g_text_base - 32);
		glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);
		glPopAttrib();
	}

	void draw_text_right(float x, float y, DWORD color, const char* format, ...)
	{
		char text[100];
		va_list args;

		va_start(args, format);
		vsprintf_s(text, 100, format, args);
		va_end(args);

		x -= ((strlen(text) * g_font_metrics.tmAveCharWidth));
		y += (g_font_metrics.tmHeight / 1.5f);

		colconv cv;
		cv.color = color;

		glColor4ub(cv.r, cv.g, cv.b, cv.a);
		glRasterPos2f(x, y);


		glPushAttrib(GL_LIST_BIT);
		glListBase(g_text_base - 32);
		glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);
		glPopAttrib();
	}
	void draw_text_center(float x, float y, DWORD color, const char* format, ...)
	{
		char text[100];
		va_list args;

		va_start(args, format);
		vsprintf_s(text, 100, format, args);
		va_end(args);

		x -= ((strlen(text) * g_font_metrics.tmAveCharWidth) / 2);
		y += (g_font_metrics.tmHeight / 1.5f);

		colconv cv;
		cv.color = color;

		glColor4ub(cv.r, cv.g, cv.b, cv.a);
		glRasterPos2f(x, y);


		glPushAttrib(GL_LIST_BIT);
		glListBase(g_text_base - 32);
		glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);
		glPopAttrib();
	}
}