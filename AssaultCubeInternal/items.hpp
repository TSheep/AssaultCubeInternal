#pragma once

#include <cstdint>
#include <fstream>
#include <string>

class Items
{
public:
	std::uint32_t espFolder;
	std::uint32_t espEnabled;
	std::uint32_t espName;
	std::uint32_t espBone;
	std::uint32_t espDistance;
	std::uint32_t espBox;
	std::uint32_t espHP;
	std::uint32_t espLine;
	std::uint32_t aimbotFolder;
	std::uint32_t aimbotHumanize;
	std::uint32_t aimbotEnabled;
	std::uint32_t aimbotMethod;
	std::uint32_t aimbotFOV;
	std::uint32_t aimbotKey;
	std::uint32_t ghostAimFolder;
	std::uint32_t ghostAimKey;
	std::uint32_t ghostAimImpactRange;
	std::uint32_t aimbotStayOnAimed;
	std::uint32_t aimbotSpawnshieldCheck;
	std::uint32_t aimbotBone;
	std::uint32_t aimbotSmooth;
	std::uint32_t aimbotDrawFOV;
	std::uint32_t aimbotAutoFire;
	std::uint32_t aimbotDistance;
	std::uint32_t aimbotWallbang;
	std::uint32_t unlimitedAmmo;
	std::uint32_t miscFolder;
	std::uint32_t weaponFolder;
	std::uint32_t saveMenu;
	std::uint32_t saveProfile;
	std::uint32_t loadProfile;
	std::uint32_t loadMenu;
	std::uint32_t noRecoil;
	std::uint32_t noSpread;
	std::uint32_t noReload;
	std::uint32_t fastSwitch;
	std::uint32_t alwaysHeadshot;
	std::uint32_t ghostAim;
	std::uint32_t d3dFolder;
	std::uint32_t d3dWireframes;
	std::uint32_t d3dFullbright;
	std::uint32_t d3dNofog;
	std::uint32_t d3dChams;
	std::uint32_t d3dChamsA;
	std::uint32_t d3dChamsB;
	std::uint32_t d3dCrosshair;
	std::uint32_t radar2DFolder;
	std::uint32_t radar2DSize;
	std::uint32_t radar2DPos;
	std::uint32_t radar2DEnable;
	std::uint32_t radar2DBackground;
	std::uint32_t radar2DBorder;
	std::uint32_t speedHack;

	void save_to_file(int profile);
	void load_from_file(int profile);

};

extern Items g_items;