#pragma once
#include "Include.h"
#include "Engine_Include.h"

#define GetOnceLBtnDown()		CMouseMgr::GetInstance()->IsOnceLBtnDown()
#define GetOnceRBtnDown()		CMouseMgr::GetInstance()->IsOnceRBtnDown()

#define GetOnceLbtnUp()			CMouseMgr::GetInstance()->IsOnceLbtnUp()
#define GetOnceRbtnUp()			CMouseMgr::GetInstance()->IsOnceRbtnUp()


class CMouseMgr
{
	DECLARE_SINGLETON(CMouseMgr)
private:
	CMouseMgr();
	~CMouseMgr();
public:
	static POINT GetCurPos(void);

public:
	void Update(void);

	bool IsOnceLBtnDown(void) { return m_IsOnceLBtnDown; }
	bool IsOnceRBtnDown(void) { return m_IsOnceRBtnDown; }
	bool IsOnceLbtnUp(void) { return m_IsOnceLbtnUp; }
	bool IsOnceRbtnUp(void) { return m_IsOnceRbtnUp; }
	void ShowMouseCursor(bool bShow);
private:
	bool			m_IsOnceLBtnDown;
	bool			m_IsOnceRBtnDown;
	bool			m_IsOnceLbtnUp;
	bool			m_IsOnceRbtnUp;
};

