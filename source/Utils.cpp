#include "Utils.h"
#include "Menu.h"
#include "OldMenu.h"

CVector utils::get_bone_position(CPed* pPed, const std::size_t& bone, const bool& transformed)
{
	RwV3d vBonePos;
	if (transformed)
		pPed->GetTransformedBonePosition(vBonePos, bone, true);
	else
		pPed->GetBonePosition(vBonePos, bone, true);
	CVector vReturn(vBonePos);
	return vReturn;
}

bool utils::world_to_screen(const CVector& vecWorld, CVector2D& vecScreen)
{
	auto pCamera = &TheCamera;
	if (!pCamera)
		return false;

	const auto matrix = &pCamera->m_mViewMatrix; //reinterpret_cast<CMatrix*>(0xB6FA2C);

	const auto dwScreenWidth = RsGlobal.maximumWidth; //*reinterpret_cast<ulong_t*>(0xC17044);
	const auto dwScreenHeight = RsGlobal.maximumHeight; //*reinterpret_cast<ulong_t*>(0xC17048);

	CVector vTransform;
	vTransform.x = (vecWorld.x * matrix->right.x) + (vecWorld.y * matrix->up.x) + (vecWorld.z * matrix->at.x) + matrix->pos.x;
	vTransform.y = (vecWorld.x * matrix->right.y) + (vecWorld.y * matrix->up.y) + (vecWorld.z * matrix->at.y) + matrix->pos.y;
	vTransform.z = (vecWorld.x * matrix->right.z) + (vecWorld.y * matrix->up.z) + (vecWorld.z * matrix->at.z) + matrix->pos.z;

	if (vTransform.z < 0.0f)
		return false;

	const double dRecip = static_cast<double>(1.0f) / vTransform.z;
	vecScreen.x = vTransform.x * static_cast<float>(dRecip * dwScreenWidth);
	vecScreen.y = vTransform.y * static_cast<float>(dRecip * dwScreenHeight);

	return true;
}

bool utils::world_to_screen(const CVector& vecWorld)
{
	CVector2D temp;
	return world_to_screen(vecWorld, temp);
}

CVector2D utils::vecWorldToScreen(const CVector& vecWorld)
{
	CVector2D vecScreen;
	world_to_screen(vecWorld, vecScreen);
	return vecScreen;
}

CVector utils::extend_vec3(const CVector& start, const CVector& end)
{
	auto diff = end - start;
	auto len = abs(diff.Magnitude());

	return diff * len + start;
}

// custom target selection impl. cuz of membership, green gang model check & no vis check at all
// https://github.com/gta-reversed/gta-reversed/blob/master/source/game_sa/Entity/Ped/PlayerPed.cpp#L851-L892
CPed* utils::find_ped_to_attack()
{
	CPed* closestPed{};
	float closestDistance = std::numeric_limits<float>::max();

	auto local = FindPlayerPed();
	if (!local)
		return closestPed;

	CVector origin = FindPlayerCoors(-1);
	CVector end = origin + TheCamera.GetForward() * 100.0f;

	for (auto ped : CPools::ms_pPedPool)
	{
		if (!ped || ped == local || !ped->IsAlive() || ped->m_fHealth == 0.0f)
			continue;

		if (g_Config.sp_AimIgnoreGroove && ped->m_nPedType == PED_TYPE_GANG2)
			continue;

		auto local_head = get_bone_position(local, BONE_HEAD2), target_head = get_bone_position(ped, BONE_HEAD2);
		if (g_Config.aim_TargetVisible && !CWorld::GetIsLineOfSightClear(local_head, target_head, true, false, false, true, true, false, false))
			continue;

		auto target_s = CVector2D(), crosshair_pos = CVector2D(SCREEN_WIDTH * CCamera::m_f3rdPersonCHairMultX, SCREEN_HEIGHT * CCamera::m_f3rdPersonCHairMultY);
		if (g_Config.aim_TargetOnScreen && !world_to_screen(target_head, target_s))
			continue;

		CVector point = ped->GetPosition();

		float dist = CCollision::DistToLine(&origin, &end, &point);
		float pointDist = (crosshair_pos - target_s).Magnitude(); // closest to crosshair
		//float pointDist = (point - origin).Magnitude2D(); // closest to player
		if (pointDist > 20.0f)
			dist += (pointDist - 20.0f) / 5.0f;

		if ((g_Config.aim_GangsPriority && (ped->m_nPedType >= PED_TYPE_GANG1 && ped->m_nPedType <= PED_TYPE_GANG10)) // IsPedTypeGang(ped->m_nPedType)
			|| (g_Config.aim_CopsPriority && ped->m_nPedType == PED_TYPE_COP))
			dist = std::max(0.0f, dist / 2.0f - 2.0f);

		if (dist < closestDistance) {
			closestDistance = dist;
			closestPed = ped;
		}
	}
	return closestPed;
}

