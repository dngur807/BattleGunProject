#pragma once

#include "Export_Function.h"
#include "afxwin.h"

// CMapEdit 폼 뷰입니다.

class CMainFrame;
class CIncludeView;
class CBackGround;

namespace Engine
{
	class CResourceMgr;
	class CStaticMesh;
}

class CMapEdit : public CFormView
{
	DECLARE_DYNCREATE(CMapEdit)

private:
	CMainFrame*			m_pMainFrame;
	CIncludeView*		m_pMainView;
	CBackGround*		m_pBackGround;
	Engine::CResourceMgr*	m_pResourceMgr;
	vector<Engine::CStaticMesh*>* m_pVecMesh;
public:
	bool bInitial;

protected:
	CMapEdit();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CMapEdit();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAPEDIT };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCbnSelchangeMeshid();
	virtual void OnInitialUpdate();
	CComboBox m_ComMeshID;
	afx_msg void OnBnClickedBtnMapsave();
	afx_msg void OnBnClickedBtnMapload();
	afx_msg void OnEnChangeEditcubeanglex();
	afx_msg void OnEnChangeEditcubeangley();
	afx_msg void OnEnChangeEditcubeanglez();
	CEdit m_SliderCubeAngleX;
	afx_msg void OnBnClickedBtnCubeangleapply();
	afx_msg void OnBnClickedBtnCubeangleinit();
	float m_fCubeAngleX;
	float m_fCubeAngleY;
	float m_fCubeAngleZ;
	//float m_fCubeX;
	//float m_fCubeY;
	//float m_fCubeZ;
	//float m_fCubeSizeX;
	//float m_fCubeSizeY;
	//float m_fCubeSizeZ;
	afx_msg void OnBnClickedCameraini();
	afx_msg void OnEnChangeEditcubex();
	afx_msg void OnEnChangeEditcubey();
	afx_msg void OnEnChangeEditcubez();
	afx_msg void OnBnClickedBtnCubeposapply();
	afx_msg void OnBnClickedBtnCubeposinit();
	afx_msg void OnEnChangeEditcubecx();
	afx_msg void OnEnChangeEditcubecy();
	afx_msg void OnEnChangeEditcubecz();
	afx_msg void OnBnClickedBtnCubesizeapply();
	afx_msg void OnBnClickedBtnCubesizeinit();
	float m_fCubeX;
	float m_fCubeY;
	float m_fCubeZ;
	float m_fCubeSizeX;
	float m_fCubeSizeY;
	float m_fCubeSizeZ;
	afx_msg void OnBnClickedBtnNaviloadt();
	afx_msg void OnBnClickedBtnNavisavet();
};


