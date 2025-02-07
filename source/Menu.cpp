#include "Menu.h"
#include "Renderer.h"
#include "Utils.h"

using namespace plugin;

#ifdef USE_NEW_MENU
Menu::Menu()
	: m_selectedOptionIndex(0), m_isOpen(true), m_menuRect(0.f, 0.f, 0.f, 0.f)
{
	m_menuRect = { 100.0f, 100.0f, 202.0f, 1337.228f };

	// todo: menu header ("gta:sa funny mod", "how you doin?");

	auto esp_submenu = std::make_shared<SubMenu>("esp", "zoloft helps me everyday");
		esp_submenu->add_option(std::make_shared<BoolOption>("esp_Enabled",			g_Config->esp_Enabled,		"visuals"));
		esp_submenu->add_option(std::make_shared<BoolOption>("esp_Box",				g_Config->esp_Box,			"draws bounding box on peds"));
		esp_submenu->add_option(std::make_shared<BoolOption>("esp_Health",			g_Config->esp_Health,		"draws health bar on peds"));
		esp_submenu->add_option(std::make_shared<BoolOption>("esp_Armor",			g_Config->esp_Armor,		"draws armor bar on peds"));
		esp_submenu->add_option(std::make_shared<BoolOption>("esp_Outline",			g_Config->esp_Outline,		"outline makes things look sexier"));
		esp_submenu->add_option(std::make_shared<BoolOption>("esp_AimTarget",		g_Config->esp_AimTarget,	"know your customer"));
		esp_submenu->add_option(std::make_shared<BoolOption>("esp_Crosshair",		g_Config->esp_Crosshair,	"draws custom crosshair"));
		esp_submenu->add_option(std::make_shared<BoolOption>("esp_Radar",			g_Config->esp_Radar,		"draws peds on game radar"));
		esp_submenu->add_option(std::make_shared<BoolOption>("esp_Graffiti",		g_Config->esp_Graffiti,		"draws unpainted graffiti"));
		esp_submenu->add_option(std::make_shared<BoolOption>("esp_Collectibles",	g_Config->esp_Collectibles,	"draws collectibles"));
	add_option(std::move(esp_submenu));

	auto aim_submenu = std::make_shared<SubMenu>("aim", "bosch");
		aim_submenu->add_option(std::make_shared<BoolOption> ("aim_Enabled",			g_Config->aim_Enabled,								"silent aimbot"));
		aim_submenu->add_option(std::make_shared<BoolOption> ("aim_TargetVisible",		g_Config->aim_TargetVisible,						"checks if target visible"));
		aim_submenu->add_option(std::make_shared<BoolOption> ("aim_TargetOnScreen",		g_Config->aim_TargetOnScreen,						"checks if target on screen"));
		aim_submenu->add_option(std::make_shared<BoolOption> ("aim_GangsPriority",		g_Config->aim_GangsPriority,						"aimbot targets gangs first"));
		aim_submenu->add_option(std::make_shared<BoolOption> ("aim_CopsPriority",		g_Config->aim_CopsPriority,							"aimbot targets cops first"));
		aim_submenu->add_option(std::make_shared<BoolOption> ("aim_Smart",				g_Config->aim_Smart,								"bone closest to crosshair"));
		aim_submenu->add_option(std::make_shared<FloatOption>("aim_SmartFov",			g_Config->aim_SmartFov, 10.0f, 500.0f, 1.0f,		"fov in pixels"));
		aim_submenu->add_option(std::make_shared<BoolOption> ("aim_Randomize",			g_Config->aim_Randomize,							"randomize aim spot"));
		aim_submenu->add_option(std::make_shared<FloatOption>("aim_RandomizeFactor",	g_Config->aim_RandomizeFactor, 0.05f, 1.0f, 0.05f,	"randomize factor"));
		aim_submenu->add_option(std::make_shared<BoolOption> ("aim_PerfectAccuracy",	g_Config->aim_PerfectAccuracy,						"near perfect accuracy"));
		aim_submenu->add_option(std::make_shared<BoolOption> ("aim_SniperRifle",		g_Config->aim_SniperRifle,							"it works but its very ridiculous\nyou need to shoot yourself in the foot\nsounds like another one c++ joke\nbut its not. also hits through walls"));
	add_option(std::move(aim_submenu));

	auto misc_submenu = std::make_shared<SubMenu>("misc", "why im here?");
		misc_submenu->add_option(std::make_shared<BoolOption>("sp_InvPlayer",			g_Config->sp_InvPlayer,			"invulnerable player"));
		misc_submenu->add_option(std::make_shared<BoolOption>("sp_InvCar",				g_Config->sp_InvCar,			"invulnerable player car"));
		misc_submenu->add_option(std::make_shared<BoolOption>("sp_NeverWanted",			g_Config->sp_NeverWanted,		"cops dont really care"));
		misc_submenu->add_option(std::make_shared<BoolOption>("sp_RussianMostWanted",	g_Config->sp_RussianMostWanted,	"kizaru mode"));
		misc_submenu->add_option(std::make_shared<BoolOption>("sp_AimIgnoreGroove",		g_Config->sp_AimIgnoreGroove,	"aimbot will ignore greenpeace members"));
		misc_submenu->add_option(std::make_shared<BoolOption>("sp_Speedhack",			g_Config->sp_Speedhack,			"explore the world on foot faster"));
		misc_submenu->add_option(std::make_shared<BoolOption>("sp_SpeedhackFaster",		g_Config->sp_SpeedhackFaster,	"waaaay faster"));
		misc_submenu->add_option(std::make_shared<BoolOption>("sp_NoReload",			g_Config->sp_NoReload,			"now your gun hungry af"));
		misc_submenu->add_option(std::make_shared<BoolOption>("sp_LegSlide",			g_Config->sp_LegSlide,			"ewww you stepped on shit")); // tony hawk's pro skater
		misc_submenu->add_option(std::make_shared<BoolOption>("sp_Telekinesis",			g_Config->sp_Telekinesis,		"tp peds infront of player"));
		misc_submenu->add_option(std::make_shared<BoolOption>("sp_MinigunMode",			g_Config->sp_MinigunMode,		"turn CiJey into destroy machine"));
		misc_submenu->add_option(std::make_shared<BoolOption>("sp_DumbassPeds",			g_Config->sp_DumbassPeds,		"superpower everyone wants\nbe careful, you can get softlock on mission with this"));
		misc_submenu->add_option(std::make_shared<BoolOption>("sp_Airbrake",			g_Config->sp_Airbrake,			"break this stupid air"));
		misc_submenu->add_option(std::make_shared<BoolOption>("sp_AirbrakeMode",		g_Config->sp_AirbrakeMode,		"disabled: mod_sa original\nenabled: follows camera"));
	add_option(std::move(misc_submenu));

	auto funcs_submenu = std::make_shared<SubMenu>("funcs", "what is the meaning of my life");
		funcs_submenu->add_option(std::make_shared<FuncOption>("fn_GiveAmmo",			functions::give_ammo,				"gives 100 bullets to weapon in hands"));
		funcs_submenu->add_option(std::make_shared<FuncOption>("fn_GiveMoney",			functions::give_money,				"gives three hundred bucks to poor CiJey"));
		funcs_submenu->add_option(std::make_shared<FuncOption>("fn_GiveMinigun",		functions::give_minigun,			"gives minigun"));
		funcs_submenu->add_option(std::make_shared<FuncOption>("fn_Collectibles",		functions::collect_collectibles,	"collect all collectibles & paint all graffiti"));
		funcs_submenu->add_option(std::make_shared<FuncOption>("fn_UnlockNearbyCars",	functions::unlock_nearby_cars,		"now you can kick that stupid cops out of their cars"));
	add_option(std::move(funcs_submenu));
}

