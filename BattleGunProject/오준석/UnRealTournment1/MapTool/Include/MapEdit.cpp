// MapEdit.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Include.h"
#include "MapEdit.h"

#include "BackGround.h"
#include "Transform.h"
#include "StaticMesh.h"

#include "IncludeView.h"
#include "MainFrm.h"
#include "StaticMesh.h"



// CMapEdit

IMPLEMENT_DYNCREATE(CMapEdit, CFormView)

CMapEdit::CMapEdit()
	: CFormView(IDD_MAPEDIT)
	, m_pResourceMgr(Engine::Get_ResourceMgr())
	, bInitial(false)
	, m_fCubeAngleX(0)
	, m_fCubeAngleY(0)
	, m_fCubeAngleZ(0)
	, m_fCubeX(0)
	, m_fCubeY(0)
	, m_fCubeZ(0)
	, m_fCubeSizeX(0.f)
	, m_fCubeSizeY(0.f)
	, m_fCubeSizeZ(0.f)
{

}

CMapEdit::~CMapEdit()
{
}

void CMapEdit::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MeshID, m_ComMeshID);
	DDX_Control(pDX, IDC_EditCubeAngleX, m_SliderCubeAngleX);
	DDX_Text(pDX, IDC_EditCubeAngleX, m_fCubeAngleX);
	DDV_MinMaxFloat(pDX, m_fCubeAngleX, 0, 360);
	DDX_Text(pDX, IDC_EditCubeAngleY, m_fCubeAngleY);
	DDV_MinMaxFloat(pDX, m_fCubeAngleY, 0, 360);
	DDX_Text(pDX, IDC_EditCubeAngleZ, m_fCubeAngleZ);
	DDV_MinMaxFloat(pDX, m_fCubeAngleZ, 0, 360);
	DDX_Text(pDX, IDC_EditCubeX, m_fCubeX);
	DDX_Text(pDX, IDC_EditCubeY, m_fCubeY);
	DDX_Text(pDX, IDC_EditCubeZ, m_fCubeZ);
	DDX_Text(pDX, IDC_EditCubeCX, m_fCubeSizeX);
	DDX_Text(pDX, IDC_EditCubeCY, m_fCubeSizeY);
	DDX_Text(pDX, IDC_EditCubeCZ, m_fCubeSizeZ);
}

BEGIN_MESSAGE_MAP(CMapEdit, CFormView)
	ON_CBN_SELCHANGE(IDC_MeshID, &CMapEdit::OnCbnSelchangeMeshid)
	ON_BN_CLICKED(IDC_BTN_MAPSAVE, &CMapEdit::OnBnClickedBtnMapsave)
	ON_BN_CLICKED(IDC_BTN_MAPLOAD, &CMapEdit::OnBnClickedBtnMapload)
	ON_EN_CHANGE(IDC_EditCubeAngleX, &CMapEdit::OnEnChangeEditcubeanglex)
	ON_EN_CHANGE(IDC_EditCubeAngleY, &CMapEdit::OnEnChangeEditcubeangley)
	ON_EN_CHANGE(IDC_EditCubeAngleZ, &CMapEdit::OnEnChangeEditcubeanglez)
	ON_BN_CLICKED(IDC_BTN_CubeAngleApply, &CMapEdit::OnBnClickedBtnCubeangleapply)
	ON_BN_CLICKED(IDC_BTN_CubeAngleInit, &CMapEdit::OnBnClickedBtnCubeangleinit)
	ON_BN_CLICKED(IDC_CAMERAINI, &CMapEdit::OnBnClickedCameraini)
	ON_EN_CHANGE(IDC_EditCubeX, &CMapEdit::OnEnChangeEditcubex)
	ON_EN_CHANGE(IDC_EditCubeY, &CMapEdit::OnEnChangeEditcubey)
	ON_EN_CHANGE(IDC_EditCubeZ, &CMapEdit::OnEnChangeEditcubez)
	ON_BN_CLICKED(IDC_BTN_CubePosApply, &CMapEdit::OnBnClickedBtnCubeposapply)
	ON_BN_CLICKED(IDC_BTN_CubePosInit, &CMapEdit::OnBnClickedBtnCubeposinit)
	ON_EN_CHANGE(IDC_EditCubeCX, &CMapEdit::OnEnChangeEditcubecx)
	ON_EN_CHANGE(IDC_EditCubeCY, &CMapEdit::OnEnChangeEditcubecy)
	ON_EN_CHANGE(IDC_EditCubeCZ, &CMapEdit::OnEnChangeEditcubecz)
	ON_BN_CLICKED(IDC_BTN_CubeSizeApply, &CMapEdit::OnBnClickedBtnCubesizeapply)
	ON_BN_CLICKED(IDC_BTN_CubeSizeInit, &CMapEdit::OnBnClickedBtnCubesizeinit)
	ON_BN_CLICKED(IDC_BTN_NAVILOADT, &CMapEdit::OnBnClickedBtnNaviloadt)
	ON_BN_CLICKED(IDC_BTN_NAVISAVET, &CMapEdit::OnBnClickedBtnNavisavet)
