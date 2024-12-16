#pragma once

#include "Main.h"

class Feature
{
public:
	Feature() = default;
	virtual ~Feature() = default;

	virtual void on_draw() = 0;
	virtual void on_game_process() = 0;
};

class MapFix : public Feature
{
public:
	void on_draw() { };
	void on_game_process() override;
} extern g_MapFix;

class Esp : public Feature
{
	void process_ped(CPed* ped);
	void process_graffiti(CBuilding* building);
	void process_collectible(CPickup* pickup);

public:
	void on_draw() override;
	void on_game_process() { };
} extern g_Esp;

class Radar : public Feature
{
	// https://github.com/gta-reversed/gta-reversed/blob/32690be1776b9d541145238a418031431257ce6f/source/game_sa/common.h#L40
	inline float SCREEN_STRETCH_X(float a) { return a * SCREEN_WIDTH / (float)DEFAULT_SCREEN_WIDTH; } // RsGlobal.maximumWidth * 0.0015625 * value
	inline float SCREEN_STRETCH_Y(float a) { return a * SCREEN_HEIGHT / (float)DEFAULT_SCREEN_HEIGHT; }
	inline float SCREEN_STRETCH_FROM_RIGHT(float a) { return SCREEN_WIDTH - SCREEN_STRETCH_X(a); }
	inline float SCREEN_STRETCH_FROM_BOTTOM(float a) { return SCREEN_HEIGHT - SCREEN_STRETCH_Y(a); }

	void process_ped(CPlayerPed* local, CPed* ped);

public:
	void on_draw() override;
	void on_game_process() { };
} extern g_Radar;

class PlayerCheats : public Feature
{
	struct airbrake_info
	{
		bool air_brake = false;
		bool air_brake_slowmo = false;
		float air_brake_speed = 200.0f;
		float air_brake_accel_time = 2.0f;
	} set;

	// https://github.com/BlastHackNet/mod_sa/blob/2eaed8ed3acebbf02f25995b52be17291fd56bf0/src/cheat_actor.cpp#L206
	void airbrake(CPlayerPed* ped, double time_diff);

public:
	void on_draw() override;
	void on_game_process() override;
} extern g_PlayerCheats;