void Menu::on_draw()
{
	if (!m_isOpen) return;

	const auto update_visible_options = [&]() -> void { // ghetto c+p codez by deepseek or chatgpt. sorry.
		if (!m_visibleOptions.empty()) m_visibleOptions.clear();

		for (const auto& item : m_options)
		{
			m_visibleOptions.push_back(item);
			if (auto sub_menu = dynamic_cast<SubMenu*>(item.get()))
			{
				if (sub_menu->is_open())
				{
					for (const auto& sub_item : sub_menu->get_options()) {
						m_visibleOptions.push_back(sub_item);
					}
				}
			}
		}
	};
	update_visible_options();

	if (m_options.empty() || m_visibleOptions.empty()) return;

	const auto& options_count = m_visibleOptions.size(); // кол-во видимых (с учётом открытых подменю) опций

	float x = m_menuRect.left,
			y = m_menuRect.top,
			w = m_menuRect.right,
			h = float(options_count * FONT_SIZE); // dynamic height calculation

	// draw menu box
	g_Renderer->rect_out(x, y - 2.f, w, h + 4.f, CRGBA(100, 100, 100, 127), CRGBA(200, 200, 200, 200));

	// draw controls help
	g_Renderer->text(x + w + 4.f, y, DT_LEFT, color::White,
		"controls:\n"
		"INSERT: show/hide menu\n"
		"UP/DOWN: select\n"
		"LEFT/RIGHT: open/close | toggle | change\n"
		"DELETE: panic\n\n"
		"RSHIFT: airbrake toggle\n"
		"NUMPAD0: airbrake slowdown\n"
		"Z: anim reset");

	// отрисовка названий опций и их состояний
	for (auto i = 0u; i < options_count; ++i)
	{
		const CRGBA& nameColor = (i == m_selectedOptionIndex) ? color::Gold : color::White;

		// вызываем метод on_draw у текущей опции
		m_visibleOptions.at(i)->on_draw(x, y, w, nameColor);

		// смещаем следующую опцию вниз
		y += FONT_SIZE;
	}
	
	float desc_x = x + w / 2.f; // центр меню
	float desc_y = y + 4.f; // позиция описания под меню

	std::string description = m_visibleOptions.at(m_selectedOptionIndex)->description(); // получаем описание активной опции
	g_Renderer->text(desc_x, desc_y, DT_CENTER, color::White, "%s", description.c_str()); // рисуем
}