END_MESSAGE_MAP()


// CMapEdit 진단입니다.

#ifdef _DEBUG
void CMapEdit::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CMapEdit::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMapEdit 메시지 처리기입니다.


void CMapEdit::OnCbnSelchangeMeshid()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_pBackGround->SetMeshNum(m_ComMeshID.GetCurSel());
}


void CMapEdit::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	m_pMainFrame = ((CMainFrame*)AfxGetMainWnd());
	m_pMainView = m_pMainFrame->GetMainView();
	m_pBackGround = m_pMainView->GetBackGround();

	//
	TCHAR szPath[255];

	//for (int i = 1; i<134; ++i)
	//{
	//	if (i<10)
	//	{
	//		wsprintf(szPath, L"00%d", i);	//요건 이해 했지? 0~64
	//	}
	//	else if (i>=10&& i<100)
	//	{
	//		wsprintf(szPath, L"0%d", i);	//요건 이해 했지? 0~64
	//	}
	//	else
	//	{
	//		wsprintf(szPath, L"%d", i);		//요건 이해 했지? 0~64
	//	}

	//	m_ComMeshID.AddString(szPath);
	//}

	for (int i = 1; i < 173; ++i)
	{
		if (i < 10)
		{
			wsprintf(szPath, L"00%d", i);	//요건 이해 했지? 0~64
		}
		else if (i >= 10 && i < 100)
		{
			wsprintf(szPath, L"0%d", i);	//요건 이해 했지? 0~64
		}
		else
		{
			wsprintf(szPath, L"%d", i);		//요건 이해 했지? 0~64
		}

		m_ComMeshID.AddString(szPath);
	}
}


