#include "Renderer.h"

using namespace plugin;

Renderer g_Renderer;

void Renderer::on_shutdown()
{
	on_lost();
}

void Renderer::on_init()
{
	m_pDevice = reinterpret_cast<IDirect3DDevice9*>(RwD3D9GetCurrentD3DDevice());

	on_reset();
}

void Renderer::on_lost()
{
	if (m_pStateBlock) {
		m_pStateBlock->Release();
		m_pStateBlock = NULL;
	}

	if (m_pD3DXFont) {
		m_pD3DXFont->Release();
		m_pD3DXFont = NULL;
	}
}

void Renderer::on_reset()
{
	if (FAILED(m_pDevice->CreateStateBlock(D3DSBT_ALL, &m_pStateBlock)))
	{
		Error("Failed to create 'state block'!");
	}

	if (FAILED(D3DXCreateFontA(m_pDevice, FONT_SIZE, 0, FW_MEDIUM, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE, "Tahoma", &m_pD3DXFont)))
	{
		Error("Failed to create D3DX font!");
	}
}

void Renderer::on_draw_start()
{
	m_pStateBlock->Capture();

	m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);

	m_pDevice->SetRenderState(D3DRS_SRGBWRITEENABLE, FALSE);
	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_ALPHAREF, 0x08);
	m_pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
	m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);
	m_pDevice->SetRenderState(D3DRS_CLIPPING, TRUE);
	m_pDevice->SetRenderState(D3DRS_CLIPPLANEENABLE, FALSE);
	m_pDevice->SetRenderState(D3DRS_VERTEXBLEND, D3DVBF_DISABLE);
	m_pDevice->SetRenderState(D3DRS_INDEXEDVERTEXBLENDENABLE, FALSE);
	m_pDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);
	m_pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA);
	m_pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	m_pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	m_pDevice->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
	m_pDevice->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
	m_pDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
	m_pDevice->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
	m_pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	m_pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	m_pDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
	m_pDevice->SetRenderState(D3DRS_ANTIALIASEDLINEENABLE, FALSE);
	m_pDevice->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, NULL);

	m_pDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);

	m_pDevice->GetFVF(&m_FVF);
	m_pDevice->GetTexture(0, &m_pTexture);
	m_pDevice->GetPixelShader(&m_pShader);

	m_pDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
	m_pDevice->SetTexture(0, nullptr);
	m_pDevice->SetPixelShader(nullptr);
}

void Renderer::on_draw_end()
{
	m_pDevice->SetPixelShader(m_pShader);
	m_pDevice->SetTexture(0, m_pTexture);
	m_pDevice->SetFVF(m_FVF);

	m_pStateBlock->Apply();
}

void Renderer::text(float x, float y, int align, const CRGBA& color, const char* fmt, ...)
{
	char format[1024];

	va_list args;
	va_start(args, fmt);
	vsnprintf(format, sizeof(format), fmt, args);
	va_end(args);

	std::size_t len = strlen(format);
	
	LONG _x = std::lround(x),
		_y = std::lround(y);

	// calc the width and height
	RECT rect_size{ _x, _y, 0, 0 };
	m_pD3DXFont->DrawTextA(NULL, format, len, &rect_size, DT_CALCRECT, NULL);

	LONG w = rect_size.right - rect_size.left,
		h = rect_size.bottom - rect_size.top;

	if (align & DT_RIGHT)
		_x -= w;
	else if (align & DT_CENTER)
		_x -= w / 2;
	else if (align & DT_VCENTER)
		_y -= h / 2;

	// drop shadow
	RECT rect_shadow{ _x + 1, _y + 1, rect_size.right + 1, rect_size.bottom + 1 };
	m_pD3DXFont->DrawTextA(NULL, format, len, &rect_shadow, DT_NOCLIP | DT_LEFT, color::Black.ToIntARGB());

	RECT rect_text{ _x, _y, rect_size.right, rect_size.bottom };
	m_pD3DXFont->DrawTextA(NULL, format, len, &rect_text, DT_NOCLIP | DT_LEFT, color.ToIntARGB());
}

void Renderer::rect(float x, float y, float w, float h, const CRGBA& color)
{
	Vertex vertices[4] =
	{
		Vertex(x, y + h, 1.0f, color),
		Vertex(x, y, 1.0f, color),
		Vertex(x + w, y + h, 1.0f, color),
		Vertex(x + w, y, 1.0f, color)
	};
	
	m_pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vertices, sizeof(Vertex));
}

void Renderer::rect_out(float x, float y, float w, float h, const CRGBA& color, const CRGBA& out)
{
	rect(x, y, w, h, color);
	border_box(x, y, w, h, 1, out);
}

void Renderer::border_box(float x, float y, float w, float h, float t, const CRGBA& color)
{
	rect(x, y, w, t, color);
	rect(x, y, t, h, color);
	rect(x + w, y, t, h, color);
	rect(x, y + h, w + t, t, color);
}

void Renderer::border_box_out(float x, float y, float w, float h, float t, const CRGBA& color, const CRGBA& out)
{
	border_box(x, y, w, h, t, color);
	border_box(x - t, y - t, w + t * 2, h + t * 2, 1, out);
	border_box(x + t, y + t, w - t * 2, h - t * 2, 1, out);
}

void Renderer::line(float x0, float y0, float x1, float y1, const CRGBA& color)
{
	Vertex vertices[2] =
	{
		Vertex(x0, y0, 1.0f, color),
		Vertex(x1, y1, 1.0f, color)
	};

	m_pDevice->DrawPrimitiveUP(D3DPT_LINELIST, 1, vertices, sizeof(Vertex));
}

void Renderer::cursor(float x, float y, const CRGBA& color)
{
	static auto outline = CRGBA(0, 0, 0, 127);

	Vertex vertices[6] =
	{
		Vertex(x, y + 10.0f, 1.0f, color),
		Vertex(x, y,		 1.0f, color),
		Vertex(x + 10.0f, y, 1.0f, color),
		Vertex(x + 1.0f,  y + 11.0f, 1.0f, outline),
		Vertex(x + 1.0f,  y + 1.0f,  1.0f, outline),
		Vertex(x + 11.0f, y + 1.0f,  1.0f, outline)
	};

	m_pDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, vertices, sizeof(Vertex));
}

void Renderer::text_size(const char* text, int& w, int& h)
{
	std::size_t len = strlen(text);

	// calc the width and height
	RECT rect_size{ 0, 0, 0, 0 };
	m_pD3DXFont->DrawTextA(NULL, text, len, &rect_size, DT_CALCRECT, NULL);

	w = rect_size.right - rect_size.left;
	h = rect_size.bottom - rect_size.top;
}