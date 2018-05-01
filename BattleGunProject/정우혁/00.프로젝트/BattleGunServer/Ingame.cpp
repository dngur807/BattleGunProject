#include "Default.h"
#include "Ingame.h"
#include "Constant.h"
#include "Lobby.h"
#include "Protocol.h"
#include "Coder.h"
#include "IO.h"


int InitIngame()
{
	g_Server.pIngame = new CIngame;//게임 시작할때 초기화
	g_Server.pIngame->Initialize();

	// 핸들러 
	g_OnTransFunc[REQUEST_POSDIR].proc = OnRequestPosDir;
	g_OnTransFunc[REQUEST_INPUT].proc = OnRequestInput;
	g_OnTransFunc[REQUEST_GIVEDAMAGE].proc = OnRequestGiveDamage; 
	g_OnTransFunc[REQUEST_HPSYNC].proc = OnRequestHpSync; 
	g_OnTransFunc[REQUEST_DEAD].proc = OnRequestDead;
	g_OnTransFunc[REQUEST_REVIVE].proc = OnRequestRevive; 
	g_OnTransFunc[REQUEST_CHANGEWEAPON].proc = OnRequestChangeWeapon;
	g_OnTransFunc[REQUEST_HOLDSTATE].proc = OnRequestHoldState;
	g_OnTransFunc[REQUEST_FIREWEAPON].proc = OnRequestFireWeapon;
	return 0;
}
void CIngame::Initialize()
{
	switch (g_Server.m_eMapType)
	{
	case MAP_GESTALT:
		//맵생성
		break;
	case MAP_CONVOY:
		//맵 생성
		break;
	default:
		break;
	}
	

}

int CIngame::Update()
{
	if (m_eGameServ != SERVSTATE_INGAME_PLAYING
		&& m_eGameServ != SERVSTATE_GAMEEND)
		return 0 ;

	if (m_eGameServ != SERVSTATE_GAMEEND)
	{
		if (m_dwTime + 1000 < GetTickCount())// 1초에 한번
		{
			m_dwTime = GetTickCount();
			m_fGameTime--;
			NotifyGameTimer(m_fGameTime);
		}
		if (m_fGameTime < 0.0f)
		{
			m_eGameServ = SERVSTATE_GAMEEND;
			m_fGameTime = -1;
			GameEnd();
			NotifyGameResult();
		}
	}

	switch (m_eGameServ)
	{
	case SERVSTATE_GAMEEND:
		if (m_dwTime + 3000 < GetTickCount())
		{
			m_dwTime = GetTickCount();
			NotifyGoLobby();
			m_eGameServ = SERVSTATE_INGAME_WAITING;
			return 0;
		}
		break;
	}
	
	return 0;
}

void CIngame::GameStart()
{
	Initialize();
	//게임 프로세스 쓰레드 생성
	m_fGameTime = 60;
	m_dwTime = 0;
	m_eGameServ = SERVSTATE_INGAME_PLAYING;

}

void CIngame::GameEnd()
{
	g_Server.pLobby->Initialize();
	m_eGameServ = SERVSTATE_GAMEEND;
}



int OnRequestHoldState(LPCLIENTCONTEXT lpSocketContext, char* cpPacket)
{
	CCoder coder;
	char   cIndex;
	char   cState;
	char   cPacket[MIN_STR];
	int    iPacketSize;

	coder.SetBuf(cpPacket);
	coder.GetChar(&cIndex);
	coder.GetChar(&cState);


	coder.SetBuf(cPacket);
	coder.PutChar(cIndex);
	coder.PutChar(cState);
	iPacketSize = coder.SetHeader(NOTIFY_HOLDSTATE);
	PostTcpSend(g_Server.iUserBegin, cPacket, iPacketSize);
	return 0;
}
int OnRequestFireWeapon(LPCLIENTCONTEXT lpSocketContext, char* cpPacket)
{
	CCoder		 coder;
	char		cIndex;
	char		cType;
	XMFLOAT3	vPos;
	XMFLOAT3	vDir;
	char   cPacket[MIN_STR];
	int    iPacketSize;

	coder.SetBuf(cpPacket);
	coder.GetChar(&cIndex);
	coder.GetChar(&cType);
	coder.GetXMFLOAT3(&vPos);
	coder.GetXMFLOAT3(&vDir);

	coder.SetBuf(cPacket);
	coder.PutChar(cIndex);
	coder.PutChar(cType);
	coder.PutXMFLOAT3(&vPos);
	coder.PutXMFLOAT3(&vDir);
	iPacketSize = coder.SetHeader(NOTIFY_FIREWEAPON);
	PostTcpSend(g_Server.iUserBegin, cPacket, iPacketSize);

	return 0;
}

