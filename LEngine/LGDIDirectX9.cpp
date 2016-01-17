#include"LGDIDirectX9.h"
#include"LMath.h"

LGDIDirectX9::LGDIDirectX9(HWND hWnd, LGDI_Type type) :
	LGDIWrapper(hWnd, type),
	m_pD3D(NULL),
	m_pD3DDevice(NULL)
{
	initDevice();
	initVB();
}

LGDIDirectX9::~LGDIDirectX9()
{
	m_pVB->Release();
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

	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	return 1;
}

bool LGDIDirectX9::initVB()
{
	VERTEX1 vertices[] =
	{
		{-1.0f, -1.0f, 0.0f, 0xffff0000, },
		{1.0f, -1.0f, 0.0f, 0xff0000ff, },
		{0.0f, 1.0f, 0.0f, 0xffffffff, },
	};

	if (FAILED(m_pD3DDevice->CreateVertexBuffer(3 * sizeof(VERTEX1), 0, D3DFVF_VERTEX1, D3DPOOL_DEFAULT, &m_pVB, NULL)))
	{
		return false;
	}

	VOID* pVertices;
	if (FAILED(m_pVB->Lock(0, sizeof(vertices), (void**)&pVertices, 0)))
		return false;

	memcpy(pVertices, vertices, sizeof(vertices));
	m_pVB->Unlock();

	return true;
}

void LGDIDirectX9::setupMatrices()
{
	D3DXMATRIXA16 matWorld;
	UINT iTime = timeGetTime() % 1000;
	FLOAT fAngle = iTime * (2.0f * D3DX_PI) / 1000.0f;
	D3DXMatrixRotationY(&matWorld, fAngle);
	m_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);

	D3DXVECTOR3 vEyePt(0.0f, 3.0f, -5.0f);
	D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);
	D3DXMATRIXA16 matView;
	D3DXMatrixLookAtLH(&matView, &vEyePt, &vLookatPt, &vUpVec);
	m_pD3DDevice->SetTransform(D3DTS_VIEW, &matView);

	D3DXMATRIXA16 matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 4, 1.0f, 1.0f, 100.0f);
	m_pD3DDevice->SetTransform(D3DTS_PROJECTION, &matProj);
}

bool LGDIDirectX9::render()
{
	m_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

	if (SUCCEEDED(m_pD3DDevice->BeginScene()))
	{
		setupMatrices();

		m_pD3DDevice->SetStreamSource(0, m_pVB, 0, sizeof(VERTEX1));
		m_pD3DDevice->SetFVF(D3DFVF_VERTEX1);
		m_pD3DDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);

		m_pD3DDevice->EndScene();
	}

	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);

	return true;
}