void CMapEdit::OnBnClickedBtnMapsave()
{
	// 맵 저장 버튼
	CFileDialog Dlg(FALSE, L"dat", L"*.dat", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, L"*.dat", this);
	if (Dlg.DoModal() == IDCANCEL)
		return;

	Dlg.m_ofn.lpstrInitialDir = L"..\\Data";
	HANDLE hFile = CreateFile(Dlg.GetPathName(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	DWORD dwByte;

	vector<Engine::CStaticMesh*>::iterator		iter = m_pBackGround->GetMeshVector()->begin();
	vector<Engine::CStaticMesh*>::iterator		iter_end = m_pBackGround->GetMeshVector()->end();
	
	for (iter; iter != iter_end; ++iter)
	{
		{
			Engine::MYMESH tTemp;
			tTemp.vPos = (*iter)->GetInfo()->m_vPos;
			tTemp.vSize = (*iter)->GetInfo()->m_vScale;
			tTemp.fAngle[Engine::ANGLE_X] = (*iter)->GetInfo()->m_fAngle[Engine::ANGLE_X];
			tTemp.fAngle[Engine::ANGLE_Y] = (*iter)->GetInfo()->m_fAngle[Engine::ANGLE_Y];
			tTemp.fAngle[Engine::ANGLE_Z] = (*iter)->GetInfo()->m_fAngle[Engine::ANGLE_Z];
			tTemp.dwObjNum = (*iter)->GetMeshNum();
			WriteFile(hFile, &tTemp, sizeof(Engine::MYMESH), &dwByte, NULL);
		}
	}
	iter = m_pBackGround->GetMeshVector()->begin();

	CloseHandle(hFile);
}


void CMapEdit::OnBnClickedBtnMapload()
{
	// 맵 로드 버튼
	CFileDialog Dlg(TRUE, L"dat", L"*.dat", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, L"*.dat", this);
	if (Dlg.DoModal() == IDCANCEL)
		return;
	Dlg.m_ofn.lpstrInitialDir = L"..\\Data";
	HANDLE hFile = CreateFile(Dlg.GetPathName(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	DWORD dwByte;

	// 벡터 초기화
	vector<Engine::CStaticMesh*>::iterator	iter = m_pBackGround->GetMeshVector()->begin();
	vector<Engine::CStaticMesh*>::iterator	iter_end = m_pBackGround->GetMeshVector()->end();
	for (iter; iter != iter_end; ++iter)
	{
		(*iter)->Release();
		Safe_Delete(*iter);
	}
	m_pBackGround->GetMeshVector()->clear();
	while (true)
	{


		Engine::MYMESH tTemp;
		ReadFile(hFile, &tTemp, sizeof(Engine::MYMESH), &dwByte, NULL);

		if (dwByte == 0)
			break;

		Engine::CStaticMesh*		pBuffer;
		Engine::CComponent*		pComponent = NULL;
		switch (tTemp.dwObjNum)
		{
		//case 0: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"build1"); break;
		//case 1: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"build2"); break;
		//case 2: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"build3"); break;
		//case 3: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"build4"); break;
		//case 4: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"build5"); break;
		//case 5: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"build6"); break;
		//case 6: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"build7"); break;
		//case 7: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"build8"); break;
		//case 8: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"build9"); break;
		//case 9: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"build10"); break;
		//case 10: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"build11"); break;
		//case 11: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"build12"); break;
		//case 12: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd13_1"); break;
		//case 13: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd13_2"); break;
		//case 14: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd14_1"); break;
		//case 15: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd14_2"); break;
		//case 16: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd14_3"); break;
		//case 17: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd14_4"); break;
		//case 18: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd14_5"); break;
		//case 19: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd14_6"); break;
		//case 20: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd14_7"); break;
		//case 21: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd14_8"); break;
		//case 22: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd14_9"); break;
		//case 23: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd14_10"); break;
		//case 24: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd14_11"); break;
		//case 25: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd14_12"); break;
		//case 26: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd14_13"); break;
		//case 27: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd14_14"); break;
		//case 28: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd14_15"); break;
		//case 29: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd14_16"); break;
		//case 30: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd14_17"); break;
		//case 31: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd14_18"); break;
		//case 32: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd14_19"); break;
		//case 33: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd14_20"); break;
		//case 34: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd14_21"); break;
		//case 35: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd14_22"); break;
		//case 36: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd14_23"); break;
		//case 37: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd14_24"); break;
		//case 38: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd14_25"); break;
		//case 39: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_1"); break;
		//case 40: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_2"); break;
		//case 41: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_3"); break;
		//case 42: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_4"); break;
		//case 43: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_5"); break;
		//case 44: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_6"); break;
		//case 45: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_7"); break;
		//case 46: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_8"); break;
		//case 47: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_9"); break;
		//case 48: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_10"); break;
		//case 49: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_11"); break;
		//case 50: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_12"); break;
		//case 51: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_13"); break;
		//case 52: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_14"); break;
		//case 53: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_15"); break;
		//case 54: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_16"); break;
		//case 55: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_17"); break;
		//case 56: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_18"); break;
		//case 57: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_19"); break;
		//case 58: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_20"); break;
		//case 59: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_21"); break;
		//case 60: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_22"); break;
		//case 61: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_23"); break;
		//case 62: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_24"); break;
		//case 63: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_25"); break;
		//case 64: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_26"); break;
		//case 65: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_27"); break;
		//case 66: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_28"); break;
		//case 67: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_29"); break;
		//case 68: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_30"); break;
		//case 69: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_31"); break;
		//case 70: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_32"); break;
		//case 71: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_33"); break;
		//case 72: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_34"); break;
		//case 73: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_35"); break;
		//case 74: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_36"); break;
		//case 75: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_37"); break;
		//case 76: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_38"); break;
		//case 77: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_39"); break;
		//case 78: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_40"); break;
		//case 79: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_41"); break;
		//case 80: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_42"); break;
		//case 81: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_43"); break;
		//case 82: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_44"); break;
		//case 83: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_45"); break;
		//case 84: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_46"); break;
		//case 85: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd16_1"); break;
		//case 86: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd16_2"); break;
		//case 87: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd16_3"); break;
		//case 88: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd16_4"); break;
		//case 89: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd16_5"); break;
		//case 90: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd16_6"); break;
		//case 91: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd16_7"); break;
		//case 92: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd16_8"); break;
		//case 93: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd16_9"); break;
		//case 94: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd16_10"); break;
		//case 95: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd16_11"); break;
		//case 96: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd16_12"); break;
		//case 97: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd16_13"); break;
		//case 98: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd16_14"); break;
		//case 99: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd16_15"); break;
		//case 100: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd16_16"); break;
		//case 101: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd16_17"); break;
		//case 102: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd16_18"); break;
		//case 103: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd16_19"); break;
		//case 104: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd16_20"); break;
		//case 105: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd16_21"); break;
		//case 106: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd16_22"); break;
		//case 107: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd16_23"); break;
		//case 108: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd16_24"); break;
		//case 109: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd16_25"); break;
		//case 110: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd16_26"); break;
		//case 111: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Terrain_1"); break;
		//case 112: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Terrain_2"); break;
		//case 113: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Terrain_3"); break;
		//case 114: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Terrain_4"); break;
		//case 115: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Terrain_5"); break;
		//case 116: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Terrain_6"); break;
		//case 117: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Terrain_7"); break;
		//case 118: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Terrain_8"); break;
		//case 119: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Terrain_9"); break;
		//case 120: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Terrain_10"); break;
		//case 121: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Terrain_11"); break;
		//case 122: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Terrain_12"); break;
		//case 123: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Terrain_13"); break;
		//case 124: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Terrain_14"); break;
		//case 125: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Terrain_15"); break;
		//case 126: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Terrain_16"); break;
		//case 127: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Terrain_17"); break;
		//case 128: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Terrain_18"); break;
		//case 129: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Terrain_19"); break;
		//case 130: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Terrain_20"); break;
		//case 131: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Terrain_21"); break;
		//case 132: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Terrain_22"); break;

		case 0: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_1"); break;
		case 1: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_2"); break;
		case 2: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_3"); break;
		case 3: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_4"); break;
		case 4: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_5"); break;
		case 5: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_6"); break;
		case 6: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_7"); break;
		case 7: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_8"); break;
		case 8: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_9"); break;
		case 9: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_10"); break;
		case 10: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_11"); break;
		case 11: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_12"); break;
		case 12: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_13"); break;
		case 13: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_14"); break;
		case 14: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_15"); break;
		case 15: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_16"); break;
		case 16: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_17"); break;
		case 17: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_18"); break;
		case 18: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_19"); break;
		case 19: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_20"); break;
		case 20: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_21"); break;
		case 21: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_22"); break;
		case 22: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_23"); break;
		case 23: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_24"); break;
		case 24: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_25"); break;
		case 25: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_26"); break;
		case 26: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_27"); break;
		case 27: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_28"); break;
		case 28: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_29"); break;
		case 29: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_30"); break;
		case 30: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_31"); break;
		case 31: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_32"); break;
		case 32: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_33"); break;
		case 33: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_34"); break;
		case 34: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_35"); break;
		case 35: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_36"); break;
		case 36: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_37"); break;
		case 37: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_38"); break;
		case 38: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_39"); break;
		case 39: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_40"); break;
		case 40: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_41"); break;
		case 41: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_42"); break;
		case 42: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_43"); break;
		case 43: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_44"); break;
		case 44: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_45"); break;
		case 45: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_46"); break;
		case 46: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_47"); break;
		case 47: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_48"); break;
		case 48: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_49"); break;
		case 49: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_50"); break;
		case 50: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_51"); break;
		case 51: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_52"); break;
		case 52: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_53"); break;
		case 53: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_54"); break;
		case 54: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_55"); break;
		case 55: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_56"); break;
			//case 56: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_57"); break;
			//case 57: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_58"); break;
		case 58: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_59"); break;
		case 59: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_60"); break;
		case 60: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_61"); break;
		case 61: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_62"); break;
		case 62: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_63"); break;
		case 63: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_64"); break;
		case 64: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_65"); break;
		case 65: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_66"); break;
		case 66: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_67"); break;
		case 67: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_68"); break;
		case 68: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_69"); break;
		case 69: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_70"); break;
		case 70: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_71"); break;
		case 71: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_72"); break;
		case 72: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_73"); break;
		case 73: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_74"); break;
		case 74: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_75"); break;
		case 75: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_76"); break;
		case 76: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_77"); break;
		case 77: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_78"); break;
		case 78: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_79"); break;
		case 79: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_80"); break;
		case 80: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_81"); break;
		case 81: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_82"); break;
		case 82: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_83"); break;
		case 83: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_84"); break;
		case 84: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_85"); break;
		case 85: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_86"); break;
		case 86: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_87"); break;
		case 87: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_88"); break;
		case 88: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_89"); break;
		case 89: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_90"); break;
		case 90: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_91"); break;
		case 91: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_92"); break;
		case 92: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_93"); break;
		case 93: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_94"); break;
		case 94: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_95"); break;
		case 95: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_96"); break;
		case 96: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_97"); break;
		case 97: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_98"); break;
		case 98: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_99"); break;
		case 99: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_100"); break;
		case 100: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_101"); break;
		case 101: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_102"); break;
		case 102: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_103"); break;
		case 103: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_104"); break;
		case 104: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_105"); break;
		case 105: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_106"); break;
		}

		pBuffer = dynamic_cast<Engine::CStaticMesh*>(pComponent);
		NULL_CHECK(pBuffer);
		pBuffer->SetMeshNum(tTemp.dwObjNum);
		pBuffer->SetvPosInfo(tTemp.vPos);
		pBuffer->SetvSizeInfo(tTemp.vSize);
		pBuffer->SetAngleInfo(tTemp.fAngle[Engine::ANGLE_X], tTemp.fAngle[Engine::ANGLE_Y], tTemp.fAngle[Engine::ANGLE_Z]);
		//pBuffer->SetObjOption(tTemp.eOption);
		//pBuffer->SetTextureNumber(tTemp.dwTextureNumber);
		m_pBackGround->GetMeshVector()->push_back(pBuffer);
	}
	CloseHandle(hFile);
}


void CMapEdit::OnEnChangeEditcubeanglex()
{
	if (!bInitial)
		return;
	UpdateData(TRUE);
	//m_SliderCubeAngleX.SetPos(static_cast<int>(m_fCubeAngleX));
	UpdateData(FALSE);
}


void CMapEdit::OnEnChangeEditcubeangley()
{
	if (!bInitial)
		return;
	UpdateData(TRUE);
	//m_SliderCubeAngleY.SetPos(static_cast<int>(m_fCubeAngleY));
	UpdateData(FALSE);
}


void CMapEdit::OnEnChangeEditcubeanglez()
{
	if (!bInitial)
		return;
	UpdateData(TRUE);
	//m_SliderCubeAngleZ.SetPos(static_cast<int>(m_fCubeAngleZ));
	UpdateData(FALSE);
}


void CMapEdit::OnBnClickedBtnCubeangleapply()
{
	// 각도 적용 버튼
	UpdateData(TRUE);
	m_pBackGround->SetCubeAngle(m_fCubeAngleX, m_fCubeAngleY, m_fCubeAngleZ);
	//cout << m_fCubeAngleX << endl;
	UpdateData(FALSE);
}


void CMapEdit::OnBnClickedBtnCubeangleinit()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	m_pBackGround->SetAngleIni();
	//cout << m_fCubeAngleX << endl;
	UpdateData(FALSE);
}


void CMapEdit::OnBnClickedCameraini()
{
	UpdateData(TRUE);
	m_pBackGround->CameraIni();
	UpdateData(FALSE);
}


void CMapEdit::OnEnChangeEditcubex()
{
	UpdateData(TRUE);
	m_pBackGround->SetCubeAngle(m_fCubeAngleX, m_fCubeAngleY, m_fCubeAngleZ);
	//cout << m_fCubeAngleX << endl;
	UpdateData(FALSE);
	

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CMapEdit::OnEnChangeEditcubey()
{
	

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CMapEdit::OnEnChangeEditcubez()
{
	
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CMapEdit::OnBnClickedBtnCubeposapply()
{
	// 위치 적용 버튼
	UpdateData(TRUE);
	m_pVecMesh=m_pBackGround->GetMeshVector();


	int iDestIndex = -1;
	float fDestDist = 99999.f;

	if (m_pVecMesh->size() != 0)
	{
		vector<Engine::CStaticMesh*>::iterator	iter = m_pVecMesh->end();
		iter--;

		for (int i = 0; i < iDestIndex; ++i)
		{
			++iter;
		}
		(*iter)->SetvPosInfo(XMFLOAT3(m_fCubeX, m_fCubeY, m_fCubeZ));
		//(*iter)->GetInfo()->m_fAngle[Engine::ANGLE_X] += 0.01f;
		//(*iter)->SetAngleInfo(_x, _y, _z);
	}
	
	UpdateData(FALSE);
}


void CMapEdit::OnBnClickedBtnCubeposinit()
{
	XMFLOAT3 tTemp;
	// 위치 적용 버튼
	UpdateData(TRUE);
	m_pVecMesh = m_pBackGround->GetMeshVector();

	int iDestIndex = -1;
	float fDestDist = 99999.f;

	if (m_pVecMesh->size() != 0)
	{
		vector<Engine::CStaticMesh*>::iterator	iter = m_pVecMesh->end();
		iter--;
		tTemp = (*iter)->GetInfo()->m_vPos;

		for (int i = 0; i < iDestIndex; ++i)
		{
			++iter;
		}
		(*iter)->SetvPosInfo(tTemp);
	}

	UpdateData(FALSE);
}


void CMapEdit::OnEnChangeEditcubecx()
{

}


void CMapEdit::OnEnChangeEditcubecy()
{
	
}


void CMapEdit::OnEnChangeEditcubecz()
{

}


void CMapEdit::OnBnClickedBtnCubesizeapply()
{
	// 위치 적용 버튼
	UpdateData(TRUE);
	m_pVecMesh = m_pBackGround->GetMeshVector();


	int iDestIndex = -1;
	float fDestDist = 99999.f;

	if (m_pVecMesh->size() != 0)
	{
		vector<Engine::CStaticMesh*>::iterator	iter = m_pVecMesh->end();
		iter--;

		for (int i = 0; i < iDestIndex; ++i)
		{
			++iter;
		}
		(*iter)->SetvSizeInfo(XMFLOAT3(m_fCubeSizeX*0.01f, m_fCubeSizeY*0.01f, m_fCubeSizeZ*0.01f));
		//(*iter)->GetInfo()->m_fAngle[Engine::ANGLE_X] += 0.01f;
		//(*iter)->SetAngleInfo(_x, _y, _z);
	}

	UpdateData(FALSE);
}


void CMapEdit::OnBnClickedBtnCubesizeinit()
{
	// 위치 적용 버튼
	UpdateData(TRUE);
	m_pVecMesh = m_pBackGround->GetMeshVector();


	int iDestIndex = -1;
	float fDestDist = 99999.f;

	if (m_pVecMesh->size() != 0)
	{
		vector<Engine::CStaticMesh*>::iterator	iter = m_pVecMesh->end();
		iter--;

		for (int i = 0; i < iDestIndex; ++i)
		{
			++iter;
		}
		(*iter)->SetvSizeInfo(XMFLOAT3(0.01f,0.01f, 0.01f));
		//(*iter)->GetInfo()->m_fAngle[Engine::ANGLE_X] += 0.01f;
		//(*iter)->SetAngleInfo(_x, _y, _z);
	}

	UpdateData(FALSE);
}


void CMapEdit::OnBnClickedBtnNavisavet()
{
	//네비저장
	UpdateData(TRUE);
	m_pBackGround->SaveNavMesh();
	UpdateData(FALSE);

}


void CMapEdit::OnBnClickedBtnNaviloadt()
{
	//네비로드
	UpdateData(TRUE);
	m_pBackGround->LoadNavMesh();
	UpdateData(FALSE);
}

