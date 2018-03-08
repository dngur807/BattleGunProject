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
	int			iIndex; // ���μ��� ��ü �ε���
	// n �� �ε����� iBaseChannelIndex���� n * MAXProess�� �Ǵ� ���̴�.

	CRITICAL_SECTION		cs;// ����ȭ �뵵 �� ���ۿ� �����͸� �ְų� ���� �� ����մϴ�.
	LPCLIENTCONTEXT			pGameRingBuf[GAMERINGBUFSIZE];

	int						iBegin; // ���� ť ���� �ε���
	int						iEnd;//���� ť �� �ε���
	int						iRestCount;// ���� ť�� �����ϴ� ������ ��

	HANDLE					hGameEvent;
};

