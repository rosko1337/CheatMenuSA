#include "UndoneMenu.h"
#include "Renderer.h"
#include "Utils.h"

using namespace plugin;

Config g_ConfigNew;
Menu g_MenuNew;

void Menu::on_draw()
{
	if (!m_isOpen)
		return;

	// draw menu box
	g_Renderer.rect_out(m_menuRect.left, m_menuRect.top, m_menuRect.right, m_menuRect.bottom + 6.f, CRGBA(100, 100, 100, 127), CRGBA(200, 200, 200, 200));

	// draw controls help
	g_Renderer.text(m_menuRect.left + m_menuRect.right + 4.f, m_menuRect.top, DT_LEFT, color::White,
		"controls:\nINSERT: show/hide menu\nUP/DOWN: select\nLEFT/RIGHT: toggle\nDELETE: panic\n\nRSHIFT: airbrake toggle\nNUMPAD0: airbrake slowdown");

	// draw feature description
	g_Renderer.text(m_menuRect.left + m_menuRect.right / 2.f, m_menuRect.top + m_menuRect.bottom + 8.f, DT_CENTER, color::White, m_menuItems.at(m_selectedOption).get_description());


	for (auto item : m_menuItems)
		item.on_draw();
}

void ItemSubmenu::on_draw()
{
	// todo: переделать
	// draw 'submenu'
	const auto state_ch = is_open ? '+' : '-';
	g_Renderer.text(parent.get_rect().left + parent.get_rect().right / 2.f, parent.get_rect().top + 2.f + (index * FONT_SIZE), DT_CENTER, index == parent.get_selected() ? color::Gold : color::White, "[%c] %s [%c]", state_ch, name, state_ch);

	if (!is_open)
		return;

	for (auto item : submenu_items)
		item.on_draw();
}
void ItemToggle::on_draw()
{
	//if (parent.is_open)

	// draw variable name
	g_Renderer.text(parent.get_rect().left + 4.f, parent.get_rect().top + 2.f + (index * FONT_SIZE), DT_LEFT, index == parent.get_selected() ? color::Gold : color::White, name);

	// draw its state
	g_Renderer.text(parent.get_rect().left + parent.get_rect().right - 3.f, parent.get_rect().top + 2.f + (index * FONT_SIZE), DT_RIGHT, value ? color::LimeGreen : color::Tomato, value ? "enabled" : "disabled");
}
void ItemButton::on_draw()
{
	// draw 'button'
	g_Renderer.text(parent.get_rect().left + parent.get_rect().right / 2.f, parent.get_rect().top + 2.f + (index * FONT_SIZE), DT_CENTER, index == parent.get_selected() ? color::Gold : color::White, name);
}
void ItemValue::on_draw()
{
	// draw variable name
	g_Renderer.text(parent.get_rect().left + 4.f, parent.get_rect().top + 2.f + (index * FONT_SIZE), DT_LEFT, index == parent.get_selected() ? color::Gold : color::White, name);

	// draw its value
	g_Renderer.text(parent.get_rect().left + parent.get_rect().right - 3.f, parent.get_rect().top + 2.f + (index * FONT_SIZE), DT_RIGHT, color::White, "%d", value);
}

