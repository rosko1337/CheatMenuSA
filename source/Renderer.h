#pragma once

#include "Main.h"

class Renderer
{
	struct Vertex
	{
		Vertex(float _x, float _y, float _z, const CRGBA& _color) : x(_x), y(_y), z(_z), color(_color.ToIntARGB()) {}

		float x, y, z, rhw = 1.0f;
		D3DCOLOR color;
	};

	IDirect3DDevice9*		m_pDevice;
	IDirect3DStateBlock9*	m_pStateBlock;
	ID3DXFont*				m_pD3DXFont;
	IDirect3DBaseTexture9*	m_pTexture;
	IDirect3DPixelShader9*	m_pShader;
	DWORD					m_FVF;

public:
	Renderer();
	~Renderer();

	void on_lost();
	void on_reset();
	void on_draw_start();
	void on_draw_end();

	void text(float x, float y, int align, const CRGBA& color, const char* fmt, ...);
	void rect(float x, float y, float w, float h, const CRGBA& color);
	void rect_out(float x, float y, float w, float h, const CRGBA& color, const CRGBA& out = plugin::color::Black);
	void border_box(float x, float y, float w, float h, float t, const CRGBA& color);
	void border_box_out(float x, float y, float w, float h, float t, const CRGBA& color, const CRGBA& out = plugin::color::Black);
	void line(float x0, float y0, float x1, float y1, const CRGBA& color);
	void cursor(float x, float y, const CRGBA& color);
	void text_size(const char* text, int& w, int& h);
};