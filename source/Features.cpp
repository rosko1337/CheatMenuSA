#include "Features.h"
#include "Utils.h"
#include "Renderer.h"
#include "Menu.h"
#include "OldMenu.h"
#include "Debug.h"

using namespace plugin;

MapFix g_MapFix;
Esp g_Esp;
Radar g_Radar;
PlayerCheats g_PlayerCheats;

// very ghetto fix. it just works for me so idc.
void MapFix::on_game_process()
{
	// store original value
	static auto frame_limiter = FrontEndMenuManager.m_bPrefsVsync;

	// when we enable frame limiter by ourself -> store new value
	if (FrontEndMenuManager.m_nCurrentMenuPage == MENUPAGE_DISPLAY_ADVANCED && frame_limiter != FrontEndMenuManager.m_bPrefsVsync)
		frame_limiter = FrontEndMenuManager.m_bPrefsVsync;

	// when map is open & frame limiter is disabled -> set frame limiter to true
	if (FrontEndMenuManager.m_nCurrentMenuPage == MENUPAGE_MAP && frame_limiter == false)
		FrontEndMenuManager.m_bPrefsVsync = true;

	// when map is closed & frame limiter not equal previously saved value -> set frame limiter to saved value
	if (FrontEndMenuManager.m_nCurrentMenuPage != MENUPAGE_MAP && FrontEndMenuManager.m_bPrefsVsync != frame_limiter)
		FrontEndMenuManager.m_bPrefsVsync = frame_limiter;
}

void Esp::process_ped(CPed* ped)
{
	auto pos_s = CVector2D(), top_bone_s(pos_s), bottom_bone_s(pos_s), left_bone_s(pos_s), right_bone_s(pos_s);
	float x = 0.0f, y = 0.0f, w = 0.0f, h = 0.0f;

	const auto pos(ped->GetPosition()), top_bone(pos), bottom_bone(pos), left_bone(pos), right_bone(pos);
	if (!utils::world_to_screen(top_bone, top_bone_s) || !utils::world_to_screen(bottom_bone, bottom_bone_s) ||
		!utils::world_to_screen(left_bone, left_bone_s) || !utils::world_to_screen(right_bone, right_bone_s))
		return;

	// just basic maths, nothing overcomplicated
	for (std::size_t i = BONE_RIGHTFOOT; i--;)
	{
		if (!utils::is_bone_valid(i))
			continue;

		const auto bone = utils::get_bone_position(ped, i);
		auto bone_screen = CVector2D();
		if (utils::world_to_screen(bone, bone_screen))
		{
			if (bone_screen.x < left_bone_s.x) left_bone_s = bone_screen;
			if (bone_screen.y < top_bone_s.y) top_bone_s = bone_screen;
			if (bone_screen.x > right_bone_s.x) right_bone_s = bone_screen;
			if (bone_screen.y > bottom_bone_s.y) bottom_bone_s = bone_screen;

			x = left_bone_s.x;
			y = top_bone_s.y;
			w = right_bone_s.x - left_bone_s.x;
			h = bottom_bone_s.y - top_bone_s.y;
		}
	}

	// know your customer
	const auto& color_target = g_Config.esp_AimTarget ?
		utils::find_ped_to_attack() == ped ? color::Red : color::White :
		color::White;

	if (g_Config.esp_Box)
	{
		if (g_Config.esp_Outline)
			g_Renderer.border_box_out(x, y, w, h, 1.f, color_target, CRGBA(0, 0, 0, 127));
		else
			g_Renderer.border_box(x, y, w, h, 1.f, color_target);
	}

	if (g_Config.esp_Health) // Bar
	{
		float health = ped->m_fHealth;

		if (health > 100) // ped->m_fMaxHealth
			health = 100;

		const float size_h = std::round(((h + 1.0f) * health) / 100.0f);
		const float size_w = std::round(((w + 1.0f) * health) / 100.0f);
		const float real_h = (h + 1.0f) - size_h;

		// s/o kwezee xaxa. it will look better if you do it this weird way.
		const auto bar_color = CRGBA(std::min(510 - int(health * 5.1f), 255), std::min(int(health * 5.1f), 255), 0);

		if (g_Config.esp_Outline)
		{
			g_Renderer.rect(x - 1.f, y + h + 3.f, w + 3.f, 4.f, CRGBA(0, 0, 0, 127));
			g_Renderer.rect(x, y + h + 4.f, size_w, 2.f, bar_color);
		}
		else
		{
			g_Renderer.rect(x, y + h + 2.f, size_w, 2.f, bar_color);
		}
	}

	if (g_Config.esp_Armor && ped->m_fArmour) // Bar
	{
		float armor = ped->m_fArmour;

		if (armor > 100) // pPed->m_fMaxHealth
			armor = 100;

		const float size_h = std::round(((h + 1.0f) * armor) / 100.0f);
		const float size_w = std::round(((w + 1.0f) * armor) / 100.0f);
		const float real_h = (h + 1.0f) - size_h;

		// move down armor bar a bit
		if (g_Config.esp_Health)
			y += g_Config.esp_Outline ? 5.0f : 3.0f;

		if (g_Config.esp_Outline)
		{
			g_Renderer.rect(x - 1.f, y + h + 3.f, w + 3.f, 4.f, CRGBA(0, 0, 0, 127));
			g_Renderer.rect(x, y + h + 4.f, size_w, 2.f, color::Silver);
		}
		else
		{
			g_Renderer.rect(x, y + h + 2.f, size_w, 2.f, color::Silver);
		}
	}

	// draw distance
	//g_Renderer.text(x + w / 2, y - 15, DT_LEFT, color::White, "%.f", (pPed->GetPosition() - local->GetPosition()).Magnitude());
}

