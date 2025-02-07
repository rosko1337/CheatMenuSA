#pragma once

#include "Main.h"

class Debug
{
	std::vector<std::string>					m_debugStrings;
	std::vector<std::tuple<CVector, CVector>>	m_debugShots;
	FILE*										m_consoleOut;
	std::size_t									m_drawStringsTime, m_drawShotsTime;

public:
	Debug();
	~Debug();

	void on_draw();

	void print(const char* fmt, ...);
	void shot(CVector a, CVector b);
};