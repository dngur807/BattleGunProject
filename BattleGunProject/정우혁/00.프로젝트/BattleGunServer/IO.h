#pragma once

#include "Struct.h"

int InitIO();
//���� ����ü�� �ʱ�ȭ
int InitSocketContext(int maxUser);


UINT WINAPI AcceptProc(void* pParam);//���� �㰡 ������

// �� ���� 
void RecvTcpBufEnqueue(LPCLIENTCONTEXT lpSockContext, int iPacketSize);
void RecvTcpBufDequeue(LPCLIENTCONTEXT lpSockContext, char **cpPacket, int *iPacketSize);

void PostTcpRecv(LPCLIENTCONTEXT lpSockContext);
void PostTcpSend(int iSockNum, int iSockAddr[], char *cpPacket, int iPacketSize);
void GameBufEnqueueTcp(LPCLIENTCONTEXT lpSockContext);