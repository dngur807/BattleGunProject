#include "Default.h"
#include "IO.h"
#include "Include.h"
#include "ServerProcess.h"

ONTRANSFUNC				g_OnTransFunc[MAXTRANSFUNC];

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
	CreateIoCompletionPort((HANDLE)g_Server.sockListener, g_Server.hIocpAcpt , 0, 0);


	// �ִ� ���� ����ŭ�� ����ü�� �̸� ����� ���´�.
	g_Server.sc = new CLIENTCONTEXT[g_Server.iMaxUserNum];
	if (g_Server.sc == NULL)
		return -1;

	// �ִ� ���� ����ŭ ��带 �����.
	g_Server.pn = new OBJECTNODE[g_Server.iMaxUserNum];

	// �ش� ���� ���� �ʱ�ȭ
	if (InitSocketContext(g_Server.iMaxUserNum) == -1)
		return -1;

	//Ŭ���̾�Ʈ ���� �޴� ������
	hThread = (HANDLE)_beginthreadex(NULL, 0, AcceptProc, NULL, 0, (UINT*)&itv);
	if (hThread == NULL)
		return -1;

// ����ڿ��� ������ ����� ���� ������� Worker ���� ��ŭ ��������
	for (int i = 0; i < g_Server.iWorkerTNum; ++i)
	{
		//hThread = (HANDLE)_beginthreadex(NULL , 0 , )
	}

	return 0;
}

int InitSocketContext(int maxUser)
{
	int itv;
	DWORD	dwReceived;

	LPCLIENTCONTEXT		lpClient = g_Server.sc;


	// ���� ���ؽ�Ʈ ����ü ��ü�� �ʱ�ȭ�մϴ�.
	ZeroMemory(lpClient, sizeof(CLIENTCONTEXT) * maxUser);

	for (int i = 0; i < maxUser; ++i)
	{
		// ���� ����ü ���� �ε����� �ο��մϴ�.
		lpClient[i].iKey = i;
		lpClient[i].eovSend.mode = SENDEOVTCP;
		lpClient[i].eovRecv.mode = RECVEOVTCP;

		// ���� �ʱ� ���� ����
		lpClient[i].pRecvMark  = lpClient[i].pRecvBegin = lpClient[i].pRecvEnd = lpClient[i].RecvRingBuf;
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


		// �⺻ ���μ���
		lpClient[i].iProcess = DEFAULTPROCESS;//�⺻ ���μ���
	}
	return 0;
}

