#include "OldMenu.h"
#include "Renderer.h"
#include "Utils.h"

using namespace plugin;

#ifndef NEW_MENU

Config g_Config;
Menu g_Menu;

void Menu::on_init()
{
	// set menu items
	std::size_t i = 0u;
	m_menuItems.emplace_back(i++, "gta:sa funny mod",	  ItemType::MENUITEM_SUBMENU, nullptr,						   "how you doin?");
	m_menuItems.emplace_back(i++, "----- [ esp ] -----",  ItemType::MENUITEM_SUBMENU, nullptr,						   "zoloft helps me everyday");
	m_menuItems.emplace_back(i++, "esp_Enabled",		  ItemType::MENUITEM_TOGGLE,  &g_Config.esp_Enabled,		   "visuals");
	m_menuItems.emplace_back(i++, "esp_Box",			  ItemType::MENUITEM_TOGGLE,  &g_Config.esp_Box,			   "draws bounding box on peds");
	m_menuItems.emplace_back(i++, "esp_Health",			  ItemType::MENUITEM_TOGGLE,  &g_Config.esp_Health,			   "draws health bar on peds");
	m_menuItems.emplace_back(i++, "esp_Armor",			  ItemType::MENUITEM_TOGGLE,  &g_Config.esp_Armor,			   "draws armor bar on peds");
	m_menuItems.emplace_back(i++, "esp_Outline",		  ItemType::MENUITEM_TOGGLE,  &g_Config.esp_Outline,		   "outline makes things look sexier");
	m_menuItems.emplace_back(i++, "esp_AimTarget",		  ItemType::MENUITEM_TOGGLE,  &g_Config.esp_AimTarget,		   "know your customer");
	m_menuItems.emplace_back(i++, "esp_Crosshair",		  ItemType::MENUITEM_TOGGLE,  &g_Config.esp_Crosshair,		   "draws custom crosshair");
	m_menuItems.emplace_back(i++, "esp_Radar",			  ItemType::MENUITEM_TOGGLE,  &g_Config.esp_Radar,			   "draws peds on game radar");
	m_menuItems.emplace_back(i++, "esp_Graffiti",		  ItemType::MENUITEM_TOGGLE,  &g_Config.esp_Graffiti,		   "draws unpainted graffiti");
	m_menuItems.emplace_back(i++, "esp_Collectibles",	  ItemType::MENUITEM_TOGGLE,  &g_Config.esp_Collectibles,	   "draws collectibles");
	m_menuItems.emplace_back(i++, "----- [ aim ] -----",  ItemType::MENUITEM_SUBMENU, nullptr,						   "bosch");
	m_menuItems.emplace_back(i++, "aim_Enabled",		  ItemType::MENUITEM_TOGGLE,  &g_Config.aim_Enabled,		   "silent aimbot");
	m_menuItems.emplace_back(i++, "aim_TargetVisible",	  ItemType::MENUITEM_TOGGLE,  &g_Config.aim_TargetVisible,	   "checks if target visible");
	m_menuItems.emplace_back(i++, "aim_TargetOnScreen",	  ItemType::MENUITEM_TOGGLE,  &g_Config.aim_TargetOnScreen,	   "checks if target on screen");
	m_menuItems.emplace_back(i++, "aim_GangsPriority",	  ItemType::MENUITEM_TOGGLE,  &g_Config.aim_GangsPriority,	   "aimbot targets gangs first");
	m_menuItems.emplace_back(i++, "aim_CopsPriority",	  ItemType::MENUITEM_TOGGLE,  &g_Config.aim_CopsPriority,	   "aimbot targets cops first");
	m_menuItems.emplace_back(i++, "aim_Smart",			  ItemType::MENUITEM_TOGGLE,  &g_Config.aim_Smart,			   "bone closest to crosshair");
	m_menuItems.emplace_back(i++, "aim_SmartFov",		  ItemType::MENUITEM_VALUE,   &g_Config.aim_SmartFov,		   "fov in pixels");
	m_menuItems.emplace_back(i++, "aim_Randomize",		  ItemType::MENUITEM_TOGGLE,  &g_Config.aim_Randomize,		   "randomize aim spot");
	m_menuItems.emplace_back(i++, "aim_RandomizeFactor",  ItemType::MENUITEM_VALUE,   &g_Config.aim_RandomizeFactor,   "randomize factor");
	m_menuItems.emplace_back(i++, "aim_SmallFov",		  ItemType::MENUITEM_TOGGLE,  &g_Config.aim_SmallFov,		   "small field of view for aimbot (not done yet)");
	m_menuItems.emplace_back(i++, "aim_PerfectAccuracy",  ItemType::MENUITEM_TOGGLE,  &g_Config.aim_PerfectAccuracy,   "near perfect accuracy");
	m_menuItems.emplace_back(i++, "aim_SniperRifle",	  ItemType::MENUITEM_TOGGLE,  &g_Config.aim_SniperRifle,	   "it works but its very ridiculous\nyou need to shoot yourself in the foot\nsounds like another one c++ joke\nbut its not. also hits through walls");
	m_menuItems.emplace_back(i++, "----- [ misc ] -----", ItemType::MENUITEM_SUBMENU, nullptr,						   "why im here?");
	m_menuItems.emplace_back(i++, "sp_InvPlayer",		  ItemType::MENUITEM_TOGGLE,  &g_Config.sp_InvPlayer,		   "invulnerable player");
	m_menuItems.emplace_back(i++, "sp_InvCar",			  ItemType::MENUITEM_TOGGLE,  &g_Config.sp_InvCar,			   "invulnerable player car");
	m_menuItems.emplace_back(i++, "sp_NeverWanted",		  ItemType::MENUITEM_TOGGLE,  &g_Config.sp_NeverWanted,		   "cops dont really care");
	m_menuItems.emplace_back(i++, "sp_RussianMostWanted", ItemType::MENUITEM_TOGGLE,  &g_Config.sp_RussianMostWanted,  "kizaru mode");
	m_menuItems.emplace_back(i++, "sp_AimIgnoreGroove",	  ItemType::MENUITEM_TOGGLE,  &g_Config.sp_AimIgnoreGroove,	   "aimbot will ignore greenpeace members");
	m_menuItems.emplace_back(i++, "sp_Speedhack",		  ItemType::MENUITEM_TOGGLE,  &g_Config.sp_Speedhack,		   "explore the world on foot faster");
	m_menuItems.emplace_back(i++, "sp_SpeedhackFaster",	  ItemType::MENUITEM_TOGGLE,  &g_Config.sp_SpeedhackFaster,	   "waaaay faster");
	m_menuItems.emplace_back(i++, "sp_NoReload",		  ItemType::MENUITEM_TOGGLE,  &g_Config.sp_NoReload,		   "now your gun hungry af");
	m_menuItems.emplace_back(i++, "sp_LegSlide",		  ItemType::MENUITEM_TOGGLE,  &g_Config.sp_LegSlide,		   "ewww you stepped on shit"); // tony hawk's pro skater
	m_menuItems.emplace_back(i++, "sp_Telekinesis",		  ItemType::MENUITEM_TOGGLE,  &g_Config.sp_Telekinesis,		   "tp peds infront of player");
	m_menuItems.emplace_back(i++, "sp_MinigunMode",		  ItemType::MENUITEM_TOGGLE,  &g_Config.sp_MinigunMode,		   "turn CiJey into destroy machine");
	m_menuItems.emplace_back(i++, "sp_DumbassPeds",		  ItemType::MENUITEM_TOGGLE,  &g_Config.sp_DumbassPeds,		   "superpower everyone wants\nbe careful, you can get softlock on mission with this");
	m_menuItems.emplace_back(i++, "sp_Airbrake",		  ItemType::MENUITEM_TOGGLE,  &g_Config.sp_Airbrake,		   "break this stupid air");
	m_menuItems.emplace_back(i++, "sp_AirbrakeMode",	  ItemType::MENUITEM_TOGGLE,  &g_Config.sp_AirbrakeMode,	   "disabled: mod_sa original\nenabled: follows camera");
	m_menuItems.emplace_back(i++, "----- [ func ] -----", ItemType::MENUITEM_SUBMENU, nullptr,						   "what is the meaning of my life");
	m_menuItems.emplace_back(i++, "fn_GiveAmmo",		  ItemType::MENUITEM_BUTTON,  functions::give_ammo,			   "gives 100 bullets to weapon in hands");
	m_menuItems.emplace_back(i++, "fn_GiveMoney",		  ItemType::MENUITEM_BUTTON,  functions::give_money,		   "gives three hundred bucks to poor CiJey");
	m_menuItems.emplace_back(i++, "fn_GiveMinigun",		  ItemType::MENUITEM_BUTTON,  functions::give_minigun,		   "gives minigun");
	m_menuItems.emplace_back(i++, "fn_Collectibles",	  ItemType::MENUITEM_BUTTON,  functions::collect_collectibles, "collect all collectibles & paint all graffiti");
	m_menuItems.emplace_back(i++, "fn_UnlockNearbyCars",  ItemType::MENUITEM_BUTTON,  functions::unlock_nearby_cars,   "now you can kick that stupid cops out of their cars");

	// set menu position and size
	m_menuRect = CRect(100.0f, 100.0f, 202.0f, float(m_menuItems.size() * FONT_SIZE));
}

