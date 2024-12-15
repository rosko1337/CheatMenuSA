#include "Debug.h"
#include "Renderer.h"
#include "Utils.h"

Debug g_Debug;

void Debug::on_init() {
#ifdef DEBUG
	//AllocConsole();
	//AttachConsole(GetCurrentProcessId());
	//freopen_s(&m_ConsoleOut, "CON", "w", stdout); // redirect stdout to console
#endif // DEBUG
}

void Debug::on_shutdown() {

	if (!m_debugStrings.empty())
		m_debugStrings.clear();

	if (!m_debugShots.empty())
		m_debugShots.clear();

#ifdef DEBUG
	//fclose(m_ConsoleOut);
	//FreeConsole();
#endif // DEBUG
}

void Debug::on_draw() {
	std::size_t i = 0u;
	for (auto str : m_debugStrings)
	{
		g_Renderer.text(5.f, 50.f + (i++ * FONT_SIZE), DT_LEFT, plugin::color::White, str.c_str());
	}
	if (CTimer::m_snTimeInMilliseconds - m_drawStringsTime > 10000 /*|| m_debugStrings.size() > 20*/)
	{
		m_debugStrings.clear();
		i = 0u;
	}

	for (auto [tracer_start, tracer_end] : m_debugShots)
	{
		auto start_s = utils::vecWorldToScreen(tracer_start), end_s = utils::vecWorldToScreen(tracer_end);
		g_Renderer.line(start_s.x, start_s.y, end_s.x, end_s.y, D3DCOLOR_RGBA(0, 255, 255, 255));
	}
	if (CTimer::m_snTimeInMilliseconds - m_drawShotsTime > 10000)
	{
		m_debugShots.clear();
	}
}

void Debug::print(const char* fmt, ...)
{
	char format[1024];

	va_list args;
	va_start(args, fmt);
	vsnprintf(format, sizeof(format), fmt, args);
	va_end(args);

#ifdef DEBUG
	//printf(format); printf("\n");
#endif // DEBUG

	m_debugStrings.push_back(format);
	m_drawStringsTime = CTimer::m_snTimeInMilliseconds;
}

void Debug::shot(CVector a, CVector b)
{
	m_debugShots.push_back({a, b});
	m_drawShotsTime = CTimer::m_snTimeInMilliseconds;
}