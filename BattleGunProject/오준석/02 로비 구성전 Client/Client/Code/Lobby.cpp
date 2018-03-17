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

int g_iIndex = 0;

CLobby::CLobby(Engine::MYGDI* pMyGDI)
	: Engine::CScene(pMyGDI)
	, m_pResourceMgr(Engine::CResourceMgr::GetInstance())
	, m_pManagement(Engine::CManagement::GetInstance())
{
	m_bActivate = false;
	m_bRoomManager = false;
}

CLobby::~CLobby()
{

}

HRESULT CLobby::InitScene()
{
	//LodingTexture();
	return S_OK;
}

int CLobby::Update()
{
	POINT	ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	EnterCriticalSection(&CNetMgr::GetInstance()->CS);
	AnalysisInterMsg();
	LeaveCriticalSection(&CNetMgr::GetInstance()->CS);
	return 0;
}

void CLobby::Render()
{

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
		default:
			break;
		}
		qInterMsg->pop();
	}
}

void CLobby::LodingTexture()
{

}

