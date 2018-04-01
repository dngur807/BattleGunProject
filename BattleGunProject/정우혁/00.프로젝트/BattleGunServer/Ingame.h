#pragma once
#include "Include.h"
class CIngame
{
public:
	void Initialize();
	void Update();
	void GameStart();
	void GameEnd();
public:
	friend int OnRequestPosDir(LPCLIENTCONTEXT lpSockContext, char *cpPacket);
	friend int OnRequestInput(LPCLIENTCONTEXT lpSockContext, char *cpPacket);

	friend int OnRequestGiveDamage(LPCLIENTCONTEXT lpSockContext, char *cpPacket);
	friend int OnRequestHpSync(LPCLIENTCONTEXT lpSockContext, char* cpPacket);
	friend int OnRequestDead(LPCLIENTCONTEXT lpSockContext, char* cpPacket);

	friend int OnRequestRevive(LPCLIENTCONTEXT lpSockContext, char* cpPacket);


};

int InitIngame();