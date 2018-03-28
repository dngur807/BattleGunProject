#include "stdafx.h"
#include "MouseMgr.h"
#include "KeyMgr.h"

IMPLEMENT_SINGLETON(CMouseMgr)

CMouseMgr::CMouseMgr()
{
	m_IsOnceLBtnDown = false;
	m_IsOnceRBtnDown = false;
	m_IsOnceLbtnUp = false;
	m_IsOnceRbtnUp = false;
}


CMouseMgr::~CMouseMgr()
{
}

POINT CMouseMgr::GetCurPos(void)
{
	POINT ptCursor;
	GetCursorPos(&ptCursor);
	ScreenToClient(g_hWnd, &ptCursor);
	return ptCursor;
}

void CMouseMgr::Update(void)
{
	m_IsOnceLBtnDown = GetOnceKeyDown(VK_LBUTTON);
	m_IsOnceRBtnDown = GetOnceKeyDown(VK_RBUTTON);
	m_IsOnceLbtnUp = GetOnceKeyUp(VK_LBUTTON);
	m_IsOnceRbtnUp = GetOnceKeyUp(VK_RBUTTON);
}
void CMouseMgr::ShowMouseCursor(bool bShow)
{
	if (bShow)
	{
		while (ShowCursor(bShow) < 0)
		{

		}
	}
	else
	{
		while (ShowCursor(bShow) >= 0)
		{

		}
	}
}
