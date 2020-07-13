#include <windows.h>
#include <iostream>

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

	auto wgl_swap_buffers = GetProcAddress(opengl32, "wglSwapBuffers");

	printf("opengl32.dll found @ 0x%p\n", opengl32);
	printf("wgl_swap_buffers found @ 0x%p\n", wgl_swap_buffers);


	return 1;
}

FILE*	g_stream = nullptr;
HANDLE	g_init_thread = nullptr;

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
		TerminateThread(g_init_thread, 0);

		printf("unloading dll...\n");
		fclose(g_stream);
		FreeConsole();
	}
	return 1;
}