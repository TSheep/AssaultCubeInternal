#include <windows.h>
#include <iostream>

#include "opengl.hpp"
#include "globals.hpp"
#include "ogl_renderer.hpp"
#include "colors.hpp"
#include "items.hpp"
#include "menu.hpp"
#include "assault_cube.hpp"
#include "opengl_render.hpp"
#include "MemoryPatch.hpp"

#pragma warning(disable: 4244)

Items				g_items;
globals				g_globals;
FILE*				g_stream = nullptr;
HANDLE				g_init_thread = nullptr;
bool				g_menu_initialized = false;
ListMenu			g_menu;


mem_patch			unlimited_weapon_ammo(0x04637E9, { 0x90, 0x90 });
mem_patch			unlimited_grenade_ammo(0x00463378, { 0x90, 0x90 });


#define SHOW_HIDE				{ "+","-"}
#define ON_OFF					{ "Off","On"}
#define ESP_OPTIONS				{ "Off","Enemys","All" }
#define AIM_BONE_OPTIONS		{ "Head","Chest","Belly"}
#define GHOSTAIM_IMPACTS		{ "10%","20%","30%","40%","50%","60%","70%","80%","90%","100%"}
#define AIM_BONE_SCAN_PRIO		{ "IntelliScan","Equal","Head","Chest","Neck","Left Shoulder","Right Shoulder","Left Torso","Right Torso","Left Knee","Right Knee"}
#define FOV_OPTIONS				{ "Unlimited","1deg","2deg","3deg","4deg","5deg","10deg","30deg","50deg","70deg","100deg","120deg","150deg","180deg" }
#define CROSSHAIR_SIZE_OPTIONS	{ "1","2","3","4","5","6" }
#define ESP_LINE_OPTIONS		{ "Off","Bottom","Crosshair"}
#define ESP_BOX_OPTIONS			{ "Off","3D","2D"}
#define AIM_METHOD_OPTIONS		{ "Nearest Crosshair","Nearest Player","Lowest Health" }
#define TRIGGER_METHOD_OPTIONS	{ "Angle FOV","Crosshair"}
#define TRIGGER_DELAY_OPTION	{ "0ms","50ms","100ms","200ms","300ms","500ms","1000ms" }
#define AIM_KEY_OPTIONS			{ "Always","Left Mouse","Right Mouse","Middle Mouse","ALT","Shift","Ctrl","Mouse 5","Mouse 6" }
#define AIM_SMOOTH_OPTIONS		{ "Instant","1","2","3","4","5","6","7","8","9","10","11","12","13","14","15","16","17","18","19"}
#define AIM_FOV_OPTIONS 		{ "Unlimited","5","10","15","20","25","30","35","40","45","50","55","60","65","70","75","80","85","90","95","100","105","110","115","120","125","130","135","140","145","150","155","160","165","170","175","180","185","190","195","200"}
#define CHAMS_COLORS			{ "Off", "SkyBlue", "Red", "Gold", "LawnGreen", "Orchid", "Fuchsia", "Blue", "Cyan", "Lime", "Black" }
#define AIM_PREDICTION			{"Off","Small","Medium","High"}
#define AIM_MAX_DISTANCE		{"Unlimited","Close Range","Medium Range","High Range"}
#define RADAR_POSITIONS			{"Center","Top Left","Top Right","Bottom Left","Bottom Right"}
#define RADAR_SIZES				{"100","200","300","400"}

