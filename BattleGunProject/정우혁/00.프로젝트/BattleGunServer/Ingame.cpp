#include "Default.h"
#include "Ingame.h"
#include "Constant.h"
#include "Lobby.h"

void CIngame::Initialize()
{
	switch (g_Server.m_eMapType)
	{
	case MAP_GESTALT:
		//�ʻ���
		break;
	case MAP_CONVOY:
		//�� ����
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
	//���� ���μ��� ������ ����
}

void CIngame::GameEnd()
{
	g_Server.pLobby->m_iLoadingCnt = 0;
	
}

int InitIngame()
{
	g_Server.pIngame = new CIngame;//���� �����Ҷ� �ʱ�ȭ
	//�ڵ鷯

	return 0;
}
