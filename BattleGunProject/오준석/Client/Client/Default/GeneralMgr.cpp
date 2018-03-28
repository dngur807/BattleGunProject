#include "stdafx.h"
#include "GeneralMgr.h"
#include "CameraMgr.h"
#include "Include.h"
IMPLEMENT_SINGLETON(CGeneralMgr)

CGeneralMgr::CGeneralMgr()
{
	m_iCameraMode = CAMERA_FPS;
}

CGeneralMgr::~CGeneralMgr()
{

}

void CGeneralMgr::Update()
{
	if ((GetAsyncKeyState('0') & 0x8001) == 0x8001)
	{
		g_bCollRender = !g_bCollRender;
	}
	if ((GetAsyncKeyState('9') & 0x8001) == 0x8001)
	{
		g_bNaviRender = !g_bNaviRender;
	}
	switch (g_eMyState)
	{
	case CHARSTATE_ALIVE:
		AliveKeyCheck();
		break;
	case CHARSTATE_DEAD:
		DaedKeyCehck();
		break;

	}
}

void CGeneralMgr::AliveKeyCheck(void)
{
	if ((GetAsyncKeyState('C') & 0x8001) == 0x8001
		|| (GetAsyncKeyState('c') & 0x8001) == 0x8001)
	{
		m_iCameraMode++;
		if (m_iCameraMode > CAMERA_STATIC)
			m_iCameraMode = CAMERA_FPS;
		CCameraMgr::GetInstance()->SetCamera((CAMERAMODE)m_iCameraMode);
	}
}

void CGeneralMgr::DaedKeyCehck()
{

}