// ���� �㰡 ������ �Դϴ�.
// Ŭ���̾�Ʈ�� �����ϴ� ������ �۵��� �� �ִ� �������Դϴ�.
UINT WINAPI AcceptProc(void* pParam)
{
	BOOL					IsResult;
	DWORD					dwTransferred;
	DWORD					CompletionKey;
	sockaddr_in				*pLocal;
	sockaddr_in				*pRemote;
	int						localLen;
	int						remoteLen;
	LPCLIENTCONTEXT			lpClientContext;//Ȯ��� ������ ����ü


	LPEOVERLAPPED			lpEov;//�̺�Ʈ�� ������ ��
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

/*
�۾� ������

������ �������� ������ ó���Ѵ�.
������ ���� �����͸� ���ۿ� �����ϰ� �װ��� ó���ϵ��� �䱸�ϸ�
���� �������� ������ �� �����͸� ������ ������ �����忡�� ó���Ѵ�.

�����δ� ��������� �Ǹ� �װ͵��� Ǯ���ϴ� �۾��� Ŀ�ο��� �Ѵ�.
������ ����ŭ�� �����带 �̸� �����Ͽ� ���� �Ŀ� ������ ��쿡�� ��� ���·� �ִٰ�
��� �۾��� ��û ���� ���� �� ���� �ϳ��� Ȱ��ȭ�Ǿ� �� �۾��� ó���ϴ� ���Դϴ�.

�̹� ��� �����尡 Ȱ��ȭ�Ǿ� �ִٸ� ������ ��û�Ǵ� �۾��� �� ������� �߿� ��� ����
��� ������ ������ ��ٸ��� �ִ�. ���� ��� �����带 ����ϰ� ������
�������� ������·� ���Ͽ� �� �� ����.
*/
UINT WINAPI WorkerProc(void* pParam)
{
	DWORD			dwTransferred;
	LPEOVERLAPPED	lpEov;
	LPCLIENTCONTEXT lpSockContext;
	BOOL			bResult;

	while (1)
	{
		// ������ ���� �����͸� �ްų� �������� ��������
		// ���� �������� �Ϸᰡ �Ͼ ������ ���
		// ��� ��û�� ���� �� �� �۾����� ���Ͽ� ���۹��� �������� ���
		// Accept �ÿ� IOCP�� ������ ��
		// Completion key�� �Ѱ��־��� �ش� ���� ����ü�� ������ , �� �۾����� ���Ǿ��� ������ ����ü�� �����͸� ���� �� �ִ�.
		bResult = GetQueuedCompletionStatus(g_Server.hIocpWorkTcp, &dwTransferred,  (LPDWORD)&lpSockContext
			, (LPOVERLAPPED*)&lpEov, INFINITE);

		if (lpEov != NULL)
		{
			// ���� �Ѱ��� ������ ����ü�� NULL �� �ƴϸ鼭
			// ó���� ����Ʈ ���� 0�̶�� �װ��� ������ ��� ������������  ���� ����

			if (dwTransferred == 0)
			{
				//������ ������ ��û LOGOUT
			}
			else
			{
				// �Ѿ�� ������ ����ü�� �����ʹ� ��� �۾��� ��û�� �� ���ڷ� �־����� ���̴�.
				// �̷��� ��û�� ������ ������ ���Ͽ� �ϰ� �ǹǷ� ���� ����ü �ȿ� ����� ���ԵǾ� �ִ� ���Դϴ�.
				// ������ ����� ���� �� ������ ����ü�� ��� �۾��� ��������� ��Ÿ���� ���Ͽ� ���� ���� ������ �ִ� Ȯ��� ������ ���� ����մϴ�.
				// Ŭ���̾�Ʈ�� ���� ���� �����Ͱ� ������ �Ϳ� ���� �̺�Ʈ�� Ŭ���̾�Ʈ�� ���� �������� �����Ⱑ �Ϸ�� �̺�Ʈ�� ���� �����
				// ��� �� �۾� ������� �Ѿ���� ������ �߻��� �̺�Ʈ�� � �Ϳ� ���� ����ΰ��� �����ϱ� ���Ͽ� ���˴ϴ�.


				// �б���� ������ ����ü���� ��ȣ
				if (lpEov->mode == RECVEOVTCP)
				{
					//�̺�Ʈ�� WSARecv�� ���Ͽ� �߻��� ���̶��
					// RecvTcpBufEnque�Լ��� �̿��Ͽ� �� �������� ť�� �ִ� �۾��� �մϴ�.
					// �б� ���� ��ġ ����
					RecvTcpBufEnqueue(lpSockContext, dwTransferred);

					// ��Ŷ ó���� ��û
					GameBufEnqueueTcp(lpSockContext);

					// ������ �б⸦ ���� �񵿱� �б⸦ �õ�
					PostTcpRecv(lpSockContext);
				}
				else if (lpEov->mode == SENDEOVTCP)
				{
					// �̺�Ʈ�� WSASend�� ���Ͽ� �߻��� ���̶�� ���� �Ѱ��� PostTcpSendRest �Լ����� ȣ���մϴ�.
					// �� �Լ��� ������ ���ۿ� ���� ������ �����͵��� �����ϴ� ������ �մϴ�.
					// ���� ���ʿ� WSASend�� ȣ���Ѵٸ� �װ��� �����⸦ ���ߴ� ��ŭ�� ũ���� ������ �Ϸ� �ǰų�
					// TCP�� Ư������ �Ϻθ� ���۵Ǿ��� �� �ִ� ���Դϴ�.
					// �׷��� ������ �ϴ� ���ۿ� ���� �� / ����� �Ϸ�Ǹ� �����⸦ ���ϴ� ����Ʈ ���� ������ ����Ʈ ����
					// �˻��ϴ� �׸��� �ʿ��ϸ� �װ��� �ٷ� postSnedRest�Լ���.

					//������ �� ���� �� �ִٸ� �������� �������� ��û
					PostTcpSendRest(lpSockContext, dwTransferred);
				}
			}

		}
	}
	return 0;
}

void RecvTcpBufEnqueue(LPCLIENTCONTEXT lpSockContext, int iPacketSize)
{
	int iExtra;

	// ���� ���� ��Ŷ���� ���Ͽ� �ޱ� ���۸� �ʰ��ϴ� ���� �˻� (�������� ���� (�ּҰ� ����))
	iExtra = lpSockContext -> pRecvEnd + iPacketSize - lpSockContext->RecvRingBuf - RINGBUFSIZE;


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
	printf( "RecvTcpBufEnqueue : %d, %d byte\n", lpSockContext->iKey, iPacketSize );
#endif
}

/*
ť�� �ִ� ��Ŷ�� �����ִ� �Լ��Դϴ�.
RecvTcpBufEnqueue�Լ��� ���۵� �����͸� ������ RingBuf ���ۿ� ������� �״� �ݸ鿡
�� �Լ��� �ѹ��� ȣ��� �ϼ��� �ϳ��� ��Ŷ�� ��� �κ��Դϴ�.
������ ������ Ÿ���� ��Ŷ���� �����Ƿ� �װ��� ������ ó���� �� �� �ִ� ���̴�.
�����ϰ� �ϳ����� �����͸� ���� �� �ִ� �� �Լ��� ���Ͽ� ������ ������ �߿��� ������ Ȯ���Ѵ�.

RecvRingBuf ���۴� ����� ũ�� �����ϴ� �� �־ Begin�� �ѹ����� ���ƿ� End�� ���� ���� ���� �ʴ´�.

�� �ΰ��� ������ ���⼭ ����� ������� ���۸� ������ �� ���� �߿��� �����Դϴ�.
RingBuf��� ���� �����κ��� ���۵� �����͵��� ����Ǵ� ���Դϴ�. ���۵� �����͸� ��� ó���ϱ� ����
�� �ٸ� �����Ͱ� ���۵Ǿ� �� �� �����Ƿ� �� ���۴� ����� Ŀ�߸� �ϴ� ���Դϴ�.

���� ��  ��° ������ ���ؼ��� ���۰� ����� Ŀ�� �� �ʿ䰡 �ִ� ���Դϴ�.
���۹��� �����ʹ� ���ۿ��� ��밡���� ���� ��Ÿ���� End �κп� ���̸� �����͸� ������ ���� Begin �κп��� ������.


�߿��� ���� ���⼭ ������ ��� ǥ���� �����δ� �������ٴ� ���� �ƴ϶�� ���̴�.
�߿��� �ǹ̷� ���⼭�� �������ٶ�� ���� �ܼ��� �� �������� �����͸� �������ٶ�� ���Դϴ�.
�� �κп��� �ѹ��� ���縦 ���̴� ���� ���������� ���Դϴ�.
�ܼ��� �� ��ġ�� �����͸��� �������� �����ν� ���� �����ʹ� RecvTcpRingBuf ��  �ִ� ���� ������ ���� �Ǵ� ���Դϴ�.
�׷��� ������ ��ó �� �����Ͱ� ó���Ǳ� ���� �ٸ� �����Ͱ� ���� �������� ��츦 ���� ���۰� ����� Ŀ���մϴ�.
*/

/*
�� �Լ��� ���ڷδ� �ش� ������ ���� ����ü(lpsockcontext)�� �ϳ��� ��Ŷ�� ��ġ�� ���� ���� ������(cpPacket)�� �ϳ��� ��Ŷ�� ũ�⸦
���� ������ ������ �̴�. ������ �� ���� ������ ���� �տ��� ���Ѵ�� �̰͵��� ��ġ�� ũ�⸸�� �޴� �κ��̶�� ���� ���� ���̴�.

��Ŷ�� �̰��� ó���� ������ �Ҹ���� �ʴ´ٴ� ������ �Ͽ����Ƿ�
�����ϴ� �������� ��ġ�� �ѱ�� ���� �����մϴ�.
�� ������ �ٵ��� ũ�⸦ �˱� ���Ͽ� �� ũ�⸦ ��Ÿ���� 2����Ʈ�� �б� ���������� �˻��ϴ� �κ��Դϴ�.
�� 2����Ʈ ��迡�� �� ���۰� ȸ���� �� �ֱ� �����̴�. �׷��� �� �� 2����Ʈ�� �ٵ� ũ�⸦ ��Ÿ���� ���� ��´�.
�׷� �Ŀ� �̹����� ���� �ٵ� �ش��ϴ� �����͸� ��� ���� �۾��� �մϴ�.

�װ��� �� ���۰� ȸ���ϴ� ��ġ�� �ִ°��� �˻��Ͽ� , �� ������ ���� �տ��� Ư�� ũ�⸦ 
���� �������� �����ϴ� ������ �����Ѵ�. �׷� �Ŀ��� ��Ŷ�� �б� ���� �������� ����� �� ��ŭ�� �ڷ� �ű�� ���Դϴ�.
*/

void RecvTcpBufDequeue(LPCLIENTCONTEXT lpSockContext, char **cpPacket, int *iPacketSize)
{
	short iBodysize;
	int	  iExtra;

	*cpPacket = lpSockContext->pRecvBegin;
	// �� ��ġ�� ������ ���������� ���̸� ����(begin�� ��ġ�� ������ ������ ���̸� ���ϴ� ���Դϴ�.)
	iExtra = lpSockContext->RecvRingBuf + RINGBUFSIZE - lpSockContext -> pRecvBegin;

	// ���� ���̰� �ٵ��� ũ�⸦ ��Ÿ���� ������� ������
	if (iExtra < sizeof(short))
	{
		// ���ʿ��� 1����Ʈ�� ������ ( �ǳ� �����Ͱ� �Ǿ��� �����͸� ����Ű�� �մϴ�.)
		*(lpSockContext->RecvRingBuf + RINGBUFSIZE) = *lpSockContext->RecvRingBuf;
	}
	// �ٵ��� ũ�⸦ ��´�.
	CopyMemory(&iBodysize, lpSockContext->pRecvBegin, sizeof(short));

	// �ٵ��� �߰��� ������ ���� ��ħ
	if (iExtra <= iBodysize + HEADERSIZE)
	{
		// ���ʿ��� �ٵ��� ������ ������ �����ؿ´�.
		CopyMemory(lpSockContext->RecvRingBuf + RINGBUFSIZE, lpSockContext->RecvRingBuf, iBodysize + HEADERSIZE - iExtra);
		// ��Ŷ�� ������ ���� ��ġ ����
		lpSockContext->pRecvBegin = lpSockContext->RecvRingBuf + iBodysize + HEADERSIZE - iExtra;
	}
	else
	{
		//��Ŷ�� ������ ���� ��ġ ����
		lpSockContext->pRecvBegin = lpSockContext->pRecvBegin + iBodysize + HEADERSIZE;
	}

	//���� ��Ŷ ũ�� ����
	*iPacketSize = iBodysize + HEADERSIZE;
#ifdef _LOGLEVEL1_
	printf( "RecvTcpBufDequeue : %d, %d byte\n", lpSockContext->index, *iPacketSize );
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
		printf("***** PostTcpRecv error : %d, %d\n", lpSockContext->iKey, WSAGetLastError());
	}


}
/*
�� �Լ��� �׷쿡 ���ϴ� �����鿡�� ������ �� �� ����ϴ� �Լ��Դϴ�.
iBegin�� �ش� �׷쿡���� ù��° ������ ���� �ε����� �Ǵ� ���Դϴ�.
*/

