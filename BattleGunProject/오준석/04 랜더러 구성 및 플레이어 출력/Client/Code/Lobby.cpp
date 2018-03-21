#include "stdafx.h"
#include "Lobby.h"
#include "Export_Function.h"
#include "Coder.h"
#include "NetStruct.h"
#include "NetMgr.h"
#include "Protocol.h"
#include "Include.h"
#include "UserMgr.h"
#include "Me.h"
#include "Engine_Include.h"
#include "ReactUI_2D.h"
#include "Layer.h"
#include "Text.h"
#include "User.h"
#include "MouseMgr.h"
#include "KeyMgr.h"
#include "TextMgr.h"
#include "GameMgr.h"
#include "SceneSelector.h"

int g_iIndex = 0;

CLobby::CLobby(Engine::MYGDI* pMyGDI)
	: Engine::CScene(pMyGDI)
	, m_pResourceMgr(Engine::CResourceMgr::GetInstance())
	, m_pManagement(Engine::CManagement::GetInstance())
{
	m_eMapType = MAP_CONVOY;

	m_bActivate = false;
	m_bRoomManager = false;
	m_bChatMode = false;
	m_pMapName = NULL;
	m_pChatUI = NULL;
	ZeroMemory(m_szText, sizeof(m_szText));

	m_pSmallMap1 = NULL;
	m_pSmallMap2 = NULL;



	for (int i = 0; i < 6; i++)
	{
		ZeroMemory(m_szChatList[i], MAX_STR);
		m_arrChatColor[i] = D3DCOLOR_ARGB(255 , 255, 255, 255);
	}

	//팀 이동 버튼 위치 결정
	SetRect(&m_rtATeamBtn, 820, 124, 846, 152);
	SetRect(&m_rtBTeamBtn, 1200, 124, 1226, 152);
	SetRect(&m_rtPrevMapBtn, 36, 480, 78, 516);
	SetRect(&m_rtNextMapBtn, 430, 480, 472, 516);
	SetRect(&m_rtStartBtn, 1000 + 12, 630, 1200 - 12, 680);
}

CLobby::~CLobby()
{
	Release();

}

HRESULT CLobby::InitScene()
{
	LodingTexture();

	FAILED_CHECK_RETURN_MSG(Add_UI_Layer(), E_FAIL, L"UI_Layer Add Fail");

	CMouseMgr::GetInstance()->ShowMouseCursor(true);
	ClipCursor(NULL);

	return S_OK;
}

