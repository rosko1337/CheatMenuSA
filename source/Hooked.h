#pragma once

#include "Main.h"

namespace hooked
{
	CWeaponInfo* __cdecl GetWeaponInfo(eWeaponType weaponID, unsigned char skill);
	void __cdecl HandleGunShot(CVector* start, CVector* end);
	void __fastcall FireInstantHitFromCar2(CWeapon* ecx, void*, CVector origin, CVector target, CVehicle* vehicle, CPed* owner);
	char __cdecl AddBullet(CEntity* creator, int weaponType, CVector position, CVector velocity);
	bool __fastcall CTaskSimpleUseGun_ProcessPed(CTaskSimpleUseGun* thisptr, void*, CPed* ped);
	RsEventStatus __cdecl inputEventHandler(RsEvent event, void* param);
	BOOL WINAPI SetCursorPos(int X, int Y);
}