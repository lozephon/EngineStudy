#pragma once
#include<Windows.h>

enum EGDIType
{
	EGDIType_DX9,
	EGDIType_OpenGL,
	EGDIType_DX11
};

const EGDIType g_EGDIType = EGDIType_DX9;