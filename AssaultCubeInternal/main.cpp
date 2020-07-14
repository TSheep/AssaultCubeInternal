#include <windows.h>
#include <iostream>

#include "opengl.hpp"


FILE*				g_stream = nullptr;
HANDLE				g_init_thread = nullptr;

DWORD __stdcall start_cheat(LPVOID param)
{
	printf("cheat injected!\n");
	printf("waiting for d3d8.dll\n");

	HMODULE opengl32 = nullptr;

	do
	{
		opengl32 = GetModuleHandleA("opengl32.dll");
		Sleep(100);
	} while (!opengl32);

	printf("opengl32.dll found @ 0x%p\n", opengl32);
	
	if (!opengl::hook(opengl32))
	{
		printf("failed to hook opengl\n");
		return 1;
	}

	return 1;
}


int __stdcall DllMain(HMODULE dll, DWORD reason, LPVOID argument)
{

	if (reason == DLL_PROCESS_ATTACH)
	{
		AllocConsole();
		AttachConsole(GetCurrentProcessId());

		freopen_s(&g_stream, "CON", "w", stdout);
		g_init_thread = CreateThread(nullptr, 0, start_cheat, nullptr, 0, nullptr);
	}
	else if (reason == DLL_PROCESS_DETACH)
	{
		opengl::unhook();

		TerminateThread(g_init_thread, 0);

		printf("unloading dll...\n");
		fclose(g_stream);
		FreeConsole();
	}
	return 1;
}