#include "Default.h"
#include "IO.h"
#include "Include.h"
#include "ServerProcess.h"

ONTRANSFUNC				g_OnTransFunc[MAXTRANSFUNC];

/*
여기서는 IOCP를 사용합니다. 입 / 출력 완료 포트라 부르는 IOCP는 읽거나 쓰기 중 
그 어떠한 작업이 완료 되었을 시점에 이벤트가 발생합니다.
따라서 그 시점에 적절한 작업을 취해줄 수 있는 것이다.
그러므로 IOCP를 사용하는 데에는 멀티 쓰레드가 필수로 요구됩니다.

이것은 보통 비동기 소켓과 함께 사용합니다.

WSARecv와 WSASend가 각각 데이터를 받을 경우와 보낼 경우에 사용되며 여기에 윈도우 에만 특화되어
확장된 함수인 AcceptEx와 GetAcceptExSockaddr를 사용한다.

AcceptEx 는 소켓을 미리 생성한 후 새로운 접속이 이뤄졌을 때 그 소켓을 사용하는 방법입니다.
그 특징으로는 이러한 접속이 이루어 지는 단계는 클라이언트가 connect를 사용하여 접속한 후 첫 번째 데이터 블록이 전송되었을 때가 된다는 것이다.
그러므로 첫 번째 블록을 보내지 않고 Connect만을 하는 경우는 서버측에서는 접속으로 인식하지 않는다.
완전하게 접속이 이뤄졌을 때는 Acceptex의 인자에 주어진 변수들로 자신과 상대방의 주소가 함께 넘어온다.
이것들은 GetAcceptExSockaddrs API를 사용하여 적절하게 해석을 할 수 있다.
*/

int InitIO()
{
	WSADATA			wsa;
	SOCKADDR_IN		addr;
	HANDLE			hThread;
	int				itv;

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return -1;

	//  클라이언트의 접속을 받기위한 리슨 소켓 생성
	// 오버랩드 옵션을 사용하기 위해서 넌블로킹 모드 소켓
	g_Server.sockListener = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (g_Server.sockListener == INVALID_SOCKET)
		return -1;

	//리슨 포트와 바인딩
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(PORTNUMBER);

	itv = bind(g_Server.sockListener, (sockaddr*)&addr, sizeof(addr));
	if (itv == SOCKET_ERROR)
		return -1;

	// 접속 대기
	itv = listen(g_Server.sockListener, 512);
	if (itv == SOCKET_ERROR)
		return -1;

	// 클라이언트와 실제 통신을 위한 IOCP 생성
	g_Server.hIocpWorkTcp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	if (g_Server.hIocpWorkTcp == NULL)return -1;

	// 유저 접속을 받아 드릴때 사용합니다.
	g_Server.hIocpAcpt = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	if (g_Server.hIocpAcpt == NULL)return -1;


	// 리슨 소켓을 IocpAcpt에 연결합니다.
	// 따라서 이벤트가 발생하면 바로 클라이언트가 접속한 때라는 것을 알 수 있습니다.
	CreateIoCompletionPort((HANDLE)g_Server.sockListener, g_Server.hIocpAcpt , 0, 0);


	// 최대 유저 수만큼의 구조체를 미리 만들어 놓는다.
	g_Server.sc = new CLIENTCONTEXT[g_Server.iMaxUserNum];
	if (g_Server.sc == NULL)
		return -1;

	// 최대 유저 수만큼 노드를 만든다.
	g_Server.pn = new OBJECTNODE[g_Server.iMaxUserNum];

	// 해당 유저 정보 초기화
	if (InitSocketContext(g_Server.iMaxUserNum) == -1)
		return -1;

	//클라이언트 접속 받는 쓰레드
	hThread = (HANDLE)_beginthreadex(NULL, 0, AcceptProc, NULL, 0, (UINT*)&itv);
	if (hThread == NULL)
		return -1;

// 사용자와의 데이터 통신을 위한 쓰레드는 Worker 개수 만큼 생성하자
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


	// 소켓 컨텍스트 구조체 전체를 초기화합니다.
	ZeroMemory(lpClient, sizeof(CLIENTCONTEXT) * maxUser);

	for (int i = 0; i < maxUser; ++i)
	{
		// 유저 구조체 별로 인덱스를 부여합니다.
		lpClient[i].iKey = i;
		lpClient[i].eovSend.mode = SENDEOVTCP;
		lpClient[i].eovRecv.mode = RECVEOVTCP;

		// 버퍼 초기 상태 설정
		lpClient[i].pRecvMark  = lpClient[i].pRecvBegin = lpClient[i].pRecvEnd = lpClient[i].RecvRingBuf;
		lpClient[i].pSendBegin = lpClient[i].pSendEnd = lpClient[i].SendRingBuf;


		// 클라이언트와의 통신을 위한 소켓 생성
		lpClient[i].sockClnt = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
		if (lpClient[i].sockClnt == INVALID_SOCKET)
			return -1;


		// AcceptEx는 미리 소켓을 생성하여 인자로 받고 접속이 완료되면 CompletionPort객체나
		// 인벤트 또는 콜백 함수로 알려준다.
		// 접속시 부하를 줄일 수 있다.

		itv = AcceptEx(g_Server.sockListener, lpClient[i].sockClnt, lpClient[i].pRecvEnd
			, MAXPACKETSIZE, sizeof(sockaddr_in) + 16, sizeof(sockaddr_in) + 16
			, &dwReceived, (OVERLAPPED*)&lpClient[i].eovRecv);

		if (itv == FALSE  &&  GetLastError() != ERROR_IO_PENDING)
			return -1;

		// SendBuffer를 위한 임계영역을 생성한다.
		InitializeCriticalSection(&lpClient[i].CS);


		// 기본 프로세스
		lpClient[i].iProcess = DEFAULTPROCESS;//기본 프로세스
	}
	return 0;
}