void Menu::on_shutdown()
{
	if (!m_menuItems.empty())
		m_menuItems.clear();
}

bool Menu::is_open()
{
	return m_isOpen;
}

void Menu::on_draw()
{
	if (!m_isOpen)
		return;

	// draw menu box
	g_Renderer.rect_out(m_menuRect.left, m_menuRect.top, m_menuRect.right, m_menuRect.bottom + 6.f, CRGBA(100, 100, 100, 127), CRGBA(200, 200, 200, 200));

	// draw controls help
	g_Renderer.text(m_menuRect.left + m_menuRect.right + 4.f, m_menuRect.top, DT_LEFT, color::White,
		"controls:\nINSERT: show/hide menu\nUP/DOWN: select\nLEFT/RIGHT: toggle\nDELETE: panic\n\nRSHIFT: airbrake toggle\nNUMPAD0: airbrake slowdown\nZ: anim reset");

	// draw menu items
	for (auto q = 0u; q < m_menuItems.size(); q++)
	{
		auto item = m_menuItems.at(q);
		switch (item.type)
		{
		case ItemType::MENUITEM_SUBMENU:
			button(m_menuRect.left, m_menuRect.top + 2.f + (q * FONT_SIZE), item.name, item.index, m_selectedOption);
			break;
		case ItemType::MENUITEM_TOGGLE:
			if (item.data)
				toggle(m_menuRect.left, m_menuRect.top + 2.f + (q * FONT_SIZE), item.name, item.data, item.index, m_selectedOption);
			break;
		case ItemType::MENUITEM_VALUE:
			if (item.data)
				value(m_menuRect.left, m_menuRect.top + 2.f + (q * FONT_SIZE), item.name, item.data, item.index, m_selectedOption);
			break;
		case ItemType::MENUITEM_BUTTON:
			if (item.data)
				button(m_menuRect.left, m_menuRect.top + 2.f + (q * FONT_SIZE), item.name, item.index, m_selectedOption);
			break;
		}
	}

	// draw feature description
	g_Renderer.text(m_menuRect.left + m_menuRect.right / 2.f, m_menuRect.top + m_menuRect.bottom + 8.f, DT_CENTER, color::White, m_menuItems.at(m_selectedOption).description);
}

