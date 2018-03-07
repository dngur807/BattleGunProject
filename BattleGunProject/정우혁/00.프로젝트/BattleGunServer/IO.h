#pragma once

#include "Struct.h"

int InitIO();
//유저 구조체의 초기화
int InitSocketContext(int maxUser);


UINT WINAPI AcceptProc(void* pParam);//접속 허가 쓰레드

// 링 버퍼 
void RecvTcpBufEnqueue(LPCLIENTCONTEXT lpSockContext, int iPacketSize);
void PostTcpRecv(LPCLIENTCONTEXT lpSockContext);
void GameBufEnqueueTcp(LPCLIENTCONTEXT lpSockContext);