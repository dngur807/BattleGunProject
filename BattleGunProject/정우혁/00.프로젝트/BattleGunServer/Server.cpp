#include "Default.h"
#include "Server.h"
#include "Include.h"
#include "IO.h"


SERVERCONTEXT			g_Server;

int main()
{
	g_Server.iCurUserNum = 0;
	g_Server.iMaxUserNum = MAXUSER;

	InitializeCriticalSection(&g_Server.CS);//�Ӱ迵�� �ʱ�ȭ

	// ����� ���� IO �ʱ�ȭ
	if (InitIO() == -1)
		return 0;
	// �κ� �ʱ�ȭ

	// �ΰ��� �ʱ�ȭ

	return 0;

}
