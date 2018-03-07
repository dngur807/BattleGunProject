#pragma once
#include "Default.h"
#include "Define.h"

typedef struct
{
	OVERLAPPED				ovl; // hEvent멤버만 신경쓰면 된다.
	int						mode; // IOCP는 IO 완료만 알려줄 뿐 입력인지 출력인지 알려주지않는다.

}EOVERLAPPED, *LPEOVERLAPPED;		//확장된 오버랩 구조체

// 클라이언트를 담당하는 유저 구조체이다.
typedef struct
{
	EOVERLAPPED		eovRecv;	//	오버랩 구조체를 기본으로 하여 하나의 상태 모드를 나타내는 플래그를 가지고 있다.
	EOVERLAPPED		eovSend;	//	상태 모드는 그 구조체가 읽기에서 사용될 것인지 쓰기에서 사용된 것인지를 나타내는 플래그입니다.
								//	IOCP에서 발생한 이벤트가 어떠한 것에 의해 발생된 것인가를 알기 위해서 이러한 확장된 오버랩 구조체가 필요합니다.

								// 링버퍼의 크기는 최대 패킷 크기 * 10 넉넉하게
	char			RecvRingBuf[RINGBUFSIZE + MAXPACKETSIZE];	//실제 데이터가 저장될 버퍼입니다.
	char*			pRecvBegin;	// 읽기 버퍼의 시작 위치
	char*			pRecvEnd;	// 읽기 버퍼의 끝 위치
	char*			pRecvMark; // 게임버퍼 시작 위치

	char			SendRingBuf[RINGBUFSIZE + MAXPACKETSIZE];	//실제 데이터가 저장될 버퍼입니다.
	char*			pSendBegin;	// 쓰기 버퍼의 시작 위치
	char*			pSendEnd;	// 쓰기 버퍼의 끝 위치


	SOCKET			sockClnt;//실제 통신을 위한 소켓
	CRITICAL_SECTION	CS;//쓰기 버퍼에서의 동기화를 위한 임계영역 객체이다.
	sockaddr_in				remoteAddr;		//접속한 클라이언트의 주소를 나타낸다.
	int				iKey;// 각각의 클라이언트가 가지고 있는 고유 인덱스


}CLIENTCONTEXT, *LPCLIENTCONTEXT;
//서버 정보 구조체이다. 
// 서버 프로그램 전체에서 사용하는 서버 정보를 위한 전역 구조체이다.
typedef struct 
{
	SOCKET					sockListener;//유저의 접속을 기다리는 리슨 소켓이다.
	HANDLE					hIocpWorkTcp;	//	IOCP 핸들로서 보통의 읽기 쓰기 용도
	HANDLE					hIocpAcpt;		// IOCP 핸들로서 유저의 접속을 받아드릴 때 사용합니다.

	int						iMaxUserNum;// 최대 유저 수
	int						iCurUserNum;// 현재 유저 수

	CLIENTCONTEXT*			pClientBegin;// 클라이언트 구조체들에 대한 시작 포인터 , 이것을 이용해서 전체 유저 탐색 가능

	//게임 전용
	CRITICAL_SECTION		 CS;


}SERVERCONTEXT, *LPSERVERCONTEXT;



