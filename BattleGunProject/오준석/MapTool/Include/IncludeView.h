
// IncludeView.h : CIncludeView Ŭ������ �������̽�
//

#pragma once

#include "Engine_Include.h"
#include "Include1.h"


namespace Engine
{
	class CGraphicDev;
	class CManagement;
	class CResourceMgr;
	class CTimeMgr;
}

class CIncludeDoc;
class CMainFrame;
class CMapEdit;
class CBackGround;

class CIncludeView : public CView
{
protected: // serialization������ ��������ϴ�.
	CIncludeView();
	DECLARE_DYNCREATE(CIncludeView)

private:
	Engine::CGraphicDev*	m_pGraphicDev;
	Engine::MYGDI*			m_pMyGDI;

	Engine::CManagement*	m_pManagement;
	Engine::CResourceMgr*	m_pResourceMgr;
	Engine::CTimeMgr*		m_pTimeMgr;
	ID3DX11Effect*	m_pFX;
	
public:
	CBackGround* GetBackGround() { return m_pBackGround; }
private:
	CMainFrame*					m_pMainFrame;
	CMapEdit*					m_pMapEdit;
	CBackGround*				m_pBackGround;
	DirectionalLight m_DirLight;


// Ư���Դϴ�.
public:
	CIncludeDoc* GetDocument() const;

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual void OnDraw(CDC* pDC);  // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// �����Դϴ�.
public:
	virtual ~CIncludeView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // IncludeView.cpp�� ����� ����
inline CIncludeDoc* CIncludeView::GetDocument() const
   { return reinterpret_cast<CIncludeDoc*>(m_pDocument); }
#endif

