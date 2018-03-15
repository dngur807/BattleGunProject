#pragma once

#include "Struct.h"
#define NOTLINKED			-1

int InitIO();
//유저 구조체의 초기화
int InitSocketContext(int maxUser);


UINT WINAPI AcceptProc(void* pParam);//접속 허가 쓰레드
UINT WINAPI WorkerProc(void* pParam);//작업 쓰레드
// 링 버퍼 
void RecvTcpBufEnqueue(LPCLIENTCONTEXT lpSockContext, int iPacketSize);
void RecvTcpBufDequeue(LPCLIENTCONTEXT lpSockContext, char **cpPacket, int *iPacketSize);

void PostTcpRecv(LPCLIENTCONTEXT lpSockContext);
void PostTcpSend(int ibegin, char *cpPacket, int iPacketSize);// 모든 유저에게 전송
void PostTcpSend(int iUserNum, int iSockAddr[], char *cpPacket, int iPacketSize);//선택된 유저에게만 선송
void PostTcpSendRest(LPCLIENTCONTEXT lpSockContext, int iTransferred);

void GameBufEnqueueTcp(LPCLIENTCONTEXT lpSockContext);