/*
�����鿡�� �����͸� �����ϴ� ������ �־ SendRingBuf��� �����۸� ����մϴ�.
�⺻���� � ����� �����͸� ���۹��� �� ����Ͽ��� RecvRingbuf������ ����� �����ϴ�
�Ѱ��� ���̶�� ���⿡���� �ϳ��� ��Ŷ�� �����ϱ� ���� ������ ���ʿ��ϴٴ� ���̴�.


�̰� ���� TCP�� Ư���� ����ϴ� ���̴�. �������� ����ǹǷ� �װ��� �ѹ��� ������ �� ���� ������ ������ ���� �Ǵ� ���̴�.
*/

void PostTcpSend(int ibegin, char *cpPacket, int iPacketSize)
{
	LPCLIENTCONTEXT			lpSockContext;
	WSABUF					wsaBuf;
	DWORD					dwSent;
	int						iResult, iExtra, iSendNow;
	int						iNext;

	iNext = ibegin;

	// �� �̻� ������ �Ǿ� ���� ���� �� ���� �ݺ�(�׷쿡 �����ִ� ���� ������)

	while (iNext != NOTLINKED)
	{
		//�ش� �ε����� ���� ���� ���ؽ��� ��´�.
		lpSockContext = &g_Server.sc[iNext];

		EnterCriticalSection(&lpSockContext->CS);
		//���� �����͸� ������ ���ۿ� ����
		CopyMemory(lpSockContext->pSendEnd, cpPacket, iPacketSize);
		// ���� �����Ͱ� ������ ������ �������� �Ѿ�°��� �˻�
		iExtra = lpSockContext->pSendEnd + iPacketSize - lpSockContext->SendRingBuf - RINGBUFSIZE;

		if (iExtra >= 0)
		{
			CopyMemory(lpSockContext->SendRingBuf, lpSockContext->SendRingBuf + RINGBUFSIZE, iExtra);
			// ������ ������ �����͸� �״� ��ġ ����
			lpSockContext->pSendEnd = lpSockContext->SendRingBuf + iExtra;
		}
		else
		{
			lpSockContext->pSendEnd += iPacketSize;
		}

		// IOCP�� Ư������ ����ȭ�� ������ �ִ� �κ��̴�.
		// �����鿡�� �����͸� �����ϱ� ���Ͽ� WSASend�� �ϰ� �Ǿ��� ��
		// �� ������ �Ϻε� , ��ü�� �Ϸ�Ǿ��� �� IOCP�� �ѹ��� �̺�Ʈ�� �߻��մϴ�.
		// �� �̺�Ʈ�� �߻��Ͽ��� �� �װ��� ��ΰ� ���۵Ǿ��ٸ� �ϷḦ,
		// �׷��� �ʴٸ� �߰����� ������ �ϴ� ���Դϴ�.
		// ������ �׷��� ������ �䱸�ϴ� , �� PostTcpSend �Լ��� ȣ���ϴ� �������
		// WSASend�� �Ϸ�Ǿ��� ���� �̺�Ʈ�� �޴� ������� �ٸ��ٴ� �� �ִ�.
		// �� ���� ���� ���� ����ü�� ���� ������ �ϱ� ������ ����ȭ�� �ݵ�� �ʿ��մϴ�.


		// ��Ȯ�ϰԴ� �����͸� �����ϱ� ���� (PostTcpSend)������� ���μ��� ��ü���� �Ҵ�� �����尡 , 
		// PostTcpSendRest ������� IOCP�� �۾� �����忡 ���ؼ� ȣ��ȴ�.


		// �����߸� �ϴ� ��ü ũ�� ����
		lpSockContext->iSTRestCnt += iPacketSize;
		// ������ �ϴ� ��ü ���� ���� ��û�� ���� ������ �˻�
		iSendNow = (lpSockContext->iSTRestCnt == iPacketSize) ? TRUE : FALSE;



		// ���⼭�� �������� �����͸� ������ ��û�� ���� WSASend �۾��� ���� �ϰ� ����
		// �����͸��� ť�������� �����ϴ� �÷��׿� ���� ���� ��� �κ��̴�.
		// �̹� ������ ���� �����Ͱ� SendRingBuf���ۿ� ����Ǿ���
		// SendRestCnt�� ���� �����⸦ ��û���� iPacketSize ��ŭ
		// ������ �����̹Ƿ� ť���۾� ��ü�� �Ϸ�� ���°� �˴ϴ�.
		// �׷��� ���¿��� iSendRentCnt�� ���� iPacketSize�� ���ٸ� ��û , 
		// ó���ؾ� �� �����Ͱ� �������� ���� ���¿��� �ش��ϴ� ��û�� �ٷ� ���� WSASend�� �������� �˴ϴ�.
		// ���� �׷��� ��쿡 iSendNow��� ���� ������ ����� ������ ������ ��� �����ϵ���
		// WSASend�� ȣ���ϴ� ���̴�.

		// �̹� ������ �������� �ƴϾ����Ƿ� 
		// ���� ��û�� ���� ������ ����


		if (iSendNow)
		{
			// ������ ���� ����
			wsaBuf.buf = lpSockContext->pSendBegin;
			wsaBuf.len = iPacketSize;

			iResult = WSASend(lpSockContext->sockClnt, &wsaBuf, 1, &dwSent
				, 0, (OVERLAPPED*)&lpSockContext->eovSend, NULL);

			// PENDING�� ������ ����
			if (iResult == SOCKET_ERROR && WSAGetLastError() != ERROR_IO_PENDING)
			{
				printf("***** PostTcpSend error : %d, %d\n", lpSockContext->iKey, WSAGetLastError());
			}
		}
		// �̹� �ٸ� �����Ⱑ ��������
		else
		{
			// ������ ���۰� �� á�� ��
			if (lpSockContext->iSTRestCnt > RINGBUFSIZE)
			{
				printf("*********** tcp sendbuf overflow!(%d) : %d ***********\n", lpSockContext->iKey, lpSockContext->iSTRestCnt);
			}
		}
		//���� ��ũ�� �ѱ�
		iNext = g_Server.pn[iNext].next;
		LeaveCriticalSection(&lpSockContext->CS);
	}
}

