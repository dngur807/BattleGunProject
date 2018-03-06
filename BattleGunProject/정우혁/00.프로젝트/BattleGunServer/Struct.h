#pragma once
#include "Default.h"

//서버 정보 구조체이다. 
// 서버 프로그램 전체에서 사용하는 서버 정보를 위한 전역 구조체이다.
typedef struct 
{
	SOCKET					sockListener;//유저의 접속을 기다리는 리슨 소켓이다.
	HANDLE					hIocpWorkTcp;	//	IOCP 핸들로서 보통의 읽기 쓰기 용도
	HANDLE					hIocpAcpt;		// IOCP 핸들로서 유저의 접속을 받아드릴 때 사용합니다.

	int						iMaxUserNum;// 최대 유저 수
	int						iCurUserNum;// 현재 유저 수
	//게임 전용
	CRITICAL_SECTION		 m_CS;


}SERVERCONTEXT, *LPSERVERCONTEXT;

// 클라이언트를 담당하는 유저 구조체이다.

typedef struct
{
	

}CLIENTCONTEXT , *LPCLIENTCONTEXT;