// 접속 허가 쓰레드 입니다.
// 클라이언트와 반응하는 모든것을 작동할 수 있는 시작점입니다.
UINT WINAPI AcceptProc(void* pParam)
{
	BOOL					IsResult;
	DWORD					dwTransferred;
	DWORD					CompletionKey;
	sockaddr_in				*pLocal;
	sockaddr_in				*pRemote;
	int						localLen;
	int						remoteLen;
	LPCLIENTCONTEXT			lpClientContext;//확장된 오버랩 구조체


	LPEOVERLAPPED			lpEov;//이벤트가 들어오는 곳
	while (1)
	{
		// 리슨 소켓과 연결된 IOCP에 접속이 들어올 때까지 대기한다.
		IsResult = GetQueuedCompletionStatus(g_Server.hIocpAcpt, &dwTransferred , (LPDWORD)&CompletionKey
			, (LPOVERLAPPED*)&lpEov, INFINITE);

		// dwTransferred는 전송된 바이트 수이다.
		// 이것은 읽기에 의하여 발생한 것이라면 읽어들인 바이트 수를
		// 쓰기에 의하여 발생한 것이라면 전송한 바이트의 수를 가리킨다.
		// &dummy는 Completion key이다.

		// CreateIOCompletionPort를 이용하여 소켓과 
		// IOCP를 연결할 때 바로 이 Completion key 전달한다.
		// 따라서 이것을 하나의 구분값으로서 사용 할 수 있다
		
		// 사실 이 Accept 쓰레드에서는 이 값이 무의미 하다. 전달될 값이 존재하지 않기 때문 입니다.
		// 이 쓰레드에서의 이벤트라는 것은 서버의 리슨 소켓에 연결된 것이기 때문에
		// 클라이언트와 연관이 없기 때문이다.
		// 그러한 이유로 InitIo 부분에서 리슨 소켓과 IOCP를 연결할 때 0으로만 호출한 것이다.

		// &lpEov는 오버랩 구조체의 포인터 형이다.
		// 기것은 바로 입 , 출력에 사용되었던 오버랩 구조체의 주소를 넘겨주는 부분이다.
		// 모든 것이 비동기로 작동하게 되므로 어떠한 작업이 일어날 때는 항상 오버랩 구조체를 사용하며
		// 그에 따라 주소가 전달된다.


		// AcceptEx는 클라이언트가 접속한 후에 첫 번째 데이터 블록이 전송되어야만 
		// 서버에서 접속으로 인식한다.
		// 중요한 것은 이렇게 첫 번째 블록이 넘어오는 것 또한 네트워크 전송이므로
		// AcceptEx를 호출할 당시에도 오버랩 구조체를 사용한다는 것이다.
		// 따라서 GetQueueCompletionStatus에서 얻는 것은 첫 번째 읽어들인 첫 번째 데이터 블록의 양 ,
		// 두 번째 Completion key 언제나 0 (앞에서의 연결 과정에서 항상 0을 주었기 때문입니다.)
		// 세 번째는 AcceptEx에 인자로 주었던 그 확장 오버랩 구조체의 주소입니다.
		// 이 이벤트가 발생한 시점에서는 클라이언트와의 접속을 위하여
		// 할당된 소켓과 또 그 확장된 소켓과 또 그 확장된 오버랩 구조체의 관계가
		// 모두 하나의 유저 구조체안에 속하는 것들로 이루어진다.
		// AcceptEx를 호출할 때 모두 같은 유저 구조체 안의 변수들로만 사용되었기 때문입니다.
		// 이 점은 현재 접속한 클라이언트가 어떠한 유저 구조체를 할당받았는지를 알 수 있는 근거가 된다.

		// 이벤트가 발생하여 GetQueuedCompletionStatus가 블록 해제된 시점에서 
		// 접속한 클라이언트는 자기와 통신을 위한 하나의 소켓과 또한 확장된
		// 오버랩 구조체를 이미 할당받은 시점이기 때문에 또한 그들이 같은 유저 구조체 안에 포함되어 있기 때문에 그 확장된
		// 오버랩 구조체의 주소만을 알게 된다면 접속한 클라이언트가 어떠한 유저 구조체를 사용하는지를 알 수 있다.
		// 따라서 lpsockContext = (LPSOCKETCONTEXT)lpEov;//와 같은 관계가 성립한다.


		// 에러가 없을 경우
		if (lpEov != NULL && dwTransferred != 0)
		{
			g_Server.iCurUserNum++;//유저 접속~
			lpClientContext = (LPCLIENTCONTEXT)lpEov;// 이벤트가 발생한 소켓컨텍스트를 얻는다.
			// AcceptEx에 넣는 확장된 오버랩 구조체는 첫 번째 블록을 받기 위한 것이므로  유저 구조체에서
			// eovRecvTcp를 사용하게 되는데 이것이 바로 그 유저 구조체의 첫 번째 멤버이기 때문에
			// 그것의 주소 값을 유저 구조체의 시작 값이라 볼 수 있다.


			// 접속 받은 곳의 주소를 얻어서 저장
			GetAcceptExSockaddrs(lpClientContext->pRecvEnd, MAXPACKETSIZE, sizeof(sockaddr_in) + 16
				, sizeof(sockaddr_in) + 16, (sockaddr**)&pLocal, &localLen, (sockaddr**)&pRemote, &remoteLen);


			CopyMemory(&lpClientContext->remoteAddr, pRemote, sizeof(sockaddr_in));

			// 링버퍼 end pointer 설정
			RecvTcpBufEnqueue(lpClientContext , dwTransferred);

			// Accept 쓰레드에서의 일을 모두 마쳤기 때문에 Worker 쓰레드로 접속된
			// 클라이언트의 유저 구조체에 할당된 소켓을 클라이언트와의
			// 실제 통신용 IOCP와 연결시키는 작업을 합니다.
			// 이 때는 유저에 대한 명확한 구조가 존재하고 있으므로 Completion key 값에 이 유저 구조체의 포인터를 넣을 수가 있다.
			// 그렇게 함으로서 앞서 Accept 쓰레드에서처럼 형 변환 없이 바로바로 사용이 가능합니다.

			CreateIoCompletionPort((HANDLE)lpClientContext->sockClnt, g_Server.hIocpWorkTcp, (DWORD)lpClientContext, 0);


			// 바로 읽기에 대한 요청을 한다.
			// 이것은 비동기 소켓이라는 부분에 아주 밀접한 부분입니다.
			// 블록킹 소켓에서는 recv에서 어떠한 크기일지라도 반드시 읽은 후에 블록이 해제되는 반면에
			// 비동기 소켓은 일단은 무조건 리턴을 하며 후에 어떠한 이벤트가 발생하였을 때 처리하는 방식입니다.
			// 물론 여기에서는 앞에서와 같이 이미 IOCP와 유저 구조체의 소켓을 연결하였기 때문에
			// 어떠한 읽기가 발생하면 worker 쓰레드 쪽의 GetQueueCompletionStatus가 풀리는 것이다.
			PostTcpRecv(lpClientContext);


			// 이 함수는 클라이언트로부터 날아온 데이터를 해석합니다.
			// 함수의 이름 그대로 요청이 왔다는 것을 쌓는 함수로
			// 그렇게 쌓인 큐와 그것의 처리가 프로세스라는 개념입니다.
			GameBufEnqueueTcp(lpClientContext);
		}

	}
	return 0;
}