int CLobby::Update()
{
	POINT ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	Engine::CScene::Update();
	if (m_pChatUI)
		m_pChatUI->Update();
	for (auto iter = m_vecTextChat.begin()
		; iter != m_vecTextChat.end(); ++iter)
	{
		(*iter)->Update();
	}
	if (m_pSmallMap1)
		m_pSmallMap1->Update();
	if (m_pSmallMap2)
		m_pSmallMap2->Update();

	if (CMouseMgr::GetInstance()->IsOnceLBtnDown())
	{
		if (PtInRect(&m_rtATeamBtn, ptMouse))
		{
			if (CUserMgr::GetInstance()->GetMe()->GetUserInfo().eTeam == TEAM_B)
			{
				CNetMgr::GetInstance()->RequestChangeTeam(g_iIndex, TEAM_A);
			}
		}
		else if (PtInRect(&m_rtBTeamBtn, ptMouse))
		{
			if (CUserMgr::GetInstance()->GetMe()->GetUserInfo().eTeam == TEAM_A)
			{
				CNetMgr::GetInstance()->RequestChangeTeam(g_iIndex, TEAM_B);
			}
		}
		else if (m_bRoomManager && PtInRect(&m_rtPrevMapBtn, ptMouse))
		{
			if (m_eMapType > MAP_GESTALT)
				CNetMgr::GetInstance()->RequestMapChange(m_eMapType - 1);
		}

		else if (m_bRoomManager && PtInRect(&m_rtNextMapBtn, ptMouse))
		{
			if (m_eMapType < MAP_CONVOY)
				CNetMgr::GetInstance()->RequestMapChange(m_eMapType + 1);
		}
		else if (m_bRoomManager && PtInRect(&m_rtStartBtn, ptMouse))
		{
			CNetMgr::GetInstance()->RequestGameStart();
		}
	}
	if (CKeyMgr::GetInstance()->IsOnceKeyDown(VK_RETURN))
	{
		if (m_bChatMode)
		{
			m_bChatMode = false;
			g_bInputMode = false;
			//채팅 전달
			if (lstrlen(m_pChatText->GetText()) > 0)
			{
				char cText[MAX_STR];
				char cTemp[MAX_STR];
				WideCharToMultiByte(CP_ACP, 0, m_pChatText->GetText(), -1, cTemp, MAX_STR, NULL, NULL);
				WideCharToMultiByte(CP_ACP, 0, CUserMgr::GetInstance()->GetMe()->GetUserInfo().szID, -1, cText, MAX_STR, NULL, NULL);
				strcat_s(cText, "  :  ");
				strcat_s(cText, cTemp);

				CNetMgr::GetInstance()->RequestAllChat((UCHAR)(strlen(cText) + 1), cText);

				m_pChatText->ClearText();

			}
			CTextMgr::GetInstance()->ReleaseText();
		}
		else
		{
			m_bChatMode = true;
			g_bInputMode = true;
			CTextMgr::GetInstance()->SetText(m_pChatText);
		}
	}

	EnterCriticalSection(&CNetMgr::GetInstance()->CS);
	AnalysisInterMsg();
	LeaveCriticalSection(&CNetMgr::GetInstance()->CS);
	return 0;
}

void CLobby::Render()
{
	switch (m_eMapType)
	{
	case MAP_GESTALT:
		m_pSmallMap1->Render();
		break;
	case MAP_CONVOY:
		m_pSmallMap2->Render();
		break;
	}
	// 맵 배경 출력
	Engine::CScene::Render();
	//HDC  hdc = GetDC(g_hWnd);
	//Rectangle(hdc, m_rtATeamBtn.left , m_rtATeamBtn.top , m_rtATeamBtn.right , m_rtATeamBtn.bottom);
	//ReleaseDC(g_hWnd, hdc);
	for (auto iter = m_mapUserText.begin(); iter != m_mapUserText.end(); ++iter)
	{
		iter->second->Render();
	}
	// 맵 이름 출력
	if (m_pMapName)
		m_pMapName->Render();

	// 팀리스트 출력 로비가 활성화 된 상태에서만 출력한다
	if (m_bActivate == true)
	{
		int iATeamIndex = 0;
		int iBTeamIndex = 0;

		map<int, CUser*>::iterator iter = CUserMgr::GetInstance()->GetUserList()->begin();
		map<int, CUser*>::iterator iter_end = CUserMgr::GetInstance()->GetUserList()->end();

		for (iter; iter != iter_end; ++iter)
		{
			USERINFO& tUserInfo = iter->second->GetUserInfo();

			lstrcpy(m_szText , tUserInfo.szID);

			if (m_iRoomManagerIndex == tUserInfo.iIndex)
			{
				lstrcat(m_szText, L"(방장)");
			}
			if (tUserInfo.eTeam == TEAM_A)
			{
				CText* pText = m_mapUserText[tUserInfo.iIndex];
				if (pText)
				{
					pText->SetPos(520, 190 + iATeamIndex * 70.0f);
					pText->SetSize(30);
					pText->SetColor(D3DCOLOR_ARGB(255, 255, 0, 255));
					pText->SetText(m_szText);
					++iATeamIndex;
				}
			}
			else if (tUserInfo.eTeam == TEAM_B)
			{
				CText* pText = m_mapUserText[tUserInfo.iIndex];
				if (pText)
				{
					pText->SetPos(900, 190 + iBTeamIndex * 70.0f);
					pText->SetSize(30);
					pText->SetColor(D3DCOLOR_ARGB(255, 255, 255, 0));
					pText->SetText(m_szText);
					++iBTeamIndex;
				}
			}
		}
	}
	switch (m_eMapType)
	{
	case MAP_GESTALT:
		if (m_pMapName)
		{
			lstrcpy(m_szText, L"GESTALT");
			m_pMapName->SetPos(240.0f - lstrlen(m_szText) / 2 * 12, 492.0f);
			m_pMapName->SetSize(30);
			m_pMapName->SetColor(D3DCOLOR_ARGB(255, 255, 255, 255));
			m_pMapName->SetText(m_szText);
			
			//m_pSmallMap1->Render();
		}
		break;
	case MAP_CONVOY:
	{
		lstrcpy(m_szText, L"CONVOY");
		m_pMapName->SetPos(240.0f - lstrlen(m_szText) / 2 * 12, 492.0f);
		m_pMapName->SetSize(30);
		m_pMapName->SetColor(D3DCOLOR_ARGB(255, 255, 255, 255));
		m_pMapName->SetText(m_szText);
		//m_pSmallMap2->Render();
	}
		break;
	case MAP_END:
		break;
	default:
		break;
	}

	// 채팅 모드 활성화시
	if (m_pChatUI && m_bChatMode)
	{
		//m_pChatUI->Render();
		m_pChatText->Render();
	}
	RenderChatList();
}