bool Menu::on_input(int key_code)
{
	if (key_code == RsKeyCodes::rsINS) m_isOpen ^= 1; // change visible state
	if (!m_isOpen) return false; // return if closed

	if (m_visibleOptions.empty()) return false;

	const auto& options_count = m_visibleOptions.size();

	if (key_code == RsKeyCodes::rsUP)
		m_selectedOptionIndex = (m_selectedOptionIndex - 1 + options_count) % options_count;
	else if (key_code == RsKeyCodes::rsDOWN)
		m_selectedOptionIndex = (m_selectedOptionIndex + 1) % options_count;

	if (m_selectedOptionIndex < options_count)
		m_visibleOptions.at(m_selectedOptionIndex)->on_input(key_code);

	return true;
}

void BoolOption::on_draw(float x, float y, float w, const CRGBA& color)
{
	// draw variable name
	g_Renderer->text(x + 4.f, y, DT_LEFT, color, m_optionName.c_str());

	// draw its state
	g_Renderer->text(x + w - 3.f, y, DT_RIGHT, m_optionValue ? color::LimeGreen : color::Tomato, m_optionValue ? "enabled" : "disabled");
}

void BoolOption::on_input(int key_code)
{
	if (key_code == RsKeyCodes::rsLEFT || key_code == RsKeyCodes::rsRIGHT)
		m_optionValue ^= 1; // change value state
}

void IntOption::on_draw(float x, float y, float w, const CRGBA& color)
{
	// draw variable name
	g_Renderer->text(x + 4.f, y, DT_LEFT, color, m_optionName.c_str());

	// draw its value
	g_Renderer->text(x + w - 3.f, y, DT_RIGHT, color, "%d", m_optionValue);
}

void IntOption::on_input(int key_code)
{
	if (key_code == RsKeyCodes::rsRIGHT)
		m_optionValue = std::min(m_optionValue + 1, m_optionValueMax); // +	
	else if (key_code == RsKeyCodes::rsLEFT)
		m_optionValue = std::max(m_optionValue - 1, m_optionValueMin); // -
}

void FloatOption::on_draw(float x, float y, float w, const CRGBA& color)
{
	// draw variable name
	g_Renderer->text(x + 4.f, y, DT_LEFT, color, m_optionName.c_str());

	// draw its value
	g_Renderer->text(x + w - 3.f, y, DT_RIGHT, color, "%.2f", m_optionValue);
}

void FloatOption::on_input(int key_code)
{
	if (key_code == RsKeyCodes::rsRIGHT)
		m_optionValue = std::min(m_optionValue + m_optionValueStep, m_optionValueMax); // +	
	else if (key_code == RsKeyCodes::rsLEFT)
		m_optionValue = std::max(m_optionValue - m_optionValueStep, m_optionValueMin); // -
}

void FuncOption::on_draw(float x, float y, float w, const CRGBA& color)
{
	// draw 'button'
	g_Renderer->text(x + w / 2.f, y, DT_CENTER, color, m_optionName.c_str());
}

