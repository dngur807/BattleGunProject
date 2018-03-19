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
};

int InitIngame();