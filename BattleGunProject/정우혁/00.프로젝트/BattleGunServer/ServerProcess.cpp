#include "ServerProcess.h"
#include "Default.h"
#include "IO.h"
#include "Protocol.h"
#include "Constant.h"
#include "Util.h"
#include "Coder.h"


CServerProcess::CServerProcess()
{
}


CServerProcess::~CServerProcess()
{
}

// 프로세스 객체는 유저들에게서 전송받은 패킷을 처리하는데 있어서 핵심이 되는 부분
// 이것은 요청에 대한 큐와 그것의 처리를 위한 쓰레드로 구성되어 있는 형태로  그것들 한 쌍이 일정한 개수의 특정 지역(로비 , 채널) 에  위치한 유저들의
// 요청을 처리하는 것이다. 그러므로 하나의 서버 프로세스 내에서는 처리를 위한 부분에 프로세스 객체 수만큼의 쓰레드가 운영된다.

int InitProcess()
{
	// 설정된 수만큼 프로세스 객체 메모리 할당
	g_Server.ps = new CServerProcess[g_Server.iMaxProcess];
	if (g_Server.ps == NULL)
		return -1;

	for (int i = 0; i < g_Server.iMaxProcess; ++i)
	{
		// 생성된 프로세스 객체를 초기화
		g_Server.ps[i].Initialize(i);
	}
	// 핸들러 함수 맵핑

	g_OnTransFunc[REQUEST_LOGIN].proc = OnRequestLogin;
	return 0;
}

void CServerProcess::GameBufEnqueue(LPCLIENTCONTEXT lpSockContext)
{
	// 원형 큐에 데이터를 넣을 때의 보호
	EnterCriticalSection(&cs);
	// 항상 원형 큐의 끝 인덱스에 추가
	pGameRingBuf[iEnd++] = lpSockContext;

	// 버퍼의 끝에 도달하면 다시 앞으로 돌림
	if (iEnd >= GAMERINGBUFSIZE)
		iEnd = 0;
	iRestCount++;

#ifdef _LOGLEVEL6_
	if (iRestCount >= GAMERINGBUFSIZE)
		printf("---- GameRingBuf Overflow ----\n");
#endif

	LeaveCriticalSection(&cs);
}

void CServerProcess::GameBufDequeue(LPCLIENTCONTEXT* ppSockContext)
{
	// 큐에 존재하는 데이터가 없음
	if (iRestCount <= 0)
	{
		*ppSockContext = NULL;
		return;
	}
	// 항상 원형 큐의 시작 인덱스에서 꺼냄
	*ppSockContext = pGameRingBuf[iBegin++];

	// 버퍼의 끝에 도달하면 다시 앞으로 돌림
	if (iBegin >= GAMERINGBUFSIZE)
		iBegin = 0;
	iRestCount--;
}

