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

	GetSystemInfo(&si);// ���� ��� ���� ��ǻ���� �ý��ۿ� ���õ� ������ ��ȯ�մϴ�.
	g_Server.iWorkerTNum = min(si.dwNumberOfProcessors * 2, 16); // CPU ������ŭ ��Ŀ ������ �����ϱ� ���ؼ�
	// dwNumberOfProcessors CPU �ھ� ����
	

	InitializeCriticalSection(&g_Server.CS);//�Ӱ迵�� �ʱ�ȭ

	// ����� ���� IO �ʱ�ȭ
	if (InitIO() == -1)
		return 0;
	// �κ� �ʱ�ȭ
	if (InitLobby() == -1)
		return 0;
	// �ΰ��� �ʱ�ȭ
	if (InitIngame() == -1)
		return 0;
	// ���μ��� �ʱ�ȭ
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