CLobby* CLobby::Create(Engine::MYGDI* pMyGDI)
{
	CLobby* pLobby = new CLobby(pMyGDI);
	if (FAILED(pLobby->InitScene()))
		Engine::Safe_Delete(pLobby);
	return pLobby;
}

void CLobby::Release()
{
	for (auto iter = m_mapUserText.begin()
		; iter != m_mapUserText.end(); ++iter)
	{
		delete iter->second;
	}
	m_mapUserText.clear();
	for (auto iter = m_vecTextChat.begin()
		; iter != m_vecTextChat.end(); ++iter)
		delete *iter;

	Engine::Safe_Delete(m_pSmallMap1);
	Engine::Safe_Delete(m_pSmallMap2);
	Engine::Safe_Delete(m_pChatText);
	Engine::Safe_Delete(m_pChatUI);
	Engine::Safe_Delete(m_pMapName);
}

void CLobby::AnalysisInterMsg()
{
	CCoder coder;
	queue<INTERMSGDATA>*		qInterMsg = CNetMgr::GetInstance()->GetInterMsgQueue();

	while (qInterMsg->empty() == false)
	{
		switch (qInterMsg->front().sHeaderType)
		{
		case NOTIFY_YOURINFO:
		{
			char cIndex;
			char cTeam;

			coder.SetBuf(qInterMsg->front().tcInterMsg);
			coder.GetChar(&cIndex);
			coder.GetChar(&cTeam);

			g_iIndex = cIndex;
			CUserMgr::GetInstance()->GetMe()->GetUserInfo().iIndex = cIndex;
			CUserMgr::GetInstance()->GetMe()->GetUserInfo().eTeam = (eTeamType)cTeam;

#ifdef _CLIENTDEBUGING_
			printf( "NOTIFY_YOURINFO ID : %ls MyIndex : %d  Team : %d \n" , 
				CUserMgr::GetInstance()->GetMe()->GetUserInfo().szID, int(cIndex) , int(cTeam));
#endif
			//폰트 추가
			m_mapUserText.insert(make_pair(g_iIndex, CText::Create(m_pMyGDI, L"Font_koverwatch", 15)));
			CUserMgr::GetInstance()->GetUserList()->insert(make_pair(g_iIndex, CUserMgr::GetInstance()->GetMe()));
			CNetMgr::GetInstance()->RequestLobbyInfo();
		}
			break;
		case NOTIFY_USERLIST:
		{
			char cIndex;
			char cIDLen;
			char cID[MAX_STR];
			char cTeam;
			char cCharType;

			coder.SetBuf(qInterMsg->front().tcInterMsg);
			coder.GetChar(&cIndex);
			coder.GetChar(&cIDLen);
			coder.GetText(cID, cIDLen);
			coder.GetChar(&cTeam);
			coder.GetChar(&cCharType);

			if (g_iIndex == cIndex)
				break;

			USERINFO tUserInfo;
			tUserInfo.iIndex = cIndex;
			tUserInfo.eTeam = eTeamType(cTeam);

			tUserInfo.CharType = eCharType(cCharType);
			MultiByteToWideChar(CP_ACP, 0, cID, MAX_STR, tUserInfo.szID, MAX_STR);
#ifdef _CLIENTDEBUGING_
			printf("NOTIFY_USERLIST Index : %d IDLen : %d  ID : %s  Team : %d CharType : %d\n"
				, cIndex, cIDLen, cID, cTeam, cCharType);
#endif
			CUser* pUser = new CUser;
			pUser->SetUserInfo(tUserInfo);

			//폰트 추가
			m_mapUserText.insert(make_pair(tUserInfo.iIndex, CText::Create(m_pMyGDI, L"Font_koverwatch", 15)));
			CUserMgr::GetInstance()->GetUserList()->insert(make_pair(tUserInfo.iIndex, pUser));


		}
			break;
		case NOTIFY_ROOMMANAGER:
		{
			// 룸매니저 정보가 왔다는 것은 로비에 대한 모든 정보를 보냈다는 뜻이므로 활성화 시킨다.
			m_bActivate = true;
			char cIndex;

			coder.SetBuf(qInterMsg->front().tcInterMsg);
			coder.GetChar(&cIndex);
			m_iRoomManagerIndex = cIndex;

#ifdef _CLIENTDEBUGING_
			printf("NOTIFY_ROOMMANAGER  방장 Index  %d  나의 인덱스 %d \n"
				, cIndex, g_iIndex);
#endif
			if (g_iIndex == cIndex)
			{
				m_bRoomManager = true;
			}
		}
			break;
		case NOTIFY_ALLCHAT:
		{
			char cMsgLen;
			char cMsg[MAX_STR];
			TCHAR tcMsg[MAX_STR];
			coder.SetBuf(qInterMsg->front().tcInterMsg);
			coder.GetChar(&cMsgLen);
			coder.GetText(cMsg, cMsgLen);
#ifdef _CLIENTDEBUGING_
			cout << "채팅이 왔다! : " << cMsg << endl;
#endif
			MultiByteToWideChar(CP_ACP, 0, cMsg, MAX_STR, tcMsg, MAX_STR);
			PushChatList(tcMsg, D3DCOLOR_ARGB(255 , 255, 255, 255));
		}
			break;
		case NOTIFY_MAPCHANGE:
		{
			char cMap;
			coder.SetBuf(qInterMsg->front().tcInterMsg);
			coder.GetChar(&cMap);
			switch ((eMapType)cMap)
			{
			case MAP_GESTALT:
				m_eMapType = MAP_GESTALT;
				m_wstrSelectMap = L"Gestal";
				break;
			case MAP_CONVOY:
				m_eMapType = MAP_CONVOY;
				m_wstrSelectMap = L"Convoy";
				break;
			default:
				break;
			}

#ifdef _CLIENTDEBUGING_
			string str = "";
			str.assign(m_wstrSelectMap.begin(), m_wstrSelectMap.end());
			cout << "NOTIFY_MAPCHANGE  맵이름 :" << str << endl;
#endif
		}
			break;
		case NOTIFY_CHANGETEAM:
		{
			char cIndex;
			char cTeam;

			coder.SetBuf(qInterMsg->front().tcInterMsg);
			coder.GetChar(&cIndex);
			coder.GetChar(&cTeam);

			(*CUserMgr::GetInstance()->GetUserList())[cIndex]->GetUserInfo().eTeam = (eTeamType)cTeam;
#ifdef _CLIENTDEBUGING_
			printf("NOTIFY_CHANGETEAM 인덱스 : %d 팀 : %d\n" , cIndex , cTeam);
#endif
		}
			break;
		case NOTIFY_GAMELOADING:
		{
			//로딩 전에 맵정보가 잘몬 되었으면 수정 후 로딩
			char cMap;
			coder.SetBuf(qInterMsg->front().tcInterMsg);
			coder.GetChar(&cMap);
			switch (eMapType(cMap))
			{
			case MAP_GESTALT:
				m_eMapType = MAP_GESTALT;
				m_wstrSelectMap = L"Gestal";
				break;
			case MAP_CONVOY:
				m_eMapType = MAP_CONVOY;
				m_wstrSelectMap = L"Convoy";
				break;
			case MAP_END:
				break;
			default:
				break;
			}

			//팀 분리
			CUserMgr::GetInstance()->DivideTeam();
			CGameMgr::GetInstance()->GetGameInfo().m_eMapType = m_eMapType;
			//로딩 씬으로 이동
			m_pManagement->SceneChange(CSceneSelector(CSceneSelector::SCENE_LOADING));

#ifdef _CLIENTDEBUGING_
			cout << "NOTIFY_GAMELOADING 맵 : " << m_wstrSelectMap.c_str() << endl;
#endif
			//return;
		}
			break;
		default:
			break;
		}
		qInterMsg->pop();
	}
}

