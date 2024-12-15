#include "Hooked.h"
#include "Menu.h"
#include "OldMenu.h"
#include "Utils.h"
#include "Debug.h"
#include "Features.h"

CWeaponInfo* __cdecl hooked::GetWeaponInfo(eWeaponType weaponID, unsigned char skill)
{
	const auto ret = g_hookGetWeaponInfo.call<CWeaponInfo*>(weaponID, skill);

	if (!g_Config.aim_PerfectAccuracy)
		return ret;

	const auto return_address = reinterpret_cast<std::uint32_t>(_ReturnAddress());
	if (return_address == 0x73FB65 /*FireInstantHit*/)
	{
		const auto ebp = GetCurrentStackFrame(); // -4
		const auto firing_entity = *reinterpret_cast<CPed**>(ebp + 0x4); // 
		const auto local = FindPlayerPed();

		if (weaponID != 0 && firing_entity == local)
		{
			// does this even works lmao?
			ret->m_nFlags.bMoveAim = ret->m_nFlags.bMoveFire = 1;
			ret->m_nFlags.bHeavy = ret->m_nFlags.bSlowdown = 0;

			// this shit works 100%
			ret->m_fWeaponRange = 4096.0f;
			ret->m_fAccuracy = 1.0f;

			//g_Debug.print("[gwi] who: 0x%X | local: 0x%X", firing_entity, local);
		}
	}

	return ret;
}

void __cdecl hooked::HandleGunShot(CVector* start, CVector* end)
{
	const auto return_address = reinterpret_cast<std::uint32_t>(_ReturnAddress());
	if (g_Config.aim_Enabled && (return_address == 0x7406EB /*FireInstantHit: while aiming*/ || return_address == 0x740B33 /*FireInstantHit: from the hip*/))
	{
		const auto ebp = GetCurrentStackFrame(); // -4
		const auto firing_entity = *reinterpret_cast<CPed**>(ebp); // read first local var of FireInstantHit
		const auto local = FindPlayerPed();

		//g_Debug.print("[hgs] ra: 0x%X | ebp: 0x%X | who: 0x%X | local: 0x%X", return_address, ebp, firing_entity, local);

		if (firing_entity == local)
		{
			const auto aimbot_target = utils::find_ped_to_attack();
			if (aimbot_target && utils::is_weapon_valid(local->m_nActiveWeaponSlot))
			{
				// todo: fix somehow? (https://www.youtube.com/watch?v=a_tKa9B2Sdk)
				//const auto target_in_veh = !!(aimbot_target->m_pVehicle);
				//const auto target_bone = aimbot_target->m_matrix->pos.z > local->m_matrix->pos.z && !target_in_veh ? BONE_SPINE1 : BONE_HEAD2;

				auto best_bone = g_Config.aim_SmartFov;
				const auto crosshair_pos = CVector2D(SCREEN_WIDTH * CCamera::m_f3rdPersonCHairMultX, SCREEN_HEIGHT * CCamera::m_f3rdPersonCHairMultY);

				if (g_Config.aim_Smart)
				{
					for (auto i = 0u; i <= 8; i++)
					{
						const auto bone = utils::get_bone_position(aimbot_target, i);
						auto bone_s = CVector2D();
						if (utils::world_to_screen(bone, bone_s))
						{
							auto closest_bone = (crosshair_pos - bone_s).Magnitude();
							if (closest_bone < best_bone)
							{
								best_bone = closest_bone;

								*end = utils::extend_vec3(*start, bone);
							}
						}
					}
				}
				else
				{
					auto head = utils::get_bone_position(aimbot_target, BONE_HEAD2); // BONE_NECK = 5, BONE_HEAD2 = 6, BONE_HEAD1 = 7, BONE_HEAD = 8
					*end = utils::extend_vec3(*start, head);
				}

				if (g_Config.aim_Randomize)
				{
					float dist = plugin::RandomNumberInRange<float>(-g_Config.aim_RandomizeFactor, g_Config.aim_RandomizeFactor);
					dist /= (aimbot_target->GetPosition() - local->GetPosition()).Magnitude() / 10.0f;

					end->x += dist;
					end->y += dist;
					end->z += dist;
				}
			}
		}
	}

	return g_hookHandleGunShot.call<void>(start, end);
}

