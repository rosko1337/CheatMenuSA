#pragma once

#include "Main.h"

struct Config
{
	bool esp_Enabled = true;
	bool esp_Box = true;
	bool esp_Health = true;
	bool esp_Armor = true;
	bool esp_BarsDown = true;
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
	bool aim_SmallFov = false; // todo: ...
	bool aim_PerfectAccuracy = true;
	bool aim_SniperRifle = false;

	bool sp_InvPlayer = true;
	bool sp_InvCar = true;
	bool sp_RussianMostWanted = false;
	bool sp_NeverWanted = true;
	bool sp_AimIgnoreGroove = true;
	bool sp_Speedhack = false;
	bool sp_SpeedhackFaster = false;
	bool sp_NoReload = true;
	bool sp_LegSlide = true;
	bool sp_DumbassPeds = false;
	bool sp_Airbrake = false;
	bool sp_Drunk = false;
} extern g_ConfigNew;

//item_type_toggle
//item_type_value
//item_type_button
//item_type_submenu

//union {
//	int value;
//	float value;
//};

enum class ItemType
{
	MENUITEM_NULL = 0,
	MENUITEM_SUBMENU = 1,
	MENUITEM_TOGGLE = 2,
	MENUITEM_VALUE = 3,
	MENUITEM_BUTTON = 4,
};

class ItemBase
{
protected:
	const int		index;
	const ItemType	type;
	const char*		name;
	const char*		description;
	class Menu		parent;

public:
	ItemBase(const int& _index = 0, const ItemType& _type = ItemType::MENUITEM_NULL, const char* _name = nullptr, const char* _description = nullptr)
		: index(_index), type(_type), name(_name), description(_description) { }

	ItemBase(const ItemBase&) = default;
	ItemBase& operator=(const ItemBase&) = default;

	ItemBase(ItemBase&&) = default;
	ItemBase& operator=(ItemBase&&) = default;

	virtual ~ItemBase() = default;

	virtual void on_draw() = 0;
	virtual void on_input(int key_code) = 0;

	const char* get_description()
	{
		return description;
	}
};

class ItemSubmenu : public ItemBase
{
	bool					is_open;
	std::vector<ItemBase>	submenu_items;

public:
	ItemSubmenu(const int& _index = 0, const char* _name = nullptr, const char* _description = nullptr)
		: ItemBase(_index, ItemType::MENUITEM_SUBMENU, _name, _description), is_open(false), submenu_items() { }

	ItemSubmenu(const ItemSubmenu&) = default;
	ItemSubmenu& operator=(const ItemSubmenu&) = default;

	ItemSubmenu(ItemSubmenu&&) = default;
	ItemSubmenu& operator=(ItemSubmenu&&) = default;

	virtual ~ItemSubmenu() = default;

	void on_draw() override;
	void on_input(int key_code)
	{
		if (key_code == RsKeyCodes::rsLEFT || key_code == RsKeyCodes::rsRIGHT)
			is_open ^= 1;

		if (!is_open)
			return;

		for (auto item : submenu_items)
			item.on_input(key_code);
	}

	void add_item(ItemBase item)
	{
		submenu_items.push_back(item);
	}
};

class ItemToggle : public ItemBase
{
	bool* value;

public:
	ItemToggle(const int& _index = 0, const char* _name = nullptr, bool* _value = nullptr, const char* _description = nullptr)
		: ItemBase(_index, ItemType::MENUITEM_TOGGLE, _name, _description), value(_value) { }

	ItemToggle(const ItemToggle&) = default;
	ItemToggle& operator=(const ItemToggle&) = default;

	ItemToggle(ItemToggle&&) = default;
	ItemToggle& operator=(ItemToggle&&) = default;

	virtual ~ItemToggle() = default;

	void on_draw() override;
	void on_input(int key_code)
	{
		if (!value)
			return;

		if (key_code != RsKeyCodes::rsLEFT && key_code != RsKeyCodes::rsRIGHT)
			return;

		*value ^= 1;
	}
};

class ItemButton : public ItemBase
{
	using fn = void(*)();
	const fn function;

public:
	ItemButton(const int& _index = 0, const char* _name = nullptr, const fn& _function = nullptr, const char* _description = nullptr)
		: ItemBase(_index, ItemType::MENUITEM_BUTTON, _name, _description), function(_function) { }

	ItemButton(const ItemButton&) = default;
	ItemButton& operator=(const ItemButton&) = default;

	ItemButton(ItemButton&&) = default;
	ItemButton& operator=(ItemButton&&) = default;

	virtual ~ItemButton() = default;

	void on_draw() override;
	void on_input(int key_code)
	{
		if (!function)
			return;

		if (key_code != RsKeyCodes::rsLEFT && key_code != RsKeyCodes::rsRIGHT)
			return;

		function();
	}
};

class ItemValue : public ItemBase
{
	int* value;

public:
	ItemValue(const int& _index = 0, const char* _name = nullptr, int* _value = nullptr, const char* _description = nullptr)
		: ItemBase(_index, ItemType::MENUITEM_VALUE, _name, _description), value(_value) { }

	ItemValue(const ItemValue&) = default;
	ItemValue& operator=(const ItemValue&) = default;

	ItemValue(ItemValue&&) = default;
	ItemValue& operator=(ItemValue&&) = default;

	virtual ~ItemValue() = default;

	void on_draw() override;
	void on_input(int key_code)
	{
		if (!value)
			return;

		// todo: bound check

		if (key_code == RsKeyCodes::rsLEFT)
			*value--;

		if (key_code == RsKeyCodes::rsRIGHT)
			*value++;
	}
};

class Menu
{
	bool					m_isOpen;
	int						m_selectedOption;
	std::vector<ItemBase>	m_menuItems;
	CRect					m_menuRect;

public:
	Menu();

	bool is_open();

	void on_draw();
	bool on_input(RsEvent event, int key_code);

	auto get_rect() -> CRect
	{
		return m_menuRect;
	}

	auto get_selected() -> int
	{
		return m_selectedOption;
	}

private:
	void toggle(const float& x, const float& y, const char* name, const bool& value, const int& index, const int& selected) const;
	void button(const float& x, const float& y, const char* name, const int& index, const int& selected) const;
} extern g_MenuNew;