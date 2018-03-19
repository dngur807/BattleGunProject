#include "stdafx.h"
#include "NetMgr.h"
#include "Coder.h"
#include "Protocol.h"

IMPLEMENT_SINGLETON(CNetMgr);

ONTRANSFUNC					OnTransFunc[256];
queue<INTERMSGDATA>*		CNetMgr::m_qInterMsg = NULL;


CNetMgr::CNetMgr()
{
	Initialize();
}


CNetMgr::~CNetMgr()
{
	Release();
}

HRESULT CNetMgr::Initialize()
{
	m_qInterMsg = new queue<INTERMSGDATA>;
	hSock = NULL;
	//핸들러 매핑
	OnTransFunc[ANSWER_LOGIN].proc = OnAnswerLogin;
	InitializeCriticalSection(&CS);
	return S_OK;
}

HRESULT CNetMgr::Login(char* szIP , short sPortnum)
{
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		MSG_BOX(L"WSAStartup() error!");
		return E_FAIL;
	}
	hSock = socket(AF_INET, SOCK_STREAM, 0);


	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.s_addr = inet_addr(szIP);
	servAdr.sin_port = htons(sPortnum);

	if (connect(hSock, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR)
	{
		MSG_BOX(L"Connect() error");
		return E_FAIL;
	}

	m_bNetOn = true;
	g_bNetwork = true;

	hRecvThread = (HANDLE)_beginthreadex(NULL, 0, RecvMsg, (void*)this, 0, NULL);
	return S_OK;
}

UINT WINAPI CNetMgr::RecvMsg(void* arg)
{
	CNetMgr*  cls = (CNetMgr*)arg;

	// 패킷을 받는 버퍼
	char cRecvBuf[32768];
	int iEnd = 0;
	int iRest = sizeof(cRecvBuf);

	int iRecv, iUsed;

	CCoder coder;
	short sBodySize, sType;
	char* pPacket;

	// 넷 매니저에 있는 소켓
	int hSock = cls->hSock;

	while (1)
	{
		if (cls->m_bNetOn == false)
			return;

		iRecv = recv(hSock, cRecvBuf, iRest, 0);
		iEnd += iRecv;
		iRest -= iRecv;

		if (iRecv == -1)
			return;

		while (1)
		{
			coder.SetBuf(cRecvBuf);
			coder.GetHeader(&sBodySize, &sType);
			iUsed = sBodySize + HEADERSIZE;

			if (iEnd < iUsed)
				break;

			CopyMemory(pPacket, cRecvBuf, iEnd - iUsed);
			OnPacket(pPacket);
			CopyMemory(cRecvBuf, cRecvBuf + iUsed, iEnd - iUsed);
			iEnd -= iUsed;
			iRest += iUsed;
			if (iEnd <= 0)
				break;
		}

	}
	return 0;
}

void CNetMgr::OnPacket(char* pPacket)
{
	CCoder coder;
	short sBodySize;
	short sType;

	coder.SetBuf(pPacket);
	coder.GetHeader(&sBodySize, &sType);
	try
	{
		//패킷에 타당한 핸들러 실행
		OntransFunc[sType].proc(pPacket);
	}
	catch (...)
	{
#ifdef _CLIENTDEBUGING_
		MSG_BOX(L"Incorrect Packet!");
#endif	
	}
}

void CNetMgr::Release()
{
	shutdown(hSock, SD_SEND);
	shutdown(hSock, SD_RECEIVE);

	if (hSock != NULL)
		closesocket(hSock);

	WSACleanup();
	m_bNetOn = false;

	while (m_qInterMsg->empty() == false)
	{
		m_qInterMsg->pop();
	}
	Engine::Safe_Delete(m_qInterMsg);
}

// 로그인 요청을 보내는 함수
void CNetMgr::RequestLogin(int iIDlen, char* szID)
{
	CCoder		coder;
	char	cPacket[MAX_STR];
	//int	
}

int OnAnswerLogin(char* pPacket)
{
#ifdef _CLIENTDEBUGING_
	cout << "From Server : ANSWER_LOGIN" << endl;
#endif

	INTERMSGDATA InterMsg;
	InterMsg.sHeaderType = ANSWER_LOGIN;

	EnterCriticalSection(&CNetMgr::GetInstance()->CS);
	CNetMgr::m_qInterMsg->push(InterMsg);
	LeaveCriticalSection(&CNetMgr::GetInstance()->CS);
	return 0;
}
