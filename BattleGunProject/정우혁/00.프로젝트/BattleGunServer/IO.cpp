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
	HANDLE			hThread;
	int				itv;

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return -1;

	//  Ŭ���̾�Ʈ�� ������ �ޱ����� ���� ���� ����
	// �������� �ɼ��� ����ϱ� ���ؼ� �ͺ��ŷ ��� ����
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
	g_Server.pClientBegin = new CLIENTCONTEXT[g_Server.iMaxUserNum];
	if (g_Server.pClientBegin == NULL)
		return -1;

	// �ش� ���� ���� �ʱ�ȭ
	if (InitSocketContext(g_Server.iMaxUserNum) == -1)
		return -1;

	//Ŭ���̾�Ʈ ���� �޴� ������
	hThread = (HANDLE)_beginthreadex(NULL, 0, AcceptProc, NULL, 0, (UINT*)&itv);
	if (hThread == NULL)
		return -1;


	return 0;
}

int InitSocketContext(int maxUser)
{
	int itv;
	DWORD	dwReceived;

	LPCLIENTCONTEXT		lpClient = g_Server.pClientBegin;


	// ���� ���ؽ�Ʈ ����ü ��ü�� �ʱ�ȭ�մϴ�.
	ZeroMemory(lpClient, sizeof(CLIENTCONTEXT) * maxUser);

	for (int i = 0; i < maxUser; ++i)
	{
		// ���� ����ü ���� �ε����� �ο��մϴ�.
		lpClient[i].iKey = i;
		lpClient[i].eovSend.mode = SENDEOVTCP;
		lpClient[i].eovRecv.mode = RECVEOVTCP;

		// ���� �ʱ� ���� ����
		lpClient[i].pRecvBegin = lpClient[i].pRecvEnd = lpClient[i].RecvRingBuf;
		lpClient[i].pSendBegin = lpClient[i].pSendEnd = lpClient[i].SendRingBuf;


		// Ŭ���̾�Ʈ���� ����� ���� ���� ����
		lpClient[i].sockClnt = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
		if (lpClient[i].sockClnt == INVALID_SOCKET)
			return -1;


		// AcceptEx�� �̸� ������ �����Ͽ� ���ڷ� �ް� ������ �Ϸ�Ǹ� CompletionPort��ü��
		// �κ�Ʈ �Ǵ� �ݹ� �Լ��� �˷��ش�.
		// ���ӽ� ���ϸ� ���� �� �ִ�.

		itv = AcceptEx(g_Server.sockListener, lpClient[i].sockClnt, lpClient[i].pRecvEnd
			, MAXPACKETSIZE, sizeof(sockaddr_in) + 16, sizeof(sockaddr_in) + 16
			, &dwReceived, (OVERLAPPED*)&lpClient[i].eovRecv);
		if (itv == FALSE  &&  GetLastError() != ERROR_IO_PENDING)
			return -1;

		// SendBuffer�� ���� �Ӱ迵���� �����Ѵ�.
		InitializeCriticalSection(&lpClient[i].CS);
	}
	return 0;
}

