#include "Default.h"
#include "IO.h"
#include "Include.h"

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
	int				itv;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return -1;

	//  클라이언트의 접속을 받기위한 리슨 소켓 생성
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
	CreateIoCompletionPort((HANDLE)g_Server.sockListener, g_Server.hIocpAcpt, 0, 0);


	// 최대 유저 수만큼의 구조체를 미리 만들어 놓는다.



}