CRGBA utils::from_hsb(const float& hue, const float& saturation, const float& brightness)
{
	float h = hue == 1.0f ? 0 : hue * 6.0f;
	float f = h - (int)h;
	float p = brightness * (1.0f - saturation);
	float q = brightness * (1.0f - saturation * f);
	float t = brightness * (1.0f - (saturation * (1.0f - f)));

	if (h < 1) {
		return CRGBA((unsigned char)(brightness * 255), (unsigned char)(t * 255), (unsigned char)(p * 255));
	}
	else if (h < 2) {
		return CRGBA((unsigned char)(q * 255), (unsigned char)(brightness * 255), (unsigned char)(p * 255));
	}
	else if (h < 3) {
		return CRGBA((unsigned char)(p * 255), (unsigned char)(brightness * 255), (unsigned char)(t * 255));
	}
	else if (h < 4) {
		return CRGBA((unsigned char)(p * 255), (unsigned char)(q * 255), (unsigned char)(brightness * 255));
	}
	else if (h < 5) {
		return CRGBA((unsigned char)(t * 255), (unsigned char)(p * 255), (unsigned char)(brightness * 255));
	}
	else {
		return CRGBA((unsigned char)(brightness * 255), (unsigned char)(p * 255), (unsigned char)(q * 255));
	}
}

std::uint64_t utils::time_get()
{
	static int				init = 0;
	static ULARGE_INTEGER	init_time;
	FILETIME				ft;
	ULARGE_INTEGER			time_now;

	GetSystemTimeAsFileTime(&ft);
	time_now.LowPart = ft.dwLowDateTime;
	time_now.HighPart = ft.dwHighDateTime;

	time_now.QuadPart /= (std::uint64_t)1000; /* convert to 100 usec */

	if (!init)
	{
		init_time.QuadPart = time_now.QuadPart - MSEC_TO_TIME(1000);
		init = 1;
	}

	return (std::uint64_t)(time_now.QuadPart - init_time.QuadPart);
}

void functions::give_ammo()
{
	auto local = FindPlayerPed();
	if (!local)
		return;

	auto weapon = &local->m_aWeapons[local->m_nActiveWeaponSlot];
	if (!g_isSamp && weapon)
		local->GrantAmmo(weapon->m_eWeaponType, 100);
}

void functions::give_money()
{
	auto local = FindPlayerPed();
	if (!local)
		return;

	if (!g_isSamp)
		CPickups::GenerateNewOne(local->GetPosition(), MODEL_MONEY, PICKUP_MONEY, 300, 0, false, nullptr);
}

void functions::give_minigun()
{
	auto local = FindPlayerPed();
	if (!local)
		return;

	CStreaming::RequestModel(MODEL_MINIGUN, 2);
	CStreaming::LoadAllRequestedModels(false);

	local->GiveWeapon(WEAPON_MINIGUN, 100, true);
	local->SetCurrentWeapon(WEAPON_MINIGUN);

	CStreaming::SetModelIsDeletable(MODEL_MINIGUN);
}

