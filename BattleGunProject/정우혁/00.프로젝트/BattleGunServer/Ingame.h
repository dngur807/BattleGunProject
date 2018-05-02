#pragma once
#include "Include.h"
class CIngame
{
public:
	void Initialize();
	int Update();
	void GameStart();
	void GameEnd();
	void InLobby();

public:
	friend int OnRequestPosDir(LPCLIENTCONTEXT lpSockContext, char *cpPacket);
	friend int OnRequestInput(LPCLIENTCONTEXT lpSockContext, char *cpPacket);

	friend int OnRequestGiveDamage(LPCLIENTCONTEXT lpSockContext, char *cpPacket);
	friend int OnRequestHpSync(LPCLIENTCONTEXT lpSockContext, char* cpPacket);
	friend int OnRequestDead(LPCLIENTCONTEXT lpSockContext, char* cpPacket);

	friend int OnRequestRevive(LPCLIENTCONTEXT lpSockContext, char* cpPacket);
	friend int OnRequestChangeWeapon(LPCLIENTCONTEXT lpSockContext, char* cpPacket);
	friend int OnRequestHoldState(LPCLIENTCONTEXT lpSocketContext, char* cpPacket);
	friend int OnRequestFireWeapon(LPCLIENTCONTEXT lpSocketContext, char* cpPacket);
	friend int OnRequestNextNavi(LPCLIENTCONTEXT lpSocketContext, char* cpPacket);

	void NotifyGameResult();
	void NotifyGameTimer(int iTimer);
	void NotifyGameStart();
	void NotifyGoLobby(void);
private:
	float			m_fGameTime;
	DWORD			m_dwTime;
	eServState   m_eGameServ;
	int			m_iLoadingEnd;
	bool		m_IsGameStart;
};

int InitIngame();