// ���� �㰡 ������ �Դϴ�.
// Ŭ���̾�Ʈ�� �����ϴ� ������ �۵��� �� �ִ� �������Դϴ�.
UINT WINAPI AcceptProc(void* pParam)
{
	BOOL				IsResult;
	DWORD				dwTransferred;
	DWORD				CompletionKey;
	sockaddr_in				*pLocal;
	sockaddr_in				*pRemote;
	int						localLen;
	int						remoteLen;
	LPCLIENTCONTEXT	lpClientContext;//Ȯ��� ������ ����ü


	LPEOVERLAPPED	lpEov;//�̺�Ʈ�� ������ ��
	while (1)
	{
		// ���� ���ϰ� ����� IOCP�� ������ ���� ������ ����Ѵ�.
		IsResult = GetQueuedCompletionStatus(g_Server.hIocpAcpt, &dwTransferred , (LPDWORD)&CompletionKey
			, (LPOVERLAPPED*)&lpEov, INFINITE);

		// dwTransferred�� ���۵� ����Ʈ ���̴�.
		// �̰��� �б⿡ ���Ͽ� �߻��� ���̶�� �о���� ����Ʈ ����
		// ���⿡ ���Ͽ� �߻��� ���̶�� ������ ����Ʈ�� ���� ����Ų��.
		// &dummy�� Completion key�̴�.

		// CreateIOCompletionPort�� �̿��Ͽ� ���ϰ� 
		// IOCP�� ������ �� �ٷ� �� Completion key �����Ѵ�.
		// ���� �̰��� �ϳ��� ���а����μ� ��� �� �� �ִ�
		
		// ��� �� Accept �����忡���� �� ���� ���ǹ� �ϴ�. ���޵� ���� �������� �ʱ� ���� �Դϴ�.
		// �� �����忡���� �̺�Ʈ��� ���� ������ ���� ���Ͽ� ����� ���̱� ������
		// Ŭ���̾�Ʈ�� ������ ���� �����̴�.
		// �׷��� ������ InitIo �κп��� ���� ���ϰ� IOCP�� ������ �� 0���θ� ȣ���� ���̴�.

		// &lpEov�� ������ ����ü�� ������ ���̴�.
		// ����� �ٷ� �� , ��¿� ���Ǿ��� ������ ����ü�� �ּҸ� �Ѱ��ִ� �κ��̴�.
		// ��� ���� �񵿱�� �۵��ϰ� �ǹǷ� ��� �۾��� �Ͼ ���� �׻� ������ ����ü�� ����ϸ�
		// �׿� ���� �ּҰ� ���޵ȴ�.


		// AcceptEx�� Ŭ���̾�Ʈ�� ������ �Ŀ� ù ��° ������ ����� ���۵Ǿ�߸� 
		// �������� �������� �ν��Ѵ�.
		// �߿��� ���� �̷��� ù ��° ����� �Ѿ���� �� ���� ��Ʈ��ũ �����̹Ƿ�
		// AcceptEx�� ȣ���� ��ÿ��� ������ ����ü�� ����Ѵٴ� ���̴�.
		// ���� GetQueueCompletionStatus���� ��� ���� ù ��° �о���� ù ��° ������ ����� �� ,
		// �� ��° Completion key ������ 0 (�տ����� ���� �������� �׻� 0�� �־��� �����Դϴ�.)
		// �� ��°�� AcceptEx�� ���ڷ� �־��� �� Ȯ�� ������ ����ü�� �ּ��Դϴ�.
		// �� �̺�Ʈ�� �߻��� ���������� Ŭ���̾�Ʈ���� ������ ���Ͽ�
		// �Ҵ�� ���ϰ� �� �� Ȯ��� ���ϰ� �� �� Ȯ��� ������ ����ü�� ���谡
		// ��� �ϳ��� ���� ����ü�ȿ� ���ϴ� �͵�� �̷������.
		// AcceptEx�� ȣ���� �� ��� ���� ���� ����ü ���� ������θ� ���Ǿ��� �����Դϴ�.
		// �� ���� ���� ������ Ŭ���̾�Ʈ�� ��� ���� ����ü�� �Ҵ�޾Ҵ����� �� �� �ִ� �ٰŰ� �ȴ�.

		// �̺�Ʈ�� �߻��Ͽ� GetQueuedCompletionStatus�� ��� ������ �������� 
		// ������ Ŭ���̾�Ʈ�� �ڱ�� ����� ���� �ϳ��� ���ϰ� ���� Ȯ���
		// ������ ����ü�� �̹� �Ҵ���� �����̱� ������ ���� �׵��� ���� ���� ����ü �ȿ� ���ԵǾ� �ֱ� ������ �� Ȯ���
		// ������ ����ü�� �ּҸ��� �˰� �ȴٸ� ������ Ŭ���̾�Ʈ�� ��� ���� ����ü�� ����ϴ����� �� �� �ִ�.
		// ���� lpsockContext = (LPSOCKETCONTEXT)lpEov;//�� ���� ���谡 �����Ѵ�.


		// ������ ���� ���
		if (lpEov != NULL && dwTransferred != 0)
		{
			g_Server.iCurUserNum++;//���� ����~
			lpClientContext = (LPCLIENTCONTEXT)lpEov;// �̺�Ʈ�� �߻��� �������ؽ�Ʈ�� ��´�.
			// AcceptEx�� �ִ� Ȯ��� ������ ����ü�� ù ��° ����� �ޱ� ���� ���̹Ƿ�  ���� ����ü����
			// eovRecvTcp�� ����ϰ� �Ǵµ� �̰��� �ٷ� �� ���� ����ü�� ù ��° ����̱� ������
			// �װ��� �ּ� ���� ���� ����ü�� ���� ���̶� �� �� �ִ�.


			// ���� ���� ���� �ּҸ� �� ����
			GetAcceptExSockaddrs(lpClientContext->pRecvEnd, MAXPACKETSIZE, sizeof(sockaddr_in) + 16
				, sizeof(sockaddr_in) + 16, (sockaddr**)&pLocal, &localLen, (sockaddr**)&pRemote, &remoteLen);


			CopyMemory(&lpClientContext->remoteAddr, pRemote, sizeof(sockaddr_in));

			// ������ end pointer ����
			RecvTcpBufEnqueue(lpClientContext , dwTransferred);

			// Accept �����忡���� ���� ��� ���Ʊ� ������ Worker ������� ���ӵ�
			// Ŭ���̾�Ʈ�� ���� ����ü�� �Ҵ�� ������ Ŭ���̾�Ʈ����
			// ���� ��ſ� IOCP�� �����Ű�� �۾��� �մϴ�.
			// �� ���� ������ ���� ��Ȯ�� ������ �����ϰ� �����Ƿ� Completion key ���� �� ���� ����ü�� �����͸� ���� ���� �ִ�.
			// �׷��� �����μ� �ռ� Accept �����忡��ó�� �� ��ȯ ���� �ٷιٷ� ����� �����մϴ�.

			CreateIoCompletionPort((HANDLE)lpClientContext->sockClnt, g_Server.hIocpWorkTcp, (DWORD)lpClientContext, 0);


			// �ٷ� �б⿡ ���� ��û�� �Ѵ�.
			// �̰��� �񵿱� �����̶�� �κп� ���� ������ �κ��Դϴ�.
			// ���ŷ ���Ͽ����� recv���� ��� ũ�������� �ݵ�� ���� �Ŀ� ����� �����Ǵ� �ݸ鿡
			// �񵿱� ������ �ϴ��� ������ ������ �ϸ� �Ŀ� ��� �̺�Ʈ�� �߻��Ͽ��� �� ó���ϴ� ����Դϴ�.
			// ���� ���⿡���� �տ����� ���� �̹� IOCP�� ���� ����ü�� ������ �����Ͽ��� ������
			// ��� �бⰡ �߻��ϸ� worker ������ ���� GetQueueCompletionStatus�� Ǯ���� ���̴�.
			PostTcpRecv(lpClientContext);


			// �� �Լ��� Ŭ���̾�Ʈ�κ��� ���ƿ� �����͸� �ؼ��մϴ�.
			// �Լ��� �̸� �״�� ��û�� �Դٴ� ���� �״� �Լ���
			// �׷��� ���� ť�� �װ��� ó���� ���μ������ �����Դϴ�.
			GameBufEnqueueTcp(lpClientContext);
		}

	}
	return 0;
}