HRESULT CLobby::LodingTexture()
{
	HRESULT hr = NULL;;
	hr = m_pResourceMgr->AddTexture(m_pMyGDI, RESOURCE_LOBBY, Engine::TEXTURE_NORMAL
		, L"Texture_lobbyui"
		, L"../bin/Resources/Texture/Lobby/lobbyui.png", 1);
	FAILED_CHECK_RETURN(hr, E_FAIL);

	hr = m_pResourceMgr->AddTexture(m_pMyGDI, RESOURCE_LOBBY, Engine::TEXTURE_NORMAL
		, L"Texture_chatui"
		, L"../bin/Resources/Texture/Lobby/chatui.png", 1);
	FAILED_CHECK_RETURN(hr, E_FAIL);

	hr = m_pResourceMgr->AddTexture(m_pMyGDI, RESOURCE_LOBBY, Engine::TEXTURE_NORMAL
		, L"Texture_kingsrowback"
		, L"../bin/Resources/Texture/Lobby/kingsrowback.png", 1);
	FAILED_CHECK_RETURN(hr, E_FAIL);


	hr = m_pResourceMgr->AddTexture(m_pMyGDI, RESOURCE_LOBBY, Engine::TEXTURE_NORMAL
		, L"Texture_kingsrowlobby"
		, L"../bin/Resources/Texture/Lobby/kingsrow_lobby.png", 1);
	FAILED_CHECK_RETURN(hr, E_FAIL);

	hr = m_pResourceMgr->AddTexture(m_pMyGDI, RESOURCE_LOBBY, Engine::TEXTURE_NORMAL
		, L"Texture_StartBtn"
		, L"../bin/Resources/Texture/Lobby/startbtn.png", 1);
	FAILED_CHECK_RETURN(hr, E_FAIL);


	hr = m_pResourceMgr->AddTexture(m_pMyGDI, RESOURCE_LOBBY, Engine::TEXTURE_NORMAL
		, L"Texture_LijiangTower_Back"
		, L"../bin/Resources/Texture/Lobby/lijiangback.jpg", 1);
	FAILED_CHECK_RETURN(hr, E_FAIL);


	hr = m_pResourceMgr->AddTexture(m_pMyGDI, RESOURCE_LOBBY, Engine::TEXTURE_NORMAL
		, L"Texture_LijiangTower_Lobby"
		, L"../bin/Resources/Texture/Lobby/lijiang_lobby.png", 1);
	FAILED_CHECK_RETURN(hr, E_FAIL);



	return S_OK;
}

