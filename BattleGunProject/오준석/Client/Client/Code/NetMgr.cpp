#include "stdafx.h"
#include "NetMgr.h"
#include "Protocol.h"
#include "Coder.h"

IMPLEMENT_SINGLETON(CNetMgr);

ONTRANSFUNC		OnTransFunc[256];












queue<INTERMSGDATA>* CNetMgr::m_qInterMsg = NULL;

CNetMgr::CNetMgr()
{
	Initialize();
}


CNetMgr::~CNetMgr()
{

	if (m_qInterMsg)
		delete m_qInterMsg;
	Release();
}

HRESULT CNetMgr::Initialize()
{
	m_qInterMsg = new queue<INTERMSGDATA>;


	hSock = NULL;
	OnTransFunc[ANSWER_LOGIN].proc = OnAnswerLogin;
	OnTransFunc[NOTIFY_YOURINFO].proc = OnNotifyYourInfo;
	OnTransFunc[NOTIFY_USERLIST].proc = OnNotifyUserList;
	OnTransFunc[NOTIFY_ROOMMANAGER].proc = OnNotifyRoomManager;
	OnTransFunc[NOTIFY_MAPCHANGE].proc = OnNotifyMapChange;
	OnTransFunc[NOTIFY_CHANGETEAM].proc = OnNotifyChangeTeam;
	OnTransFunc[NOTIFY_ALLCHAT].proc = OnNotifyAllChat;
	OnTransFunc[NOTIFY_GAMELOADING].proc = OnNotifyGameLoading;

	OnTransFunc[NOTIFY_LOADINGEND].proc = OnNotifyLoadingEnd;
	OnTransFunc[NOTIFY_INPUT].proc = OnNotifyInput;
	OnTransFunc[NOTIFY_POSDIR].proc = OnNotifyPosDir;

	OnTransFunc[NOTIFY_GIVEDAMAGE].proc = OnNotifyGiveDamage;
	OnTransFunc[NOTIFY_HPSYNC].proc = OnNotifyHpSync;

	OnTransFunc[NOTIFY_DEAD].proc = OnNotifyDead;
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
	char RecvBuf[32768];
	int iEnd = 0;
	int iRest = sizeof(RecvBuf);

	int iRecv, iUsed ;

	CCoder coder;
	short sBodysize, sType;
	char* pPacket;

	int hSock = cls->hSock;
	while (1)
	{
		if (cls->m_bNetOn == false)
			break;

		iRecv = recv(hSock, RecvBuf, iRest, 0);
		iEnd += iRecv;
		iRest -= iRecv;

		if (iRecv == -1)
			break;

		while (1)
		{
			coder.SetBuf(RecvBuf);
			coder.GetHeader(&sBodysize, &sType);
			iUsed = sBodysize + HEADERSIZE;

			if (iEnd < iUsed)
				break;

			pPacket = new char[iUsed];
			CopyMemory(pPacket, RecvBuf, iUsed);
			OnPacket(pPacket);
			CopyMemory(RecvBuf, RecvBuf + iUsed, iEnd - iUsed);

			delete[] pPacket;
			iEnd -= iUsed;
			iRest += iUsed;
			if (iEnd <= 0) 
				break;
		}
	}
	return 0;
}

