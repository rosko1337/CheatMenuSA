#pragma once

// base
#include "plugin.h"
#include "CWorld.h"
#include "CPickups.h"
#include "CHud.h"
#include "CCollision.h"
#include "CCamera.h"
#include "CMenuManager.h"
#include "CModelInfo.h"
#include "CStreaming.h"
#include "CControllerConfigManager.h"
#include "CTaskSimpleFight.h"
#include "CRadar.h"
#include "ePedBones.h"

// system & stl
#include <intrin.h>

// dx9
#include <d3d9.h>
#include <d3dx9.h>

// useful
#define GetCurrentStackFrame() (reinterpret_cast<std::uint32_t*>(std::uint32_t(_AddressOfReturnAddress()) - sizeof(std::uint32_t*)))
#define InRange(val, _min, _max) !!(val > _min && val < _max) // exclusive
#define MSEC_TO_TIME( v )	( (std::uint64_t) (v) * 10 )
#define TIME_TO_FLOAT( v )	( (float)((double)(v) / (double)MSEC_TO_TIME(1000)) )
#define TIME_TO_DOUBLE( v ) ( (double)((double)(v) / (double)MSEC_TO_TIME(1000)) )
#define FONT_SIZE 12
#define FLOAT_EPSILON 0.0001f
#define DEFAULT_SCREEN_WIDTH 640
#define DEFAULT_SCREEN_HEIGHT 448

// comment this line if you want to use old menu
//#define NEW_MENU

// watermark shit
extern std::string g_watermarkStrings[3];
extern std::size_t g_watermarkSelected;

// hooking related
extern SafetyHookInline	   g_hookGetWeaponInfo;
extern SafetyHookInline	   g_hookHandleGunShot;
extern SafetyHookInline	   g_hookFireInstantHitFromCar2;
extern SafetyHookInline	   g_hookAddBullet;
extern SafetyHookInline	   g_hookProcessPed;
extern SafetyHookInline	   g_hookSetCursorPos;
extern RsInputEventHandler g_originalInputEventHandler;

// samp checks
extern bool g_isSamp;
extern bool g_isPanic;

// used for airbrake cuz c+p from s0beit
extern std::uint64_t g_timeCurrent;
extern double g_timeDiff;

// found it somewhere. there is no such thing in plugin-sdk somehow
enum eWeaponSlot
{
	WEAPONSLOT_TYPE_UNARMED = 0,
	WEAPONSLOT_TYPE_MELEE,
	WEAPONSLOT_TYPE_HANDGUN,
	WEAPONSLOT_TYPE_SHOTGUN,
	WEAPONSLOT_TYPE_SMG,
	WEAPONSLOT_TYPE_MG,
	WEAPONSLOT_TYPE_RIFLE,
	WEAPONSLOT_TYPE_HEAVY,
	WEAPONSLOT_TYPE_THROWN,
	WEAPONSLOT_TYPE_SPECIAL,
	WEAPONSLOT_TYPE_GIFT,
	WEAPONSLOT_TYPE_PARACHUTE,
	WEAPONSLOT_TYPE_DETONATOR,
	WEAPONSLOT_MAX
};