void FuncOption::on_input(int key_code)
{
	if (key_code == RsKeyCodes::rsLEFT || key_code == RsKeyCodes::rsRIGHT)
		m_optionFn(); // call a function
}

void SubMenu::on_draw(float x, float y, float w, const CRGBA& color)
{
	g_Renderer->text(x + w / 2.f, y, DT_CENTER, color, m_isOpen ? "----> [ %s ] <----" : "----- [ %s ] -----", m_optionName.c_str());

	// отказываемся от этого, так как в конце концов мы как всегда идём по какому-то ебаному пути...
	/*
	// если подменю закрыто или в нём нет элементов, рисуем только название
	if (!m_isOpen || m_options.empty()) return;

	// если подменю открыто, рисуем все его опции
	float current_y = y + FONT_SIZE;
	for (auto& option : m_options) {
		option->on_draw(x, current_y, w, color);
		current_y += FONT_SIZE;
	}
	*/
}

void SubMenu::on_input(int key_code)
{
	if (key_code == RsKeyCodes::rsLEFT || key_code == RsKeyCodes::rsRIGHT)
		m_isOpen ^= 1; // change visible state
}

namespace menu_api
{
	std::stack<std::shared_ptr<SubMenu>> submenu;

	void begin_submenu(const std::string& name, const std::string& description)
	{
		auto ptr = std::make_shared<SubMenu>(name, description);
		submenu.push(ptr);
	}

	void end_submenu()
	{
		g_Menu->add_option(std::move(submenu.top()));
		submenu.pop();
	}

	void add_bool(const std::string& name, bool& value, const std::string& description)	{
		submenu.top()->add_option(std::make_shared<BoolOption>(name, value, description));
	}

	void add_int(const std::string& name, int& value, int min_value, int max_value, const std::string& description) {
		submenu.top()->add_option(std::make_shared<IntOption>(name, value, min_value, max_value, description));
	}

	void add_float(const std::string& name, float& value, float min_value, float max_value, float step, const std::string& description) {
		submenu.top()->add_option(std::make_shared<FloatOption>(name, value, min_value, max_value, step, description));
	}

	void add_func(const std::string& name, func_fn func, const std::string& description) {
		submenu.top()->add_option(std::make_shared<FuncOption>(name, func, description));
	}
}
#endif // USE_NEW_MENU