void CServerProcess::Initialize(int idx)
{
	int				itv;
	HANDLE			hGame;

	iIndex = idx;
	iBegin = 0;
	iEnd = 0;
	iRestCount = 0;

	InitializeCriticalSection(&cs);
	hGameEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	hGame = (HANDLE)_beginthreadex(NULL, 0, GameProc, this, 0, (UINT *)&itv);
	CloseHandle(hGame);
}
// 프로세스 객체마다 생성되는 쓰레드는 실제적인 패킷의 처리를 위하여 작동합니다.
// 즉 큐에 하나씩 쌓이는 데이터를 꺼내와서 해석 , 처리하는 것입니다.
// (정확하게는 큐에 쌓이는 유저 구조체의 포인터를 이용하여 그 포인터에 해당하는 유저의 RingBuf의 값을 얻어오는 것이다.
// 프로세스 객체 내에서 쓰레드는 GameProc이라는 이름으로 실행됩니다.
// 이것은 프로세스 객체의 멤버 함수 InitProcess에서 생성되는 것입니다.
// 큐에 쌓인 데이터를 가져오는데는 몇 가지 방법이 있을 수있다.
// 여기서는 일정시간 간격으로 큐를 검사하여 데이터가 있으면 가져오고 그렇지 않으면 무효시키는 방법을 사용하였다.
UINT WINAPI GameProc(void* pParam)
{
	CServerProcess*			pProcess = (CServerProcess*)pParam;
	DWORD					dwResult;
	LPCLIENTCONTEXT			lpSockContext;
	char*					cpPacket;
	int						iPacketSize;
	short					sType;

	while (1)
	{
		//타임 아웃이 나오도록 대기
		// 큐에 데이터가 있기 시작하면 그것이 빌 때까지 모조리 가져와서 처리한다.
		// 100ms라는 시간은 서버의 상태에 맞게 조정할 필요가 있는 값이다.
		//이 방법은 보통 윈도우가 없어서 타이머를 사용할 수 없을 때, 대용으로 활용 가능한 좋은 방법이다.
		dwResult = WaitForSingleObject(pProcess->hGameEvent, 10);
		if (dwResult == WAIT_OBJECT_0) 
			break;

		// 그 순간에 큐에 있는 모든 내용을 꺼내기 위한 루프
		while (1)
		{
			// 큐에서 데이터 꺼내기를 시도
			pProcess->GameBufDequeue(&lpSockContext);

			// 빈것이라면 루프 탈출
			if (lpSockContext == NULL)
				break;

			// 해당하는 유저의 소켓 컨텍스트에서 하나의 패킷을 얻음
			RecvTcpBufDequeue(lpSockContext, &cpPacket, &iPacketSize);

			// 패킷에서 패킷 타입을 얻음
			// 패킷의 타입별 해석을 합니다.
			CopyMemory(&sType, cpPacket + sizeof(short), sizeof(short));

			g_OnTransFunc[sType].proc(lpSockContext, cpPacket);
		}

	}
	return 0;

}

 int OnRequestLogin(LPCLIENTCONTEXT lpSockContext, char *cpPacket)
{
	 CCoder			coder;
	 char			IDlen;
	 char			szID[MAX_STR];
	 char			szPacket[MAX_STR];

	 char			cIndex;
	 char			cTeam;
	 int			iSize;
	 

	 coder.SetBuf(cpPacket);
	 coder.GetChar(&IDlen);//아이디 길이
	 coder.GetText(szID, IDlen);//아이디

	 wchar_t wcID[32];
	 //char to 
	 CharToWChar(lpSockContext->szID, wcID);

	 //컨텍스트에 아이디 저장
	 CopyMemory(lpSockContext->szID, szID, IDlen);
	 lstrcpy(lpSockContext->tUserInfo.szID, wcID);
	 lpSockContext->idLen = IDlen;

	 lpSockContext->tUserInfo.iIndex = lpSockContext->iKey;
	 cIndex = lpSockContext->iKey;
	 g_Server.m_mapAllUserList.insert(make_pair(lpSockContext->iKey, lpSockContext));


	 // 서버에 현재 유저가 아무도 없다면
	 if (g_Server.iAllUserNum == 0)
	 {
		 // 새로 들어온 유저의 전 노드는 링크가 되어 있지 않다.
		 // 서버의 시작 유저를 지금 들어온 유저로 한다.
		 g_Server.pn[lpSockContext->iKey].prev = NOTLINKED;
		 g_Server.iUserBegin = g_Server.iUserEnd = lpSockContext->iKey;
	 }
	 else
	 {
		 g_Server.pn[g_Server.iUserEnd].next = lpSockContext->iKey;
		 g_Server.pn[lpSockContext->iKey].prev = g_Server.iUserEnd;
	 }
	 g_Server.pn[lpSockContext->iKey].next = NOTLINKED;
	 g_Server.iAllUserNum++;


	 if (g_Server.iATeamNum < 4)
	 {
		 g_Server.iATeamNum++;
		 g_Server.m_mapATeam.insert(make_pair(lpSockContext->iKey, lpSockContext));
		 lpSockContext->tUserInfo.eTeam = TEAM_A;
		 cTeam = TEAM_A;
	 }
	 else
	 {
		 g_Server.iBTeamNum++;
		 g_Server.m_mapBTeam.insert(make_pair(lpSockContext->iKey, lpSockContext));
		 lpSockContext->tUserInfo.eTeam = TEAM_B;
		 cTeam = TEAM_B;
	 }

#ifdef _LOGLEVEL6_
	 printf("OnRequestLogin(%d) : %s\n", lpSockContext->index, cID);
	 printf("ID : %s\n", lpSockContext->m_tUserInfo.m_szID);
#endif


	 // 로그인 요청에 대한 응답
	 coder.SetBuf(szPacket);
	 coder.PutChar(SUCCESS_LOGIN);
	 iSize = coder.SetHeader(ANSWER_LOGIN);//
	 PostTcpSend(1, (int*)&lpSockContext, szPacket, iSize);

	 coder.SetBuf(szPacket);
	 coder.PutChar(cIndex);
	 coder.PutChar(cTeam);

	 iSize = coder.SetHeader(NOTIFY_YOURINFO);
	 PostTcpSend(1, (int *)&lpSockContext, szPacket, iSize);



	  coder.SetBuf(szPacket);
	  coder.PutChar(cIndex);
	  coder.PutChar(IDlen);
	  coder.PutText(szID, IDlen);
	  coder.PutChar(cTeam);
	  coder.PutChar(lpSockContext->tUserInfo.CharType);

	  iSize = coder.SetHeader(NOTIFY_USERLIST);
	  PostTcpSend(g_Server.iUserBegin, szPacket, iSize);

	 return 0;
}