void Esp::process_graffiti(CBuilding* building)
{
	static std::unordered_map<unsigned short, std::pair<std::string, CRGBA>> ids = {
		{MODEL_TAG_01, {"ballas", color::MediumVioletRed}},
		{MODEL_TAG_FRONT, {"front", color::MediumVioletRed}},
		{MODEL_TAG_KILO, {"kilo", color::MediumVioletRed}},
		{MODEL_TAG_RIFA, {"rifa", color::Cyan}},
		{MODEL_TAG_ROLLIN, {"rollin", color::MediumVioletRed}},
		{MODEL_TAG_SEVILLE, {"seville", color::DarkSeaGreen}},
		{MODEL_TAG_TEMPLE, {"temple", color::MediumVioletRed}},
		{MODEL_TAG_VAGOS, {"vagos", color::Orange}},
		{MODEL_TAG_AZTECA, {"azteca", color::Orange}},
	};

	static auto CTagManager_GetAlpha = [](CEntity* entity) -> std::uint8_t {
		return plugin::CallAndReturn<std::uint8_t, 0x49CF90, CEntity*>(entity);
	};

	const auto model_index = building->m_nModelIndex;

	// по идее есть шанс что крашнет без проверки, но эта хуйня столько fps сжирает.
	// думаю, CBaseModelInfo::IsTagModel() не должен допустить этого
	//if (!ids.contains(model_index))
	//	continue;

	const auto info = CModelInfo::GetModelInfo(model_index);
	if (!info || !info->IsTagModel())
		return;

	// https://github.com/gta-reversed/gta-reversed/blob/master/source/game_sa/TagManager.cpp#L128
	static constexpr std::uint8_t ALPHA_TAGGED = 228; // это статья такая
	const std::uint8_t alpha = CTagManager_GetAlpha(building);
	if (alpha > ALPHA_TAGGED)
		return;

	const auto pos = building->GetPosition();
	auto pos_s = CVector2D();
	if (!utils::world_to_screen(pos, pos_s))
		return;

	g_Renderer.text(pos_s.x, pos_s.y, DT_LEFT, ids.at(model_index).second, ids.at(model_index).first.c_str());
}

void Esp::process_collectible(CPickup* pickup)
{
	static std::unordered_map<unsigned short, std::string> ids = {
		{MODEL_CJ_OYSTER, "oyster"},
		{MODEL_CJ_HORSE_SHOE, "horseshoe"},
		{MODEL_CAMERAPICKUP, "snapshot"},
		{MODEL_MINIGUN, "minigun"},
		// eModelID.h
	};

	// можно еще оружие и прочую гавну рисовать (type == PICKUP_ON_STREET_SLOW), но мне лень с индексами моделей ебаться
	const auto type = pickup->m_nPickupType;
	if (type != PICKUP_ONCE && type != PICKUP_ON_STREET_SLOW && type != PICKUP_SNAPSHOT)
		return;

	// там еще куча пикапов, поэтому отсеиваем ненужные
	const auto model_index = pickup->m_nModelIndex;
	if (model_index != MODEL_CJ_OYSTER && model_index != MODEL_CJ_HORSE_SHOE && model_index != MODEL_CAMERAPICKUP && model_index != MODEL_MINIGUN)
		return;

	if (model_index == MODEL_MINIGUN && pickup->m_nFlags.bDisabled)
		return;

	const auto pos = pickup->GetPosn();
	auto pos_s = CVector2D();
	if (!utils::world_to_screen(pos, pos_s))
		return;

	g_Renderer.text(pos_s.x, pos_s.y, DT_LEFT, color::Pink, ids.at(model_index).c_str());
}

