#pragma once

#include "Struct.h"

int InitIO();
//���� ����ü�� �ʱ�ȭ
int InitSocketContext(int maxUser);


UINT WINAPI AcceptProc(void* pParam);//���� �㰡 ������

// �� ���� 
void RecvTcpBufEnqueue(LPCLIENTCONTEXT lpSockContext, int iPacketSize);
void PostTcpRecv(LPCLIENTCONTEXT lpSockContext);
void GameBufEnqueueTcp(LPCLIENTCONTEXT lpSockContext);