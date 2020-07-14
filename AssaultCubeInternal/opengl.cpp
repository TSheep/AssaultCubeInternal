#include <Windows.h>
#include <iostream>

namespace opengl
{
	using wglSwapBuffers_t = BOOL(__stdcall*) (HDC hdc);

	wglSwapBuffers_t	g_swap_buffers = nullptr;
	uint8_t				g_original_bytes[5];
	uintptr_t			g_wgl_swap_buffers = 0;
	uintptr_t			g_trampoline = 0;
	bool				g_is_hooked = false;

	BOOL __stdcall hkSwapBuffers(HDC hdc)
	{
		printf("opengl.render called\n");

		return g_swap_buffers(hdc);
	}

	bool hook(HMODULE opengl32)
	{
		g_wgl_swap_buffers = (uintptr_t)GetProcAddress(opengl32, "wglSwapBuffers");
		printf("wgl_swap_buffers found @ 0x%p\n", g_wgl_swap_buffers);

		if (!g_wgl_swap_buffers)
			return false;

		printf("press enter to apply hook\n");


		//apply very simple e9 detours
		//backup original instructions
		memcpy(&g_original_bytes[0], (void*)g_wgl_swap_buffers, sizeof(g_original_bytes));


		g_trampoline = (uintptr_t)VirtualAlloc(nullptr, 5 + 5, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
		if (!g_trampoline)
			return false;

		//build our jump rel =  dst - pos + 5
		int rel32 = (g_wgl_swap_buffers + 0x5) - (g_trampoline + 0x5) - 0x5;

		printf("trampoline allocated @ 0x%p", g_trampoline);

		memcpy((void*)(g_trampoline + 0x00), &g_original_bytes[0], sizeof(g_original_bytes));

		*(BYTE*)(g_trampoline + 0x5) = 0xE9;
		*(int*)(g_trampoline + 0x6) = rel32;

		g_swap_buffers = (wglSwapBuffers_t)(g_trampoline);

		rel32 = ((uintptr_t)hkSwapBuffers) - g_wgl_swap_buffers - 0x5;

		DWORD old_protection = 0;
		if (VirtualProtect((LPVOID)g_wgl_swap_buffers, 0x05, PAGE_EXECUTE_READWRITE, &old_protection))
		{
			uint8_t new_bytes[] = { 0xE9, 0x00, 0x00, 0x00, 0x00 };
			*(int*)(new_bytes + 0x01) = rel32;
			memcpy((void*)g_wgl_swap_buffers, &new_bytes[0], 5);

			VirtualProtect((LPVOID)g_wgl_swap_buffers, 0x05, old_protection, &old_protection);
			FlushInstructionCache(GetCurrentProcess(), (void*)g_wgl_swap_buffers, 0x05);

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
			if (VirtualProtect((LPVOID)g_wgl_swap_buffers, 0x05, PAGE_EXECUTE_READWRITE, &old_protection))
			{
				memcpy((void*)g_wgl_swap_buffers, &g_original_bytes[0], 5);
				VirtualProtect((LPVOID)g_wgl_swap_buffers, 0x05, old_protection, &old_protection);
				FlushInstructionCache(GetCurrentProcess(), (void*)g_wgl_swap_buffers, 0x05);

				g_is_hooked = false;
			}
		}
	}
}