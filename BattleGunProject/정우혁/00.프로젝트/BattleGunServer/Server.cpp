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


	GetSystemInfo(&si);// ���� ��� ���� ��ǻ���� �ý��ۿ� ���õ� ������ ��ȯ�մϴ�.
	g_Server.iWorkerTNum = min(si.dwNumberOfProcessors * 2, 16);
	// dwNumberOfProcessors CPU �ھ� ����
	

	InitializeCriticalSection(&g_Server.CS);//�Ӱ迵�� �ʱ�ȭ

	// ����� ���� IO �ʱ�ȭ
	if (InitIO() == -1)
		return 0;
	// �κ� �ʱ�ȭ
	// �ΰ��� �ʱ�ȭ

	// ���μ��� �ʱ�ȭ
	if (InitProcess() == -1)
		return 0;

	while (1)
	{
		
	}
	return 0;

}

