#pragma once

#include "Main.h"

class Debug
{
	std::vector<std::string>					m_debugStrings;
	std::vector<std::tuple<CVector, CVector>>	m_debugShots;
	FILE*										m_consoleOut;
	std::size_t									m_drawStringsTime, m_drawShotsTime;

public:
	Debug() : m_consoleOut(nullptr), m_drawStringsTime(0u), m_drawShotsTime(0u) {}

	void on_init();
	void on_shutdown();
	void on_draw();

	void print(const char* fmt, ...);
	void shot(CVector a, CVector b);
} extern g_Debug;