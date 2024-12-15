#include "Main.h"
#include "Utils.h"
#include "Renderer.h"
#include "Menu.h"
#include "OldMenu.h"
#include "Hooked.h"
#include "Debug.h"
#include "Features.h"

using namespace plugin;

// credits:
// https://github.com/zxvnme/zgui
// https://github.com/DK22Pac/plugin-sdk
// https://github.com/gta-reversed/gta-reversed
// https://github.com/BlastHackNet/mod_sa (mostly original creators & contributors of s0beit)
// gta_sa_compact.idb ("fastman92, TheLink2012, listener and many others")
// gta_sa_full.idb

// скины:
// 49 - дедок кунг-фу падла
// 99 - роллер
// 162 - любимый деревенщина
// 228 - €понец стил€ full black
// 264 - клоун)
// 306 - ленни ебать его ферст

// ай че нашел ахах. просто хвастаюсь. сделал эту гр€зь с помощью Tonyx97.
// кстати мне за это дали вечный бан по железу.
// https://imgur.com/a/Fl3h7

// что ты мне сделаешь в другмо городе
std::string g_watermarkStrings[3] = { "gabapentin <3", "psilocybin <3", "memantine <3" };
std::size_t g_watermarkSelected = 0;

SafetyHookInline	g_hookGetWeaponInfo{};
SafetyHookInline	g_hookHandleGunShot{};
SafetyHookInline	g_hookFireInstantHitFromCar2{};
SafetyHookInline	g_hookAddBullet{};
SafetyHookInline	g_hookProcessPed{};
SafetyHookInline	g_hookSetCursorPos{};
RsInputEventHandler g_originalInputEventHandler = nullptr;

bool g_isSamp = false;
bool g_isPanic = false;

std::uint64_t g_timeCurrent;
double g_timeDiff;

static CdeclEvent <AddressList<0x53C23A, H_JUMP>, PRIORITY_AFTER, ArgPickNone, void()> preRenderWaterEvent;

class CheatMenu {
public:
    CheatMenu() {
		g_isSamp = GetModuleHandleA("samp.dll") != 0;

		Events::initRwEvent.AddAfter(on_init);
		Events::shutdownRwEvent.AddBefore(on_shutdown);

		Events::d3dLostEvent.AddAfter(on_d3d_lost);
		Events::d3dResetEvent.AddAfter(on_d3d_reset);

		Events::drawHudEvent.AddAfter(on_draw_hud);

		if (g_isSamp)
		{
			preRenderWaterEvent.AddBefore(on_game_process); // gameProcessEvent patched in samp
			Events::drawAfterFadeEvent.AddAfter(on_draw); // drawHudEvent drawAfterFadeEvent

			return;
		}

		Events::gameProcessEvent.AddAfter(on_game_process);
		Events::drawingEvent.AddAfter(on_draw);
	};

	static void on_init()
	{
		// a la safe mode
		if (g_isSamp)
		{
			g_Config.aim_Smart = true;
			g_Config.aim_Randomize = true;
			g_Config.aim_SmallFov = true;
			g_Config.sp_InvPlayer = false;
			g_Config.sp_InvCar = false;
			g_Config.sp_NeverWanted = false;
			g_Config.sp_AimIgnoreGroove = false;
			g_Config.sp_NoReload = false;
		}

		// useless tbh
		g_watermarkSelected = plugin::RandomNumberInRange(0, 2);

		// hook game funcs
		g_hookGetWeaponInfo					= safetyhook::create_inline(0x743C60, hooked::GetWeaponInfo);
		g_hookHandleGunShot					= safetyhook::create_inline(0x712E40, hooked::HandleGunShot);
		g_hookFireInstantHitFromCar2		= safetyhook::create_inline(0x73CBA0, hooked::FireInstantHitFromCar2);
		g_hookAddBullet						= safetyhook::create_inline(0x736010, hooked::AddBullet);
		g_hookProcessPed					= safetyhook::create_inline(0x62A380, hooked::CTaskSimpleUseGun_ProcessPed);

#ifdef NEW_MENU
		auto user32 = GetModuleHandleA("user32.dll");
		if (user32)
		{
			auto scp = GetProcAddress(user32, "SetCursorPos");
			if (scp)
				g_hookSetCursorPos			= safetyhook::create_inline(scp, hooked::SetCursorPos);
		}
#endif // NEW_MENU

		// hook input. im too lazy to mess with wndproc tbh
		g_originalInputEventHandler			= RsGlobal.keyboard.inputEventHandler;
		RsGlobal.keyboard.inputEventHandler = hooked::inputEventHandler;

		g_Renderer.on_init();
		g_Debug.on_init();
		g_Menu.on_init();
	}

	static void on_shutdown()
	{
		// unhook game funcs
		g_hookGetWeaponInfo			 = {};
		g_hookHandleGunShot			 = {};
		g_hookFireInstantHitFromCar2 = {};
		g_hookAddBullet				 = {};
		g_hookProcessPed			 = {};

#ifdef NEW_MENU
		g_hookSetCursorPos			 = {};
#endif // NEW_MENU

		// unhook input
		RsGlobal.keyboard.inputEventHandler = g_originalInputEventHandler;

		g_Menu.on_shutdown();
		g_Debug.on_shutdown();
		g_Renderer.on_shutdown();
	}

	static void on_game_process()
	{
		static std::uint64_t time_last;
		g_timeCurrent = utils::time_get();
		g_timeDiff = TIME_TO_DOUBLE(g_timeCurrent - time_last);

		// sp only
		if (!g_isSamp)
			g_MapFix.on_game_process();

		// do nothing while in menu
		if (utils::gta_menu_active())
			return;

		g_PlayerCheats.on_game_process();

		time_last = g_timeCurrent;
	}

	static void on_draw()
	{
		// do nothing while in menu
		if (utils::gta_menu_active())
			return;

		g_Renderer.on_draw_start();
		{
			g_Esp.on_draw();
			g_Menu.on_draw();
			//g_Debug.on_draw();
			g_PlayerCheats.on_draw();
		}
		g_Renderer.on_draw_end();
	}

	static void on_draw_hud()
	{
		// do nothing while in menu
		if (utils::gta_menu_active())
			return;

		if (!g_Config.esp_Radar)
			return;

		g_Renderer.on_draw_start();
		{
			g_Radar.on_draw();
		}
		g_Renderer.on_draw_end();
	}

	static void on_d3d_lost()
	{
		g_Renderer.on_lost();
	}

	static void on_d3d_reset()
	{
		g_Renderer.on_reset();
	}
} CheatMenuPlugin;