inline CVector operator+(const CVector& vecOne, float multiplier) {
	return CVector(vecOne.x + multiplier, vecOne.y + multiplier, vecOne.z);
}

void Esp::on_draw()
{
	if (g_isPanic)
		return;

	auto local = FindPlayerPed();
	if (!local)
		return;

	for (auto ped : CPools::ms_pPedPool)
	{
		if (!ped || !ped->IsAlive() || ped == local)
			continue;

		if (g_Config.esp_Enabled)
			process_ped(ped);

		// gag
		if (g_Config.sp_DumbassPeds)
		{
			if (auto intelligence = ped->m_pIntelligence)
				intelligence->ClearTasks(true, true);
		}

		if (g_Config.sp_Telekinesis)
		{
			if (ped == utils::find_ped_to_attack())
			{
				ped->m_matrix->pos = local->GetPosition() + (local->GetForward() * 1.1f);
				ped->m_vecMoveSpeed.Set(0.0f, 0.0f, 0.0f);

				ped->m_nPedFlags.bIsStanding = true;
				ped->m_nPedFlags.bWasStanding = true;
				ped->m_nPedFlags.bStayInSamePlace = true;
			}
		}
	}

	// we dont want to loop through building pool while graffiti esp isnt enabled
	if (g_Config.esp_Graffiti)
	{
		// dont ask anything
		for (auto building : CPools::ms_pBuildingPool)
		{
			if (!building)
				continue;

			process_graffiti(building);
		}
	}

	// we dont want to loop through pickups pool while collectibles esp isnt enabled
	if (g_Config.esp_Collectibles)
	{
		for (auto i = 0u; i <= MAX_NUM_PICKUPS; i++)
		{
			auto pickup = &CPickups::aPickUps[i];
			if (!pickup)
				continue;

			process_collectible(pickup);
		}
	}

	// watermark
	static float rainbow = 0.f;
	rainbow = rainbow > 1.f ? 0.0f : rainbow + 0.0001f;
	g_Renderer.text(5.f, SCREEN_HEIGHT - FONT_SIZE - 5.0f, DT_LEFT, utils::from_hsb(rainbow, 1.f, 1.f), "%s | FPS: %d", g_watermarkStrings[g_watermarkSelected].c_str(), std::size_t(CTimer::game_FPS));

	// draw crosshair
	if (g_Config.esp_Crosshair && TheCamera.m_PlayerWeaponMode.m_nMode != MODE_SNIPER)
		g_Renderer.border_box(SCREEN_WIDTH * CCamera::m_f3rdPersonCHairMultX - 5.f, SCREEN_HEIGHT * CCamera::m_f3rdPersonCHairMultY - 5.f, 10.f, 10.f, 1.f, color::Yellow);
}

void Radar::process_ped(CPlayerPed* local, CPed* ped)
{
	auto pos = ped->GetPosition(), local_pos = local->GetPosition(), diff = pos - local_pos;

	if (diff.Magnitude() > 1000.0f)
		return;

	float angle = TheCamera.m_mCameraMatrix.right.Heading() - float(M_PI) / 2.0f;

	auto out = CVector2D();
	out.x = diff.x * cosf(angle) - -diff.y * sinf(angle);
	out.y = diff.x * sinf(angle) + -diff.y * cosf(angle);

	const auto aspect_ratio = SCREEN_COORD_MAX_X / SCREEN_COORD_MAX_Y;
	out.x /= aspect_ratio;
	out.y /= aspect_ratio;

	out.x = SCREEN_STRETCH_X(70.0f) + roundf(out.x);
	out.y = SCREEN_STRETCH_FROM_BOTTOM(66.0f) + roundf(out.y);

	g_Renderer.rect_out(out.x - 3.0f, out.y - 3.0f, 6.0f, 6.0f, color::White, CRGBA(0, 0, 0, 127));
}

void Radar::on_draw()
{
	if (g_isPanic)
		return;

	auto local = FindPlayerPed();
	if (!local)
		return;

	for (auto ped : CPools::ms_pPedPool)
	{
		if (!ped || !ped->IsAlive() || ped == local)
			continue;

		if (!CHud::bDrawingVitalStats)
			process_ped(local, ped);
	}
}

