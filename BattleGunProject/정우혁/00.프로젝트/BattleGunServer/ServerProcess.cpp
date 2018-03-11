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

// ���μ��� ��ü�� �����鿡�Լ� ���۹��� ��Ŷ�� ó���ϴµ� �־ �ٽ��� �Ǵ� �κ�
// �̰��� ��û�� ���� ť�� �װ��� ó���� ���� ������� �����Ǿ� �ִ� ���·�  �װ͵� �� ���� ������ ������ Ư�� ����(�κ� , ä��) ��  ��ġ�� ��������
// ��û�� ó���ϴ� ���̴�. �׷��Ƿ� �ϳ��� ���� ���μ��� �������� ó���� ���� �κп� ���μ��� ��ü ����ŭ�� �����尡 ��ȴ�.

int InitProcess()
{
	// ������ ����ŭ ���μ��� ��ü �޸� �Ҵ�
	g_Server.ps = new CServerProcess[g_Server.iMaxProcess];
	if (g_Server.ps == NULL)
		return -1;

	for (int i = 0; i < g_Server.iMaxProcess; ++i)
	{
		// ������ ���μ��� ��ü�� �ʱ�ȭ
		g_Server.ps[i].Initialize(i);
	}
	// �ڵ鷯 �Լ� ����

	g_OnTransFunc[REQUEST_LOGIN].proc = OnRequestLogin;
	return 0;
}

void CServerProcess::GameBufEnqueue(LPCLIENTCONTEXT lpSockContext)
{
	// ���� ť�� �����͸� ���� ���� ��ȣ
	EnterCriticalSection(&cs);
	// �׻� ���� ť�� �� �ε����� �߰�
	pGameRingBuf[iEnd++] = lpSockContext;

	// ������ ���� �����ϸ� �ٽ� ������ ����
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
	// ť�� �����ϴ� �����Ͱ� ����
	if (iRestCount <= 0)
	{
		*ppSockContext = NULL;
		return;
	}
	// �׻� ���� ť�� ���� �ε������� ����
	*ppSockContext = pGameRingBuf[iBegin++];

	// ������ ���� �����ϸ� �ٽ� ������ ����
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
// ���μ��� ��ü���� �����Ǵ� ������� �������� ��Ŷ�� ó���� ���Ͽ� �۵��մϴ�.
// �� ť�� �ϳ��� ���̴� �����͸� �����ͼ� �ؼ� , ó���ϴ� ���Դϴ�.
// (��Ȯ�ϰԴ� ť�� ���̴� ���� ����ü�� �����͸� �̿��Ͽ� �� �����Ϳ� �ش��ϴ� ������ RingBuf�� ���� ������ ���̴�.
// ���μ��� ��ü ������ ������� GameProc�̶�� �̸����� ����˴ϴ�.
// �̰��� ���μ��� ��ü�� ��� �Լ� InitProcess���� �����Ǵ� ���Դϴ�.
// ť�� ���� �����͸� �������µ��� �� ���� ����� ���� ���ִ�.
// ���⼭�� �����ð� �������� ť�� �˻��Ͽ� �����Ͱ� ������ �������� �׷��� ������ ��ȿ��Ű�� ����� ����Ͽ���.
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
		//Ÿ�� �ƿ��� �������� ���
		// ť�� �����Ͱ� �ֱ� �����ϸ� �װ��� �� ������ ������ �����ͼ� ó���Ѵ�.
		// 100ms��� �ð��� ������ ���¿� �°� ������ �ʿ䰡 �ִ� ���̴�.
		//�� ����� ���� �����찡 ��� Ÿ�̸Ӹ� ����� �� ���� ��, ������� Ȱ�� ������ ���� ����̴�.
		dwResult = WaitForSingleObject(pProcess->hGameEvent, 10);
		if (dwResult == WAIT_OBJECT_0) 
			break;

		// �� ������ ť�� �ִ� ��� ������ ������ ���� ����
		while (1)
		{
			// ť���� ������ �����⸦ �õ�
			pProcess->GameBufDequeue(&lpSockContext);

			// ����̶�� ���� Ż��
			if (lpSockContext == NULL)
				break;

			// �ش��ϴ� ������ ���� ���ؽ�Ʈ���� �ϳ��� ��Ŷ�� ����
			RecvTcpBufDequeue(lpSockContext, &cpPacket, &iPacketSize);

			// ��Ŷ���� ��Ŷ Ÿ���� ����
			// ��Ŷ�� Ÿ�Ժ� �ؼ��� �մϴ�.
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
	 coder.GetChar(&IDlen);//���̵� ����
	 coder.GetText(szID, IDlen);//���̵�

	 wchar_t wcID[32];
	 //char to 
	 CharToWChar(lpSockContext->szID, wcID);

	 //���ؽ�Ʈ�� ���̵� ����
	 CopyMemory(lpSockContext->szID, szID, IDlen);
	 lstrcpy(lpSockContext->tUserInfo.szID, wcID);
	 lpSockContext->idLen = IDlen;

	 lpSockContext->tUserInfo.iIndex = lpSockContext->iKey;
	 cIndex = lpSockContext->iKey;
	 g_Server.m_mapAllUserList.insert(make_pair(lpSockContext->iKey, lpSockContext));


	 // ������ ���� ������ �ƹ��� ���ٸ�
	 if (g_Server.iAllUserNum == 0)
	 {
		 // ���� ���� ������ �� ���� ��ũ�� �Ǿ� ���� �ʴ�.
		 // ������ ���� ������ ���� ���� ������ �Ѵ�.
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


	 // �α��� ��û�� ���� ����
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
