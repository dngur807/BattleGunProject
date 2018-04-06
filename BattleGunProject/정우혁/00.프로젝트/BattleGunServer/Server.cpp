#include "Default.h"
#include "Server.h"
#include "Include.h"
#include "IO.h"
#include "ServerProcess.h"
#include "Lobby.h"
#include "Ingame.h"

SERVERCONTEXT			g_Server;


int main()
{
	SYSTEM_INFO				si;
	g_Server.iCurUserNum = 0;
	g_Server.iMaxUserNum = MAXUSER;
	g_Server.iMaxProcess = 4;
	g_Server.iAllUserNum = 0;
	g_Server.iATeamNum = 0;
	g_Server.iBTeamNum = 0;
	g_Server.m_eMapType = MAP_GESTALT;

	GetSystemInfo(&si);// 현재 사용 중인 컴퓨터의 시스템에 관련된 정보를 반환합니다.
	g_Server.iWorkerTNum = min(si.dwNumberOfProcessors * 2, 16); // CPU 개수만큼 워커 스레드 생성하기 위해서
	// dwNumberOfProcessors CPU 코어 개수
	

	InitializeCriticalSection(&g_Server.CS);//임계영역 초기화

	// 통신을 위한 IO 초기화
	if (InitIO() == -1)
		return 0;
	// 로비 초기화
	if (InitLobby() == -1)
		return 0;
	// 인게임 초기화
	if (InitIngame() == -1)
		return 0;
	// 프로세스 초기화
	if (InitProcess() == -1)
		return 0;

	while (1)
	{
		if (g_Server.pIngame)
		{
			g_Server.pIngame->Update();
		}
	}
	if (g_Server.sc)
		delete[] g_Server.sc;
	if (g_Server.pn)
		delete[] g_Server.pn;
	if (g_Server.ps)
		delete[] g_Server.ps;
	if (g_Server.pLobby)
		delete g_Server.pLobby;
	if (g_Server.pIngame)
		delete g_Server.pIngame;
	DeleteCriticalSection(&g_Server.CS);
	return 0;

}

