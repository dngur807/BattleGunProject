#include "Default.h"
#include "Lobby.h"
#include "Constant.h"
#include "Protocol.h"
#include "Coder.h"
#include "IO.h"
#include "Ingame.h"

int InitLobby()
{
	
	g_Server.pLobby = new CLooby;
	g_Server.pLobby->Initialize();
	g_OnTransFunc[REQUEST_LOBBYINFO].proc = OnRequestLobbyInfo;
	g_OnTransFunc[REQUEST_CHANGETEAM].proc = OnRequestChangeTeam;
	g_OnTransFunc[REQUEST_MAPCHANGE].proc = OnRequestMapChange;
	g_OnTransFunc[REQUEST_GAMESTART].proc = OnRequestGameStart;
	g_OnTransFunc[REQUEST_LOADINGEND].proc = OnRequestLoadingEnd;
	g_OnTransFunc[REQUEST_NAVIMESH].proc = OnRequestNaviMesh;
	return 0;
}
void CLooby::Initialize()
{
	m_iLoadingCnt = 0;
	m_IsReadNaviMesh = false;

}
int OnRequestNaviMesh(LPCLIENTCONTEXT lpSockContext, char *cpPacket)
{
	if (g_Server.pLobby->m_IsReadNaviMesh)
	{
#ifdef _FROM_CLIENT_
		printf("이미 네비메쉬 로딩 완료됨\n");
#endif
		return 0;
	}

	g_Server.pLobby->m_IsReadNaviMesh = true;
	DWORD dwByte = 0;
	//여기서 읽고 데이터 전달하자
	HANDLE hFile;
	int isize = 0;
//	if (g_Server.m_eMapType == MAP_GESTALT)
	hFile = CreateFile(L"./Data/Gestalt_Navi.dat", GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	ReadFile(hFile, &isize, sizeof(int), &dwByte, NULL);
	CCoder coder;
	char cPacket[MAX_STR];
	int iPacketSize;
	coder.SetBuf(cPacket);
	coder.PutInt(isize);
	iPacketSize = coder.SetHeader(NOTIFY_NAVIMESH_SIZE);
	PostTcpSend(g_Server.iUserBegin, cPacket, iPacketSize);

#ifdef _FROM_CLIENT_
	printf("FROM CLIENT OnRequestNaviMesh Size : %d \n " , isize);
#endif

	int i = 0;
	while (true)
	{
		NAVMESH navi;
		ReadFile(hFile, &navi, sizeof(NAVMESH), &dwByte, NULL);
		coder.SetBuf(cPacket);
		coder.PutNaviMesh(navi);
		iPacketSize = coder.SetHeader(NOTIFY_NAVIMESH);
		cout << ++i << "네비 정보 전달" << endl;// 이거 빼면 뻑나네 ... 뭐지


//#ifdef _FROM_CLIENT_
//		cout << ++i << "네비 정보 전달" << endl;
//#endif
		PostTcpSend(g_Server.iUserBegin, cPacket, iPacketSize);
		if (dwByte == 0)
		{
			break;
		}
	}
	CloseHandle(hFile);
	return 0;
}
 int OnRequestGameStart(LPCLIENTCONTEXT lpSockContext, char* cpPacket)
{
	 CCoder coder;
	 char cPacket[MIN_STR];

	 int iPacketSize;

	 coder.SetBuf(cPacket);
	 coder.PutChar(g_Server.m_eMapType);

	 iPacketSize = coder.SetHeader(NOTIFY_GAMELOADING);
	 PostTcpSend(g_Server.iUserBegin, cPacket, iPacketSize);
	 return 0;
}

int OnRequestLobbyInfo(LPCLIENTCONTEXT lpSockContext, char *cpPacket)
{
	CCoder	coder;
	int iPacketSize;
	char cPacket[MAX_STR];

#ifdef _FROM_CLIENT_
	printf("FROM CLIENT OnRequestLobbyInfo \n");
#endif
	OnNotifyUserList(lpSockContext, NULL);

	coder.SetBuf(cPacket);
	coder.PutChar(g_Server.iUserBegin);

	iPacketSize = coder.SetHeader(NOTIFY_ROOMMANAGER);
	PostTcpSend(1, (int *)&lpSockContext, cPacket, iPacketSize);

	coder.SetBuf(cPacket);
	coder.PutChar(g_Server.m_eMapType);

	iPacketSize = coder.SetHeader(NOTIFY_MAPCHANGE);
	PostTcpSend(1, (int *)&lpSockContext, cPacket, iPacketSize);

	return 0;
}
int OnRequestChangeTeam(LPCLIENTCONTEXT lpSockContext, char *cpPacket)
{
#ifdef _FROM_CLIENT_
	printf("FROM CLIENT OnRequestChangeTeam \n");
#endif

	CCoder coder;
	char cIndex;
	char cTeam;
	bool bFind = false;
	char cPacket[MIN_STR];
	int iPacketSize;

	coder.SetBuf(cpPacket);
	coder.GetChar(&cIndex);
	coder.GetChar(&cTeam);

	// 팀 바꿔주고 팀 정보 교환
	if ((eMapType)cTeam == TEAM_A)
	{
		auto iter = g_Server.m_mapBTeam.find(cIndex);

		if (iter != g_Server.m_mapBTeam.end())
		{
			lpSockContext->tUserInfo.eTeam = TEAM_A;
			g_Server.m_mapATeam.insert(make_pair(iter->first, iter->second));
			g_Server.m_mapBTeam.erase(iter);
			bFind = true;
		}
	}
	else
	{
		
		auto iter = g_Server.m_mapATeam.find(cIndex);

		if (iter != g_Server.m_mapATeam.end())
		{
			lpSockContext->tUserInfo.eTeam = TEAM_B;
			g_Server.m_mapBTeam.insert(make_pair(iter->first, iter->second));
			g_Server.m_mapATeam.erase(iter);
			bFind = true;
		}
	}

	if (bFind == true)
	{
		coder.SetBuf(cPacket);
		coder.PutChar(cIndex);
		coder.PutChar(cTeam);

		iPacketSize = coder.SetHeader(NOTIFY_CHANGETEAM);
		PostTcpSend(g_Server.iUserBegin, cPacket, iPacketSize);
	}
	return 0;
}
int OnRequestMapChange(LPCLIENTCONTEXT lpSockContext, char *cpPacket)
{
#ifdef _FROM_CLIENT_
	printf("FROM CLIENT OnRequestMapChange \n");
#endif
	CCoder coder;
	char cPacket[MIN_STR];
	char cMap;
	int iPacketSize;
	coder.SetBuf(cpPacket);
	coder.GetChar(&cMap);

	g_Server.m_eMapType = eMapType(cMap);

	coder.SetBuf(cPacket);
	coder.PutChar(cMap);

	iPacketSize = coder.SetHeader(NOTIFY_MAPCHANGE);

	PostTcpSend(g_Server.iUserBegin, cPacket, iPacketSize);
	return 0;
}

int OnNotifyUserList(LPCLIENTCONTEXT lpSockContext, char *cpPacket)
{
#ifdef _TO_CLIENT_
	printf("TO CLIENT OnNotifyUserList \n");
#endif

	CCoder coder;
	int iPacketSize;
	char cPacket[MAX_STR];

	map<int, LPCLIENTCONTEXT>::iterator iter = g_Server.m_mapAllUserList.begin();
	map<int, LPCLIENTCONTEXT>::iterator iter_end = g_Server.m_mapAllUserList.end();

	USERINFO tUserInfo;

	for (iter; iter != iter_end; ++iter)
	{
		tUserInfo = iter->second->tUserInfo;
		char szName[MIN_STR];
		WideCharToMultiByte(CP_ACP, 0, tUserInfo.szID, -1, szName, sizeof(tUserInfo.szID), NULL , NULL);

		coder.SetBuf(cPacket);
		coder.PutChar(tUserInfo.iIndex);
		coder.PutChar(lstrlen(tUserInfo.szID) + 1);
		coder.PutText(szName, lstrlen(tUserInfo.szID) + 1);
		coder.PutChar(tUserInfo.eTeam);
		coder.PutChar(tUserInfo.CharType);
		iPacketSize = coder.SetHeader(NOTIFY_USERLIST);

		PostTcpSend(1, (int*)&lpSockContext, cPacket, iPacketSize);
	}
	return 0;
}
int OnRequestLoadingEnd(LPCLIENTCONTEXT	lpSockContext, char *cpPacket)
{
#ifdef _FROM_CLIENT_
	printf("FROM CLIENT OnRequestLoadingEnd \n");
#endif

	CCoder coder;
	char cPacket[MIN_STR];
	int iPacketSize;

	EnterCriticalSection(&g_Server.CS);
	g_Server.pLobby->m_iLoadingCnt++;
	LeaveCriticalSection(&g_Server.CS);

	if (g_Server.iAllUserNum <= g_Server.pLobby->m_iLoadingCnt)
	{
		coder.SetBuf(cPacket);
		iPacketSize = coder.SetHeader(NOTIFY_LOADINGEND);

		PostTcpSend(g_Server.iUserBegin, cPacket, iPacketSize);

		EnterCriticalSection(&g_Server.CS);
		g_Server.pLobby->m_iLoadingCnt = 0;
		LeaveCriticalSection(&g_Server.CS);
		g_Server.pIngame->GameStart();
	}
	return 0;
}

