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
	static void OnPacket(char* pPacket);
private:
	void Release();
private:
	WSADATA			wsaData;
	SOCKET			hSock;
	SOCKADDR_IN		servAdr;

	static queue<INTERMSGDATA>*			m_qInterMsg;

public:
	HANDLE					hRecvThread;
	CRITICAL_SECTION		CS;
	bool					m_bNetOn;


	//통신 함수들의 모임
public:
	void RequestLogin(int iIDlen, char* szID);
	//통신 핸들러
	friend int OnAnswerLogin(char* pPacket);

	queue<INTERMSGDATA>* GetInterMsgQueue(void)
	{
		return m_qInterMsg;
	}

};

