#include"LGDIDirectX9.h"
#include"LMath.h"
#include<stdio.h>

LGDIDirectX9::LGDIDirectX9(HWND hWnd, LGDI_Type type) :
	LGDIWrapper(hWnd, type),
	m_pD3D(NULL),
	m_pD3DDevice(NULL)
{
	initDevice();
	initGeometry();
	initIB();
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
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		m_hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp, &m_pD3DDevice)))
		return 0;

	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	//m_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	return 1;
}

bool LGDIDirectX9::initGeometry()
{
	LPD3DXBUFFER pMtrlBuffer;

	if (FAILED(D3DXLoadMeshFromX("..\\Resources\\Tiger.x", D3DXMESH_SYSTEMMEM, m_pD3DDevice, NULL, &pMtrlBuffer, NULL, &m_dwNumMaterials, &m_pMesh)))
		return false;

	D3DXMATERIAL* d3dxMaterials = (D3DXMATERIAL*)pMtrlBuffer->GetBufferPointer();

	m_pMeshMaterials = new D3DMATERIAL9[m_dwNumMaterials];
	m_pMeshTextures = new LPDIRECT3DTEXTURE9[m_dwNumMaterials];

	for (DWORD i = 0; i < m_dwNumMaterials;i++)
	{
		m_pMeshMaterials[i] = d3dxMaterials[i].MatD3D;

		m_pMeshMaterials[i].Ambient = m_pMeshMaterials[i].Diffuse;

		m_pMeshTextures[i] = NULL;
		if (d3dxMaterials[i].pTextureFilename != NULL && lstrlenA(d3dxMaterials[i].pTextureFilename) > 0)
		{
			const CHAR* prefix = "..\\Resources\\";
			CHAR texPath[MAX_PATH];
			sprintf_s(texPath, "..\\Resources\\%s", d3dxMaterials[i].pTextureFilename);
			if (FAILED(D3DXCreateTextureFromFileA(m_pD3DDevice, texPath, &m_pMeshTextures[i])))
				return false;
		}
	}

	pMtrlBuffer->Release();

	return true;

	/*
	if (FAILED(D3DXCreateTextureFromFile(m_pD3DDevice, "..\\Resources\\banana.bmp", &m_pTexture)))
	{
		return false;
	}
	
	if (FAILED(m_pD3DDevice->CreateVertexBuffer(50 * 2 * sizeof(VERTEX1), 0, D3DFVF_VERTEX1, D3DPOOL_DEFAULT, &m_pVB, NULL)))
	{
		return false;
	}

	VERTEX1* pVertices;
	if (FAILED(m_pVB->Lock(0, 0, (void**)&pVertices, 0)))
		return false;

	for (DWORD i = 0;i < 50;i++)
	{
		FLOAT theta = (2 * D3DX_PI*i) / (50 - 1);
		pVertices[2 * i + 0].position = D3DXVECTOR3(sinf(theta), -1.0f, cosf(theta));
		pVertices[2 * i + 0].color = 0x00000000;
		pVertices[2 * i + 0].tu = ((FLOAT)i) / (50 - 1);
		pVertices[2 * i + 0].tv = 1.0f;

		pVertices[2 * i + 1].position = D3DXVECTOR3(sinf(theta), 1.0f, cosf(theta));
		pVertices[2 * i + 1].color = 0xff808080;
		pVertices[2 * i + 1].tu = ((FLOAT)i) / (50 - 1);
		pVertices[2 * i + 1].tv = 0.0f;
	}

	m_pVB->Unlock();

	return true;
	*/
}

bool LGDIDirectX9::initIB()
{
	INDEX1 indices[] = 
	{
		{0,1,2}, {0,2,3},
		{4,6,5}, {4,7,6},
		{0,3,7}, {0,7,4},
		{1,5,6}, {1,6,2},
		{3,2,6}, {3,6,7},
		{0,4,5}, {0,5,1}
	};

	if (FAILED(m_pD3DDevice->CreateIndexBuffer(12 * sizeof(INDEX1), 0, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &m_pIB, NULL)))
		return false;

	VOID* pIndices;
	if (FAILED(m_pIB->Lock(0, sizeof(indices), (void**)&pIndices, 0)))
		return false;

	memcpy(pIndices, indices, sizeof(indices));
	m_pIB->Unlock();

	return true;
}

void LGDIDirectX9::setupMatrices()
{
	D3DXMATRIXA16 matWorld;
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixRotationX(&matWorld, timeGetTime()/ 500.0f);
	D3DXMATRIXA16 mat2;
	D3DXMatrixRotationY(&mat2, timeGetTime() / 500.0f);
	m_pD3DDevice->SetTransform(D3DTS_WORLD, &(matWorld * mat2));

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

void LGDIDirectX9::setupLights()
{
	D3DMATERIAL9 mtrl;
	ZeroMemory(&mtrl, sizeof(D3DMATERIAL9));

	mtrl.Diffuse.r = mtrl.Ambient.r = 1.0f;
	mtrl.Diffuse.g = mtrl.Ambient.g = 1.0f;
	mtrl.Diffuse.b = mtrl.Ambient.b = 0.0f;
	mtrl.Diffuse.a = mtrl.Ambient.a = 1.0f;
	m_pD3DDevice->SetMaterial(&mtrl);

	D3DXVECTOR3 vecDir;
	D3DLIGHT9 light;
	ZeroMemory(&light, sizeof(D3DLIGHT9));
	light.Type = D3DLIGHT_DIRECTIONAL;

	light.Diffuse.r = 1.0f;
	light.Diffuse.g = 1.0f;
	light.Diffuse.b = 1.0f;
	vecDir = D3DXVECTOR3(cosf(timeGetTime() / 350.0f), 1.0f, sinf(timeGetTime() / 350.0f));
	D3DXVec3Normalize((D3DXVECTOR3*)&light.Direction, &vecDir);
	light.Range = 1000.0f;
	m_pD3DDevice->SetLight(0, &light);
	m_pD3DDevice->LightEnable(0, TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_AMBIENT, 0x00202020);
}

bool LGDIDirectX9::render()
{
	m_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 255), 1.0f, 0);

	if (SUCCEEDED(m_pD3DDevice->BeginScene()))
	{
		//setupLights();
		setupMatrices();

		for (DWORD i = 0;i < m_dwNumMaterials;i++)
		{
			m_pD3DDevice->SetMaterial(&m_pMeshMaterials[i]);
			m_pD3DDevice->SetTexture(0, m_pMeshTextures[i]);

			m_pMesh->DrawSubset(i);
		}

// 		m_pD3DDevice->SetTexture(0, m_pTexture);
// 		m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
// 		m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
// 		m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
// 		m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

 		m_pD3DDevice->SetStreamSource(0, m_pVB, 0, sizeof(VERTEX1));
 		m_pD3DDevice->SetFVF(D3DFVF_VERTEX1);
		m_pD3DDevice->SetIndices(m_pIB);
		m_pD3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 8, 0, 12);
 		//m_pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2 * 50 -2 );

		m_pD3DDevice->EndScene();
	}

	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);

	return true;
}