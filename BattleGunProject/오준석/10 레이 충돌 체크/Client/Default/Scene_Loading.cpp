#include "stdafx.h"
#include "Scene_Loading.h"
#include "GameMgr.h"
#include "Layer.h"
#include "ReactUI_2D.h"
#include "Export_Function.h"
#include "Include.h"
#include "MathHelper.h"
#include "Text.h"
#include "Loading.h"
#include "Ingame.h"
#include "Protocol.h"
#include "NetMgr.h"


CScene_Loading::CScene_Loading(Engine::MYGDI* pMyGDI)
	: Engine::CScene(pMyGDI)
	, m_pResourceMgr(Engine::CResourceMgr::GetInstance())
	, m_pManagement(Engine::CManagement::GetInstance())
	, m_pMyGDI(pMyGDI)

{
	m_fLogoAngle = 0.0f;
	m_bLoadingEnd = false;
	m_pLoadingUI = NULL;
	m_pLoading = NULL;
	m_pIngame = NULL;
}

CScene_Loading::~CScene_Loading()
{
	Release();
}

HRESULT CScene_Loading::InitScene()
{
#ifdef _CLIENTDEBUGING_
	cout << "Loading 진입" << endl;
#endif

	LodingTexture();
	FAILED_CHECK_RETURN_MSG(Add_UI_Layer(), E_FAIL, L"UI_Layer Add Fail");

	m_eMapType = CGameMgr::GetInstance()->GetGameInfo().m_eMapType;
	m_pLoading = CLoading::Create(m_pMyGDI, CLoading::LOADING_INGAME);
	NULL_CHECK_RETURN(m_pLoading, E_FAIL);

	return S_OK;
}

int CScene_Loading::Update()
{
	
	CScene::Update();

	if (m_pLoadingUI)
	{
		m_fLogoAngle += 200 * Engine::Get_TimeMgr()->DeltaTime();
		/*if (m_fLogoAngle >= 3.141592f * 2)
			m_fLogoAngle = 0.f;*/
		m_pLoadingUI->UIRoationZ(m_fLogoAngle);
		m_pLoadingUI->Update();
	}

	if (m_pLoading&& m_pLoading->GetComplete())
	{
		//로딩 완료
		CreateIngame();//인게임 생성
	}
	EnterCriticalSection(&CNetMgr::GetInstance()->CS);
	//메시지 해석
	AnalysisInterMsg();
	LeaveCriticalSection(&CNetMgr::GetInstance()->CS);
	return 0;
}

void CScene_Loading::Render()
{
	//if (m_bLoadingEnd == true)
	//{
	//	return;
	//}
	if (m_pLoadingUI)
		m_pLoadingUI->Render();

	CScene::Render();
	
}

void CScene_Loading::Release()
{
	Engine::Safe_Delete(m_pLoadingUI);
	Engine::Safe_Delete(m_pLoading);
}
CScene_Loading* CScene_Loading::Create(Engine::MYGDI* pMyGDI)
{
	CScene_Loading* pLoading = new CScene_Loading(pMyGDI);
	if (FAILED(pLoading->InitScene()))
		Engine::Safe_Delete(pLoading);
	return pLoading;
}

HRESULT CScene_Loading::Add_UI_Layer(void)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create(m_pMyGDI);
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CReactUI_2D*	pReactUI = NULL;

	pReactUI = CReactUI_2D::Create(m_pMyGDI, RESOURCE_LOBBY, L"Texture_kingsrowback");
	pReactUI->SetPos(640.f, 360.f);
	pReactUI->SetSize(1280.f, 720.f);
	pLayer->AddObject(L"kingsrowback", pReactUI);

	m_pLoadingUI = CReactUI_2D::Create(m_pMyGDI, RESOURCE_LOBBY, L"Texture_loadinglogo");
	m_pLoadingUI->SetPos(120.0f, 600.0f);
	m_pLoadingUI->SetSize(100, 100);

	CText* pText = NULL;
	pText = CText::Create(m_pMyGDI, L"Font_koverwatch", 15);
	pText->SetPos(760, 600);
	pText->SetSize(50);
	pText->SetColor(D3DCOLOR_ARGB(255, 0, 200, 255));
	pText->SetText(L"불러오는 중...");
	pLayer->AddObject(L"TEXT", pText);


	m_mapLayer.insert(MAPLAYER::value_type(LAYER_UI, pLayer));
	return S_OK;
}

HRESULT CScene_Loading::LodingTexture()
{
	HRESULT hr = NULL;;
	hr = m_pResourceMgr->AddTexture(m_pMyGDI, RESOURCE_LOBBY, Engine::TEXTURE_NORMAL
		, L"Texture_loadinglogo"
		, L"../bin/Resources/Texture/Lobby/loadinglogo.png", 1);
	FAILED_CHECK_RETURN(hr, E_FAIL);

	if (g_bNetwork == false)
	{
		//네트워크 꺼있으면 텍스처 추가 백그라운드
		hr = m_pResourceMgr->AddTexture(m_pMyGDI, RESOURCE_LOBBY, Engine::TEXTURE_NORMAL
			, L"Texture_kingsrowback"
			, L"../bin/Resources/Texture/Lobby/kingsrowback.png", 1);
		FAILED_CHECK_RETURN(hr, E_FAIL);
	}
	return S_OK;
}

void CScene_Loading::CreateIngame()
{
	if (m_pIngame == NULL)
	{
		m_pIngame = CIngame::Create(m_pMyGDI);
		// 로딩 끝
		CNetMgr::GetInstance()->RequestLoadingEnd();
	}
}

void CScene_Loading::AnalysisInterMsg()
{
	queue<INTERMSGDATA>* qInterMsg = CNetMgr::GetInstance()->GetInterMsgQueue();

	while (qInterMsg->empty() == false)
	{
		switch (qInterMsg->front().sHeaderType)
		{
		case NOTIFY_LOADINGEND:
		{
#ifdef _CLIENTDEBUGING_
			cout << "로딩이 끝났으니 넘어가시오" << endl;
#endif
			Engine::Get_Management()->SceneSwap(this, m_pIngame);
			return;
			break;
		}
		}
		qInterMsg->pop();
	}
}
