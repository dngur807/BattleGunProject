#include "Default.h"
#include "Ingame.h"
#include "Constant.h"
#include "Lobby.h"

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

int InitIngame()
{
	g_Server.pIngame = new CIngame;//게임 시작할때 초기화
	//핸들러

	return 0;
}