/*
작업 쓰레드

실제로 유저와의 응답을 처리한다.
유저가 보낸 데이터를 버퍼에 저장하고 그것을 처리하도록 요구하며
또한 유저에게 보내야 할 데이터를 보내는 과정을 쓰레드에서 처리한다.

실제로는 쓰레드들이 되며 그것들을 풀링하는 작업은 커널에서 한다.
적정한 수만큼의 쓰레드를 미리 생성하여 놓은 후에 보통의 경우에는 대기 상태로 있다가
어떠한 작업을 요청 받을 때는 그 중의 하나가 활성화되어 그 작업을 처리하는 것입니다.

이미 모든 쓰레드가 활성화되어 있다면 새로이 요청되는 작업은 그 쓰레드들 중에 어떠한 것이
사용 가능할 때까지 기다리고 있다. 물론 어떠한 쓰레드를 사용하게 될지는
쓰레드의 경쟁상태로 인하여 알 수 없다.
*/
UINT WINAPI WorkerProc(void* pParam)
{
	DWORD			dwTransferred;
	LPEOVERLAPPED	lpEov;
	LPCLIENTCONTEXT lpSockContext;
	BOOL			bResult;

	while (1)
	{
		// 유저로 부터 데이터를 받거나 서버에서 유저에게
		// 보낸 데이터의 완료가 일어날 때까지 대기
		// 어떠한 요청이 왔을 때 그 작업으로 인하여 전송받은 데이터의 양과
		// Accept 시에 IOCP와 연결할 때
		// Completion key로 넘겨주었던 해당 유저 구조체의 포인터 , 그 작업에서 사용되었던 오버랩 구조체의 포인터를 얻을 수 있다.
		bResult = GetQueuedCompletionStatus(g_Server.hIocpWorkTcp, &dwTransferred,  (LPDWORD)&lpSockContext
			, (LPOVERLAPPED*)&lpEov, INFINITE);

		if (lpEov != NULL)
		{
			// 만약 넘겨진 오버랩 구조체가 NULL 이 아니면서
			// 처리된 바이트 수가 0이라면 그것은 연결이 어떠한 이유에서든지  접속 끊김

			if (dwTransferred == 0)
			{
				//소켓이 끊김을 요청 LOGOUT
			}
			else
			{
				// 넘어온 오버랩 구조체의 포인터는 어떠한 작업을 요청할 때 인자로 주어지는 것이다.
				// 이러한 요청은 각각의 유저에 대하여 하게 되므로 유저 구조체 안에 멤버로 포함되어 있는 것입니다.
				// 실제로 사용한 것은 그 오버랩 구조체가 어떠한 작업의 결과인지를 나타내기 위하여 상태 값을 가지고 있는 확장된 형태의 것을 사용합니다.
				// 클라이언트로 부터 받은 데이터가 읽혀진 것에 대한 이벤트와 클라이언트로 보낼 데이터의 보내기가 완료된 이벤트에 대한 결과가
				// 모두 이 작업 쓰레드로 넘어오기 때문에 발생한 이벤트가 어떤 것에 대한 결과인가를 구분하기 위하여 사용됩니다.


				// 읽기용의 오버랩 구조체에서 신호
				if (lpEov->mode == RECVEOVTCP)
				{
					//이벤트가 WSARecv에 의하여 발생한 것이라면
					// RecvTcpBufEnque함수를 이용하여 그 데이터의 큐에 넣는 작업을 합니다.
					// 읽기 버퍼 위치 조정
					RecvTcpBufEnqueue(lpSockContext, dwTransferred);

					// 패킷 처리를 요청
					GameBufEnqueueTcp(lpSockContext);

					// 다음번 읽기를 위해 비동기 읽기를 시도
					PostTcpRecv(lpSockContext);
				}
				else if (lpEov->mode == SENDEOVTCP)
				{
					// 이벤트가 WSASend에 의하여 발생한 것이라면 오직 한가지 PostTcpSendRest 함수만을 호출합니다.
					// 그 함수는 보내기 버퍼에 쌓인 나머지 데이터들을 전송하는 역할을 합니다.
					// 가장 최초에 WSASend를 호출한다면 그것은 보내기를 원했던 만큼의 크기의 전송이 완료 되거나
					// TCP의 특성으로 일부만 전송되었을 수 있는 것입니다.
					// 그러한 이유로 일단 전송에 관한 입 / 출력이 완료되면 보내기를 원하는 바이트 수와 보내진 바이트 수를
					// 검사하는 항목이 필요하며 그것이 바로 postSnedRest함수다.

					//보내야 할 것이 더 있다면 나머지도 보내도록 요청
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

	// 전송 받은 패킷으로 인하여 받기 버퍼를 초과하는 지를 검사 (포인터의 뺄셈 (주소값 연산))
	iExtra = lpSockContext -> pRecvEnd + iPacketSize - lpSockContext->RecvRingBuf - RINGBUFSIZE;


	if (iExtra >= 0)
	{
		// 받기 버퍼를 초과한다면 그 부분을 앞으로 이동
		CopyMemory(lpSockContext->RecvRingBuf, lpSockContext->RecvRingBuf + RINGBUFSIZE, iExtra);
		// 큐가 쌓일 위치 조정
		lpSockContext->pRecvEnd = lpSockContext->RecvRingBuf + iExtra;
	}
	else
	{
		//큐가 쌓일 위치 조정
		lpSockContext->pRecvEnd += iPacketSize;
	}

#ifdef _LOGLEVEL1_
	printf( "RecvTcpBufEnqueue : %d, %d byte\n", lpSockContext->iKey, iPacketSize );
#endif
}

/*
큐에 있는 패킷을 꺼내주는 함수입니다.
RecvTcpBufEnqueue함수가 전송된 데이터를 무조건 RingBuf 버퍼에 순서대로 쌓는 반면에
이 함수는 한번의 호출로 완성된 하나의 패킷을 얻는 부분입니다.
로직상 언제나 타당한 패킷만을 얻으므로 그것을 가지고서 처리를 할 수 있는 것이다.
간단하게 하나씩의 데이터를 얻을 수 있는 이 함수에 대하여 그전에 설정된 중요한 가정을 확인한다.

RecvRingBuf 버퍼는 충분히 크고 관리하는 데 있어서 Begin이 한바퀴를 돌아온 End에 의해 덮어 지지 않는다.

이 두가지 가정은 여기서 사용한 방법으로 버퍼를 관리할 떄 아주 중요한 사항입니다.
RingBuf라는 것은 유저로부터 전송된 데이터들이 저장되는 곳입니다. 전송된 데이터를 모두 처리하기 전에
또 다른 데이터가 전송되어 올 수 있으므로 이 버퍼는 충분히 커야만 하는 것입니다.

또한 두  번째 가정에 의해서도 버퍼가 충분히 커야 할 필요가 있는 것입니다.
전송받은 데이터는 버퍼에서 사용가능한 끝을 나타내는 End 부분에 쌓이며 데이터를 꺼내갈 때는 Begin 부분에서 꺼낸다.


중요한 것은 여기서 꺼낸다 라는 표현이 실제로는 가져간다는 것이 아니라는 것이다.
중요한 의미로 여기서의 꺼내간다라는 것은 단순히 그 시작점의 포인터를 가져간다라는 것입니다.
이 부분에서 한번의 복사를 줄이는 것이 가능해지는 것입니다.
단순히 그 위치의 포인터만을 가져가게 됨으로써 실제 데이터는 RecvTcpRingBuf 에  있는 것을 고스란히 쓰게 되는 것입니다.
그러한 이유로 미처 그 데이터가 처리되기 전에 다른 데이터가 덮어 씌워지는 경우를 위해 버퍼가 충분히 커야합니다.
*/

/*
이 함수의 인자로는 해당 유저의 정보 구조체(lpsockcontext)와 하나의 패킷의 위치를 받을 이중 포인터(cpPacket)과 하나의 패킷의 크기를
받을 정수형 포인터 이다. 뒷쪽의 두 개의 변수를 보면 앞에서 말한대로 이것들은 위치나 크기만을 받는 부분이라는 것을 느낄 것이다.

패킷은 이것이 처리될 때까지 소멸되지 않는다는 가정을 하였으므로
복사하는 과정없이 위치만 넘기는 것이 가능합니다.
그 다음은 바디의 크기를 알기 위하여 그 크기를 나타내는 2바이트가 읽기 가능한지를 검사하는 부분입니다.
이 2바이트 경계에서 링 버퍼가 회전할 수 있기 때문이다. 그렇게 한 후 2바이트의 바디 크기를 나타내는 값을 얻는다.
그런 후에 이번에는 실제 바디에 해당하는 데이터를 얻기 위한 작업을 합니다.

그것이 링 버퍼가 회전하는 위치에 있는가를 검사하여 , 링 버퍼의 가장 앞에서 특정 크기를 
가장 뒤쪽으로 복사하는 과정을 진행한다. 그런 후에는 패킷을 읽기 위한 시작점을 사용한 수 만큼의 뒤로 옮기는 것입니다.
*/

void RecvTcpBufDequeue(LPCLIENTCONTEXT lpSockContext, char **cpPacket, int *iPacketSize)
{
	short iBodysize;
	int	  iExtra;

	*cpPacket = lpSockContext->pRecvBegin;
	// 그 위치와 버퍼의 끝까지와의 차이를 구함(begin의 위치와 버퍼의 끝과의 차이를 구하는 것입니다.)
	iExtra = lpSockContext->RecvRingBuf + RINGBUFSIZE - lpSockContext -> pRecvBegin;

	// 만약 차이가 바디의 크기를 나타내는 헤더보다 작으면
	if (iExtra < sizeof(short))
	{
		// 앞쪽에서 1바이트를 가져옴 ( 맨끝 포이터가 맨앞의 포인터를 가리키게 합니다.)
		*(lpSockContext->RecvRingBuf + RINGBUFSIZE) = *lpSockContext->RecvRingBuf;
	}
	// 바디의 크기를 얻는다.
	CopyMemory(&iBodysize, lpSockContext->pRecvBegin, sizeof(short));

	// 바디의 중간이 버퍼의 끝에 걸침
	if (iExtra <= iBodysize + HEADERSIZE)
	{
		// 앞쪽에서 바디의 나머지 내용을 복사해온다.
		CopyMemory(lpSockContext->RecvRingBuf + RINGBUFSIZE, lpSockContext->RecvRingBuf, iBodysize + HEADERSIZE - iExtra);
		// 패킷을 꺼내기 위한 위치 조정
		lpSockContext->pRecvBegin = lpSockContext->RecvRingBuf + iBodysize + HEADERSIZE - iExtra;
	}
	else
	{
		//패킷을 꺼내기 위한 위치 조정
		lpSockContext->pRecvBegin = lpSockContext->pRecvBegin + iBodysize + HEADERSIZE;
	}

	//실제 패킷 크기 전달
	*iPacketSize = iBodysize + HEADERSIZE;
#ifdef _LOGLEVEL1_
	printf( "RecvTcpBufDequeue : %d, %d byte\n", lpSockContext->index, *iPacketSize );
#endif
}

/*
버퍼 관리

이 부분이 IO 계층에서 가장 중요한 부분을 차지하고 있는 곳이다.
이 버퍼들은 요청된 데이터 또는 그것에 대한 응답으로 전송될 데이터들이 중간에
거쳐가는 부분으로 이것의 관리 방법을 보고자 하는 단락입니다.

복사의 최소화를 중점적으로 본다. 보통 전달받은 데이터들은 각 프로토콜별로 해당하는 양만큼을
하나의 뭉치로서 처리합니다.

가령 채널에 입장하기라는 프로토콜이 100바이트의 데이터라면 그것을 하나의 단위로 처리하는 것입니다.

그러한 과정에서 보통 하나의 패킷을 버퍼에 복사하는 식으로 사용합니다.
하지만 서버라는 것이 유저들의 요구에 해당하는 패킷을 적절하게 해석하는 것이 대부분이므로
이런 식의 복사라는 과정은 많이 CPU시간을 사용하는 부분 중의 하나가 될 것입니다.
이러한 이유로 여기서는 그것을 최대한으로 줄일 수 있는 방향을 찾아 보려 한 것입니다.

유저로부터 전송된 데이터를 읽을 수 있도록 비동기 함수를 호출하는 부분입니다.
*/

void PostTcpRecv(LPCLIENTCONTEXT lpSockContext)
{
	WSABUF			wsaBuf;
	DWORD			dwReceived, dwFlags;
	int				iResult;

	// WSARecv 함수를 위한 버퍼 설정
	dwFlags = 0;
	wsaBuf.buf = lpSockContext->pRecvEnd;// 링버퍼의 끝에서 부터 받는다.
	wsaBuf.len = MAXPACKETSIZE;	// 이곳에서는 받아들일 수 있는 최대 사이즈를 MAXPACKETSIZE 로 합니다
	// 또한 데이터를 받는 위치를 해당 받기 버퍼의 사용 가능한 끝(End)로 위치시키는 것입니다.
	// 데이터를 받기에 해당하는 오버랩 구조체(eovRecv)나 버퍼에 대한 표시변수 등은 모두 각각의 유저들에게
	// 있는 것을 사용함으로서 서로 독립적인 처리를 만들게 합니다.
	// WSARecv가 비동기 함수이므로 제대로 된 상태에서 이 호출은 언제나 바로 넘어가며
	// 데이터가 전송되어 왔을 경우에는 이벤트가 발생하는 것입니다.
	// 그 경우에서 읽어들인 데이터는 해당 구조체의 recvEnd를 시작으로 하여 전달된 바이트 수만큼이 저장됩니다.
	// 이렇게 한 후에 이 읽어들인 데이터에 대하여 버퍼를 정하는 부분이 RecvTcpBufEnqueue입니다.


	iResult = WSARecv(lpSockContext->sockClnt, &wsaBuf, 1, &dwReceived, &dwFlags
		, (OVERLAPPED*)&lpSockContext->eovRecv, NULL);

	// PENDING을 제외한 나머지 에러
	if (iResult == SOCKET_ERROR && WSAGetLastError() != ERROR_IO_PENDING)
	{
		printf("***** PostTcpRecv error : %d, %d\n", lpSockContext->iKey, WSAGetLastError());
	}


}
/*
이 함수는 그룹에 속하는 유저들에게 전송을 할 때 사용하는 함수입니다.
iBegin은 해당 그룹에서의 첫번째 유저에 대한 인덱스가 되는 것입니다.
*/

/*
유저들에게 데이터를 전송하는 과정에 있어서 SendRingBuf라는 링버퍼를 사용합니다.
기본적인 운영 방식은 데이터를 전송받을 때 사용하였던 RecvRingbuf에서의 방법과 동일하다
한가지 차이라면 여기에서는 하나의 패킷을 구분하기 위한 과정이 불필요하다는 것이다.


이것 또한 TCP의 특성을 사용하는 것이다. 순차성이 보장되므로 그것을 한번에 보내든 두 번에 보내든 동일한 것이 되는 것이다.
*/

void PostTcpSend(int ibegin, char *cpPacket, int iPacketSize)
{
	LPCLIENTCONTEXT			lpSockContext;
	WSABUF					wsaBuf;
	DWORD					dwSent;
	int						iResult, iExtra, iSendNow;
	int						iNext;

	iNext = ibegin;

	// 더 이상 연결이 되어 있지 않을 때 까지 반복(그룹에 속해있는 유저 끝까지)

	while (iNext != NOTLINKED)
	{
		//해당 인덱스에 대한 소켓 컨텍스를 얻는다.
		lpSockContext = &g_Server.sc[iNext];

		EnterCriticalSection(&lpSockContext->CS);
		//보낼 데이터를 보내기 버퍼에 복사
		CopyMemory(lpSockContext->pSendEnd, cpPacket, iPacketSize);
		// 보낼 데이터가 보내기 버퍼의 마지막을 넘어서는가를 검사
		iExtra = lpSockContext->pSendEnd + iPacketSize - lpSockContext->SendRingBuf - RINGBUFSIZE;

		if (iExtra >= 0)
		{
			CopyMemory(lpSockContext->SendRingBuf, lpSockContext->SendRingBuf + RINGBUFSIZE, iExtra);
			// 보내기 버퍼의 데이터를 쌓는 위치 수정
			lpSockContext->pSendEnd = lpSockContext->SendRingBuf + iExtra;
		}
		else
		{
			lpSockContext->pSendEnd += iPacketSize;
		}

		// IOCP의 특성으로 동기화의 문제가 있는 부분이다.
		// 유저들에게 데이터를 전송하기 위하여 WSASend를 하게 되었을 때
		// 그 전송이 일부든 , 전체든 완료되었을 때 IOCP에 한번의 이벤트가 발생합니다.
		// 그 이벤트가 발생하였을 때 그것이 모두가 전송되었다면 완료를,
		// 그렇지 않다면 추가적인 전송을 하는 것입니다.
		// 문제는 그러한 전송을 요구하는 , 즉 PostTcpSend 함수를 호출하는 쓰레드와
		// WSASend가 완료되었을 떄의 이벤트를 받는 쓰레드와 다르다는 데 있다.
		// 이 둘이 같은 유저 구조체에 대한 조작을 하기 때문에 동기화가 반드시 필요합니다.


		// 정확하게는 데이터를 전송하기 위한 (PostTcpSend)쓰레드는 프로세스 객체에서 할당된 쓰레드가 , 
		// PostTcpSendRest 쓰레드는 IOCP의 작업 쓰레드에 의해서 호출된다.


		// 보내야만 하는 전체 크기 증가
		lpSockContext->iSTRestCnt += iPacketSize;
		// 보내야 하는 전체 양이 지금 요청된 값과 같은지 검사
		iSendNow = (lpSockContext->iSTRestCnt == iPacketSize) ? TRUE : FALSE;



		// 여기서는 유저에게 데이터를 보내는 요청에 대한 WSASend 작업을 당장 하게 할지
		// 데이터만을 큐잉할지를 구분하는 플래그에 대한 값을 얻는 부분이다.
		// 이미 보내기 위한 데이터가 SendRingBuf버퍼에 복사되었고
		// SendRestCnt의 값이 보내기를 요청받은 iPacketSize 만큼
		// 증가된 상태이므로 큐잉작업 자체는 완료된 상태가 됩니다.
		// 그러한 상태에서 iSendRentCnt의 값이 iPacketSize와 같다면 요청 , 
		// 처리해야 할 데이터가 남아있지 않은 상태에서 해당하는 요청이 바로 지금 WSASend의 시작점이 됩니다.
		// 따라서 그러할 경우에 iSendNow라는 값을 참으로 만들어 데이터 전송을 즉시 실행하도록
		// WSASend를 호출하는 것이다.

		// 이미 보내기 과정중이 아니었으므로 
		// 지금 요청된 것을 보내기 시작


		if (iSendNow)
		{
			// 보내기 버퍼 설정
			wsaBuf.buf = lpSockContext->pSendBegin;
			wsaBuf.len = iPacketSize;

			iResult = WSASend(lpSockContext->sockClnt, &wsaBuf, 1, &dwSent
				, 0, (OVERLAPPED*)&lpSockContext->eovSend, NULL);

			// PENDING을 제외한 에러
			if (iResult == SOCKET_ERROR && WSAGetLastError() != ERROR_IO_PENDING)
			{
				printf("***** PostTcpSend error : %d, %d\n", lpSockContext->iKey, WSAGetLastError());
			}
		}
		// 이미 다른 보내기가 진행중임
		else
		{
			// 보내기 버퍼가 꽉 찼을 때
			if (lpSockContext->iSTRestCnt > RINGBUFSIZE)
			{
				printf("*********** tcp sendbuf overflow!(%d) : %d ***********\n", lpSockContext->iKey, lpSockContext->iSTRestCnt);
			}
		}
		//다음 링크로 넘김
		iNext = g_Server.pn[iNext].next;
		LeaveCriticalSection(&lpSockContext->CS);
	}
}

/*
이 함수는 선택된 유저들만을 대상으로 데이터를 전송할 때 사용된다.

첫 번째 인자가 바로 선택된 유저들의 수
두 번째 인자가 선택된 유저들의 인덱스에 대한 배열 리스트
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

		// 패킷 카피
		CopyMemory(lpSockContext->pSendEnd, cpPacket, iPacketSize);

		// 링버퍼 위치 이동
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
이 함수는 유저에게 보내야 하는 데이터들 중 아직 보내지 않은 나머지에 대한 전송을 하는 부분입니다.
*/
/*
이 함수가 호출되었을 때는 iTransferred만큼의 전송된 바이트 수를 가지며
그 만큼을 버퍼에서 지우는 부분이 이 함수에서 최초에 하는 작업이다.

그런 후에 postTcpSend함수에서 설명한 iStRectCnt에서 전송된 iTransferred 크기 만큼을
빼는 과정을 임계영역 내에서 하게 된다.

이렇게 전송된 후에도 보내야 할 남아있는 데이터가 존재하며 그것을
iRestSize라는 지역 변수에 넣는 것입니다. iRestSize에 0보다 큰 값이 있다면 추가적으로
보내야 할 데이터가 있다는 말이된다.

이 때 iSTRestCnt라는 값을 지역 변수인 iRestSize에 넣는 이유가 중요하다.
지역 변수가 쓰레드 별로 할당된 스택을 사용하게 되므로
그 값은 쓰레드에 안전한 것이 되는 것입니다.
그런 이유로 iRestSize에 값을 할당한 후 , 임계영영을 해제하여도
안전한 액세스가 가능한 것이다. 임계영역 또한 해제한 상태이므로
postTcpSend 함수 역시 동시에 작업이 가능하다는 것을 알 수 있다.

iRestSize에 0보다 큰 값이 있다면 아직 추가적으로 보내야 하는 데이터가 있으므로
WSASend를 호출한다. 이때는 앞서 말한 것과 같이 패킷 단위로 구분을
할 필요가 없으므로 그것이 적정수내에서 한번에 모두 전송할 수 있다.
*/
void PostTcpSendRest(LPCLIENTCONTEXT lpSockContext, int iTransferred)
{
	WSABUF				wsaBuf;
	DWORD				dwSent;
	int					iResult, iRestSize, iExtra;

	// 완료된 양만큼 보내기 시작하는 버퍼의 위치를 뒤로 옮김
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
	// 보내야하는 총량을 전송된 양만큼 줄임
	lpSockContext->iSTRestCnt -= iTransferred;
	// 아직 보내야 하는 양이 있는가??
	iRestSize = lpSockContext->iSTRestCnt;
	LeaveCriticalSection(&lpSockContext->CS);

	// 추가적으로 전송해야 할 양이 없음
	if (iRestSize == 0)
	{

	}
	// 추가적으로 전송해야 할 양이 있음
	else if (iRestSize > 0)
	{
		// 가능한 최대의 양을 한번에 전송하려고 계산
		if (iRestSize > MAXPACKETSIZE) iRestSize = MAXPACKETSIZE;

		iExtra = lpSockContext->SendRingBuf + RINGBUFSIZE - lpSockContext->pSendBegin;

		// 버퍼가 회기한다면 앞에서 일정량을 복사해옴
		if (iExtra < iRestSize)
		{
			CopyMemory(lpSockContext->SendRingBuf + RINGBUFSIZE, lpSockContext->SendRingBuf, iRestSize - iExtra);
		}

		// 패킷 전송
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
프로세스 객체의 요청

이 함수는 각각의 유저로부터 전송받은 테이터를 이제 실제로 처리하도록 프로세스 객체에게 넘겨주는 과정을 처리하는 함수입니다. 
그리하여 인자로는 유저 구조체의 포인터 단 하나만을 취하는 것입니다.

각 유저로부터 전송받은 데이터는 WSARecv API를 통해 각각의 RecvRingBuf 에 쌓이게 된다.
이렇게 받은 패킷이 제대로된 하나를 완성하였을 경우에는
실제로 그것을 처리하도록 프로세스 객체의 큐에 요청한다.
패킷이 완성되지 않았을 경우에는 아직 모두 전송받은 경우가 아니므로
다음 번 데이터를 전송받을 때 처리를 요청하는 것입니다.
*/

/*
Mark변수가 필요한 이유는 TCP에서의 패킷 단위 전송이 보장되지 않는다는데 있다.
정확하게는 하나의 패킷이 몇번에 걸쳐서 나뉘어 전송된다는 것이 그 이유입니다.
패킷 처리가 불가능해진 경우에는 다음 번 전송이 발생하였을 때로 순서를 넘겨야 합니다.
바로 이 부분에 mark가 사용됩니다.

가장 최초에는 mark와 begin이 같은 값으로 시작된다.
이 때 데이터를 위에서 말한 것처럼 전송받았다고 하면 , 일단 정상적인 두 개의 패킷이 있으므로
이것을 프로세스 객체에 처리하도록 큐에 넣을 것입니다.

이 두 패킷을 큐에 넣으면서 세 번째 패킷의 시작부를 바로 mark의 값으로 만들게 된다.
데이터를 전송받았으나 프로세스 객체에 요청되지 못한 새로운 패킷의 시작부가 바로
mark에 의하여 그 위치를 표시합니다.

두 상황이 나타납니다.

하나는 아직 덜 완성된 패킷에 대한 전송을 받는 것이며
또 다른 하나는 프로세스 객체에 처리시킨 두 개의 패킷이 처리된다.
*/
void GameBufEnqueueTcp(LPCLIENTCONTEXT lpSockContext)
{
	short			iBodySize;
	int				iRestSize, iExtra;

	// 이전에 표시된 위치와 전송받은 데이터와의 실제 크기를 구함
	// 이 차이는 이 두개의 위치가 역전될 수도 있으므로(링버퍼 이므로)
	// 음수일 경우에는 RINGBUFSIZE를 더해줍니다.
	// 이 거리라는 것이 현재 그 유저가 가지고 있는 처리되어야 할 데이터의 양입니다.
	// 그 양이 헤더의 크기보다 작다면 처리할 것이 없다는 것을 말한다.

	iRestSize = lpSockContext->pRecvEnd - lpSockContext->pRecvMark;
	if (iRestSize < 0)
		iRestSize += RINGBUFSIZE;
	// 패킷이 헤더크기 만큼도 전송되지 안항ㅆ음
	if (iRestSize < HEADERSIZE)
		return;


	// 만약 데이터를 한번 전송받았을 떄 여러 개의 패킷이 동시에 들어온 것일 수 있으니
	// while문을 이용해서 무한 루프합니다.
	// 이제 제대로 완성된 하나의 패킷을 찾기 위해서 검사하는 과정을 행하는 것입니다.
	// RecvMark로 부터 2바이트의 바디를 얻는 부분이 나온다.
	// 만약 그것을 얻는데 있어서 링 버퍼가 회기한다면 가장 앞쪽에서 가져옵니다.
	// 그런 후에 그 패킷이 타당하다면 Mark를 그곳으로 옮겨 주면서
	// 동시에 프로세스의 큐에 등록합니다.

	while (1)
	{
		// 표시된 위치와 받기 버퍼의 끝과의 차이 구함
		iExtra = lpSockContext -> RecvRingBuf + RINGBUFSIZE - lpSockContext->pRecvMark;

		// 차이가 바디의 크기를 나타내는 헤더보다 작음
		if (iExtra < sizeof(short))
		{
			// 앞쪽에서 1바이트를 가져옴
			*(lpSockContext->RecvRingBuf + RINGBUFSIZE) = *lpSockContext->RecvRingBuf;
		}
		// 바디 크기를 얻음
		CopyMemory(&iBodySize, lpSockContext->pRecvMark, sizeof(short));

		// 패킷이 실제 크기만큼 전송되지 않음
		if (iRestSize < iBodySize + HEADERSIZE)
			return;

		// 표시를 다음번 패킷 위치로 옮김
		lpSockContext->pRecvMark += iBodySize + HEADERSIZE;

		//표시가 회기하는 경우를 위한 재설정
		if (lpSockContext->pRecvMark >= lpSockContext->RecvRingBuf + RINGBUFSIZE)
			lpSockContext->pRecvMark -= RINGBUFSIZE;

		// 검사해야 할 전체의 크기에서 지금 검사된 패킷에 대한 크기를 줄임
		iRestSize -= iBodySize + HEADERSIZE;
		// 패킷 처리를 위해 프로세스 큐에 넣음
		g_Server.ps[lpSockContext->iProcess].GameBufEnqueue(lpSockContext);

#ifdef _LOGLEVEL1_
		printf( "GameBufEnqueueTcp : %d\n", lpSockContext->iKey );
#endif
		if (iRestSize < HEADERSIZE) 
			return;
	}
}