void CIngame::NotifyGameResult()
{
	CCoder coder;
	char cPacket[MIN_STR];
	int  iPacketSize;

	coder.SetBuf(cPacket);
	iPacketSize = coder.SetHeader(NOTIFY_GAMERESULT);
	PostTcpSend(g_Server.iUserBegin, cPacket, iPacketSize);
}


void CIngame::NotifyGameTimer(int iTimer)
{
//#ifdef _INGAME_MSG_CHECK_
//	printf("NotifyGameTimer \n");
//#endif

	CCoder coder;
	char cPacket[MIN_STR];
	int iPacketSize;

	coder.SetBuf(cPacket);
	coder.PutInt(iTimer);

	iPacketSize = coder.SetHeader(NOTIFY_GAMETIMER);
	PostTcpSend(g_Server.iUserBegin, cPacket, iPacketSize);
}
void CIngame::NotifyGoLobby(void)
{

#ifdef _INGAME_MSG_CHECK_
	printf("NotifyGoLobby \n");
#endif


	CCoder coder;
	char cPacket[MIN_STR];
	int  iPacketSize;

	coder.SetBuf(cPacket);

	iPacketSize = coder.SetHeader(NOTIFY_GOLOBBY);
	PostTcpSend(g_Server.iUserBegin, cPacket, iPacketSize);

	coder.SetBuf(cPacket);
	coder.PutChar(g_Server.iUserBegin);
	iPacketSize = coder.SetHeader(NOTIFY_ROOMMANAGER);
	PostTcpSend(g_Server.iUserBegin, cPacket, iPacketSize);
}


 int OnRequestRevive(LPCLIENTCONTEXT lpSockContext, char* cpPacket)
{
	 CCoder coder;
	 char szPacket[MIN_STR];
	 int  iPacketSize;
	 char cIndex;
	 XMFLOAT3 vPos;
	 coder.SetBuf(cpPacket);
	 coder.GetChar(&cIndex);
	 coder.GetXMFLOAT3(&vPos);

	 coder.SetBuf(szPacket);
	 coder.PutChar(cIndex);
	 coder.PutXMFLOAT3(&vPos);

#ifdef _INGAME_MSG_CHECK_
	 printf("OnRequestRevive  Index %d  vPos %f  %f  %f\n", (int)cIndex , vPos.x , vPos.y , vPos.z);
#endif

	 iPacketSize = coder.SetHeader(NOTIFY_REVIVE);
	 PostTcpSend(g_Server.iUserBegin, szPacket, iPacketSize);

	 return 0;
}

int OnRequestDead(LPCLIENTCONTEXT lpSockContext, char* cpPacket)
{
	short sType;
	sType = NOTIFY_DEAD;
	CopyMemory(cpPacket + sizeof(short), &sType, sizeof(short));
	CopyMemory(&sType, cpPacket, sizeof(short));
#ifdef _INGAME_MSG_CHECK_
	printf("OnRequestDead \n");
#endif
	PostTcpSend(g_Server.iUserBegin, cpPacket, sType + HEADERSIZE);
	return 0;
}

int OnRequestChangeWeapon(LPCLIENTCONTEXT lpSockContext, char* cpPacket)
{
	CCoder coder;
	char   cIndex;
	char   cWeapon;

	coder.SetBuf(cpPacket);
	coder.GetChar(&cIndex);
	coder.GetChar(&cWeapon);

	char szPacket[MIN_STR];
	int iPacketSize;

	coder.SetBuf(szPacket);
	coder.PutChar(cIndex);
	coder.PutChar(cWeapon);
#ifdef _INGAME_MSG_CHECK_
	printf("OnRequestChangeWeapon  Index %d WeaponType : %d\n" , (int)cIndex , (int)cWeapon);
#endif
	iPacketSize = coder.SetHeader(NOTIFY_CHANGEWEAPON);
	PostTcpSend(g_Server.iUserBegin, szPacket, iPacketSize);

	return 0;
}

