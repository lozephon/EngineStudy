#pragma once
#include<Windows.h>

class WinMain
{
public:
	WinMain();
	void Run();
private:
	HWND hWnd;
	WNDCLASS WndClass;
};