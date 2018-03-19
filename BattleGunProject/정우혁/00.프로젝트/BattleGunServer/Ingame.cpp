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

#ifdef _INGAME_MSG_CHECK_
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
	XMFLOAT3 vPos;
	float AngleX;
	float AngleY;


	coder.SetBuf(cpPacket);
	coder.GetChar(&cIndex);
	coder.GetXMFLOAT3(&vPos);
	coder.GetFloat(&AngleX);
	coder.GetFloat(&AngleY);

#ifdef _INGAME_MSG_CHECK_
	printf("OnRequestPosDir Index : %d Pos X : %lf Y : %lf Z : %lf Angle X : %lf Angle : %lf \n"
	, (int)cIndex , vPos.x , vPos.y , vPos.z , AngleX , AngleY);
#endif
	coder.SetBuf(szPacket);
	coder.PutChar(cIndex);
	coder.PutXMFLOAT3(&vPos);
	coder.PutFloat(AngleX);
	coder.PutFloat(AngleY);

	iPacketSize = coder.SetHeader(NOTIFY_POSDIR);
	PostTcpSend(g_Server.iUserBegin, szPacket, iPacketSize);
	return 0;
}