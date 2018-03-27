#include "stdafx.h"
#include "MainApp.h"

#include "Export_Function.h"
#include "MathHelper.h"
#include "Effects.h"
#include "SceneSelector.h"
#include "WaveFrontReader.h"
#include "ModelMgr.h"
#include "FontMgr.h"
#include "NetMgr.h"

#include "TextMgr.h"
#include "UserMgr.h"

#include "KeyMgr.h"
#include "MouseMgr.h"
#include "GameMgr.h"
#include "CameraMgr.h"
#include "GeneralMgr.h"
#include "BulletMgr.h"
CMainApp::CMainApp()
	:m_pGraphicDev(Engine::Get_GraphicDev()),
	m_pMyGDI(new Engine::MYGDI),
	m_pFX(nullptr), 
	m_pManagement(nullptr), 
	m_pResourceMgr(nullptr), 
	m_pTimeMgr(nullptr)
{
}


CMainApp::~CMainApp()
{
	Release();
}

HRESULT CMainApp::InitApp(void)
{

	g_bCollRender = true;
	HRESULT hr = NULL;

	hr = m_pGraphicDev->InitGraphicDev(Engine::CGraphicDev::MODE_WIN,
		g_hWnd, WINCX, WINCY);
	FAILED_CHECK_RETURN_MSG(hr, E_FAIL, L"����̽� ���� ����");

	m_pMyGDI->pDevice = m_pGraphicDev->GetDevice();
	NULL_CHECK_RETURN(m_pMyGDI->pDevice, E_FAIL);
	m_pMyGDI->pContext = m_pGraphicDev->GetContext();
	NULL_CHECK_RETURN(m_pMyGDI->pContext, E_FAIL);

	// .fx ���� �ε� �� effect �������̽� �ʱ�ȭ
	Engine::Get_Effects()->InitAll(m_pMyGDI, &m_pFX);
	m_pMyGDI->pEffect = Engine::Get_Effects()->GetBasicFX();
	m_pMyGDI->pTech = m_pFX->GetTechniqueByName("Light1Tex");
	NULL_CHECK_RETURN(m_pMyGDI->pTech, E_FAIL);


	Engine::Get_Management()->SetFX(m_pFX);

	m_pManagement = Engine::Get_Management();
	hr = m_pManagement->InitManagement(m_pMyGDI);
	FAILED_CHECK_RETURN_MSG(hr, E_FAIL, L"Management Init Failed");

	m_pResourceMgr = Engine::Get_ResourceMgr();
	hr = m_pResourceMgr->ReserveContainerSize(RESOURCE_END);
	FAILED_CHECK_RETURN_MSG(hr, E_FAIL, L"���ҽ� �����̳� �Ҵ� ����");

	hr = m_pResourceMgr->AddBuffer(m_pMyGDI, RESOURCE_STATIC
		, Engine::CVIBuffer::BUFFER_RCTEX, L"Buffer_RcTex");
	FAILED_CHECK_RETURN_MSG(hr, E_FAIL, L"Buffer_RcTex ���� ����");

	//Font �ε� ------------------------------------------------------

	AddFontResourceEx(L"../bin/Resources/Font/koverwatch.ttf", FR_PRIVATE, NULL);
	Engine::Get_FontMgr()->AddFont(m_pMyGDI,
		L"Font_koverwatch", L"../bin/Resources/Font/koverwatch.ttf");

	//Font �ε� �� ---------------------------------------------------
	hr = m_pManagement->SceneChange(CSceneSelector(CSceneSelector::SCENE_LOGIN));
	FAILED_CHECK_RETURN_MSG(hr, E_FAIL, L"�� ��ȯ ����");

	hr = Engine::Get_Input()->InitInputDevice(g_hInst, g_hWnd);
	FAILED_CHECK(hr);

	m_pTimeMgr = Engine::Get_TimeMgr();

	// .fx ���� �ε�	

	XMMATRIX P = XMMatrixIdentity();

	m_pMyGDI->pEffect->SetWorld(P);
	m_pMyGDI->pEffect->SetView(P);
	m_pMyGDI->pEffect->SetProj(P);

	return S_OK;
}

void CMainApp::Release(void)
{
	Engine::Safe_Delete(m_pMyGDI);
	
	Engine::Get_Management()->DestroyInstance();
	Engine::Get_ResourceMgr()->DestroyInstance();
	Engine::Get_Effects()->DestroyInstance();
	Engine::Get_TimeMgr()->DestroyInstance();
	Engine::Get_Input()->DestroyInstance();
	Engine::Get_GraphicDev()->DestroyInstance();
	Engine::Get_FontMgr()->DestroyInstance();

	CTextMgr::GetInstance()->DestroyInstance();
	CNetMgr::GetInstance()->DestroyInstance();
	CUserMgr::GetInstance()->DestroyInstance();
	CKeyMgr::GetInstance()->DestroyInstance();
	CMouseMgr::GetInstance()->DestroyInstance();
	CGameMgr::GetInstance()->DestroyInstance();
	CCameraMgr::GetInstance()->DestroyInstance();
	CBulletMgr::GetInstance()->DestroyInstance();
	CGeneralMgr::GetInstance()->DestroyInstance();
}

int CMainApp::Update(float& fTime)
{
	Engine::Get_Input()->SetInputState();
	//m_pTimeMgr->SetTime();
	CMouseMgr::GetInstance()->Update();
	m_pManagement->Update();

	return 0;
}

void CMainApp::Render(void)
{
	//m_pGraphicDev->RenderBegin();

	m_pManagement->Render(m_pTimeMgr->DeltaTime());

	//m_pGraphicDev->RenderEnd();
}

CMainApp * CMainApp::Create(void)
{
	CMainApp* pMainApp = new CMainApp;

	if (FAILED(pMainApp->InitApp()))
	{
		delete pMainApp;
		pMainApp = nullptr;
	}

	return pMainApp;
}