#ifdef USE_OLD_MENU
Menu::Menu()
	: m_isOpen(true), m_selectedOption(0u)
{
	// set menu items
	m_menuItems.emplace_back("gta:sa funny mod",		ItemType::MENUITEM_SUBMENU, nullptr,							"how you doin?");
	m_menuItems.emplace_back("----- [ esp ] -----",		ItemType::MENUITEM_SUBMENU, nullptr,							"zoloft helps me everyday");
	m_menuItems.emplace_back("esp_Enabled",				ItemType::MENUITEM_TOGGLE,	&g_Config->esp_Enabled,				"visuals");
	m_menuItems.emplace_back("esp_Box",					ItemType::MENUITEM_TOGGLE,	&g_Config->esp_Box,					"draws bounding box on peds");
	m_menuItems.emplace_back("esp_Health",				ItemType::MENUITEM_TOGGLE,	&g_Config->esp_Health,				"draws health bar on peds");
	m_menuItems.emplace_back("esp_Armor",				ItemType::MENUITEM_TOGGLE,	&g_Config->esp_Armor,				"draws armor bar on peds");
	m_menuItems.emplace_back("esp_Outline",				ItemType::MENUITEM_TOGGLE,	&g_Config->esp_Outline,				"outline makes things look sexier");
	m_menuItems.emplace_back("esp_AimTarget",			ItemType::MENUITEM_TOGGLE,	&g_Config->esp_AimTarget,			"know your customer");
	m_menuItems.emplace_back("esp_Crosshair",			ItemType::MENUITEM_TOGGLE,	&g_Config->esp_Crosshair,			"draws custom crosshair");
	m_menuItems.emplace_back("esp_Radar",				ItemType::MENUITEM_TOGGLE,	&g_Config->esp_Radar,				"draws peds on game radar");
	m_menuItems.emplace_back("esp_Graffiti",			ItemType::MENUITEM_TOGGLE,	&g_Config->esp_Graffiti,				"draws unpainted graffiti");
	m_menuItems.emplace_back("esp_Collectibles",		ItemType::MENUITEM_TOGGLE,	&g_Config->esp_Collectibles,			"draws collectibles");
	m_menuItems.emplace_back("----- [ aim ] -----",		ItemType::MENUITEM_SUBMENU, nullptr,							"bosch");
	m_menuItems.emplace_back("aim_Enabled",				ItemType::MENUITEM_TOGGLE,	&g_Config->aim_Enabled,				"silent aimbot");
	m_menuItems.emplace_back("aim_TargetVisible",		ItemType::MENUITEM_TOGGLE,	&g_Config->aim_TargetVisible,		"checks if target visible");
	m_menuItems.emplace_back("aim_TargetOnScreen",		ItemType::MENUITEM_TOGGLE,	&g_Config->aim_TargetOnScreen,		"checks if target on screen");
	m_menuItems.emplace_back("aim_GangsPriority",		ItemType::MENUITEM_TOGGLE,	&g_Config->aim_GangsPriority,		"aimbot targets gangs first");
	m_menuItems.emplace_back("aim_CopsPriority",		ItemType::MENUITEM_TOGGLE,	&g_Config->aim_CopsPriority,			"aimbot targets cops first");
	m_menuItems.emplace_back("aim_Smart",				ItemType::MENUITEM_TOGGLE,	&g_Config->aim_Smart,				"bone closest to crosshair");
	m_menuItems.emplace_back("aim_SmartFov",			ItemType::MENUITEM_VALUE,	&g_Config->aim_SmartFov,				"fov in pixels");
	m_menuItems.emplace_back("aim_Randomize",			ItemType::MENUITEM_TOGGLE,	&g_Config->aim_Randomize,			"randomize aim spot");
	m_menuItems.emplace_back("aim_RandomizeFactor",		ItemType::MENUITEM_VALUE,	&g_Config->aim_RandomizeFactor,		"randomize factor");
	m_menuItems.emplace_back("aim_PerfectAccuracy",		ItemType::MENUITEM_TOGGLE,	&g_Config->aim_PerfectAccuracy,		"near perfect accuracy");
	m_menuItems.emplace_back("aim_SniperRifle",			ItemType::MENUITEM_TOGGLE,	&g_Config->aim_SniperRifle,			"it works but its very ridiculous\nyou need to shoot yourself in the foot\nsounds like another one c++ joke\nbut its not. also hits through walls");
	m_menuItems.emplace_back("----- [ misc ] -----",	ItemType::MENUITEM_SUBMENU, nullptr,							"why im here?");
	m_menuItems.emplace_back("sp_InvPlayer",			ItemType::MENUITEM_TOGGLE,	&g_Config->sp_InvPlayer,				"invulnerable player");
	m_menuItems.emplace_back("sp_InvCar",				ItemType::MENUITEM_TOGGLE,	&g_Config->sp_InvCar,				"invulnerable player car");
	m_menuItems.emplace_back("sp_NeverWanted",			ItemType::MENUITEM_TOGGLE,	&g_Config->sp_NeverWanted,			"cops dont really care");
	m_menuItems.emplace_back("sp_RussianMostWanted",	ItemType::MENUITEM_TOGGLE,	&g_Config->sp_RussianMostWanted,		"kizaru mode");
	m_menuItems.emplace_back("sp_AimIgnoreGroove",		ItemType::MENUITEM_TOGGLE,	&g_Config->sp_AimIgnoreGroove,		"aimbot will ignore greenpeace members");
	m_menuItems.emplace_back("sp_Speedhack",			ItemType::MENUITEM_TOGGLE,	&g_Config->sp_Speedhack,				"explore the world on foot faster");
	m_menuItems.emplace_back("sp_SpeedhackFaster",		ItemType::MENUITEM_TOGGLE,	&g_Config->sp_SpeedhackFaster,		"waaaay faster");
	m_menuItems.emplace_back("sp_NoReload",				ItemType::MENUITEM_TOGGLE,	&g_Config->sp_NoReload,				"now your gun hungry af");
	m_menuItems.emplace_back("sp_LegSlide",				ItemType::MENUITEM_TOGGLE,	&g_Config->sp_LegSlide,				"ewww you stepped on shit"); // tony hawk's pro skater
	m_menuItems.emplace_back("sp_Telekinesis",			ItemType::MENUITEM_TOGGLE,	&g_Config->sp_Telekinesis,			"tp peds infront of player");
	m_menuItems.emplace_back("sp_MinigunMode",			ItemType::MENUITEM_TOGGLE,	&g_Config->sp_MinigunMode,			"turn CiJey into destroy machine");
	m_menuItems.emplace_back("sp_DumbassPeds",			ItemType::MENUITEM_TOGGLE,	&g_Config->sp_DumbassPeds,			"superpower everyone wants\nbe careful, you can get softlock on mission with this");
	m_menuItems.emplace_back("sp_Airbrake",				ItemType::MENUITEM_TOGGLE,	&g_Config->sp_Airbrake,				"break this stupid air");
	m_menuItems.emplace_back("sp_AirbrakeMode",			ItemType::MENUITEM_TOGGLE,	&g_Config->sp_AirbrakeMode,			"disabled: mod_sa original\nenabled: follows camera");
	m_menuItems.emplace_back("----- [ funcs ] -----",	ItemType::MENUITEM_SUBMENU, nullptr,							"what is the meaning of my life");
	m_menuItems.emplace_back("fn_GiveAmmo",				ItemType::MENUITEM_BUTTON,	functions::give_ammo,				"gives 100 bullets to weapon in hands");
	m_menuItems.emplace_back("fn_GiveMoney",			ItemType::MENUITEM_BUTTON,	functions::give_money,				"gives three hundred bucks to poor CiJey");
	m_menuItems.emplace_back("fn_GiveMinigun",			ItemType::MENUITEM_BUTTON,	functions::give_minigun,			"gives minigun");
	m_menuItems.emplace_back("fn_Collectibles",			ItemType::MENUITEM_BUTTON,	functions::collect_collectibles,	"collect all collectibles & paint all graffiti");
	m_menuItems.emplace_back("fn_UnlockNearbyCars",		ItemType::MENUITEM_BUTTON,	functions::unlock_nearby_cars,		"now you can kick that stupid cops out of their cars");

	// set menu position and size
	m_menuRect = { 100.0f, 100.0f, 202.0f, float(m_menuItems.size() * FONT_SIZE) };
}

