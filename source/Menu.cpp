#include "Menu.h"
#include "zgui/zgui.hh"
#include "Renderer.h"
#include "Debug.h"

#ifdef NEW_MENU

Config g_Config;
Menu g_Menu;

namespace wrapper
{
	static void line(float x, float y, float x2, float y2, zgui::color c) noexcept
	{
		g_Renderer.line(x, y, x2, y2, CRGBA(c.r, c.g, c.b, c.a));
	}

	static void rect(float x, float y, float x2, float y2, zgui::color c) noexcept
	{
		g_Renderer.border_box(x, y, x2 - 1, y2 - 1, 1.0f, CRGBA(c.r, c.g, c.b, c.a));
	}

	static void filled_rect(float x, float y, float x2, float y2, zgui::color c) noexcept
	{
		g_Renderer.rect(x, y, x2, y2, CRGBA(c.r, c.g, c.b, c.a));
	}

	static void text(float x, float y, zgui::color c, bool center, const char* text) noexcept
	{
		g_Renderer.text(x, y, center ? DT_CENTER : DT_LEFT, CRGBA(c.r, c.g, c.b, c.a), text);
	}

	static void get_text_size(const char* text, int& width, int& height) noexcept
	{
		g_Renderer.text_size(text, width, height);
	}

	static float get_frametime() noexcept
	{
		return 1.0f;
	}

	static CVector2D get_cursor_pos(HWND hwnd)
	{
		POINT p_mouse_pos;
		GetCursorPos(&p_mouse_pos);
		ScreenToClient(hwnd, &p_mouse_pos);

		return CVector2D(p_mouse_pos.x, p_mouse_pos.y);
	}
}

void Menu::on_init()
{
	zgui::functions.draw_line			= wrapper::line;
	zgui::functions.draw_rect			= wrapper::rect;
	zgui::functions.draw_filled_rect	= wrapper::filled_rect;
	zgui::functions.draw_text			= wrapper::text;
	zgui::functions.get_text_size		= wrapper::get_text_size;
	zgui::functions.get_frametime		= wrapper::get_frametime;

	zgui::globals::window_ctx.position	= { 50, 50 };
	zgui::globals::window_ctx.opened	= true;
}

void Menu::on_draw()
{
	zgui::poll_input(RsGlobal.ps->window);

	if (zgui::begin_window("gta:sa funny mod v1.0 by rosko", { 533, 287 }, 0, zgui::zgui_window_flags_no_ontoggle_animation))
	{
		zgui::begin_groupbox("Aimbot", { 161, 248 });
			zgui::checkbox("enabled #aim_Enabled", g_Config.aim_Enabled);
			zgui::checkbox("target only visible #aim_TargetVisible", g_Config.aim_TargetVisible);
			zgui::checkbox("target only on screen #aim_TargetOnScreen", g_Config.aim_TargetOnScreen);
			zgui::checkbox("gangs priority #aim_GangsPriority", g_Config.aim_GangsPriority);
			zgui::checkbox("cops priority #aim_CopsPriority", g_Config.aim_CopsPriority);
			zgui::checkbox("smart #aim_Smart", g_Config.aim_Smart);
			zgui::slider_float("fov in px #aim_SmartFov", 5.0f, 200.0f, g_Config.aim_SmartFov);
			zgui::checkbox("randomize #aim_Randomize", g_Config.aim_Randomize);
			zgui::slider_float("factor #aim_RandomizeFactor", 0.0f, 1.0f, g_Config.aim_RandomizeFactor);
			zgui::checkbox("small fov #aim_SmallFov", g_Config.aim_SmallFov);
			zgui::checkbox("perfect accuracy #aim_PerfectAccuracy", g_Config.aim_PerfectAccuracy);
			zgui::checkbox("sniper rifle #aim_SniperRifle", g_Config.aim_SniperRifle);
		zgui::end_groupbox();

		zgui::next_column(171, 0);

		zgui::begin_groupbox("ESP", { 100, 118 });
			zgui::checkbox("enabled #esp_Enabled", g_Config.esp_Enabled);
			zgui::checkbox("box #esp_Box", g_Config.esp_Box);
			zgui::checkbox("health #esp_Health", g_Config.esp_Health);
			zgui::checkbox("armor #esp_Armor", g_Config.esp_Armor);
			zgui::checkbox("outlined #esp_Outline", g_Config.esp_Outline);
			zgui::checkbox("aimbot target #esp_AimTarget", g_Config.esp_AimTarget);
		zgui::end_groupbox();

		zgui::begin_groupbox("Extra", { 100, 54 });
			zgui::checkbox("crosshair #esp_Crosshair", g_Config.esp_Crosshair);
			zgui::checkbox("radar #esp_Radar", g_Config.esp_Radar);
		zgui::end_groupbox();

		zgui::next_column(110, 0);

		zgui::begin_groupbox("Misc", { 100, 178 });
			zgui::checkbox("godmode #sp_InvPlayer", g_Config.sp_InvPlayer);
			zgui::checkbox("car godmode #sp_InvCar", g_Config.sp_InvCar);
			zgui::checkbox("speedhack #sp_Speedhack", g_Config.sp_Speedhack);
			zgui::checkbox("speedhack faster #sp_SpeedhackFaster", g_Config.sp_SpeedhackFaster);
			zgui::checkbox("leg slide #sp_LegSlide", g_Config.sp_LegSlide);
			zgui::checkbox("telekinesis #sp_Telekinesis", g_Config.sp_Telekinesis);
			zgui::checkbox("minigun mode #sp_MinigunMode", g_Config.sp_MinigunMode);
			zgui::checkbox("airbrake #sp_Airbrake", g_Config.sp_Airbrake);
			zgui::combobox("#sp_AirBrakeMode", { "s0beit", "follow cam" }, g_Config.sp_AirbrakeMode);
		zgui::end_groupbox();

		zgui::next_column(110, 0);

		zgui::begin_groupbox("SP only", { 110, 134 });
			zgui::checkbox("never wanted #sp_NeverWanted", g_Config.sp_NeverWanted);
			zgui::checkbox("ru most wanted #sp_RussianMostWanted", g_Config.sp_RussianMostWanted);
			zgui::checkbox("aim ignore groove #sp_AimIgnoreGroove", g_Config.sp_AimIgnoreGroove);
			zgui::checkbox("no reload #sp_NoReload", g_Config.sp_NoReload);
			zgui::checkbox("dumbass peds #sp_DumbassPeds", g_Config.sp_DumbassPeds);
			zgui::checkbox("graffiti #esp_Graffiti", g_Config.esp_Graffiti);
			zgui::checkbox("collectibles #esp_Collectibles", g_Config.esp_Collectibles);
		zgui::end_groupbox();

		zgui::end_window();

		auto cursor = wrapper::get_cursor_pos(RsGlobal.ps->window);
		g_Renderer.cursor(cursor.x, cursor.y, plugin::color::White);
	}
}

bool Menu::is_open() const
{
	return zgui::globals::window_ctx.opened;
}

#endif // NEW_MENU