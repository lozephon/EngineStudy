#pragma once
#include<d3d9.h>

#include"LGDIWrapper.h"

class LGDIDirectX9 : LGDIWrapper
{
public:
	LGDIDirectX9();
	bool initDevice();
	bool render();
	~LGDIDirectX9();

private:
	LPDIRECT3D9	m_pD3D;
	LPDIRECT3DDEVICE9 m_pD3DDevice;
};