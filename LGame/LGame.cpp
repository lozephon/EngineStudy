#include"../LEngine/LBuild.h"
#include "LGame.h"

LGame::LGame(HWND hWnd)
{
	m_GDIWrapper = LGDIWrapper::CreateLGDI(hWnd, LGDIWrapper::LGDI_Type_DirectX9);
}

LGame::~LGame()
{

}