void RecvTcpBufEnqueue(LPCLIENTCONTEXT lpSockContext, int iPacketSize)
{
	int iExtra;

	// ���� ���� ��Ŷ���� ���Ͽ� �ޱ� ���۸� �ʰ��ϴ� ���� �˻� (�������� ���� (�ּҰ� ����))
	iExtra = lpSockContext->pRecvEnd + iPacketSize - lpSockContext->RecvRingBuf - RINGBUFSIZE;

	if (iExtra >= 0)
	{
		// �ޱ� ���۸� �ʰ��Ѵٸ� �� �κ��� ������ �̵�
		CopyMemory(lpSockContext->RecvRingBuf, lpSockContext->RecvRingBuf + RINGBUFSIZE, iExtra);
		// ť�� ���� ��ġ ����
		lpSockContext->pRecvEnd = lpSockContext->RecvRingBuf + iExtra;
	}
	else
	{
		//ť�� ���� ��ġ ����
		lpSockContext->pRecvEnd += iPacketSize;
	}

#ifdef _LOGLEVEL1_
	printf( "RecvTcpBufEnqueue : %d, %d byte\n", lpSockContext->index, iPacketSize );
#endif
}

/*
���� ����

�� �κ��� IO �������� ���� �߿��� �κ��� �����ϰ� �ִ� ���̴�.
�� ���۵��� ��û�� ������ �Ǵ� �װͿ� ���� �������� ���۵� �����͵��� �߰���
���İ��� �κ����� �̰��� ���� ����� ������ �ϴ� �ܶ��Դϴ�.

������ �ּ�ȭ�� ���������� ����. ���� ���޹��� �����͵��� �� �������ݺ��� �ش��ϴ� �縸ŭ��
�ϳ��� ��ġ�μ� ó���մϴ�.

���� ä�ο� �����ϱ��� ���������� 100����Ʈ�� �����Ͷ�� �װ��� �ϳ��� ������ ó���ϴ� ���Դϴ�.

�׷��� �������� ���� �ϳ��� ��Ŷ�� ���ۿ� �����ϴ� ������ ����մϴ�.
������ ������� ���� �������� �䱸�� �ش��ϴ� ��Ŷ�� �����ϰ� �ؼ��ϴ� ���� ��κ��̹Ƿ�
�̷� ���� ������ ������ ���� CPU�ð��� ����ϴ� �κ� ���� �ϳ��� �� ���Դϴ�.
�̷��� ������ ���⼭�� �װ��� �ִ������� ���� �� �ִ� ������ ã�� ���� �� ���Դϴ�.

�����κ��� ���۵� �����͸� ���� �� �ֵ��� �񵿱� �Լ��� ȣ���ϴ� �κ��Դϴ�.
*/