HRESULT CLobby::Add_UI_Layer(void)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create(m_pMyGDI);
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CReactUI_2D*	pReactUI = NULL;

	pReactUI = CReactUI_2D::Create(m_pMyGDI, RESOURCE_LOBBY, L"Texture_lobbyui");
	pReactUI->SetPos(640.f, 360.f);
	pReactUI->SetSize(1280.f, 720.f);
	pLayer->AddObject(L"lobbyui", pReactUI);

	pReactUI = CReactUI_2D::Create(m_pMyGDI, RESOURCE_LOBBY, L"Texture_StartBtn");
	pReactUI->SetPos(1100, 650);
	pReactUI->SetSize(50 * 2, 200 - 100);
	pLayer->AddObject(L"StartBtn", pReactUI);

	pReactUI = CReactUI_2D::Create(m_pMyGDI, RESOURCE_LOBBY, L"Texture_kingsrowback");
	pReactUI->SetPos(640.f, 360.f);
	pReactUI->SetSize(1280.f, 720.f);
	pLayer->AddObject(L"kingsrowback", pReactUI);


	


	

	// 팀 이름 출력
	CText* pText  = NULL;
	pText = CText::Create(m_pMyGDI, L"Font_koverwatch", 15);
	pText->SetPos(520, 125);
	pText->SetSize(50);
	pText->SetColor(D3DCOLOR_ARGB(255, 0, 0, 255));
	pText->SetText(L"A Team");
	pLayer->AddObject(L"TEXT", pText);


	pText = CText::Create(m_pMyGDI, L"Font_koverwatch", 15);
	pText->SetPos(900, 125);
	pText->SetSize(50);
	pText->SetColor(D3DCOLOR_ARGB(255, 0, 255, 0));
	pText->SetText(L"B Team");
	pLayer->AddObject(L"TEXT", pText);



	m_mapLayer.insert(MAPLAYER::value_type(LAYER_UI, pLayer));


	m_pChatUI = CReactUI_2D::Create(m_pMyGDI, RESOURCE_LOBBY, L"Texture_chatui");
	m_pChatUI->SetPos(56, 540.f);
	m_pChatUI->SetSize(400, 160);

	m_pChatText = CText::Create(m_pMyGDI, L"Font_koverwatch", 15);
	m_pChatText->SetPos(66.0f, 678);
	m_pChatText->SetSize(20);
	m_pChatText->SetColor(D3DCOLOR_ARGB(255, 255, 255, 255));

	m_pMapName = CText::Create(m_pMyGDI, L"Font_koverwatch", 15);

	//채팅Text 추가해놓자
	for (int i = 0; i < MAX_CHAT_LINE; ++i)
	{
		pText = CText::Create(m_pMyGDI, L"Font_koverwatch", 15);
		pText->SetPos(66.0f, 678);
		pText->SetSize(20);
		pText->SetColor(D3DCOLOR_ARGB(255, 255, 255, 255));
		m_vecTextChat.push_back(pText);
	}
	m_pSmallMap1 = CReactUI_2D::Create(m_pMyGDI, RESOURCE_LOBBY, L"Texture_kingsrowlobby");
	m_pSmallMap1->SetPos(255, 314.0f);
	m_pSmallMap1->SetSize(508, 252);

	m_pSmallMap2 = CReactUI_2D::Create(m_pMyGDI, RESOURCE_LOBBY, L"Texture_LijiangTower_Lobby");
	m_pSmallMap2->SetPos(255, 314.0f);
	m_pSmallMap2->SetSize(508, 252);

	return S_OK;


}

void CLobby::RenderChatList(void)
{
	for (int i = 0; i < MAX_CHAT_LINE; ++i)
	{
		m_vecTextChat[i]->SetPos(66.0f, 550.0f + i * 20.0f);
		m_vecTextChat[i]->SetText(m_szChatList[i]);
		m_vecTextChat[i]->Render();
	}
}

void CLobby::PushChatList(TCHAR* pText, D3DCOLOR dColor)
{
	for (int i = 0; i < 5; i++)
	{
		lstrcpy(m_szChatList[i], m_szChatList[i + 1]);
		m_arrChatColor[i] = m_arrChatColor[i + 1];
	}
	lstrcpy(m_szChatList[5], pText);
	m_arrChatColor[5] = dColor;
}

