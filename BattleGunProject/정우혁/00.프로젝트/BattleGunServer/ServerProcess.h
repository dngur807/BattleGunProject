#pragma once
#include "Include.h"
#define GAMERINGBUFSIZE		8192

class CServerProcess
{
public:
	CServerProcess();
	~CServerProcess();
public:
	int InitProcess();
	void GameBufEnqueue(LPCLIENTCONTEXT lpSockContext);
	void GameBufDequeue(LPCLIENTCONTEXT* ppSockContext);

	friend UINT WINAPI GameProc(void* pParam);

	// ---- Handler ----
	friend int OnRequestLogin(LPCLIENTCONTEXT lpSockContext, char *cpPacket);

private:
	void Initialize(int idx);
private:
	int			iIndex; // 프로세스 객체 인덱스
	// n 번 인덱스의 iBaseChannelIndex값은 n * MAXProess가 되는 것이다.

	CRITICAL_SECTION		cs;// 동기화 용도 링 버퍼에 데이터를 넣거나 꺼낼 떄 사용합니다.
	LPCLIENTCONTEXT			pGameRingBuf[GAMERINGBUFSIZE];

	int						iBegin; // 원형 큐 시작 인덱스
	int						iEnd;//원형 큐 끝 인덱스
	int						iRestCount;// 원형 큐에 존재하는 데이터 수

	HANDLE					hGameEvent;
};

