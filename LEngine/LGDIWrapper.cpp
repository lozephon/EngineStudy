#include"LGDIWrapper.h"
#include"LGDIDirectX9.h"

LGDIWrapper* LGDIWrapper::CreateLGDI(HWND hWnd, LGDI_Type type/* = LGDI_Type_DirectX9*/)
{
	switch (type)
	{
	case LGDIWrapper::LGDI_Type_DirectX9:
		return new LGDIDirectX9(hWnd, type);
		break;
	case LGDIWrapper::LGDI_Type_OpenGL:
		break;
	case LGDIWrapper::LGDI_Type_DirectX12:
		break;
	default:
		break;
	}

	return new LGDIDirectX9(hWnd, type);
}