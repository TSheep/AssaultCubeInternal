#include "items.hpp"
#include <Windows.h>


void Items::save_to_file(int profile)
{

	char file_name[100] = "";
	char temp_dir[MAX_PATH + 100] = "";

	sprintf_s(file_name, sizeof(file_name), "cfvip_settings(%i).dxc", profile);

	if (GetTempPathA(MAX_PATH, temp_dir) != 0)
	{
		strcat_s(temp_dir, sizeof(temp_dir), file_name);

		std::ofstream out(temp_dir, std::ios_base::binary | std::ios_base::trunc);
		if (out.is_open())
		{
			out.write((const char*)this, sizeof(Items));
			out.close();
		}
	}
}
long GetFileSize(std::string FileName)
{
	std::ifstream is;
	is.open(FileName.c_str(), std::ios_base::binary);
	if (is.is_open())
	{
		is.seekg(0, std::ios::end);
		return (long)is.tellg();
	}
	return -1;
}
void Items::load_from_file(int profile)
{
	char file_name[100] = "";
	char temp_dir[MAX_PATH + 100] = "";

	sprintf_s(file_name, sizeof(file_name), "cfvip_settings(%i).dxc", profile);

	if (GetTempPathA(MAX_PATH, temp_dir) != 0)
	{
		strcat_s(temp_dir, sizeof(temp_dir), file_name);

		long file_size = GetFileSize(temp_dir);
		if (file_size >= 0)
		{
			if (file_size <= sizeof(Items))
			{
				std::ifstream in(temp_dir, std::ios_base::binary);
				if (in.is_open())
				{
					in.read((char*)this, sizeof(Items));
					in.close();
				}
			}
		}
	}
}