int OnRequestHpSync(LPCLIENTCONTEXT lpSockContext, char* cpPacket)
{
	short sType;
	sType = NOTIFY_HPSYNC;
	CopyMemory(cpPacket + sizeof(short), &sType, sizeof(short));
	CopyMemory(&sType, cpPacket, sizeof(short));
#ifdef _INGAME_MSG_CHECK_
	printf("OnRequestHpSync \n");
#endif
	PostTcpSend(g_Server.iUserBegin, cpPacket, sType + HEADERSIZE);
	return 0;
}
 int OnRequestGiveDamage(LPCLIENTCONTEXT lpSockContext, char *cpPacket)
{
	 CCoder coder;
	 char szPacket[MIN_STR];
	 char cAttackerIndex;
	 char cTargetIndex;
	 int  iDamage;
	 int iPacketSize;

	 coder.SetBuf(cpPacket);
	 coder.GetChar(&cAttackerIndex);
	 coder.GetChar(&cTargetIndex);
	 coder.GetInt(&iDamage);


	 coder.SetBuf(szPacket);
	 coder.PutChar(cAttackerIndex);
	 coder.PutChar(cTargetIndex);
	 coder.PutInt(iDamage);

#ifdef _INGAME_MSG_CHECK_
	 printf("OnRequestGiveDamage  AttackerIdx %d  TargetIndex %d Damage : %d\n", (int)cAttackerIndex , (int)cTargetIndex , iDamage);
#endif


	 iPacketSize = coder.SetHeader(NOTIFY_GIVEDAMAGE);
	 PostTcpSend(g_Server.iUserBegin, szPacket, iPacketSize);
	 return 0;
}

int OnRequestInput(LPCLIENTCONTEXT lpSockContext, char *cpPacket)
{
	CCoder coder;
	char szPacket[MIN_STR];
	int  iPacketSize;
	char cIndex;
	unsigned int iMask;
	coder.SetBuf(cpPacket);
	coder.GetChar(&cIndex);
	coder.GetUInt(&iMask);

	coder.SetBuf(szPacket);
	coder.PutChar(cIndex);
	coder.PutUint(iMask);

#ifdef _INGAME_MOVE_
	printf("OnRequestInput  Index %d \n", (int)cIndex);
#endif

	iPacketSize = coder.SetHeader(NOTIFY_INPUT);
	PostTcpSend(g_Server.iUserBegin, szPacket, iPacketSize);
	return 0 ;
}

int OnRequestPosDir(LPCLIENTCONTEXT lpSockContext, char *cpPacket)
{
	CCoder coder;
	char szPacket[MIN_STR];
	int	 iPacketSize;
	char cIndex;
	UINT iKey;
	XMFLOAT3 vPos;
	float AngleX;
	float AngleY;


	coder.SetBuf(cpPacket);
	coder.GetChar(&cIndex);
	coder.GetUInt(&iKey);
	coder.GetXMFLOAT3(&vPos);
	coder.GetFloat(&AngleX);
	coder.GetFloat(&AngleY);

#ifdef _INGAME_MOVE_
	printf("OnRequestPosDir Index : %d Pos X : %lf Y : %lf Z : %lf Angle X : %lf Angle : %lf \n"
	, (int)cIndex , vPos.x , vPos.y , vPos.z , AngleX , AngleY);
#endif
	coder.SetBuf(szPacket);
	coder.PutChar(cIndex);
	coder.PutUint(iKey);
	coder.PutXMFLOAT3(&vPos);
	coder.PutFloat(AngleX);
	coder.PutFloat(AngleY);

	iPacketSize = coder.SetHeader(NOTIFY_POSDIR);
	PostTcpSend(g_Server.iUserBegin, szPacket, iPacketSize);
	return 0;
}