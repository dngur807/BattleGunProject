#include "Default.h"
#include "Lobby.h"
#include "Constant.h"
#include "Protocol.h"
#include "Coder.h"
#include "IO.h"

int InitLobby()
{
	g_Server.pLobby = new CLooby;
	g_Server.pLobby->Initialize();


	g_OnTransFunc[REQUEST_LOBBYINFO].proc = OnRequestLobbyInfo;
	g_OnTransFunc[REQUEST_CHANGETEAM].proc = OnRequestChangeTeam;
	return 0;
}
void CLooby::Initialize()
{
	

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

	// ÆÀ ¹Ù²ãÁÖ°í ÆÀ Á¤º¸ ±³È¯
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
		coder.SetBuf(cPacket);
		coder.PutChar(tUserInfo.iIndex);
		coder.PutChar(lstrlen(tUserInfo.szID) + 1);
		coder.PutText(tUserInfo.szID, lstrlen(tUserInfo.szID) + 1);
		coder.PutChar(tUserInfo.eTeam);
		coder.PutChar(tUserInfo.CharType);
		iPacketSize = coder.SetHeader(NOTIFY_USERLIST);

		PostTcpSend(1, (int*)&lpSockContext, cPacket, iPacketSize);
	}
	return 0;
}
