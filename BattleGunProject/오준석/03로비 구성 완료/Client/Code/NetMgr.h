#pragma once

#include "Engine_Include.h"
#include "Include.h"
#include "NetStruct.h"

typedef struct 
{
	int(*proc)(char* cpPacket);
}ONTRANSFUNC;

extern ONTRANSFUNC OnTransFunc[256];

class CNetMgr
{
public:
	DECLARE_SINGLETON(CNetMgr)
private:
	CNetMgr();
	~CNetMgr();

public:
	HRESULT			Initialize();
	HRESULT			Login(char* szIP ,short portnum);

	static UINT WINAPI RecvMsg(void* arg);
	static void OnPacket(char* _cpPacket);
private:
	void Release();
private:
	WSADATA			wsaData;
	SOCKET			hSock;
	SOCKADDR_IN		servAdr;
public:
	HANDLE					hRecvThread;
	CRITICAL_SECTION		CS;
	bool					m_bNetOn;

	//통신 함수들의 모임
public:
	void RequestLogin(int iIDlen, char* szID);
	void RequestLobbyInfo();
	void RequestAllChat(unsigned char cMsgLen, char *cpMsg);
	void RequestChangeTeam(int iIndex, eTeamType eTeam);
	void RequestMapChange(int mapType);
	void RequestGameStart();
	//통신 핸들러들의 모임

	friend int OnAnswerLogin(char* cpPacket);
	friend int OnNotifyYourInfo(char* cpPacket);
	friend int OnNotifyUserList(char* cpPacket);
	friend int OnNotifyRoomManager(char* cpPacket);
	friend int OnNotifyMapChange(char* cpPacket);
	friend int OnNotifyChangeTeam(char* cpPacket);
	friend int OnNotifyAllChat(char* cpPacket);
	friend int OnNotifyGameLoading(char* cpPacket);
	queue<INTERMSGDATA>* GetInterMsgQueue(void)
	{
		return m_qInterMsg;
	}
public:
	static queue<INTERMSGDATA>* m_qInterMsg;

};

