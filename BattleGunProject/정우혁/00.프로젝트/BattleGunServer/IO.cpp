#include "Default.h"
#include "IO.h"
#include "Include.h"

/*
���⼭�� IOCP�� ����մϴ�. �� / ��� �Ϸ� ��Ʈ�� �θ��� IOCP�� �аų� ���� �� 
�� ��� �۾��� �Ϸ� �Ǿ��� ������ �̺�Ʈ�� �߻��մϴ�.
���� �� ������ ������ �۾��� ������ �� �ִ� ���̴�.
�׷��Ƿ� IOCP�� ����ϴ� ������ ��Ƽ �����尡 �ʼ��� �䱸�˴ϴ�.

�̰��� ���� �񵿱� ���ϰ� �Բ� ����մϴ�.

WSARecv�� WSASend�� ���� �����͸� ���� ���� ���� ��쿡 ���Ǹ� ���⿡ ������ ���� Ưȭ�Ǿ�
Ȯ��� �Լ��� AcceptEx�� GetAcceptExSockaddr�� ����Ѵ�.

AcceptEx �� ������ �̸� ������ �� ���ο� ������ �̷����� �� �� ������ ����ϴ� ����Դϴ�.
�� Ư¡���δ� �̷��� ������ �̷�� ���� �ܰ�� Ŭ���̾�Ʈ�� connect�� ����Ͽ� ������ �� ù ��° ������ ����� ���۵Ǿ��� ���� �ȴٴ� ���̴�.
�׷��Ƿ� ù ��° ����� ������ �ʰ� Connect���� �ϴ� ���� ������������ �������� �ν����� �ʴ´�.
�����ϰ� ������ �̷����� ���� Acceptex�� ���ڿ� �־��� ������� �ڽŰ� ������ �ּҰ� �Բ� �Ѿ�´�.
�̰͵��� GetAcceptExSockaddrs API�� ����Ͽ� �����ϰ� �ؼ��� �� �� �ִ�.
*/

int InitIO()
{
	WSADATA			wsa;
	SOCKADDR_IN		addr;
	int				itv;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return -1;

	//  Ŭ���̾�Ʈ�� ������ �ޱ����� ���� ���� ����
	g_Server.sockListener = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (g_Server.sockListener == INVALID_SOCKET)
		return -1;
	//���� ��Ʈ�� ���ε�
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(PORTNUMBER);
	itv = bind(g_Server.sockListener, (sockaddr*)&addr, sizeof(addr));
	if (itv == SOCKET_ERROR)
		return -1;

	// ���� ���
	itv = listen(g_Server.sockListener, 512);
	if (itv == SOCKET_ERROR)
		return -1;

	// Ŭ���̾�Ʈ�� ���� ����� ���� IOCP ����
	g_Server.hIocpWorkTcp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	if (g_Server.hIocpWorkTcp == NULL)return -1;

	// ���� ������ �޾� �帱�� ����մϴ�.
	g_Server.hIocpAcpt = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	if (g_Server.hIocpAcpt == NULL)return -1;


	// ���� ������ IocpAcpt�� �����մϴ�.
	// ���� �̺�Ʈ�� �߻��ϸ� �ٷ� Ŭ���̾�Ʈ�� ������ ����� ���� �� �� �ֽ��ϴ�.
	CreateIoCompletionPort((HANDLE)g_Server.sockListener, g_Server.hIocpAcpt, 0, 0);


	// �ִ� ���� ����ŭ�� ����ü�� �̸� ����� ���´�.



}

