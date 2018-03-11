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

int OnNotifyUserList(LPCLIENTCONTEXT lpSockContext, char *cpPacket)
{
	CCoder coder;
	int iPacketSize;
	char cPacket[MAX_STR];

#ifdef _LOG_LOBBY_
	printf("OnNotifyUserList(%d)\n", lpSockContext->index);
#endif

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