void functions::collect_collectibles()
{
	auto local = FindPlayerPed();
	if (!local)
		return;

	if (g_isSamp)
		return;

	// graffiti
	for (auto building : CPools::ms_pBuildingPool)
	{
		if (!building)
			continue;

		static auto CTagManager_GetAlpha = [](CEntity* entity) -> std::uint8_t {
			return plugin::CallAndReturn<std::uint8_t, 0x49CF90, CEntity*>(entity);
		};

		static auto CTagManager_SetAlpha = [](CEntity* entity, std::uint8_t alpha) -> void {
			plugin::Call<0x49CEC0, CEntity*, std::uint8_t>(entity, alpha);
		};

		auto model_index = building->m_nModelIndex;

		auto info = CModelInfo::GetModelInfo(model_index);
		if (!info || !info->IsTagModel())
			continue;

		// https://github.com/gta-reversed/gta-reversed/blob/master/source/game_sa/TagManager.cpp#L128
		static constexpr std::uint8_t ALPHA_TAGGED = 228; // это статья такая
		std::uint8_t alpha = CTagManager_GetAlpha(building);
		if (alpha > ALPHA_TAGGED)
			continue;

		// set tagged
		CTagManager_SetAlpha(building, 255);
	}

	// collectibles
	for (auto i = 0u; i <= MAX_NUM_PICKUPS; i++)
	{
		auto pickup = &CPickups::aPickUps[i];
		if (!pickup)
			continue;

		auto type = pickup->m_nPickupType;
		if (type != PICKUP_ONCE && type != PICKUP_SNAPSHOT)
			continue;

		auto model_index = pickup->m_nModelIndex;
		if (model_index != MODEL_CJ_OYSTER && model_index != MODEL_CJ_HORSE_SHOE && model_index != MODEL_CAMERAPICKUP)
			continue;

		auto pos = pickup->GetPosn(), local_pos = local->GetPosition();
		auto pos_s = CVector2D();

		// tp pickup to playa
		pickup->SetPosn(local_pos.x, local_pos.y, local_pos.z);

		// если место снимка, то щелкаем. и похуй что камеры нет. там в проверках только расстояние, остальные проверки в CWeapon::TakePhotograph
		if (model_index == MODEL_CAMERAPICKUP)
			CPickups::PictureTaken();
	}
}

void functions::unlock_nearby_cars()
{
	for (auto vehicle : CPools::ms_pVehiclePool)
	{
		if (!vehicle)
			continue;

		vehicle->m_eDoorLock = DOORLOCK_UNLOCKED;
	}
}

int math::near_zero(float v)
{
	if (!isfinite(v))
		return 1;
	return fabs(v) < FLOAT_EPSILON;
}

int math::vect3_near_zero(CVector in)
{
	return near_zero(in.x) && near_zero(in.y) && near_zero(in.z);
}

CVector math::matrix_vec4_mult(CMatrix* m, CVector point, float w)
{
	CVector out;

	out.x = m->at.x * point.z + m->right.x * point.x + m->up.x * point.y + w * m->pos.x;
	out.y = m->at.y * point.z + m->right.y * point.x + m->up.y * point.y + w * m->pos.y;
	out.z = m->at.z * point.z + m->right.z * point.x + m->up.z * point.y + w * m->pos.z;

	return out;
}

void math::matrix_matrix_mult(const CMatrix* in1, const CMatrix* in2, CMatrix* out)
{
	float	res[16], _in1[16], _in2[16];
	int		i, j;

	memcpy(_in1, in1, sizeof(float) * 16);
	memcpy(_in2, in2, sizeof(float) * 16);

	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 4; j++)
		{
			res[i * 4 + j] = _in1[i * 4 + 0] *
				_in2[0 + j] +
				_in1[i * 4 + 1] *
				_in2[4 + j] +
				_in1[i * 4 + 2] *
				_in2[8 + j] +
				_in1[i * 4 + 3] *
				_in2[12 + j];
		}
	}

	memcpy(res, out, sizeof(float) * 16);
}

void math::matrix_vect3_rotate(const CMatrix* in, const CVector vect, const float t, CMatrix* out)
{
	const float x = vect.x, y = vect.y, z = vect.z;
	const float sin_t = sinf(t), cos_t = cosf(t);
	float		res[16];
	const float matrix[16] =
	{
		cos_t + (1.0f - cos_t) * x * x,
		(1.0f - cos_t) * x * y - sin_t * z,
		(1.0f - cos_t) * x * z + sin_t * y,
		0.0f,
		(1.0f - cos_t) * y * x + sin_t * z,
		cos_t + (1.0f - cos_t) * y * y,
		(1.0f - cos_t) * y * z - sin_t * x,
		0.0f,
		(1.0f - cos_t) * z * x - sin_t * y,
		(1.0f - cos_t) * z * y + sin_t * x,
		cos_t + (1.0f - cos_t) * z * z,
		0.0f,
		0.0f,
		0.0f,
		0.0f,
		1.0f
	};

	matrix_matrix_mult(in, (CMatrix*)matrix, (CMatrix*)res);
	memcpy(&res[4 * 3], &in[4 * 3], sizeof(float) * 4);
	memcpy(res, out, sizeof(float) * 16);
}