void init_menu_items()
{
	g_menu.SetTitle("darkcheater.dev AC");
	g_menu.Add("[OpenGL]", SHOW_HIDE, &g_items.d3dFolder, MENUFOLDER);
	if (g_items.d3dFolder)
	{
		g_menu.Add("Chams", ON_OFF, &g_items.d3dChams, MENUCATEGORY);
		if (g_items.d3dChams)
		{
			g_menu.Add("ColorA", CHAMS_COLORS, &g_items.d3dChamsA);
			g_menu.Add("ColorB", CHAMS_COLORS, &g_items.d3dChamsB);
		}
	}


	g_menu.Add("[ESP]", SHOW_HIDE, &g_items.espFolder, MENUFOLDER);
	if (g_items.espFolder)
	{
		g_menu.Add("Show", ESP_OPTIONS, &g_items.espEnabled, MENUCATEGORY);
		if (g_items.espEnabled)
		{
			g_menu.Add("Name", ON_OFF, &g_items.espName);
			g_menu.Add("Distance", ON_OFF, &g_items.espDistance);
			g_menu.Add("Health", ON_OFF, &g_items.espHP);
			g_menu.Add("Box", ESP_BOX_OPTIONS, &g_items.espBox);
			g_menu.Add("Line", ON_OFF, &g_items.espLine);
			g_menu.Add("Skeleton", ON_OFF, &g_items.espBone);
		}
	}

	g_menu.Add("[Weapon]", SHOW_HIDE, &g_items.weaponFolder, MENUFOLDER);
	if (g_items.weaponFolder)
	{
		g_menu.Add("Unlimited Ammo", ON_OFF, &g_items.unlimitedAmmo);
	}
}



void on_draw()
{

	if (g_menu.InitializationNeeded())
	{
		g_menu.ResetItems();
		init_menu_items();
	}
	if (g_items.saveProfile && g_items.saveMenu)
	{
		g_items.saveMenu = 0;
		g_items.loadMenu = 0;
		g_items.save_to_file(g_items.saveProfile);
		g_menu.Invalidate();
	}
	if (g_items.loadProfile && g_items.loadMenu)
	{
		g_items.load_from_file(g_items.loadProfile);
		g_items.loadMenu = 0;
		g_items.saveMenu = 0;
		g_menu.Invalidate();
	}

	//draw here

	opengl::draw_text(g_globals.window_width / 2, 100, colors::Red, "I Bims 1 renderer lulz");
	opengl::draw_text_center(g_globals.window_width / 2, 120, colors::Red, "I Bims 2 renderer lulz");
	opengl::draw_text_right(g_globals.window_width / 2, 140, colors::Red, "I Bims 3 renderer lulz");

	opengl::fill_rect(g_globals.window_width / 2, 100, 200, 100, MAKE_ARGB(100, 20, 20, 20));
	opengl::draw_rect_outline(g_globals.window_width / 2, 100, 200, 100, 1.0f, colors::Blue);
	

	/*example, draw player position*/
	/*TODO: find all player positions, loop*/

	Vector3 position3D(166.133f, 160.289f, -1.000f);
	Vector3 position2D;
	if (ac::world_to_screen(position3D, position2D))
	{
		opengl::draw_text(position2D.x, position2D.y, colors::Red, "PLAYER NAME");
	}


	if (g_items.unlimitedAmmo)
	{
		unlimited_weapon_ammo.patch();
		unlimited_grenade_ammo.patch();
	}
	else
	{
		unlimited_weapon_ammo.restore();
		unlimited_grenade_ammo.restore();
	}

	
	g_menu.HandleMenuNavigation();
	g_menu.Render(&g_render);
}

DWORD __stdcall start_cheat(LPVOID param)
{
	printf("cheat injected!\n");

	printf("waiting for ac window\n");

	do
	{
		g_globals.game_window = FindWindowA(NULL, "AssaultCube");
		Sleep(100);
	} while (!g_globals.game_window);

	printf("ac window found 0x%p\n", g_globals.game_window);

	printf("waiting for opengl32.dll\n");

	HMODULE opengl32 = nullptr;

	
	do
	{
		opengl32 = GetModuleHandleA("opengl32.dll");
		Sleep(100);
	} while (!opengl32);

	printf("opengl32.dll found @ 0x%p\n", opengl32);
	
	if (!opengl::hook(opengl32, g_globals.game_window, on_draw))
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
		g_globals.module = dll;
		AllocConsole();
		AttachConsole(GetCurrentProcessId());

		freopen_s(&g_stream, "CON", "w", stdout);
		g_init_thread = CreateThread(nullptr, 0, start_cheat, nullptr, 0, nullptr);
	}
	else if (reason == DLL_PROCESS_DETACH)
	{
		opengl::unhook();

		if (g_init_thread != INVALID_HANDLE_VALUE)
		{
			TerminateThread(g_init_thread, 0);

			printf("unloading dll...\n");
			fclose(g_stream);
			FreeConsole();
		}
	}
	return 1;
}