void __fastcall hooked::FireInstantHitFromCar2(CWeapon* ecx, void*, CVector origin, CVector target, CVehicle* vehicle, CPed* owner)
{
	const auto return_address = reinterpret_cast<std::uint32_t>(_ReturnAddress());
	if (g_Config.aim_Enabled && return_address == 0x73F3EB /*FireInstantHitFromCar*/)
	{
		const auto local = FindPlayerPed();
		if (owner == local)
		{
			const auto aimbot_target = utils::find_ped_to_attack();
			if (aimbot_target && utils::is_weapon_valid(local->m_nActiveWeaponSlot))
			{
				auto head = utils::get_bone_position(aimbot_target, BONE_HEAD2);
				target = utils::extend_vec3(origin, head); //target = utils::get_bone_position(aimbot_target, BONE_HEAD2);
			}
		}
	}

	return g_hookFireInstantHitFromCar2.thiscall<void>(ecx, origin, target, vehicle, owner);
}

char __cdecl hooked::AddBullet(CEntity* creator, int weaponType, CVector position, CVector velocity)
{
	// и я получил блять аим работающий от стрельбы себе в ноги. прям как в ебаных шутках про ебаные плюсы.
	// одно порадовало - стреляет через стены. но как в сампе себя покажет хз, надо пробовать. обрыганство.
	// upd: в сампе работает, но только при прямой видимости, стены не прошивает
	//const auto local = FindPlayerPed();
	//if (g_Config.aim_Enabled && g_Config.aim_SniperRifle && creator == local)
	//{
	//	const auto aimbot_target = utils::find_ped_to_attack();
	//	if (aimbot_target && utils::is_weapon_valid(local->m_nActiveWeaponSlot))
	//		position = utils::get_bone_position(aimbot_target, BONE_HEAD2);
	//}

	return g_hookAddBullet.call<char>(creator, weaponType, position, velocity);
}

bool __fastcall hooked::CTaskSimpleUseGun_ProcessPed(CTaskSimpleUseGun* thisptr, void*, CPed* ped)
{
	// этот аим выглядит прикольнее, чем тот что используется, потому что сиджи наводится на врага
	// но он не пробивает водителей, по каким-то причинам, с которыми мне впадлу разбираться
	// ну и еще тут можно сделать режим минигана, что угарно

	const auto ret = g_hookProcessPed.thiscall<bool>(thisptr, ped);

	if (thisptr)
	{
		const auto local = CWorld::Players[0].m_pPed;
		if (local && ped == local)
		{
			// minigun mode for every weapon
			if (g_Config.sp_MinigunMode)
			{
				thisptr->bLefttHand = true;
				thisptr->bRightHand = true;
			}

			//const auto target = utils::find_ped_to_attack();
			//if (target && utils::is_weapon_valid(local->m_nActiveWeaponSlot))
			//{
			//	// do aimbot
			//	thisptr->m_pTarget = target;
			//	thisptr->m_vecTarget = utils::get_bone_position(target, BONE_HEAD2);
			//}
		}
	}

	return ret;
}

RsEventStatus __cdecl hooked::inputEventHandler(RsEvent event, void* param)
{
	if (utils::gta_menu_active())
		return g_originalInputEventHandler(event, param);

	const auto key_status = (RsKeyStatus*)param;
	const auto key_code = key_status->keyScanCode;

	if (event == rsKEYDOWN)
	{
		if (key_code == RsKeyCodes::rsDEL)
			g_isPanic ^= 1;
#ifdef NEW_MENU
		if (g_Menu.is_open())
#else
		if (g_Menu.on_input(event, key_code))
#endif // NEW_MENU
			return rsEVENTNOTPROCESSED; // we dont want the game to process input while the menu is open
	}

	return g_originalInputEventHandler(event, param);
}

BOOL WINAPI hooked::SetCursorPos(int X, int Y)
{
#ifdef NEW_MENU
	if (g_Menu.is_open())
		return TRUE;
#endif // NEW_MENU

	return g_hookSetCursorPos.stdcall<BOOL, int, int>(X, Y);
}