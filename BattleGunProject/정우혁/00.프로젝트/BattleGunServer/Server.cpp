#include "Default.h"
#include "Server.h"
#include "Include.h"
#include "IO.h"
#include "ServerProcess.h"

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


	GetSystemInfo(&si);// 현재 사용 중인 컴퓨터의 시스템에 관련된 정보를 반환합니다.
	g_Server.iWorkerTNum = min(si.dwNumberOfProcessors * 2, 16);
	// dwNumberOfProcessors CPU 코어 개수
	

	InitializeCriticalSection(&g_Server.CS);//임계영역 초기화

	// 통신을 위한 IO 초기화
	if (InitIO() == -1)
		return 0;
	// 로비 초기화
	// 인게임 초기화

	// 프로세스 초기화
	if (InitProcess() == -1)
		return 0;

	while (1)
	{
		
	}
	return 0;

}

