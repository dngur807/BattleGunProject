#pragma once
#include "Include.h"
#include "Engine_Include.h"

#define GetOnceKeyDown(dwKey)			CKeyMgr::GetInstance()->IsOnceKeyDown(dwKey)	
#define GetStayKeyDown(dwKey)			CKeyMgr::GetInstance()->IsStayKeyDown(dwKey)
#define GetOnceKeyUp(dwKey)				CKeyMgr::GetInstance()->IsOnceKeyUp(dwKey)
#define GetStayKeyUp(dwKey)				CKeyMgr::GetInstance()->IsStayKeyUp(dwKey)

class CKeyMgr
{
	DECLARE_SINGLETON(CKeyMgr)
private:
	CKeyMgr();
	~CKeyMgr();

public:
	enum { E_KEY_MAX = 256 };

public:
	bool IsOnceKeyDown(DWORD _dwKey);
	bool IsStayKeyDown(DWORD _dwKey);
	bool IsOnceKeyUp(DWORD _dwKey);
	bool IsStayKeyUp(DWORD _dwKey) { return !(IsStayKeyDown(_dwKey)); }


	bool* GetKeyDown(void) { return m_ArrKeyDown; }
	bool* GetKeyUp(void) { return m_ArrKeyUp; }
private:
	bool m_ArrKeyDown[E_KEY_MAX];
	bool m_ArrKeyUp[E_KEY_MAX];
};

