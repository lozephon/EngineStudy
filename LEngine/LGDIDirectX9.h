#pragma once
#include<d3d9.h>

#include"LGDIWrapper.h"

class LGDIDirectX9 : public LGDIWrapper
{
public:
	LGDIDirectX9(HWND hWnd, LGDI_Type type);
	bool render();
	~LGDIDirectX9();

private:
	bool initDevice();

	LPDIRECT3D9	m_pD3D;
	LPDIRECT3DDEVICE9 m_pD3DDevice;
};