void PostTcpRecv(LPCLIENTCONTEXT lpSockContext)
{
	WSABUF			wsaBuf;
	DWORD			dwReceived, dwFlags;
	int				iResult;

	// WSARecv �Լ��� ���� ���� ����
	dwFlags = 0;
	wsaBuf.buf = lpSockContext->pRecvEnd;// �������� ������ ���� �޴´�.
	wsaBuf.len = MAXPACKETSIZE;	// �̰������� �޾Ƶ��� �� �ִ� �ִ� ����� MAXPACKETSIZE �� �մϴ�
	// ���� �����͸� �޴� ��ġ�� �ش� �ޱ� ������ ��� ������ ��(End)�� ��ġ��Ű�� ���Դϴ�.
	// �����͸� �ޱ⿡ �ش��ϴ� ������ ����ü(eovRecv)�� ���ۿ� ���� ǥ�ú��� ���� ��� ������ �����鿡��
	// �ִ� ���� ��������μ� ���� �������� ó���� ����� �մϴ�.
	// WSARecv�� �񵿱� �Լ��̹Ƿ� ����� �� ���¿��� �� ȣ���� ������ �ٷ� �Ѿ��
	// �����Ͱ� ���۵Ǿ� ���� ��쿡�� �̺�Ʈ�� �߻��ϴ� ���Դϴ�.
	// �� ��쿡�� �о���� �����ʹ� �ش� ����ü�� recvEnd�� �������� �Ͽ� ���޵� ����Ʈ ����ŭ�� ����˴ϴ�.
	// �̷��� �� �Ŀ� �� �о���� �����Ϳ� ���Ͽ� ���۸� ���ϴ� �κ��� RecvTcpBufEnqueue�Դϴ�.


	iResult = WSARecv(lpSockContext->sockClnt, &wsaBuf, 1, &dwReceived, &dwFlags
		, (OVERLAPPED*)&lpSockContext->eovRecv, NULL);

	// PENDING�� ������ ������ ����
	if (iResult == SOCKET_ERROR && WSAGetLastError() != ERROR_IO_PENDING)
	{
		// never comes
		printf("***** PostTcpRecv error : %d, %d\n", lpSockContext->iKey, WSAGetLastError());
	}


}

/*
���μ��� ��ü�� ��û

�� �Լ��� ������ �����κ��� ���۹��� �����͸� ���� ������ ó���ϵ��� ���μ��� ��ü���� �Ѱ��ִ� ������ ó���ϴ� �Լ��Դϴ�. 
�׸��Ͽ� ���ڷδ� ���� ����ü�� ������ �� �ϳ����� ���ϴ� ���Դϴ�.

�� �����κ��� ���۹��� �����ʹ� WSARecv API�� ���� ������ RecvRingBuf �� ���̰� �ȴ�.
�̷��� ���� ��Ŷ�� ����ε� �ϳ��� �ϼ��Ͽ��� ��쿡��
������ �װ��� ó���ϵ��� ���μ��� ��ü�� ť�� ��û�Ѵ�.
��Ŷ�� �ϼ����� �ʾ��� ��쿡�� ���� ��� ���۹��� ��찡 �ƴϹǷ�
���� �� �����͸� ���۹��� �� ó���� ��û�ϴ� ���Դϴ�.
*/