void PlayerCheats::airbrake(CPlayerPed* ped, double time_diff)
{
	static CVector	orig_pos;
	static float	fall_speed_mult;
	static int		was_enabled;

	if (utils::key_pressed(RsKeyCodes::rsRSHIFT))
		set.air_brake ^= 1;

	if (set.air_brake && utils::key_pressed(RsKeyCodes::rsPADINS)) // NUMPAD0
		set.air_brake_slowmo ^= 1;

	CPhysical* info = ped;

	auto veh = FindPlayerVehicle(-1, false);
	if (veh)
		info = veh;

	if (!was_enabled && set.air_brake)
	{
		orig_pos = info->m_matrix->GetPosition();
		fall_speed_mult = 1.0f;
		was_enabled = 1;
	}

	if (!set.air_brake)
	{
		was_enabled = 0;
		set.air_brake_slowmo = 0;
	}
	else
	{
		auto matrix = info->m_matrix;

		// if there's no parachute
		if (!(ped->m_nActiveWeaponSlot == WEAPONSLOT_TYPE_PARACHUTE && ped->m_aWeapons[WEAPONSLOT_TYPE_PARACHUTE].m_eWeaponType == WEAPON_PARACHUTE))
		{
			info->m_matrix->pos = orig_pos;
			info->m_vecMoveSpeed.Set(0.0f, 0.0f, 0.0f);

			// new pedFlags
			ped->m_nPedFlags.bIsStanding = true;
			ped->m_nPedFlags.bWasStanding = true;
			ped->m_nPedFlags.bStayInSamePlace = true;

			if (veh)
			{
				info->m_vecTurnSpeed.Set(0.0f, 0.0f, 0.0f);
				info->m_vecFrictionTurnSpeed.Set(0.0f, 0.0f, 0.0f);
			}

			static std::uint64_t time_start;
			CVector			d = { 0.0f, 0.0f, 0.0f };
			float accel = (float)time_diff * set.air_brake_speed;

			if (set.air_brake_slowmo)
				accel /= 10.0f;

			if (utils::key_down('W'))
				d.x += 1.0f;
			if (utils::key_down('S'))
				d.x -= 1.0f;
			if (utils::key_down('A'))
				d.y += 1.0f;
			if (utils::key_down('D'))
				d.y -= 1.0f;
			if (utils::key_down(RsKeyCodes::rsSPACE))
				d.z += 1.0f;
			if (utils::key_down(RsKeyCodes::rsLSHIFT))
				d.z -= 1.0f;

			if (!math::near_zero(set.air_brake_accel_time))
			{
				if (!math::vect3_near_zero(d))
					time_start = (time_start == 0) ? g_timeCurrent : time_start;
				else
					time_start = 0; /* no keys pressed */

				/* acceleration */
				if (time_start != 0)
				{
					float t = TIME_TO_FLOAT(g_timeCurrent - time_start);
					if (t < set.air_brake_accel_time)
						accel *= t / set.air_brake_accel_time;
				}
			}

			if (!math::vect3_near_zero(d))
			{
				CVector vect = { -d.y, d.x, d.z };
				CVector out;

				/* out = matrix * norm(d) */
				vect.Normalise();
				out = math::matrix_vec4_mult(matrix, vect, 0.0f);

				// follows camera
				if (g_Config.sp_AirbrakeMode)
					out = TheCamera.GetForward();

				matrix->pos.x += out.x * accel;
				matrix->pos.y += out.y * accel;
				matrix->pos.z += out.z * accel;
			}
		}

		// parachute
		else
		{
			if (utils::key_down(RsKeyCodes::rsSPACE))
				fall_speed_mult += float(time_diff) / 2.0f;
			if (utils::key_down(RsKeyCodes::rsLSHIFT))
				fall_speed_mult -= float(time_diff) / 2.0f;

			if (fall_speed_mult < 0.0f)
				fall_speed_mult = 0.0f;
			else if (fall_speed_mult > 2.0f)
				fall_speed_mult = 2.0f;

			matrix->pos.x += info->m_vecMoveSpeed.x * float(time_diff) * set.air_brake_speed;
			matrix->pos.y += info->m_vecMoveSpeed.y * float(time_diff) * set.air_brake_speed;
			matrix->pos.z -= (matrix->pos.z - orig_pos.z) * fall_speed_mult;
		}

		orig_pos = info->m_matrix->GetPosition();

		// heh
		int		gonadsMult = 1000;
		float	strifeMult = 0.00001f;
		int		gonads = rand() % gonadsMult;
		float	strife = float(gonads) * strifeMult;
		if (strife < strifeMult * gonadsMult / 2)
			strife -= strifeMult * gonadsMult;
		info->m_vecMoveSpeed.x = strife;
		gonads = rand() % gonadsMult;
		strife = float(gonads) * strifeMult;
		if (strife < strifeMult * gonadsMult / 2)
			strife -= strifeMult * gonadsMult;
		info->m_vecMoveSpeed.y = strife;
	}
}

