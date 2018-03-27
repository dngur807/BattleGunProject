#pragma once

#include "Enum.h"
#include "Const.h"
#include "Define.h"
#include "Struct.h"

#include "LightHelper.h"
#include "MathHelper.h"
extern HWND g_hWnd;
extern HINSTANCE g_hInst;
extern bool g_bInputMode;
extern bool g_bNetwork;
extern int g_iIndex;

extern XMFLOAT3 g_vWorldCamera;
extern XMFLOAT3	g_vFpsAt;
extern int			g_iCamMode;

extern CHARSTATE	g_eMyState;

extern bool			g_bCollRender;