Menu::Menu()
{
	m_isOpen = true;
	m_selectedOption = 0u;

	// set menu items
	std::size_t i = 0u;
	m_menuItems.emplace_back("gta:sa funny mod",	nullptr,						i++, nullptr, "how you doin?");
	m_menuItems.emplace_back("----- [ esp ] -----", nullptr,						i++, nullptr, "zoloft helps me everyday");
	m_menuItems.emplace_back("esp_Enabled",			&g_Config.esp_Enabled,			i++, nullptr, "visuals");
	m_menuItems.emplace_back("esp_Box",				&g_Config.esp_Box,				i++, nullptr, "draws bounding box on peds");
	m_menuItems.emplace_back("esp_Health",			&g_Config.esp_Health,			i++, nullptr, "draws health bar on peds");
	m_menuItems.emplace_back("esp_Armor",			&g_Config.esp_Armor,			i++, nullptr, "draws armor bar on peds");
	m_menuItems.emplace_back("esp_BarsDown",		&g_Config.esp_BarsDown,			i++, nullptr, "draws bars on down side");
	m_menuItems.emplace_back("esp_Outline",			&g_Config.esp_Outline,			i++, nullptr, "outline makes things look sexier");
	m_menuItems.emplace_back("esp_AimTarget",		&g_Config.esp_AimTarget,		i++, nullptr, "know your customer");
	m_menuItems.emplace_back("esp_Crosshair",		&g_Config.esp_Crosshair,		i++, nullptr, "draws custom crosshair");
	m_menuItems.emplace_back("esp_Radar",			&g_Config.esp_Radar,			i++, nullptr, "draws peds on game radar");
	m_menuItems.emplace_back("esp_Graffiti",		&g_Config.esp_Graffiti,			i++, nullptr, "draws unpainted graffiti");
	m_menuItems.emplace_back("esp_Collectibles",	&g_Config.esp_Collectibles,		i++, nullptr, "draws collectibles");
	m_menuItems.emplace_back("----- [ aim ] -----",	nullptr,						i++, nullptr, "bosch");
	m_menuItems.emplace_back("aim_Enabled",			&g_Config.aim_Enabled,			i++, nullptr, "silent aimbot");
	m_menuItems.emplace_back("aim_TargetVisible",	&g_Config.aim_TargetVisible,	i++, nullptr, "checks if target visible");
	m_menuItems.emplace_back("aim_TargetOnScreen",	&g_Config.aim_TargetOnScreen,	i++, nullptr, "checks if target on screen");
	m_menuItems.emplace_back("aim_GangsPriority",	&g_Config.aim_GangsPriority,	i++, nullptr, "aimbot targets gangs first");
	m_menuItems.emplace_back("aim_CopsPriority",	&g_Config.aim_CopsPriority,		i++, nullptr, "aimbot targets cops first");
	m_menuItems.emplace_back("aim_SmallFov",		&g_Config.aim_SmallFov,			i++, nullptr, "small field of view for aimbot (not done yet)");
	m_menuItems.emplace_back("aim_PerfectAccuracy", &g_Config.aim_PerfectAccuracy,	i++, nullptr, "near perfect accuracy");
	m_menuItems.emplace_back("aim_SniperRifle",		&g_Config.aim_SniperRifle,		i++, nullptr, "it works but its very ridiculous\nyou need to shoot yourself in the foot\nsounds like another one c++ joke\nbut its not. also hits through walls");
	m_menuItems.emplace_back("----- [ misc ] -----",nullptr,						i++, nullptr, "why im here?");
	m_menuItems.emplace_back("sp_InvPlayer",		&g_Config.sp_InvPlayer,			i++, nullptr, "invulnerable player");
	m_menuItems.emplace_back("sp_InvCar",			&g_Config.sp_InvCar,			i++, nullptr, "invulnerable player car");
	m_menuItems.emplace_back("sp_RussianMostWanted",&g_Config.sp_RussianMostWanted, i++, nullptr, "kizaru mode");
	m_menuItems.emplace_back("sp_NeverWanted",		&g_Config.sp_NeverWanted,		i++, nullptr, "cops dont really care");
	m_menuItems.emplace_back("sp_AimIgnoreGroove",	&g_Config.sp_AimIgnoreGroove,	i++, nullptr, "aimbot will ignore greenpeace members");
	m_menuItems.emplace_back("sp_Speedhack",		&g_Config.sp_Speedhack,			i++, nullptr, "explore the world on foot faster");
	m_menuItems.emplace_back("sp_SpeedhackFaster",	&g_Config.sp_SpeedhackFaster,	i++, nullptr, "waaaay faster");
	m_menuItems.emplace_back("sp_NoReload",			&g_Config.sp_NoReload,			i++, nullptr, "now your gun hungry af");
	m_menuItems.emplace_back("sp_LegSlide",			&g_Config.sp_LegSlide,			i++, nullptr, "ewww you stepped on shit"); // tony hawk's pro skater
	m_menuItems.emplace_back("sp_DumbassPeds",		&g_Config.sp_DumbassPeds,		i++, nullptr, "superpower everyone wants\nbe careful, you can get softlock on mission with this");
	m_menuItems.emplace_back("sp_Airbrake",			&g_Config.sp_Airbrake,			i++, nullptr, "break this stupid air");
	m_menuItems.emplace_back("----- [ func ] -----",nullptr,						i++, nullptr, "what is the meaning of my life");
	m_menuItems.emplace_back("fn_GiveAmmo",			nullptr,						i++, functions::give_ammo, "gives 100 bullets to weapon in hands");
	m_menuItems.emplace_back("fn_GiveMoney",		nullptr,						i++, functions::give_money, "gives three hundred bucks to poor CiJey");
	m_menuItems.emplace_back("fn_GiveMinigun",		nullptr,						i++, functions::give_minigun, "gives minigun");
	m_menuItems.emplace_back("fn_Collectibles",		nullptr,						i++, functions::collect_collectibles, "collect all collectibles & paint all graffiti");
	m_menuItems.emplace_back("fn_UnlockNearbyCars",	nullptr,						i++, functions::unlock_nearby_cars, "now you can kick that stupid cops out of their cars");

	// set menu position and size
	m_menuRect = CRect(100.0f, 100.0f, 202.0f, float(m_menuItems.size() * FONT_SIZE));
}

