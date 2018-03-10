#include "StdAfx.h"
#include "Mouse.h"

POINT CMouse::m_ptMouse;

POINT CMouse::GetMousePos(void)
{
	return m_ptMouse;
}

void CMouse::SetMousePos(void)
{
	GetCursorPos(&m_ptMouse);
	ScreenToClient(g_hWnd, &m_ptMouse);
}