void Menu::on_draw()
{
	if (!m_isOpen) return;

	float x = m_menuRect.left, y = m_menuRect.top, w = m_menuRect.right, h = m_menuRect.bottom;

	// draw menu box
	g_Renderer->rect_out(x, y - 2.f, w, h + 6.f, CRGBA(100, 100, 100, 127), CRGBA(200, 200, 200, 200));

	// draw controls help
	g_Renderer->text(x + w + 4.f, y, DT_LEFT, color::White,
		"controls:\n"
		"INSERT: show/hide menu\n"
		"UP/DOWN: select\n"
		"LEFT/RIGHT: toggle/change option\n"
		"DELETE: panic\n\n"
		"RSHIFT: airbrake toggle\n"
		"NUMPAD0: airbrake slowdown\n"
		"Z: anim reset");

	// draw menu items
	for (auto i = 0u; i < m_menuItems.size(); i++)
	{
		const auto& color = i == m_selectedOption ? color::Gold : color::White;

		const auto& item = m_menuItems.at(i);
		switch (item.type)
		{
			case ItemType::MENUITEM_SUBMENU:
			{
				g_Renderer->text(x + w / 2.f, y, DT_CENTER, color, item.name); // draw 'button'
				break;
			}
			case ItemType::MENUITEM_TOGGLE:
			{
				if (!item.data) break;
				auto val = *reinterpret_cast<bool*>(item.data); // read value
				g_Renderer->text(x + 4.f, y, DT_LEFT, color, item.name); // draw variable name
				g_Renderer->text(x + w - 3.f, y, DT_RIGHT, val ? color::LimeGreen : color::Tomato, val ? "enabled" : "disabled"); // draw its state
				break;
			}
			case ItemType::MENUITEM_VALUE:
			{
				if (!item.data) break;
				auto val = *reinterpret_cast<float*>(item.data); // read value
				g_Renderer->text(x + 4.f, y, DT_LEFT, color, item.name); // draw variable name
				g_Renderer->text(x + w - 3.f, y, DT_RIGHT, color, "%.2f", val); // draw its value
				break;
			}
			case ItemType::MENUITEM_BUTTON:
			{
				if (!item.data) break;
				g_Renderer->text(x + w / 2.f, y, DT_CENTER, color, item.name);
				break;
			}
			default:
				break;
		}
		y += FONT_SIZE;
	}

	// draw feature description
	g_Renderer->text(x + w / 2.f, m_menuRect.top + h + 8.f, DT_CENTER, color::White, m_menuItems.at(m_selectedOption).description);
}

