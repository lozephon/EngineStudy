#pragma once
#include"../LEngine/LEngine.h"

class LGame
{
public:
	LGame(HWND hWnd);
	~LGame();

	LGDIWrapper* GetGDI() { return m_GDIWrapper; }
private:
	LEngine *m_engine;
	LGDIWrapper *m_GDIWrapper;
};