/*
�� �Լ��� ���õ� �����鸸�� ������� �����͸� ������ �� ���ȴ�.

ù ��° ���ڰ� �ٷ� ���õ� �������� ��
�� ��° ���ڰ� ���õ� �������� �ε����� ���� �迭 ����Ʈ
*/
void PostTcpSend(int iUserNum, int iSockAddr[], char *cpPacket, int iPacketSize)
{
	LPCLIENTCONTEXT			lpSockContext;
	WSABUF					wsaBuf;
	DWORD					dwSent;
	int						iResult, iExtra, iSendNow;

	for (int i = 0; i < iUserNum; ++i)
	{
		lpSockContext = (LPCLIENTCONTEXT)(iSockAddr[i]);

		// ��Ŷ ī��
		CopyMemory(lpSockContext->pSendEnd, cpPacket, iPacketSize);

		// ������ ��ġ �̵�
		iExtra = lpSockContext->pSendEnd + iPacketSize - lpSockContext->SendRingBuf - RINGBUFSIZE;
		if (iExtra >= 0)
		{
			CopyMemory(lpSockContext->SendRingBuf, lpSockContext->SendRingBuf + RINGBUFSIZE, iExtra);
			lpSockContext->pSendEnd = lpSockContext->SendRingBuf + iExtra;
		}
		else
		{
			lpSockContext->pSendEnd += iPacketSize;
		}

		EnterCriticalSection(&lpSockContext->CS);
		lpSockContext->iSTRestCnt += iPacketSize;
		iSendNow = (lpSockContext->iSTRestCnt == iPacketSize) ? TRUE : FALSE;
		LeaveCriticalSection(&lpSockContext->CS);

		if (iSendNow)
		{
			wsaBuf.buf = lpSockContext->pSendBegin;
			wsaBuf.len = iPacketSize;

			iResult = WSASend(lpSockContext->sockClnt, &wsaBuf, 1, &dwSent,
				0, (OVERLAPPED*)&lpSockContext->eovSend, NULL);

			if (iResult == SOCKET_ERROR && WSAGetLastError() != ERROR_IO_PENDING)
			{
				printf("***** PostTcpSend error : %d, %d\n", lpSockContext->iKey, WSAGetLastError());
			}
		}
		else
		{
			if (lpSockContext->iSTRestCnt > RINGBUFSIZE)
			{
				/* it's not disconnection case */
				printf("*********** tcp sendbuf overflow!(%d) : %d ***********\n", lpSockContext->iKey, lpSockContext->iSTRestCnt);
			}
		}

	}
}