bool Menu::on_input(RsEvent event, int key_code)
{
	if (key_code == RsKeyCodes::rsINS)
		m_isOpen = !m_isOpen;

	if (!m_isOpen)
		return false;

	// navigation
	if (key_code == RsKeyCodes::rsUP)
	{
		m_selectedOption--;

		if (m_selectedOption < 0)
			m_selectedOption = m_menuItems.size() - 1;

		// workaround to skip 'separators'. a bit complicated
		while (!m_menuItems.at(m_selectedOption).data)
			m_selectedOption = m_selectedOption == 0 ? m_menuItems.size() - 1 : m_selectedOption - 1;
	}
	else if (key_code == RsKeyCodes::rsDOWN)
	{
		m_selectedOption++;

		if (m_selectedOption > m_menuItems.size() - 1)
			m_selectedOption = 0;

		// same
		while (!m_menuItems.at(m_selectedOption).data)
			m_selectedOption++;
	}

	auto item = m_menuItems.at(m_selectedOption);

	// change value or call function
	if (key_code == RsKeyCodes::rsLEFT || key_code == RsKeyCodes::rsRIGHT)
	{
		if (item.type == ItemType::MENUITEM_TOGGLE)
			*reinterpret_cast<bool*>(item.data) ^= 1;

		if (item.type == ItemType::MENUITEM_BUTTON)
			reinterpret_cast<Item::fn>(item.data)();

		// todo: min/max for value
		if (key_code == RsKeyCodes::rsLEFT &&
			item.type == ItemType::MENUITEM_VALUE)
		{
			auto& val = *reinterpret_cast<float*>(item.data);
			if (val > 0.01f) val -= 0.1f; // so ghetto...
		}

		if (key_code == RsKeyCodes::rsRIGHT &&
			item.type == ItemType::MENUITEM_VALUE)
			*reinterpret_cast<float*>(item.data) += 0.1f;
	}

	return true;
}

void Menu::toggle(const float& x, const float& y, const char* name, const void* value, const int& index, const int& selected) const
{
	// draw variable name
	g_Renderer.text(x + 4.f, y, DT_LEFT, index == selected ? color::Gold : color::White, name);

	auto val = *reinterpret_cast<bool*>(m_menuItems.at(index).data);

	// draw its state
	g_Renderer.text(x + m_menuRect.right - 3.f, y, DT_RIGHT, val ? color::LimeGreen : color::Tomato, val ? "enabled" : "disabled");
}

void Menu::value(const float& x, const float& y, const char* name, const void* value, const int& index, const int& selected) const
{
	// draw variable name
	g_Renderer.text(x + 4.f, y, DT_LEFT, index == selected ? color::Gold : color::White, name);

	auto val = *reinterpret_cast<float*>(m_menuItems.at(index).data);

	// draw its value
	g_Renderer.text(x + m_menuRect.right - 3.f, y, DT_RIGHT, index == selected ? color::Gold : color::White, "%.2f", val);
}

void Menu::button(const float& x, const float& y, const char* name, const int& index, const int& selected) const
{
	// draw 'button'
	g_Renderer.text(x + m_menuRect.right / 2.f, y, DT_CENTER, index == selected ? color::Gold : color::White, name);
}

#endif // NEW_MENU