void CNetMgr::OnPacket(char* _cpPacket)
{
	CCoder	coder;
	short	sBodySize;
	short	sType;
	coder.SetBuf(_cpPacket);
	coder.GetHeader(&sBodySize, &sType);

	try
	{
		//패킷에 타당한 핸들러 실행
		OnTransFunc[sType].proc(_cpPacket);
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
	closesocket(hSock);
	DeleteCriticalSection(&CS);
	WSACleanup();
}

// 로그인 요청을 보내는 함수
void CNetMgr::RequestLogin(int iIDlen, char* szID)
{
	CCoder		coder;
	char		szPacket[MAX_STR];
	int			iPacketSize;

	coder.SetBuf(szPacket);
	coder.PutChar(iIDlen);
	coder.PutText(szID, iIDlen);

	iPacketSize = coder.SetHeader(REQUEST_LOGIN);

#ifdef _TO_SEVER_
	printf("RequestLogin ID : %s , IDLen : %d\n", szID, iIDlen);
#endif
	send(hSock, szPacket, iPacketSize, 0);
}

void CNetMgr::RequestLobbyInfo()
{
	CCoder coder;
	char cPacket[MIN_STR];
	int iPacketSize;

	coder.SetBuf(cPacket);

	iPacketSize = coder.SetHeader(REQUEST_LOBBYINFO);

#ifdef _TO_SEVER_
	printf("To Server RequestLobbyInfo \n");
#endif

	send(hSock, cPacket, iPacketSize, 0);
}

void CNetMgr::RequestAllChat(unsigned char cMsgLen, char *cpMsg)
{
#ifdef _TO_SEVER_
	printf("To Server RequestAllChat \n");
#endif
	CCoder coder;
	char cPacket[MAX_STR];
	int iPacketSize;

	coder.SetBuf(cPacket);
	coder.PutChar(cMsgLen);
	coder.PutText(cpMsg, cMsgLen);

	iPacketSize = coder.SetHeader(REQUEST_ALLCHAT);

	send(hSock, cPacket, iPacketSize, 0);

}

void CNetMgr::RequestChangeTeam(int iIndex, eTeamType eTeam)
{
	CCoder coder;
	char cPacket[MIN_STR];
	int iPacketSize;

	coder.SetBuf(cPacket);
	coder.PutChar(iIndex);
	coder.PutChar(eTeam);

	iPacketSize = coder.SetHeader(REQUEST_CHANGETEAM);
	send(hSock, cPacket, iPacketSize, 0);
}

void CNetMgr::RequestMapChange(int mapType)
{
	CCoder coder;
	char cPacket[MIN_STR];
	int iPacketSize;

	coder.SetBuf(cPacket);
	coder.PutChar(mapType);

	iPacketSize = coder.SetHeader(REQUEST_MAPCHANGE);
	send(hSock, cPacket, iPacketSize, 0);
}

void CNetMgr::RequestGameStart()
{
	CCoder coder;
	char cPacket[MIN_STR];
	int iPacketSize;

	coder.SetBuf(cPacket);
	iPacketSize = coder.SetHeader(REQUEST_GAMESTART);
	send(hSock, cPacket, iPacketSize, 0);
}

void CNetMgr::RequestLoadingEnd()
{
	CCoder coder;
	char cPacket[MIN_STR];
	int iPacketSize;

	coder.SetBuf(cPacket);
	iPacketSize = coder.SetHeader(REQUEST_LOADINGEND);
	send(hSock, cPacket, iPacketSize, 0);
}



void CNetMgr::RequestPosDir(UINT iKey , XMFLOAT3 vPos, float fAngleX, float fAngleY)
{
	CCoder coder;
	char cPacket[MIN_STR];
	int	 iPacketSize;

	coder.SetBuf(cPacket);
	coder.PutChar(g_iIndex);
	coder.PutUint(iKey);
	coder.PutXMFLOAT3(&vPos);
	coder.PutFloat(fAngleX);
	coder.PutFloat(fAngleY);

	iPacketSize = coder.SetHeader(REQUEST_POSDIR);
	send(hSock, cPacket, iPacketSize, 0);
}

void CNetMgr::RequestGiveDamage(int iTargetIndex, int iDamage)
{
	CCoder coder;
	char szPacket[MIN_STR];
	int	iPacketSize;

	coder.SetBuf(szPacket);
	coder.PutChar(g_iIndex);
	coder.PutChar(iTargetIndex);
	coder.PutInt(iDamage);

	iPacketSize = coder.SetHeader(REQUEST_GIVEDAMAGE);

	send(hSock, szPacket, iPacketSize, 0);
}

void CNetMgr::RequestDead(int iVictim, int iKiller)
{
	CCoder coder;
	char cPacket[MIN_STR];

	int iPacketSize;

	coder.SetBuf(cPacket);
	coder.PutChar(iVictim);
	coder.PutChar(iKiller);
	iPacketSize = coder.SetHeader(REQUEST_DEAD);
	send(hSock, cPacket, iPacketSize, 0);
}

void CNetMgr::RequestHpSync(int index, int iHp)
{
	CCoder coder;
	char szPacket[MIN_STR];

	int iPacketsize;

	coder.SetBuf(szPacket);
	coder.PutChar(index);
	coder.PutInt(iHp);

	iPacketsize = coder.SetHeader(REQUEST_HPSYNC);
	send(hSock, szPacket, iPacketsize, 0);
}

int OnNotifyGameLoading(char* cpPacket)
{
#ifdef _FROM_SERVER_
	cout << "From Server : OnNotifyGameLoading" << endl;
#endif

	INTERMSGDATA InterMsg;
	InterMsg.sHeaderType = NOTIFY_GAMELOADING;
	CopyMemory(InterMsg.tcInterMsg + HEADERSIZE, cpPacket + HEADERSIZE, 1);
	EnterCriticalSection(&CNetMgr::GetInstance()->CS);
	CNetMgr::m_qInterMsg->push(InterMsg);
	LeaveCriticalSection(&CNetMgr::GetInstance()->CS);
	return 0;
}

int OnNotifyYourInfo(char* cpPacket)
{
#ifdef _FROM_SERVER_
	cout << "From Server : OnNotifyYourInfo" << endl;
#endif

	//coder.SetBuf(cpPacket);
	//coder.GetChar(&cIndex);
	//coder.GetChar(&cTeam);


	INTERMSGDATA InterMsg;
	InterMsg.sHeaderType = NOTIFY_YOURINFO;
	CopyMemory(InterMsg.tcInterMsg + HEADERSIZE, cpPacket + HEADERSIZE, 2);

	EnterCriticalSection(&CNetMgr::GetInstance()->CS);
	CNetMgr::m_qInterMsg->push(InterMsg);
	LeaveCriticalSection(&CNetMgr::GetInstance()->CS);
	return 0;
}

int OnAnswerLogin(char* cpPacket)
{
#ifdef _FROM_SERVER_
	cout << "From Server : OnAnswerLogin" << endl;
#endif
	INTERMSGDATA InterMsg;
	InterMsg.sHeaderType = ANSWER_LOGIN;

	EnterCriticalSection(&CNetMgr::GetInstance()->CS);
	CNetMgr::m_qInterMsg->push(InterMsg);
	LeaveCriticalSection(&CNetMgr::GetInstance()->CS);

	return 0;
}
int OnNotifyUserList(char* cpPacket)
{

	CCoder coder;
	char cIndex;
	char cIdLen;
	char szID[MAX_STR];
	char cTeam;
	char cCharType;

	coder.SetBuf(cpPacket);
	coder.GetChar(&cIndex);
	coder.GetChar(&cIdLen);
	coder.GetText(szID, cIdLen);
	coder.GetChar(&cTeam);
	coder.GetChar(&cCharType);

#ifdef _FROM_SERVER_	
	printf( "From Server : OnNotifyUserList Index : %d ID %s  IDLen %d\n" , (int)cIndex , szID , (int)cIdLen) ;
#endif


	INTERMSGDATA InterMsg;
	InterMsg.sHeaderType = NOTIFY_USERLIST;
	coder.SetBuf(InterMsg.tcInterMsg);
	coder.PutChar(cIndex);
	coder.PutChar(cIdLen);
	coder.PutText(szID, cIdLen);
	coder.PutChar(cTeam);
	coder.PutChar(cCharType);

	EnterCriticalSection(&CNetMgr::GetInstance()->CS);
	CNetMgr::m_qInterMsg->push(InterMsg);
	LeaveCriticalSection(&CNetMgr::GetInstance()->CS);

	return 0;
}

int OnNotifyRoomManager(char* cpPacket)
{
#ifdef _FROM_SERVER_
	cout << "From Server : OnNotifyRoomManager" << endl;
#endif
	INTERMSGDATA InterMsg;
	InterMsg.sHeaderType = NOTIFY_ROOMMANAGER;
	CopyMemory(InterMsg.tcInterMsg + HEADERSIZE, cpPacket + HEADERSIZE, 1);

	EnterCriticalSection(&CNetMgr::GetInstance()->CS);
	CNetMgr::m_qInterMsg->push(InterMsg);
	LeaveCriticalSection(&CNetMgr::GetInstance()->CS);
	return 0;
}

int OnNotifyMapChange(char* cpPacket)
{
#ifdef _FROM_SERVER_
	cout << "From Server : OnNotifyMapChange" << endl;
#endif
	INTERMSGDATA InterMsg;
	InterMsg.sHeaderType = NOTIFY_MAPCHANGE;

	//mapNum
	CopyMemory(InterMsg.tcInterMsg + HEADERSIZE, cpPacket + HEADERSIZE, 1);

	EnterCriticalSection(&CNetMgr::GetInstance()->CS);
	CNetMgr::m_qInterMsg->push(InterMsg);
	LeaveCriticalSection(&CNetMgr::GetInstance()->CS);
	return 0;
}

int OnNotifyChangeTeam(char* cpPacket)
{
#ifdef _FROM_SERVER_
	cout << "From Server : OnNotifyChangeTeam" << endl;
#endif
	INTERMSGDATA InterMsg;
	InterMsg.sHeaderType = NOTIFY_CHANGETEAM;

	//mapNum
	CopyMemory(InterMsg.tcInterMsg + HEADERSIZE, cpPacket + HEADERSIZE, 2);

	EnterCriticalSection(&CNetMgr::GetInstance()->CS);
	CNetMgr::m_qInterMsg->push(InterMsg);
	LeaveCriticalSection(&CNetMgr::GetInstance()->CS);
	return 0;

}
int OnNotifyAllChat(char* cpPacket)
{
#ifdef _FROM_SERVER_
	cout << "From Server : OnNotifyAllChat" << endl;
#endif
	CCoder coder;
	char cMsg[MAX_STR];
	UCHAR cMsgLen;

	coder.SetBuf(cpPacket);
	coder.GetChar((char*)&cMsgLen);
	coder.GetText(cMsg, cMsgLen);

	INTERMSGDATA InterMsg;
	InterMsg.sHeaderType = NOTIFY_ALLCHAT;

	coder.SetBuf(InterMsg.tcInterMsg);
	coder.PutChar(cMsgLen);
	coder.PutText(cMsg, cMsgLen);

	EnterCriticalSection(&CNetMgr::GetInstance()->CS);
	CNetMgr::m_qInterMsg->push(InterMsg);
	LeaveCriticalSection(&CNetMgr::GetInstance()->CS);

	return 0;
}
int OnNotifyLoadingEnd(char* cpPacket)
{
#ifdef _FROM_SERVER_
	cout << "From Server : OnNotifyLoadingEnd" << endl;
#endif
	INTERMSGDATA InterMsg;
	InterMsg.sHeaderType = NOTIFY_LOADINGEND;

	EnterCriticalSection(&CNetMgr::GetInstance()->CS);
	CNetMgr::m_qInterMsg->push(InterMsg);
	LeaveCriticalSection(&CNetMgr::GetInstance()->CS);
	return 0;

}

int OnNotifyInput(char* cpPacket)
{
	INTERMSGDATA InterMsg;
	InterMsg.sHeaderType = NOTIFY_INPUT;

	short sType;
	short sBody;
	CCoder coder;
	coder.SetBuf(cpPacket);
	coder.GetHeader(&sBody , &sType);
	CopyMemory(InterMsg.tcInterMsg, cpPacket, sBody + HEADERSIZE);

	EnterCriticalSection(&CNetMgr::GetInstance()->CS);
	CNetMgr::m_qInterMsg->push(InterMsg);
	LeaveCriticalSection(&CNetMgr::GetInstance()->CS);
	return 0;
}

int OnNotifyPosDir(char* cpPacket)
{
	INTERMSGDATA InterMsg;
	InterMsg.sHeaderType = NOTIFY_POSDIR;

	short sType;
	short sBody;
	CCoder coder;
	coder.SetBuf(cpPacket);
	coder.GetHeader(&sBody, &sType);
	CopyMemory(InterMsg.tcInterMsg, cpPacket, sBody + HEADERSIZE);


	EnterCriticalSection(&CNetMgr::GetInstance()->CS);
	CNetMgr::m_qInterMsg->push(InterMsg);
	LeaveCriticalSection(&CNetMgr::GetInstance()->CS);
	return 0;
}

int OnNotifyGiveDamage(char* cpPacket)
{
	INTERMSGDATA InterMsg;
	InterMsg.sHeaderType = NOTIFY_GIVEDAMAGE;

	short sType;
	short sBody;
	CCoder coder;
	coder.SetBuf(cpPacket);
	coder.GetHeader(&sBody, &sType);
	CopyMemory(InterMsg.tcInterMsg, cpPacket, sBody + HEADERSIZE);


	EnterCriticalSection(&CNetMgr::GetInstance()->CS);
	CNetMgr::m_qInterMsg->push(InterMsg);
	LeaveCriticalSection(&CNetMgr::GetInstance()->CS);
	return 0;
}


int OnNotifyHpSync(char* cpPacket)
{
	INTERMSGDATA InterMsg;
	InterMsg.sHeaderType = NOTIFY_HPSYNC;

	short sType;
	short sBody;
	CCoder coder;
	coder.SetBuf(cpPacket);
	coder.GetHeader(&sBody, &sType);
	CopyMemory(InterMsg.tcInterMsg, cpPacket, sBody + HEADERSIZE);


	EnterCriticalSection(&CNetMgr::GetInstance()->CS);
	CNetMgr::m_qInterMsg->push(InterMsg);
	LeaveCriticalSection(&CNetMgr::GetInstance()->CS);

	return 0;
}

int OnNotifyDead(char* cpPacket)
{
	INTERMSGDATA InterMsg;
	InterMsg.sHeaderType = NOTIFY_DEAD;

	short sType;
	short sBody;
	CCoder coder;
	coder.SetBuf(cpPacket);
	coder.GetHeader(&sBody, &sType);
	CopyMemory(InterMsg.tcInterMsg, cpPacket, sBody + HEADERSIZE);


	EnterCriticalSection(&CNetMgr::GetInstance()->CS);
	CNetMgr::m_qInterMsg->push(InterMsg);
	LeaveCriticalSection(&CNetMgr::GetInstance()->CS);

	return 0;
}
