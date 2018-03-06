
// Include.h : Include ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.


// CIncludeApp:
// �� Ŭ������ ������ ���ؼ��� Include.cpp�� �����Ͻʽÿ�.
//

class CIncludeView;
class CMainFrame;
class CBackGround;
class CIncludeApp : public CWinAppEx
{
public:
	CIncludeApp();
private:
	CMainFrame*		m_pMainFrame;
	CIncludeView*		m_pMainView;
	CBackGround*	m_pBackGround;

// �������Դϴ�.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// �����Դϴ�.
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnIdle(LONG lCount);
};

extern CIncludeApp theApp;
