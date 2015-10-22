#include"LGDIDirectX9.h"

LGDIDirectX9::LGDIDirectX9() :
	m_pD3D(NULL),
	m_pD3DDevice(NULL)
{
}

LGDIDirectX9::~LGDIDirectX9()
{
	m_pD3DDevice->Release();
	m_pD3D->Release();
}

bool LGDIDirectX9::initDevice()
{
	if (NULL == (m_pD3D = Direct3DCreate9(D3D_SDK_VERSION)))
		return 0;

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = TRUE;
// TODO 윈도우 모드와 전체화면 모드 수정 가능하게
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;

	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		m_hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp, &m_pD3DDevice)))
		return 0;

	return 1;
}

bool LGDIDirectX9::render()
{
	m_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 255), 1.0f, 0);

	if (SUCCEEDED(m_pD3DDevice->BeginScene()))
	{
		m_pD3DDevice->EndScene();
	}

	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}
