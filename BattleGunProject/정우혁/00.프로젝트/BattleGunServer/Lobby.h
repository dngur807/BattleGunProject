#pragma once
#include "Enum.h"
#include "Include.h"

class CLooby
{
public:
	void Initialize();

	friend int OnRequestLobbyInfo(LPCLIENTCONTEXT lpSockContext, char *cpPacket);
	friend int OnRequestChangeTeam(LPCLIENTCONTEXT lpSockContext, char *cpPacket);
	friend int OnRequestMapChange(LPCLIENTCONTEXT lpSockContext, char *cpPacket);
	friend int OnRequestGameStart(LPCLIENTCONTEXT lpSockContext, char* cpPacket);
	friend int OnNotifyUserList(LPCLIENTCONTEXT	lpSockContext, char *cpPacket);
	
};

int InitLobby();