void PlayerCheats::on_draw()
{
	if (g_isPanic)
		return;

#ifdef DEBUG
	auto local = FindPlayerPed();
	if (local)
	{
		const auto pos = local->GetPosition();
		auto pos_screen = CVector2D();
		if (utils::world_to_screen(pos, pos_screen))
			g_Renderer.text(pos_screen.x, pos_screen.y, DT_LEFT, color::White, "0x%X", local);
	}
#endif // DEBUG

	g_Renderer.text(SCREEN_WIDTH - 5.f, SCREEN_HEIGHT - FONT_SIZE - 5.0f, DT_RIGHT, set.air_brake ? set.air_brake_slowmo ? color::Yellow : color::LimeGreen : color::White, "airbrake");
}

void PlayerCheats::on_game_process()
{
	if (g_isPanic)
		return;

	auto local = FindPlayerPed();
	if (!local)
		return;

	auto local_info = local->GetPlayerInfoForThisPlayerPed();
	if (local_info)
	{
		if (g_Config.sp_InvPlayer)
		{
			const auto max_armour = static_cast<float>(local_info->m_nMaxArmour);
			const auto max_health1 = static_cast<float>(local_info->m_nMaxHealth);
			const auto max_health2 = local->m_fMaxHealth;

			local->m_fArmour = max_armour;
			local->m_fHealth = max_health1 > max_health2 ? max_health1 : max_health2; // so bad

			if (!g_isSamp)
				local->m_nPhysicalFlags.bInvulnerable = true;

			local->m_nPedFlags.CantBeKnockedOffBike = 1;
		}

		auto local_data = &local_info->m_PlayerData;
		if (local_data)
		{
			if (g_Config.aim_PerfectAccuracy)
			{
				// set weapon accuracy to 100%
				local->m_nWeaponAccuracy = 100;

				// reset spread increase
				local_data->m_fAttackButtonCounter = 0.0f;

				// rpg, sniper, ...
				local_info->m_bFastReload = true;
			}

			if (!g_isSamp && g_Config.sp_NeverWanted)
				local->SetWantedLevel(0);

			// я в федеральном розыске нахожусь, я не приеду в россию (c) б/у
			if (!g_isSamp && g_Config.sp_RussianMostWanted)
			{
				auto wanted = local->GetWanted();
				if (wanted)
				{
					// включаем возможность получить 6 звезд на любом этапе игры
					wanted->MaximumWantedLevel = 6;
					wanted->nMaximumWantedLevel = 9200;
				}

				local->SetWantedLevel(6);

				g_Config.sp_RussianMostWanted = false;
			}
		}
	}

	if (g_Config.sp_InvCar)
	{
		auto vehicle = FindPlayerVehicle(-1, false);
		if (vehicle)
		{
			vehicle->Fix();
			vehicle->m_fHealth = 1000.0f;
		}
	}

	// just funny thingy. only you can see it in samp
	if (g_Config.sp_LegSlide)
	{
		local->m_nPedFlags.bResetWalkAnims = true;
	}

	// ghetto but it does its job
	if (g_Config.sp_Speedhack)
	{
		if (g_Config.sp_SpeedhackFaster) {
			for (auto i = 3u; i--;)
				local->ApplyMoveSpeed();
		}
		else {
			local->ApplyMoveSpeed();
		}
	}

	// same as above
	auto weapon = &local->m_aWeapons[local->m_nActiveWeaponSlot];
	if (!g_isSamp && g_Config.sp_NoReload && weapon)
	{
		weapon->m_nAmmoInClip = weapon->m_nTotalAmmo;
	}

	if (g_Config.sp_Airbrake)
		airbrake(local, g_timeDiff);

	// samp anim reset
	if (utils::key_pressed('Z'))
	{
		if (auto intelligence = local->m_pIntelligence)
			intelligence->ClearTasks(true, true);

		//local->m_pIntelligence->m_TaskMgr.Flush();
		//local->m_pIntelligence->m_TaskMgr.FlushImmediately();
	}
}