bool Menu::on_input(int key_code)
{
	if (key_code == RsKeyCodes::rsINS) m_isOpen = !m_isOpen; // change visible state
	if (!m_isOpen) return false;

	const int& options_count = m_menuItems.size();

	// navigation
	if (key_code == RsKeyCodes::rsUP)
		m_selectedOption = (m_selectedOption - 1 + options_count) % options_count;
	else if (key_code == RsKeyCodes::rsDOWN)
		m_selectedOption = (m_selectedOption + 1) % options_count;

	auto& item = m_menuItems.at(m_selectedOption);

	// change value or call function
	if (key_code == RsKeyCodes::rsLEFT || key_code == RsKeyCodes::rsRIGHT)
	{
		if (item.type == ItemType::MENUITEM_TOGGLE)
			*reinterpret_cast<bool*>(item.data) ^= 1; // change value state
		else if (item.type == ItemType::MENUITEM_BUTTON && item.data)
			reinterpret_cast<fn>(item.data)();
		else if (item.type == ItemType::MENUITEM_VALUE) // todo: min/max for value
		{
			auto& val = *reinterpret_cast<float*>(item.data);
			if (key_code == RsKeyCodes::rsLEFT)
				val = std::max(val - 0.1f, 0.0f);
			else if (key_code == RsKeyCodes::rsRIGHT)
				val = std::min(val + 0.1f, 999.0f);
		}
	}

	return true;
}
#endif // USE_OLD_MENU

#ifdef USE_ZGUI_MENU
#include "zgui/zgui.hh"
#include "Hooked.h"

namespace wrapper
{
	static void line(float x, float y, float x2, float y2, zgui::color c) noexcept				{ g_Renderer->line(x, y, x2, y2, CRGBA(c.r, c.g, c.b, c.a)); }
	static void rect(float x, float y, float x2, float y2, zgui::color c) noexcept				{ g_Renderer->border_box(x, y, x2 - 1, y2 - 1, 1.0f, CRGBA(c.r, c.g, c.b, c.a)); }
	static void filled_rect(float x, float y, float x2, float y2, zgui::color c) noexcept		{ g_Renderer->rect(x, y, x2, y2, CRGBA(c.r, c.g, c.b, c.a)); }
	static void text(float x, float y, zgui::color c, bool center, const char* text) noexcept	{ g_Renderer->text(x, y, center ? DT_CENTER : DT_LEFT, CRGBA(c.r, c.g, c.b, c.a), text); }
	static void get_text_size(const char* text, int& width, int& height) noexcept				{ g_Renderer->text_size(text, width, height); }
	static float get_frametime() noexcept														{ return 1.0f; }

	static CVector2D get_cursor_pos(HWND hwnd)
	{
		POINT p_mouse_pos;
		GetCursorPos(&p_mouse_pos);
		ScreenToClient(hwnd, &p_mouse_pos);

		return CVector2D(p_mouse_pos.x, p_mouse_pos.y);
	}
}

void zgui_input_hook() // todo: warning! there is always a chance that GMH or GPA can fail!
{
	auto user32 = GetModuleHandleA("user32.dll");
	if (!user32) return;

	auto scp = GetProcAddress(user32, "SetCursorPos");
	if (!scp) return;
			
	g_hookSetCursorPos = safetyhook::create_inline(scp, hooked::SetCursorPos);
}

Menu::Menu()
{
	zgui::functions.draw_line = wrapper::line;
	zgui::functions.draw_rect = wrapper::rect;
	zgui::functions.draw_filled_rect = wrapper::filled_rect;
	zgui::functions.draw_text = wrapper::text;
	zgui::functions.get_text_size = wrapper::get_text_size;
	zgui::functions.get_frametime = wrapper::get_frametime;

	zgui::globals::window_ctx.position = { 50, 50 };
	zgui::globals::window_ctx.opened = true;
}

