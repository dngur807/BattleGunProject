#pragma once

#include "Struct.h"
#define NOTLINKED			-1

int InitIO();
//���� ����ü�� �ʱ�ȭ
int InitSocketContext(int maxUser);


UINT WINAPI AcceptProc(void* pParam);//���� �㰡 ������
UINT WINAPI WorkerProc(void* pParam);//�۾� ������
// �� ���� 
void RecvTcpBufEnqueue(LPCLIENTCONTEXT lpSockContext, int iPacketSize);
void RecvTcpBufDequeue(LPCLIENTCONTEXT lpSockContext, char **cpPacket, int *iPacketSize);

void PostTcpRecv(LPCLIENTCONTEXT lpSockContext);
void PostTcpSend(int ibegin, char *cpPacket, int iPacketSize);// ��� �������� ����
void PostTcpSend(int iUserNum, int iSockAddr[], char *cpPacket, int iPacketSize);//���õ� �������Ը� ����
void PostTcpSendRest(LPCLIENTCONTEXT lpSockContext, int iTransferred);

void GameBufEnqueueTcp(LPCLIENTCONTEXT lpSockContext);

