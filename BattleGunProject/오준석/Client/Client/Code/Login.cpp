#include "stdafx.h"
#include "Login.h"
#include "ResourceMgr.h"
#include "Management.h"
#include "Enum.h"
#include "Engine_Macro.h"
#include "Layer.h"
#include "ReactUI_2D.h"
#include "LogoBack.h"
#include "FontMgr.h"
#include "Export_Function.h"
#include "Text.h"
#include "TextMgr.h"
#include "NetMgr.h"
#include "SceneSelector.h"
#include "Protocol.h"
#include "UserMgr.h"
#include "Me.h"
#include "MouseMgr.h"

CLogin::CLogin(Engine::MYGDI* pMyGDI)
	: Engine::CScene(pMyGDI)
	, m_pResourceMgr(Engine::CResourceMgr::GetInstance())
	, m_pManagement(Engine::CManagement::GetInstance())
	, m_pFX(NULL)
{
	m_bLoading = false;
	m_eMenuType = MENU_NONE;
	m_pMainBack = NULL;
	m_pMainTitle = NULL;
	m_pPlayerNameText = NULL;
	m_pIPNameText = NULL;
	m_pIPInput = NULL;
	m_pIDText = NULL;
	m_pIPText = NULL;

	SetRect(&m_rtIDInput, WINCX / 2 - 100, 530 - 5, WINCX / 2 + 100, 560 - 5);
	SetRect(&m_rtIPInput, WINCX / 2 - 100, 530 - 5 + 60, WINCX / 2 + 100, 560 - 5 + 60);

}



CLogin::~CLogin()
{
	Release();
}

HRESULT CLogin::InitScene()
{
	HRESULT hr = NULL;



	hr = m_pResourceMgr->AddTexture(m_pMyGDI, RESOURCE_LOGIN, Engine::TEXTURE_NORMAL
		, L"Texture_Title"
		, L"../bin/Resources/Texture/Login/Title.png", 1);
	FAILED_CHECK_RETURN(hr, E_FAIL);


	hr = m_pResourceMgr->AddTexture(m_pMyGDI, RESOURCE_LOGIN, Engine::TEXTURE_NORMAL
		, L"Texture_MainBack"
		, L"../bin/Resources/Texture/Login/MainBack.png", 1);
	FAILED_CHECK_RETURN(hr, E_FAIL);


	hr = m_pResourceMgr->AddTexture(m_pMyGDI, RESOURCE_LOGIN, Engine::TEXTURE_NORMAL
		, L"Texture_JoinBtn"
		, L"../bin/Resources/Texture/Login/JoinBtn.png", 1);
	FAILED_CHECK_RETURN(hr, E_FAIL);

	hr = m_pResourceMgr->AddTexture(m_pMyGDI, RESOURCE_LOGIN, Engine::TEXTURE_NORMAL
		, L"Texture_HostBtn"
		, L"../bin/Resources/Texture/Login/HostBtn.png", 1);
	FAILED_CHECK_RETURN(hr, E_FAIL);


	hr = m_pResourceMgr->AddTexture(m_pMyGDI, RESOURCE_LOGIN, Engine::TEXTURE_NORMAL
		, L"Texture_SmallBtn"
		, L"../bin/Resources/Texture/Login/SmallBtn.png", 1);
	FAILED_CHECK_RETURN(hr, E_FAIL);

	hr = m_pResourceMgr->AddTexture(m_pMyGDI, RESOURCE_LOGIN, Engine::TEXTURE_NORMAL
		, L"Texture_BigBtn"
		, L"../bin/Resources/Texture/Login/BigBtn.png", 1);
	FAILED_CHECK_RETURN(hr, E_FAIL);

	hr = m_pResourceMgr->AddTexture(m_pMyGDI, RESOURCE_LOGIN, Engine::TEXTURE_NORMAL
		, L"Texture_SmallInput"
		, L"../bin/Resources/Texture/Login/SmallInput.png", 1);
	FAILED_CHECK_RETURN(hr, E_FAIL);

	hr = m_pResourceMgr->AddTexture(m_pMyGDI, RESOURCE_LOGIN, Engine::TEXTURE_NORMAL
		, L"Texture_LargeInput"
		, L"../bin/Resources/Texture/Login/LargeInput.png", 1);
	FAILED_CHECK_RETURN(hr, E_FAIL);
	FAILED_CHECK_RETURN_MSG(Add_UI_Layer(), E_FAIL, L"UI_Layer Add Fail");

	return S_OK;
}

int CLogin::Update()
{
	Engine::CScene::Update();
	if (CMouseMgr::GetInstance()->IsOnceLBtnDown())
	{
		POINT ptMouse = CMouseMgr::GetInstance()->GetCurPos();
		switch (m_eMenuType)
		{
		case CLogin::MENU_NONE:
		{
	
			if (PtInRect(&m_rtIDInput, ptMouse))
			{
				g_bInputMode = true;
				CTextMgr::GetInstance()->SetText(m_pIDText);
			}
			else if (PtInRect(&m_rtIPInput, ptMouse))
			{
				g_bInputMode = true;
				CTextMgr::GetInstance()->SetText(m_pIPText);
			}
			else
			{
				CTextMgr::GetInstance()->ReleaseText();
				g_bInputMode = false;
			}
		}
			break;
		case CLogin::MENU_LOGIN:
			break;
		case CLogin::MENU_QUIT:
			break;
		default:
			break;
		}
	}

	// 로그 인 체크
	if ((GetAsyncKeyState(VK_RETURN) & 0x8001) == 0x8001)
	{
		char ID[MIN_STR];
		char IP[MIN_STR];
		WideCharToMultiByte(CP_ACP, 0, m_pIDText->GetText(), -1, ID, MIN_STR, NULL, NULL);
		WideCharToMultiByte(CP_ACP, 0, m_pIPText->GetText(), -1, IP, MIN_STR, NULL, NULL);

		//기본 서버 연결
		if (CNetMgr::GetInstance()->Login(IP, PORT_NUM) == S_OK)
		{
			//아이디로 접속 요청
			CNetMgr::GetInstance()->RequestLogin(strlen(ID) + 1, ID);
		}
	}

	// 서버로 부터 온 메시지가 있으면 해석하자
	EnterCriticalSection(&CNetMgr::GetInstance()->CS);
	AnalysisInterMsg();
	LeaveCriticalSection(&CNetMgr::GetInstance()->CS);

	return 0;
}

