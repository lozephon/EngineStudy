#pragma once
#include<d3d9.h>
#include<d3dx9.h>

#include"LGDIWrapper.h"

class LGDIDirectX9 : public LGDIWrapper
{
public:
	LGDIDirectX9(HWND hWnd, LGDI_Type type);
	bool render();
	~LGDIDirectX9();

private:
	bool initDevice();
	bool initGeometry();
	bool initIB();
	
	void setupMatrices();
	void setupLights();

	LPDIRECT3D9	m_pD3D;
	LPDIRECT3DDEVICE9 m_pD3DDevice;

	struct VERTEX1
	{
		D3DXVECTOR3 position;
		D3DCOLOR color;
		//FLOAT	tu, tv;
	};

#define D3DFVF_VERTEX1 (D3DFVF_XYZ | D3DFVF_DIFFUSE)

	IDirect3DVertexBuffer9* m_pVB;
	LPDIRECT3DTEXTURE9		m_pTexture;

	IDirect3DIndexBuffer9*	m_pIB;
	struct INDEX1
	{
		WORD _0, _1, _2;
	};

	LPD3DXMESH				m_pMesh;
	D3DMATERIAL9*			m_pMeshMaterials;
	LPDIRECT3DTEXTURE9*		m_pMeshTextures;
	DWORD					m_dwNumMaterials;
};