#include <Windows.h>
#include <TlHelp32.h>
#include <iostream>
#include <string>

int get_process_id(const char* process_name)
{
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (snapshot != INVALID_HANDLE_VALUE)
	{
		PROCESSENTRY32 pe32 = { 0 };
		pe32.dwSize = sizeof(pe32);

		if (Process32First(snapshot, &pe32))
		{
			do
			{
				if (_stricmp(process_name, pe32.szExeFile) == 0)
				{
					CloseHandle(snapshot);
					return pe32.th32ProcessID;
				}
			} while (Process32Next(snapshot, &pe32));
		}

		CloseHandle(snapshot);
	}

	return -1;
}
int main(int argc, char** argv)
{
	//get dll path
	char module_file_name[MAX_PATH + 1] = { 0 };
	DWORD len = GetModuleFileNameA(GetModuleHandleA(NULL), module_file_name, MAX_PATH);
	module_file_name[len] = 0;

	std::string startup_path(module_file_name);
	startup_path = startup_path.substr(0, startup_path.find_last_of('\\') + 1);


	std::string dll_path(startup_path);
	dll_path += "AssaultCubeInternal.dll";

	HANDLE file_handle = CreateFileA(dll_path.c_str(), FILE_ALL_ACCESS, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (file_handle == INVALID_HANDLE_VALUE)
	{
		printf("[-] failed to find AssaultCubeInternal.dll\n");
		getchar();
		return 1;
	}
	
	CloseHandle(file_handle);

	printf("[+] AssaultCubeInternal.dll found!\n");

	//get pid
	printf("[+] waiting for ac_client.exe...\n");

	int target_pid = -1;
	do
	{
		Sleep(200);
		target_pid = get_process_id("ac_client.exe");
	} while (target_pid == -1);


	printf("[+] ac_client.exe process found [%i]\n", target_pid);

	HANDLE process_handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, target_pid);
	if (process_handle)
	{

		auto remote_path_mem = VirtualAllocEx(process_handle, NULL, strlen(dll_path.c_str()) + 1, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
		if (!remote_path_mem)
		{
			CloseHandle(process_handle);
			printf("[-] failed to allocate memory for remote path\n");
			getchar();
			return 1;
		}

		SIZE_T bytes_written = 0;
		if (!WriteProcessMemory(process_handle, remote_path_mem, dll_path.data(), dll_path.length() + 1, &bytes_written))
		{
			VirtualFreeEx(process_handle, remote_path_mem, 0, MEM_RELEASE);
			CloseHandle(process_handle);
			printf("[-] failed to write dll path into remote process\n");
			getchar();
			return 1;
		}

		auto loadlib_address	 = GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryA");
		auto freelibrary_address = GetProcAddress(GetModuleHandleA("kernel32.dll"), "FreeLibrary");

		if (!loadlib_address || !freelibrary_address)
		{
			VirtualFreeEx(process_handle, remote_path_mem, 0, MEM_RELEASE);
			CloseHandle(process_handle);
			printf("[-] failed to resolve kernel32 addresses\n");
			getchar();
			return 1;
		}

		//Start Thread on kernel32.LoadLibrary, pass remote path as argument so it calls LoadLibrary("OUR_DLL_PATH.dll")
		HANDLE thread_handle = CreateRemoteThread(process_handle, nullptr, 0, (LPTHREAD_START_ROUTINE)loadlib_address, remote_path_mem, 0, nullptr);
		if (thread_handle == INVALID_HANDLE_VALUE)
		{
			VirtualFreeEx(process_handle, remote_path_mem, 0, MEM_RELEASE);
			CloseHandle(process_handle);
			printf("[-] failed to start remote thread\n");
			getchar();
			return 1;
		}


		WaitForSingleObject(thread_handle, INFINITE);

		DWORD remote_module_base = 0;
		GetExitCodeThread(thread_handle, &remote_module_base);

		if (remote_module_base != 0)
		{
			printf("[+] injection succeeded, module loaded @ 0x%p\n", remote_module_base);

			printf("press enter to unload...\n");
			getchar();


			CloseHandle(CreateRemoteThread(process_handle, 
				nullptr, 
				0, 
				(LPTHREAD_START_ROUTINE)freelibrary_address, 
				(LPVOID)remote_module_base, 
				0, 
				nullptr));

			VirtualFreeEx(process_handle, remote_path_mem, 0, MEM_RELEASE);
			CloseHandle(thread_handle);
			CloseHandle(process_handle);
			return 1;
		}
		else
		{
			printf("[+] loadlibrary failed\n");
		}

		VirtualFreeEx(process_handle, remote_path_mem, 0, MEM_RELEASE);
		CloseHandle(thread_handle);
		CloseHandle(process_handle);
	}

	printf("press enter to exit...\n");
	getchar();
	return 1;
}