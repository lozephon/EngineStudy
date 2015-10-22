#pragma once
#include"LBuild.h"

class LGDIWrapper
{
public:
	// Type Define
	enum LGDI_Type
	{
		LGDI_Type_DirectX9 = 0,
		LGDI_Type_OpenGL,
		LGDI_Type_DirectX12
	};

	// Function Define
	LGDIWrapper(HWND hWnd, LGDI_Type type = LGDI_Type_DirectX9) : m_hWnd(hWnd), m_LGID_Type(type) {};

	virtual bool initDevice() = 0;
	virtual bool render() = 0;
	// Variable Define
	LGDI_Type m_LGID_Type;

protected:
	HWND m_hWnd;

private:
	
};