void CLogin::Render()
{
	Engine::CScene::Render();
	/*if (m_pPlayerNameText)
		m_pPlayerNameText->Render();
	if (m_pIPNameText)
		m_pIPNameText->Render();

	if (m_pIDText)
		m_pIDText->Render();
	if (m_pIPText)
		m_pIPText->Render();*/
}

CLogin* CLogin::Create(Engine::MYGDI* pMyGDI)
{
	CLogin*	pLogin = new CLogin(pMyGDI);
	if (FAILED(pLogin->InitScene()))
		Engine::Safe_Delete(pLogin);
	return pLogin;
}

void CLogin::Release()
{
	/*Engine::Safe_Delete(m_pIDText);
	Engine::Safe_Delete(m_pIPText);*/
}

HRESULT CLogin::Add_UI_Layer(void)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create(m_pMyGDI);
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CReactUI_2D*	pReactUI = NULL;

	m_pMainTitle = CReactUI_2D::Create(m_pMyGDI, RESOURCE_LOGIN, L"Texture_Title");
	m_pMainTitle->SetPos(660, 250.0f);
	m_pMainTitle->SetSize(750.f, 550);
	pLayer->AddObject(L"Title", m_pMainTitle);

	m_pIDInput = CReactUI_2D::Create(m_pMyGDI, RESOURCE_LOGIN, L"Texture_LargeInput");
	m_pIDInput->SetPos(WINCX / 2, 540);
	m_pIDInput->SetSize(63, 115);
	pLayer->AddObject(L"IDInput", m_pIDInput);
	//

	m_pIPInput = CReactUI_2D::Create(m_pMyGDI, RESOURCE_LOGIN, L"Texture_LargeInput");
	m_pIPInput->SetPos(WINCX / 2, 600);
	m_pIPInput->SetSize(63, 115);
	pLayer->AddObject(L"IPInput", m_pIPInput);

	m_pMainBack = CReactUI_2D::Create(m_pMyGDI, RESOURCE_LOGIN, L"Texture_MainBack");
	m_pMainBack->SetPos(640.f, 360.f);
	m_pMainBack->SetSize(1280.f, 720.f);
	pLayer->AddObject(L"MainBack", m_pMainBack);

	

	
	
//
//
//////
	m_pPlayerNameText = CText::Create(m_pMyGDI, L"Font_koverwatch", 15);
	m_pPlayerNameText->SetPos(550, 505);
	m_pPlayerNameText->SetSize(20);
	m_pPlayerNameText->SetColor(D3DCOLOR_ARGB(255, 255, 255, 255));
	m_pPlayerNameText->SetText(L"Player_Name");
	pLayer->AddObject(L"Player_Name", m_pPlayerNameText);
//
//
	m_pIPNameText = CText::Create(m_pMyGDI, L"Font_koverwatch", 15);
	m_pIPNameText->SetPos(550, 570);
	m_pIPNameText->SetSize(20);
	m_pIPNameText->SetColor(D3DCOLOR_ARGB(255, 255, 255, 255));
	m_pIPNameText->SetText(L"IP_Address");

	pLayer->AddObject(L"IP_Address", m_pIPNameText);
//
//
	// Text
	m_pIDText = CText::Create(m_pMyGDI, L"Font_koverwatch", 15);
	m_pIDText->SetPos(560, 532 + 5);
	m_pIDText->SetSize(15);
	m_pIDText->SetColor(D3DCOLOR_ARGB(255, 255, 255, 255));
	pLayer->AddObject(L"IDText", m_pIDText);
	//pLayer->AddObject(L"IDText", m_pIDText);

	m_pIPText = CText::Create(m_pMyGDI, L"Font_koverwatch", 15);
	m_pIPText->SetPos(560, 592 + 5);
	m_pIPText->SetSize(15);
	m_pIPText->SetColor(D3DCOLOR_ARGB(255, 255, 255, 255));
	pLayer->AddObject(L"IPText", m_pIPText);
//

	m_mapLayer.insert(MAPLAYER::value_type(LAYER_UI, pLayer));

	return S_OK;
}

void CLogin::AnalysisInterMsg()
{
	queue<INTERMSGDATA>* qInterMsg = CNetMgr::GetInstance()->GetInterMsgQueue();

	while (qInterMsg->empty() == false)
	{
		switch (qInterMsg->front().sHeaderType)
		{
		case ANSWER_LOGIN:
			//씬 이동
#ifdef _CLIENTDEBUGING_
			cout << "씬 이동" << endl;
#endif
			lstrcpy(CUserMgr::GetInstance()->GetMe()->GetUserInfo().szID, m_pIDText->GetText());
			Engine::Get_Management()->SceneChange(CSceneSelector(CSceneSelector::SCENE_LOBBY));
			return;
			break;
		}
		qInterMsg->pop();
	}
}
