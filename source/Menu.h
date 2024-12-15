#pragma once

#include "Main.h"

#ifdef NEW_MENU

struct Config
{
	bool esp_Enabled = true;
	bool esp_Box = true;
	bool esp_Health = true;
	bool esp_Armor = true;
	bool esp_Outline = true;
	bool esp_AimTarget = true;
	bool esp_Crosshair = true;
	bool esp_Radar = false;
	bool esp_Graffiti = false;
	bool esp_Collectibles = false;

	bool aim_Enabled = true;
	bool aim_TargetVisible = true;
	bool aim_TargetOnScreen = true;
	bool aim_GangsPriority = false;
	bool aim_CopsPriority = false;
	bool aim_Smart = false;
	float aim_SmartFov = 70.0f;
	bool aim_Randomize = false;
	float aim_RandomizeFactor = 0.5f;
	bool aim_SmallFov = false; // todo: ...
	bool aim_PerfectAccuracy = true;
	bool aim_SniperRifle = false;

	bool sp_InvPlayer = true;
	bool sp_InvCar = true;
	bool sp_NeverWanted = true;
	bool sp_RussianMostWanted = false;
	bool sp_AimIgnoreGroove = true;
	bool sp_Speedhack = true;
	bool sp_SpeedhackFaster = false;
	bool sp_NoReload = true;
	bool sp_LegSlide = true;
	bool sp_Telekinesis = false; // works in samp
	bool sp_MinigunMode = false; // works in samp
	bool sp_DumbassPeds = false;
	bool sp_Airbrake = true;
	int sp_AirbrakeMode = 0;
} extern g_Config;

class Menu
{
public:
	Menu() { };

	bool is_open() const;

	void on_init();
	void on_shutdown() { };

	void on_draw();
} extern g_Menu;

#endif // NEW_MENU