/*
Mark������ �ʿ��� ������ TCP������ ��Ŷ ���� ������ ������� �ʴ´ٴµ� �ִ�.
��Ȯ�ϰԴ� �ϳ��� ��Ŷ�� ����� ���ļ� ������ ���۵ȴٴ� ���� �� �����Դϴ�.
��Ŷ ó���� �Ұ������� ��쿡�� ���� �� ������ �߻��Ͽ��� ���� ������ �Ѱܾ� �մϴ�.
�ٷ� �� �κп� mark�� ���˴ϴ�.

���� ���ʿ��� mark�� begin�� ���� ������ ���۵ȴ�.
�� �� �����͸� ������ ���� ��ó�� ���۹޾Ҵٰ� �ϸ� , �ϴ� �������� �� ���� ��Ŷ�� �����Ƿ�
�̰��� ���μ��� ��ü�� ó���ϵ��� ť�� ���� ���Դϴ�.

�� �� ��Ŷ�� ť�� �����鼭 �� ��° ��Ŷ�� ���ۺθ� �ٷ� mark�� ������ ����� �ȴ�.
�����͸� ���۹޾����� ���μ��� ��ü�� ��û���� ���� ���ο� ��Ŷ�� ���ۺΰ� �ٷ�
mark�� ���Ͽ� �� ��ġ�� ǥ���մϴ�.

�� ��Ȳ�� ��Ÿ���ϴ�.

�ϳ��� ���� �� �ϼ��� ��Ŷ�� ���� ������ �޴� ���̸�
�� �ٸ� �ϳ��� ���μ��� ��ü�� ó����Ų �� ���� ��Ŷ�� ó���ȴ�.
*/
void GameBufEnqueueTcp(LPCLIENTCONTEXT lpSockContext)
{
	short			iBodySize;
	int				iRestSize, iExtra;

	// ������ ǥ�õ� ��ġ�� ���۹��� �����Ϳ��� ���� ũ�⸦ ����
	// �� ���̴� �� �ΰ��� ��ġ�� ������ ���� �����Ƿ�(������ �̹Ƿ�)
	// ������ ��쿡�� RINGBUFSIZE�� �����ݴϴ�.
	// �� �Ÿ���� ���� ���� �� ������ ������ �ִ� ó���Ǿ�� �� �������� ���Դϴ�.
	// �� ���� ����� ũ�⺸�� �۴ٸ� ó���� ���� ���ٴ� ���� ���Ѵ�.

	iRestSize = lpSockContext->pRecvEnd - lpSockContext->pRecvMark;
	if (iRestSize < 0)
		iRestSize += RINGBUFSIZE;
	// ��Ŷ�� ���ũ�� ��ŭ�� ���۵��� ���פ���
	if (iRestSize < HEADERSIZE)
		return;


	// ���� �����͸� �ѹ� ���۹޾��� �� ���� ���� ��Ŷ�� ���ÿ� ���� ���� �� ������
	// while���� �̿��ؼ� ���� �����մϴ�.
	// ���� ����� �ϼ��� �ϳ��� ��Ŷ�� ã�� ���ؼ� �˻��ϴ� ������ ���ϴ� ���Դϴ�.
	// RecvMark�� ���� 2����Ʈ�� �ٵ� ��� �κ��� ���´�.
	// ���� �װ��� ��µ� �־ �� ���۰� ȸ���Ѵٸ� ���� ���ʿ��� �����ɴϴ�.
	// �׷� �Ŀ� �� ��Ŷ�� Ÿ���ϴٸ� Mark�� �װ����� �İ� �ָ鼭
	// ���ÿ� ���μ����� ť�� ����մϴ�.

	while (1)
	{
		// ǥ�õ� ��ġ�� �ޱ� ������ ������ ���� ����
		iExtra = lpSockContext->RecvRingBuf + RINGBUFSIZE - lpSockContext->pRecvMark;

		// ���̰� �ٵ��� ũ�⸦ ��Ÿ���� ������� ����
		if (iExtra < sizeof(short))
		{
			// ���ʿ��� 1����Ʈ�� ������
			*(lpSockContext->RecvRingBuf + RINGBUFSIZE) = *lpSockContext->RecvRingBuf;

		}
		// �ٵ� ũ�⸦ ����
		CopyMemory(&iBodySize, lpSockContext->pRecvMark, sizeof(short));

		// ��Ŷ�� ���� ũ�⸸ŭ ���۵��� ���פ���
		if (iRestSize < iBodySize + HEADERSIZE)
			return;

		// ǥ�ø� ������ ��Ŷ ��ġ�� �ű�
		lpSockContext->pRecvMark += iBodySize + HEADERSIZE;

		//ǥ�ð� ȸ���ϴ� ��츦 ���� �缳��
		if (lpSockContext->pRecvMark >= lpSockContext->RecvRingBuf + RINGBUFSIZE)
			lpSockContext->pRecvMark -= RINGBUFSIZE;

		// �˻��ؾ� �� ��ü�� ũ�⿡�� ���� �˻�� ��Ŷ�� ���� ũ�⸦ ����

		// ��Ŷ ó���� ���� ���μ��� ť�� ����
	}
}

