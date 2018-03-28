#include "stdafx.h"
#include "KeyMgr.h"


IMPLEMENT_SINGLETON(CKeyMgr)

CKeyMgr::CKeyMgr()
{

	memset(m_ArrKeyDown, 0, sizeof(bool) * E_KEY_MAX);
	memset(m_ArrKeyUp, 0, sizeof(bool) * E_KEY_MAX);
}


CKeyMgr::~CKeyMgr()
{
}

bool CKeyMgr::IsOnceKeyDown(DWORD _dwKey)
{
	if (GetAsyncKeyState(_dwKey) & 0x8000)
	{
		if (m_ArrKeyDown[_dwKey] == false)
		{
			m_ArrKeyDown[_dwKey] = true;
			return m_ArrKeyDown[_dwKey];
		}
	}
	else
	{
		m_ArrKeyDown[_dwKey] = false;
	}
	return false;
}

bool CKeyMgr::IsStayKeyDown(DWORD _dwKey)
{
	if ((GetAsyncKeyState(_dwKey) & 0x8000))return true;
	return false;
}

bool CKeyMgr::IsOnceKeyUp(DWORD _dwKey)
{
	if (GetAsyncKeyState(_dwKey) & 0x8000)
	{
		m_ArrKeyUp[_dwKey] = true;
	}
	else
	{
		if (m_ArrKeyUp[_dwKey])
		{
			m_ArrKeyUp[_dwKey] = false;
			return true;
		}
	}
	return false;
}
