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

	//핸들러
	g_OnTransFunc[REQUEST_POSDIR].proc = OnRequestPosDir;
	g_OnTransFunc[REQUEST_INPUT].proc = OnRequestInput;
	g_OnTransFunc[REQUEST_GIVEDAMAGE].proc = OnRequestGiveDamage;
	g_OnTransFunc[REQUEST_HPSYNC].proc = OnRequestHpSync;
	g_OnTransFunc[REQUEST_DEAD].proc = OnRequestDead;
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

void CIngame::Update()
{

}

void CIngame::GameStart()
{
	Initialize();
	//게임 프로세스 쓰레드 생성
}

void CIngame::GameEnd()
{
	g_Server.pLobby->m_iLoadingCnt = 0;
	
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