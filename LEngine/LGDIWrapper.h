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

	virtual bool initDevice() = 0;
	virtual bool render() = 0;
	// Variable Define
	LGDI_Type m_LGDI_Type;

	static LGDIWrapper* CreateLGDI(HWND hWnd, LGDI_Type type = LGDI_Type_DirectX9);

protected:
	HWND m_hWnd;
	LGDI_Type m_GDIType;

	LGDIWrapper(HWND hWnd, LGDI_Type type) :
		m_hWnd(hWnd), m_GDIType(type)
	{};

private:
	
};