bool Menu::is_open()
{
	return m_isOpen;
}

/*void Menu::on_draw()
{
	if (!m_isOpen)
		return;

	// draw menu box
	g_Renderer.rect_out(m_menuRect.left, m_menuRect.top, m_menuRect.right, m_menuRect.bottom + 6.f, CRGBA(100, 100, 100, 127), CRGBA(200, 200, 200, 200));

	// draw controls help
	g_Renderer.text(m_menuRect.left + m_menuRect.right + 4.f, m_menuRect.top, DT_LEFT, color::White,
		"controls:\nINSERT: show/hide menu\nUP/DOWN: select\nLEFT/RIGHT: toggle\nDELETE: panic\n\nRSHIFT: airbrake toggle\nNUMPAD0: airbrake slowdown");

	// draw menu items
	for (auto q = 0u; q < m_menuItems.size(); q++)
	{
		if (m_menuItems.at(q).value)
			toggle(m_menuRect.left, m_menuRect.top + 2.f + (q * FONT_SIZE), m_menuItems.at(q).name, *m_menuItems.at(q).value, m_menuItems.at(q).index, m_selectedOption);
		else
			button(m_menuRect.left, m_menuRect.top + 2.f + (q * FONT_SIZE), m_menuItems.at(q).name, m_menuItems.at(q).index, m_selectedOption);
	}

	// draw feature description
	g_Renderer.text(m_menuRect.left + m_menuRect.right / 2.f, m_menuRect.top + m_menuRect.bottom + 8.f, DT_CENTER, color::White, m_menuItems.at(m_selectedOption).description);
}*/

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

		// workaround to skip 'separators'. dont ask me why this is so complicated
		while (!m_menuItems.at(m_selectedOption).value && !m_menuItems.at(m_selectedOption).function)
			m_selectedOption = m_selectedOption == 0 ? m_menuItems.size() - 1 : m_selectedOption - 1;
	}
	else if (key_code == RsKeyCodes::rsDOWN)
	{
		m_selectedOption++;

		if (m_selectedOption > m_menuItems.size() - 1)
			m_selectedOption = 0;

		// same
		while (!m_menuItems.at(m_selectedOption).value && !m_menuItems.at(m_selectedOption).function)
			m_selectedOption++;
	}

	// change value or call function
	if (key_code == RsKeyCodes::rsLEFT || key_code == RsKeyCodes::rsRIGHT)
	{
		if (m_menuItems.at(m_selectedOption).value)
			*m_menuItems.at(m_selectedOption).value ^= 1;

		if (m_menuItems.at(m_selectedOption).function)
			m_menuItems.at(m_selectedOption).function();
	}

	return true;
}

void Menu::toggle(const float& x, const float& y, const char* name, const bool& value, const int& index, const int& selected) const
{
	// draw variable name
	g_Renderer.text(x + 4.f, y, DT_LEFT, index == selected ? color::Gold : color::White, name);

	// draw its state
	g_Renderer.text(x + m_menuRect.right - 3.f, y, DT_RIGHT, value ? color::LimeGreen : color::Tomato, value ? "enabled" : "disabled");
}

void Menu::button(const float& x, const float& y, const char* name, const int& index, const int& selected) const
{
	// draw 'button'
	g_Renderer.text(x + m_menuRect.right / 2.f, y, DT_CENTER, index == selected ? color::Gold : color::White, name);
}