void Menu::on_draw()
{
	zgui::poll_input(RsGlobal.ps->window);

	if (zgui::begin_window("gta:sa funny mod v1.0 by rosko", { 533, 287 }, 0, zgui::zgui_window_flags_no_ontoggle_animation))
	{
		zgui::begin_groupbox("Aimbot", { 161, 248 });
			zgui::checkbox("enabled #aim_Enabled", g_Config->aim_Enabled);
			zgui::checkbox("target only visible #aim_TargetVisible", g_Config->aim_TargetVisible);
			zgui::checkbox("target only on screen #aim_TargetOnScreen", g_Config->aim_TargetOnScreen);
			zgui::checkbox("gangs priority #aim_GangsPriority", g_Config->aim_GangsPriority);
			zgui::checkbox("cops priority #aim_CopsPriority", g_Config->aim_CopsPriority);
			zgui::checkbox("smart #aim_Smart", g_Config->aim_Smart);
			zgui::slider_float("fov in px #aim_SmartFov", 5.0f, 200.0f, g_Config->aim_SmartFov);
			zgui::checkbox("randomize #aim_Randomize", g_Config->aim_Randomize);
			zgui::slider_float("factor #aim_RandomizeFactor", 0.0f, 1.0f, g_Config->aim_RandomizeFactor);
			zgui::checkbox("perfect accuracy #aim_PerfectAccuracy", g_Config->aim_PerfectAccuracy);
			zgui::checkbox("sniper rifle #aim_SniperRifle", g_Config->aim_SniperRifle);
		zgui::end_groupbox();

		zgui::next_column(171, 0);

		zgui::begin_groupbox("ESP", { 100, 118 });
			zgui::checkbox("enabled #esp_Enabled", g_Config->esp_Enabled);
			zgui::checkbox("box #esp_Box", g_Config->esp_Box);
			zgui::checkbox("health #esp_Health", g_Config->esp_Health);
			zgui::checkbox("armor #esp_Armor", g_Config->esp_Armor);
			zgui::checkbox("outlined #esp_Outline", g_Config->esp_Outline);
			zgui::checkbox("aimbot target #esp_AimTarget", g_Config->esp_AimTarget);
		zgui::end_groupbox();

		zgui::begin_groupbox("Extra", { 100, 54 });
			zgui::checkbox("crosshair #esp_Crosshair", g_Config->esp_Crosshair);
			zgui::checkbox("radar #esp_Radar", g_Config->esp_Radar);
		zgui::end_groupbox();

		zgui::next_column(110, 0);

		zgui::begin_groupbox("Misc", { 100, 178 });
			zgui::checkbox("godmode #sp_InvPlayer", g_Config->sp_InvPlayer);
			zgui::checkbox("car godmode #sp_InvCar", g_Config->sp_InvCar);
			zgui::checkbox("speedhack #sp_Speedhack", g_Config->sp_Speedhack);
			zgui::checkbox("speedhack faster #sp_SpeedhackFaster", g_Config->sp_SpeedhackFaster);
			zgui::checkbox("leg slide #sp_LegSlide", g_Config->sp_LegSlide);
			zgui::checkbox("telekinesis #sp_Telekinesis", g_Config->sp_Telekinesis);
			zgui::checkbox("minigun mode #sp_MinigunMode", g_Config->sp_MinigunMode);
			zgui::checkbox("airbrake #sp_Airbrake", g_Config->sp_Airbrake);
			zgui::combobox("#sp_AirBrakeMode", { "s0beit", "follow cam" }, (int&)g_Config->sp_AirbrakeMode);
		zgui::end_groupbox();

		zgui::next_column(110, 0);

		zgui::begin_groupbox("SP only", { 110, 134 });
			zgui::checkbox("never wanted #sp_NeverWanted", g_Config->sp_NeverWanted);
			zgui::checkbox("ru most wanted #sp_RussianMostWanted", g_Config->sp_RussianMostWanted);
			zgui::checkbox("aim ignore groove #sp_AimIgnoreGroove", g_Config->sp_AimIgnoreGroove);
			zgui::checkbox("no reload #sp_NoReload", g_Config->sp_NoReload);
			zgui::checkbox("dumbass peds #sp_DumbassPeds", g_Config->sp_DumbassPeds);
			zgui::checkbox("graffiti #esp_Graffiti", g_Config->esp_Graffiti);
			zgui::checkbox("collectibles #esp_Collectibles", g_Config->esp_Collectibles);
		zgui::end_groupbox();

		zgui::end_window();

		auto cursor = wrapper::get_cursor_pos(RsGlobal.ps->window);
		g_Renderer->cursor(cursor.x, cursor.y, plugin::color::White);
	}
}

bool Menu::is_open() const
{
	return zgui::globals::window_ctx.opened;
}
#endif // USE_ZGUI_MENU