/*
�� �Լ��� �������� ������ �ϴ� �����͵� �� ���� ������ ���� �������� ���� ������ �ϴ� �κ��Դϴ�.
*/
/*
�� �Լ��� ȣ��Ǿ��� ���� iTransferred��ŭ�� ���۵� ����Ʈ ���� ������
�� ��ŭ�� ���ۿ��� ����� �κ��� �� �Լ����� ���ʿ� �ϴ� �۾��̴�.

�׷� �Ŀ� postTcpSend�Լ����� ������ iStRectCnt���� ���۵� iTransferred ũ�� ��ŭ��
���� ������ �Ӱ迵�� ������ �ϰ� �ȴ�.

�̷��� ���۵� �Ŀ��� ������ �� �����ִ� �����Ͱ� �����ϸ� �װ���
iRestSize��� ���� ������ �ִ� ���Դϴ�. iRestSize�� 0���� ū ���� �ִٸ� �߰�������
������ �� �����Ͱ� �ִٴ� ���̵ȴ�.

�� �� iSTRestCnt��� ���� ���� ������ iRestSize�� �ִ� ������ �߿��ϴ�.
���� ������ ������ ���� �Ҵ�� ������ ����ϰ� �ǹǷ�
�� ���� �����忡 ������ ���� �Ǵ� ���Դϴ�.
�׷� ������ iRestSize�� ���� �Ҵ��� �� , �Ӱ迵���� �����Ͽ���
������ �׼����� ������ ���̴�. �Ӱ迵�� ���� ������ �����̹Ƿ�
postTcpSend �Լ� ���� ���ÿ� �۾��� �����ϴٴ� ���� �� �� �ִ�.

iRestSize�� 0���� ū ���� �ִٸ� ���� �߰������� ������ �ϴ� �����Ͱ� �����Ƿ�
WSASend�� ȣ���Ѵ�. �̶��� �ռ� ���� �Ͱ� ���� ��Ŷ ������ ������
�� �ʿ䰡 �����Ƿ� �װ��� ������������ �ѹ��� ��� ������ �� �ִ�.
*/
void PostTcpSendRest(LPCLIENTCONTEXT lpSockContext, int iTransferred)
{
	WSABUF				wsaBuf;
	DWORD				dwSent;
	int					iResult, iRestSize, iExtra;

	// �Ϸ�� �縸ŭ ������ �����ϴ� ������ ��ġ�� �ڷ� �ű�
	iExtra = lpSockContext->pSendBegin + iTransferred - lpSockContext->SendRingBuf - RINGBUFSIZE;

	if (iExtra >= 0)
	{
		lpSockContext->pSendBegin = lpSockContext->SendRingBuf + iExtra;
	}
	else
	{
		lpSockContext->pSendBegin += iTransferred;
	}
	EnterCriticalSection(&lpSockContext->CS);
	// �������ϴ� �ѷ��� ���۵� �縸ŭ ����
	lpSockContext->iSTRestCnt -= iTransferred;
	// ���� ������ �ϴ� ���� �ִ°�??
	iRestSize = lpSockContext->iSTRestCnt;
	LeaveCriticalSection(&lpSockContext->CS);

	// �߰������� �����ؾ� �� ���� ����
	if (iRestSize == 0)
	{

	}
	// �߰������� �����ؾ� �� ���� ����
	else if (iRestSize > 0)
	{
		// ������ �ִ��� ���� �ѹ��� �����Ϸ��� ���
		if (iRestSize > MAXPACKETSIZE) iRestSize = MAXPACKETSIZE;

		iExtra = lpSockContext->SendRingBuf + RINGBUFSIZE - lpSockContext->pSendBegin;

		// ���۰� ȸ���Ѵٸ� �տ��� �������� �����ؿ�
		if (iExtra < iRestSize)
		{
			CopyMemory(lpSockContext->SendRingBuf + RINGBUFSIZE, lpSockContext->SendRingBuf, iRestSize - iExtra);
		}

		// ��Ŷ ����
		wsaBuf.buf = lpSockContext->pSendBegin;
		wsaBuf.len = iRestSize;

		iResult = WSASend(lpSockContext->sockClnt, &wsaBuf, 1, &dwSent
			, 0, (OVERLAPPED*)&lpSockContext->eovSend, NULL);

		if (iResult == SOCKET_ERROR && WSAGetLastError() != ERROR_IO_PENDING)
		{
			// kick
			printf("***** PostTcpSendRest error : %d, %d\n", lpSockContext->iKey, WSAGetLastError());
		}
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
	// �׷� �Ŀ� �� ��Ŷ�� Ÿ���ϴٸ� Mark�� �װ����� �Ű� �ָ鼭
	// ���ÿ� ���μ����� ť�� ����մϴ�.

	while (1)
	{
		// ǥ�õ� ��ġ�� �ޱ� ������ ������ ���� ����
		iExtra = lpSockContext -> RecvRingBuf + RINGBUFSIZE - lpSockContext->pRecvMark;

		// ���̰� �ٵ��� ũ�⸦ ��Ÿ���� ������� ����
		if (iExtra < sizeof(short))
		{
			// ���ʿ��� 1����Ʈ�� ������
			*(lpSockContext->RecvRingBuf + RINGBUFSIZE) = *lpSockContext->RecvRingBuf;
		}
		// �ٵ� ũ�⸦ ����
		CopyMemory(&iBodySize, lpSockContext->pRecvMark, sizeof(short));

		// ��Ŷ�� ���� ũ�⸸ŭ ���۵��� ����
		if (iRestSize < iBodySize + HEADERSIZE)
			return;

		// ǥ�ø� ������ ��Ŷ ��ġ�� �ű�
		lpSockContext->pRecvMark += iBodySize + HEADERSIZE;

		//ǥ�ð� ȸ���ϴ� ��츦 ���� �缳��
		if (lpSockContext->pRecvMark >= lpSockContext->RecvRingBuf + RINGBUFSIZE)
			lpSockContext->pRecvMark -= RINGBUFSIZE;

		// �˻��ؾ� �� ��ü�� ũ�⿡�� ���� �˻�� ��Ŷ�� ���� ũ�⸦ ����
		iRestSize -= iBodySize + HEADERSIZE;
		// ��Ŷ ó���� ���� ���μ��� ť�� ����
		g_Server.ps[lpSockContext->iProcess].GameBufEnqueue(lpSockContext);

#ifdef _LOGLEVEL1_
		printf( "GameBufEnqueueTcp : %d\n", lpSockContext->iKey );
#endif
		if (iRestSize < HEADERSIZE) 
			return;
	}
}

