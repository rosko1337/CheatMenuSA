#pragma once

#include "Main.h"

#ifndef NEW_MENU

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
	bool sp_AirbrakeMode = false;
} extern g_Config;

enum class ItemType
{
	MENUITEM_SUBMENU = 0,
	MENUITEM_TOGGLE  = 1,
	MENUITEM_VALUE   = 2,
	MENUITEM_BUTTON  = 3
};

struct Item
{
	using fn = void(*)();

	Item(const int& _index = 0, const char* _name = nullptr, const ItemType& _type = ItemType::MENUITEM_SUBMENU, void* _data = nullptr, const char* _description = nullptr)
		: index(_index), name(_name), type(_type), data(_data), description(_description) { }

	Item(const Item&) = default;
	Item& operator=(const Item&) = default;

	Item(Item&&) = default;
	Item& operator=(Item&&) = default;

	const int	index;
	const char*	name;
	ItemType	type;
	void*		data;
	const char*	description;
};

class Menu
{
	bool				m_isOpen;
	int					m_selectedOption;
	std::vector<Item>	m_menuItems;
	CRect				m_menuRect;

public:
	Menu() : m_isOpen(true), m_selectedOption(0u) { };

	bool is_open();

	void on_init();
	void on_shutdown();

	void on_draw();
	bool on_input(RsEvent event, int key_code);

private:
	void toggle(const float& x, const float& y, const char* name, const void* value, const int& index, const int& selected) const;
	void value(const float& x, const float& y, const char* name, const void* value, const int& index, const int& selected) const;
	void button(const float& x, const float& y, const char* name, const int& index, const int& selected) const;
} extern g_Menu;

#endif // NEW_MENU