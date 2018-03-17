#pragma once
#include "Default.h"
#include "Define.h"
#include "Enum.h"

class CServerProcess;

typedef struct
{
	OVERLAPPED				ovl; // hEvent멤버만 신경쓰면 된다.
	int						mode; // IOCP는 IO 완료만 알려줄 뿐 입력인지 출력인지 알려주지않는다.
}EOVERLAPPED, *LPEOVERLAPPED;		//확장된 오버랩 구조체


typedef struct	//링크드 리스트 노드 구조체
{
	int						prev;
	int						next;
}OBJECTNODE, *LPOBJECTNODE;

typedef struct tagUserInfo
{
	TCHAR		szID[256];
	eCharType	CharType;
	eTeamType	eTeam;
	int			iKill;
	int			iDeath;
	int			iGiveDamage;// 준 피해량
	int			iGetDamage;	// 받은 피해량
	int			iIndex;
	tagUserInfo()
	{
		CharType = CHAR_END;
		eTeam = TEAM_END;
		iKill = 0;
		iDeath = 0;
		iGiveDamage = 0;
		iGetDamage = 0;
	}
}USERINFO;

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

	int				iSTRestCnt;
	SOCKET			sockClnt;//실제 통신을 위한 소켓
	CRITICAL_SECTION	CS;//쓰기 버퍼에서의 동기화를 위한 임계영역 객체이다.
	sockaddr_in				remoteAddr;		//접속한 클라이언트의 주소를 나타낸다.
	int				iKey;// 각각의 클라이언트가 가지고 있는 고유 인덱스



	// 게임 프로세스
	char		szID[IDLEN];
	int			idLen;//아이디 길이

	int			iProcess;		//현재 유저가 위치하고 있는 곳의 인덱스

	USERINFO	tUserInfo;
}CLIENTCONTEXT, *LPCLIENTCONTEXT;
//서버 정보 구조체이다. 
// 서버 프로그램 전체에서 사용하는 서버 정보를 위한 전역 구조체이다.
class CLooby;

typedef struct 
{
	SOCKET					sockListener;//유저의 접속을 기다리는 리슨 소켓이다.
	HANDLE					hIocpWorkTcp;	//	IOCP 핸들로서 보통의 읽기 쓰기 용도
	HANDLE					hIocpAcpt;		// IOCP 핸들로서 유저의 접속을 받아드릴 때 사용합니다.

	int						iMaxUserNum;// 최대 유저 수
	int						iCurUserNum;// 현재 유저 수
	int						iWorkerTNum; // 작업 쓰레드 개수

	CLIENTCONTEXT*			sc;// 클라이언트 구조체들에 대한 시작 포인터 , 이것을 이용해서 전체 유저 탐색 가능
	CServerProcess*			ps;//프로세스 시작점
	OBJECTNODE*				pn;//플레이어들의 노드

	CLooby*						pLobby;

	int						iMaxProcess;
	//게임 전용
	CRITICAL_SECTION		 CS;

	int						iUserBegin;
	int						iUserEnd;
	int						iAllUserNum;
	int						iATeamNum;
	int						iBTeamNum;

	map<int, LPCLIENTCONTEXT>	m_mapAllUserList;
	map<int, LPCLIENTCONTEXT>	m_mapATeam;
	map<int, LPCLIENTCONTEXT>	m_mapBTeam;

	eMapType					m_eMapType;

}SERVERCONTEXT, *LPSERVERCONTEXT;


typedef struct
{
	int(*proc)(LPCLIENTCONTEXT lpSockContext, char *cpPacket);
}ONTRANSFUNC;

