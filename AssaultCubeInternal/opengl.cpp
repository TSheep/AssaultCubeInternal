#include <Windows.h>
#include <iostream>

#include "opengl.hpp"
#include "hde32.h"
#include "opengl_render.hpp"
#include "globals.hpp"


namespace opengl
{
	using wglSwapBuffers_t		= BOOL(__stdcall*) (HDC hdc);
	using wglGetCurrentDC_t		= HDC(__stdcall*)();
	wglSwapBuffers_t	g_swap_buffers = nullptr;
	wglGetCurrentDC_t	g_wgl_get_current_dc = nullptr;
	uint8_t*			g_original_bytes;
	uintptr_t			g_wgl_swap_buffers = 0;
	uintptr_t			g_trampoline = 0;
	bool				g_is_hooked = false;
	int					g_hooklen = 0;
	HWND				g_game_window = nullptr;
	OnDraw_t			g_draw_cb;

	BOOL __stdcall hkSwapBuffers(HDC hdc)
	{
		auto render_dc = g_wgl_get_current_dc();

		RECT rc;
		GetClientRect(g_globals.game_window, &rc);

		if (render_dc != get_dc() || rc.right != g_globals.window_width || rc.bottom != g_globals.window_height)
		{
			g_globals.window_width = rc.right - rc.left;
			g_globals.window_height = rc.bottom - rc.top;

			build_font(hdc, g_game_window);
		}

		opengl::setup();


		g_draw_cb();

		restore();

		return g_swap_buffers(hdc);
	}

	bool hook(HMODULE opengl32, HWND wnd, OnDraw_t on_draw)
	{
		g_draw_cb = on_draw;
		g_game_window = wnd;
		g_wgl_swap_buffers		= (uintptr_t)GetProcAddress(opengl32, "wglSwapBuffers");
		g_wgl_get_current_dc	= (wglGetCurrentDC_t)GetProcAddress(opengl32, "wglGetCurrentDC");

		printf("wgl_swap_buffers found @ 0x%x\n", g_wgl_swap_buffers);
		printf("g_wgl_get_current_dc found @ 0x%p\n", g_wgl_get_current_dc);

		if (!g_wgl_swap_buffers || !g_wgl_get_current_dc)
			return false;

		do
		{
			hde32s h32;
			g_hooklen += hde32_disasm((void*)(g_wgl_swap_buffers + g_hooklen), &h32);
		} while (g_hooklen < 5);

		printf("hook length: %i\n", g_hooklen);

		g_original_bytes = (uint8_t*)malloc(g_hooklen);

		//apply very simple e9 detours
		//backup original instructions
		memcpy(&g_original_bytes[0], (void*)g_wgl_swap_buffers, g_hooklen);


		g_trampoline = (uintptr_t)VirtualAlloc(nullptr, g_hooklen + 5, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
		if (!g_trampoline)
			return false;

		//build our jump rel =  dst - pos + 5
		int rel32 = (g_wgl_swap_buffers + g_hooklen) - (g_trampoline + g_hooklen) - 0x5;

		printf("trampoline allocated @ 0x%x\n", g_trampoline);

		memcpy((void*)(g_trampoline + 0x00), &g_original_bytes[0], g_hooklen);

		*(BYTE*)(g_trampoline + g_hooklen) = 0xE9;
		*(int*)(g_trampoline + g_hooklen + 1) = rel32;

		g_swap_buffers = (wglSwapBuffers_t)(g_trampoline);

		rel32 = ((uintptr_t)hkSwapBuffers) - g_wgl_swap_buffers - 0x5;

		DWORD old_protection = 0;
		if (VirtualProtect((LPVOID)g_wgl_swap_buffers, g_hooklen, PAGE_EXECUTE_READWRITE, &old_protection))
		{
			uint8_t new_bytes[] = { 0xE9, 0x00, 0x00, 0x00, 0x00 };
			*(int*)(new_bytes + 0x01) = rel32;
			memcpy((void*)g_wgl_swap_buffers, &new_bytes[0], 5);

			VirtualProtect((LPVOID)g_wgl_swap_buffers, g_hooklen, old_protection, &old_protection);
			FlushInstructionCache(GetCurrentProcess(), (void*)g_wgl_swap_buffers, g_hooklen);

			g_is_hooked = true;
			return true;
		}
		return false;
	}

	void unhook()
	{
		if (g_is_hooked)
		{
			DWORD old_protection = 0;
			if (VirtualProtect((LPVOID)g_wgl_swap_buffers, g_hooklen, PAGE_EXECUTE_READWRITE, &old_protection))
			{
				memcpy((void*)g_wgl_swap_buffers, &g_original_bytes[0], g_hooklen);
			
				VirtualProtect((LPVOID)g_wgl_swap_buffers, g_hooklen, old_protection, &old_protection);
				FlushInstructionCache(GetCurrentProcess(), (void*)g_wgl_swap_buffers, g_hooklen);
				Sleep(1000);
				free(g_original_bytes);
				VirtualFree((void*)g_trampoline, 0, MEM_RELEASE);
				g_original_bytes = nullptr;
				g_is_hooked = false;
			}
		}
	}
}