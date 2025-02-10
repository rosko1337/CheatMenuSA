#pragma once

#include "Main.h"

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
};

#ifdef USE_NEW_MENU
class Option
{
protected:
	std::string m_optionName, m_optionDescription;

public:
	Option(const std::string& name, const std::string& description)
		: m_optionName(name), m_optionDescription(description) { }

	virtual ~Option() = default;

	virtual void on_draw(float x, float y, float w, float h, const CRGBA& color) = 0;
	virtual void on_input(int key_code) = 0;

	std::string name()			const { return m_optionName; }
	std::string description()	const { return m_optionDescription; }
};

class BoolOption : public Option
{
	bool& m_optionValue;

public:
	BoolOption(const std::string& name, bool& value, const std::string& description)
		: Option(name, description), m_optionValue(value) { }

	void on_draw(float x, float y, float w, float h, const CRGBA& color) override;
	void on_input(int key_code) override;

	bool value() const { return m_optionValue; }
};

class IntOption : public Option
{
	int& m_optionValue;
	int m_optionValueMin, m_optionValueMax;

public:
	IntOption(const std::string& name, int& value, int min_value, int max_value, const std::string& description)
		: Option(name, description), m_optionValue(value), m_optionValueMin(min_value), m_optionValueMax(max_value) { }

	void on_draw(float x, float y, float w, float h, const CRGBA& color) override;
	void on_input(int key_code) override;

	int value() const { return m_optionValue; }
};

class FloatOption : public Option
{
	float& m_optionValue;
	float m_optionValueMin, m_optionValueMax, m_optionValueStep;

public:
	FloatOption(const std::string& name, float& value, float min_value, float max_value, float step, const std::string& description)
		: Option(name, description), m_optionValue(value), m_optionValueMin(min_value), m_optionValueMax(max_value), m_optionValueStep(step) { }

	void on_draw(float x, float y, float w, float h, const CRGBA& color) override;
	void on_input(int key_code) override;

	float value() const { return m_optionValue; }
};

class FuncOption : public Option
{
	func_fn m_optionFn; // std::function< void( ) > callback;

public:
	FuncOption(const std::string& name, func_fn func, const std::string& description)
		: Option(name, description), m_optionFn(func) { }

	void on_draw(float x, float y, float w, float h, const CRGBA& color) override;
	void on_input(int key_code) override;
};

class SubMenu : public Option
{
	std::vector<std::shared_ptr<Option>> m_options; // why shared? cuz i want to thats why
	bool m_isOpen;

public:
	SubMenu(const std::string& name, const std::string& description)
		: Option(name, description), m_isOpen(true) { }

	~SubMenu() = default;

	void on_draw(float x, float y, float w, float h, const CRGBA& color) override;
	void on_input(int key_code) override;

	auto add_option(std::shared_ptr<Option> option) -> void		{ m_options.push_back(std::move(option)); }
	auto get_options() -> std::vector<std::shared_ptr<Option>>	{ return m_options; }

	bool is_open() const { return m_isOpen; }
};

class Menu
{
	std::vector<std::shared_ptr<Option>> m_options, m_visibleOptions;

	int m_selectedOptionIndex;
	bool m_isOpen;

	CRect m_menuRect;

	void update_visible_options() { // ghetto c+p codez by deepseek or chatgpt. sorry.
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
	}

public:
	Menu();
	~Menu() = default;

	void on_draw();
	bool on_input(int key_code);

	void add_option(std::shared_ptr<Option> option) { m_options.push_back(std::move(option)); }
	void remove_option(const std::string& name) {
		static auto name_equals = [&name](const std::shared_ptr<Option>& option) {
			return option->name() == name;
		};

		auto is_available = std::any_of(m_options.begin(), m_options.end(), name_equals);
		if (!is_available) return; // we dont have any option with 'name', so do nothing

		auto it = std::remove_if(m_options.begin(), m_options.end(), name_equals);
		if (it != m_options.end()) {
			m_options.erase(it, m_options.end()); // удаляем

			update_visible_options(); // обновляем список видимых опций

			if (m_selectedOptionIndex >= m_visibleOptions.size()) // корректируем индекс выбранного элемента
				m_selectedOptionIndex = std::max(0, static_cast<int>(m_visibleOptions.size()) - 1);
		}
	}

	bool is_open() const { return m_isOpen; }
};

namespace menu_api
{
	extern std::stack<std::shared_ptr<SubMenu>> submenu;

	NEW_MENU_API void begin_submenu	(const std::string& name, const std::string& description);
	NEW_MENU_API void end_submenu	();
	NEW_MENU_API void remove_submenu(const std::string& name);
	NEW_MENU_API void add_bool		(const std::string& name, bool& value, const std::string& description);
	NEW_MENU_API void add_int		(const std::string& name, int& value, int min_value, int max_value, const std::string& description);
	NEW_MENU_API void add_float		(const std::string& name, float& value, float min_value, float max_value, float step, const std::string& description);
	NEW_MENU_API void add_func		(const std::string& name, func_fn func, const std::string& description);
}
#endif // USE_NEW_MENU

#ifdef USE_OLD_MENU
enum class ItemType : std::size_t
{
	MENUITEM_NONE = 0,
	MENUITEM_SUBMENU = 1,
	MENUITEM_TOGGLE = 2,
	MENUITEM_VALUE = 3,
	MENUITEM_BUTTON = 4
};

struct Item
{
	Item(const char* name = nullptr, const ItemType& type = ItemType::MENUITEM_NONE, void* data = nullptr, const char* description = nullptr)
		: name(name), type(type), data(data), description(description) { }

	Item(const Item&) = default;
	Item& operator=(const Item&) = default;

	Item(Item&&) = default;
	Item& operator=(Item&&) = default;

	const char* name;
	ItemType	type;
	void*		data;
	const char* description;
};

class Menu
{
	std::vector<Item>	m_menuItems;
	bool				m_isOpen;
	int					m_selectedOption;
	CRect				m_menuRect;

public:
	Menu();
	~Menu() = default;

	void on_draw();
	bool on_input(int key_code);

	bool is_open() const { return m_isOpen; }
};
#endif // USE_OLD_MENU

#ifdef USE_ZGUI_MENU
extern void zgui_input_hook();

class Menu
{
public:
	Menu();
	~Menu() = default;

	void on_draw();

	bool is_open() const;
};
#endif // USE_ZGUI_MENU