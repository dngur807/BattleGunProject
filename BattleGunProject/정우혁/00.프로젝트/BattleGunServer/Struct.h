#pragma once
#include "Default.h"

//���� ���� ����ü�̴�. 
// ���� ���α׷� ��ü���� ����ϴ� ���� ������ ���� ���� ����ü�̴�.
typedef struct 
{
	SOCKET					sockListener;//������ ������ ��ٸ��� ���� �����̴�.
	HANDLE					hIocpWorkTcp;	//	IOCP �ڵ�μ� ������ �б� ���� �뵵
	HANDLE					hIocpAcpt;		// IOCP �ڵ�μ� ������ ������ �޾Ƶ帱 �� ����մϴ�.

	int						iMaxUserNum;// �ִ� ���� ��
	int						iCurUserNum;// ���� ���� ��
	//���� ����
	CRITICAL_SECTION		 m_CS;


}SERVERCONTEXT, *LPSERVERCONTEXT;

// Ŭ���̾�Ʈ�� ����ϴ� ���� ����ü�̴�.

typedef struct
{
	

}CLIENTCONTEXT , *LPCLIENTCONTEXT;