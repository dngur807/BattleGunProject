#include "Default.h"
#include "Server.h"
#include "Include.h"
#include "IO.h"


SERVERCONTEXT			g_Server;

int main()
{
	g_Server.iCurUserNum = 0;
	g_Server.iMaxUserNum = MAXUSER;

	InitializeCriticalSection(&g_Server.CS);//임계영역 초기화

	// 통신을 위한 IO 초기화
	if (InitIO() == -1)
		return 0;
	// 로비 초기화

	// 인게임 초기화

	return 0;

}
