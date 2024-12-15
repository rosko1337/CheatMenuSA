#pragma once

#include "Main.h"

namespace utils
{
	inline bool is_bone_valid(const std::size_t& bone) {
		return InRange(bone, 0, 9) || InRange(bone, 20, 27) || InRange(bone, 30, 37) || InRange(bone, 40, 45) || InRange(bone, 50, 55);
	}

	inline bool is_weapon_valid(const unsigned char& slot) {
		return InRange(slot, 1, 8);
	}

	inline bool gta_menu_active() {
		return FrontEndMenuManager.m_bMenuActive;
	}

	inline bool key_pressed(int key_code) {
		return ControlsManager.GetIsKeyboardKeyJustDown(RsKeyCodes(key_code));
	}

	inline bool key_down(int key_code) {
		return ControlsManager.GetIsKeyboardKeyDown(RsKeyCodes(key_code));
	}

	CVector			get_bone_position(CPed* pPed, const std::size_t& bone, const bool& transformed = false);
	bool			world_to_screen(const CVector& vecWorld, CVector2D& vecScreen);
	bool			world_to_screen(const CVector& vecWorld);
	CVector2D		vecWorldToScreen(const CVector& vecWorld);
	CVector			extend_vec3(const CVector& start, const CVector& end);
	CPed*			find_ped_to_attack();
	CRGBA			from_hsb(const float& hue, const float& saturation, const float& brightness);
	std::uint64_t	time_get();
}

namespace functions
{
	void give_ammo();
	void give_money();
	void give_minigun();
	void collect_collectibles();
	void unlock_nearby_cars();
}

// lazy way
namespace math
{
	int		near_zero(float v);
	int		vect3_near_zero(CVector in);
	CVector matrix_vec4_mult(CMatrix* m, CVector point, float w);
	void	matrix_matrix_mult(const CMatrix* in1, const CMatrix* in2, CMatrix* out);
	void	matrix_vect3_rotate(const CMatrix* in, const CVector vect, const float t, CMatrix* out);
}