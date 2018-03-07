#pragma once
#include "Default.h"
#include "Define.h"

typedef struct
{
	OVERLAPPED				ovl; // hEvent����� �Ű澲�� �ȴ�.
	int						mode; // IOCP�� IO �ϷḸ �˷��� �� �Է����� ������� �˷������ʴ´�.

}EOVERLAPPED, *LPEOVERLAPPED;		//Ȯ��� ������ ����ü

// Ŭ���̾�Ʈ�� ����ϴ� ���� ����ü�̴�.
typedef struct
{
	EOVERLAPPED		eovRecv;	//	������ ����ü�� �⺻���� �Ͽ� �ϳ��� ���� ��带 ��Ÿ���� �÷��׸� ������ �ִ�.
	EOVERLAPPED		eovSend;	//	���� ���� �� ����ü�� �б⿡�� ���� ������ ���⿡�� ���� �������� ��Ÿ���� �÷����Դϴ�.
								//	IOCP���� �߻��� �̺�Ʈ�� ��� �Ϳ� ���� �߻��� ���ΰ��� �˱� ���ؼ� �̷��� Ȯ��� ������ ����ü�� �ʿ��մϴ�.

								// �������� ũ��� �ִ� ��Ŷ ũ�� * 10 �˳��ϰ�
	char			RecvRingBuf[RINGBUFSIZE + MAXPACKETSIZE];	//���� �����Ͱ� ����� �����Դϴ�.
	char*			pRecvBegin;	// �б� ������ ���� ��ġ
	char*			pRecvEnd;	// �б� ������ �� ��ġ
	char*			pRecvMark; // ���ӹ��� ���� ��ġ

	char			SendRingBuf[RINGBUFSIZE + MAXPACKETSIZE];	//���� �����Ͱ� ����� �����Դϴ�.
	char*			pSendBegin;	// ���� ������ ���� ��ġ
	char*			pSendEnd;	// ���� ������ �� ��ġ


	SOCKET			sockClnt;//���� ����� ���� ����
	CRITICAL_SECTION	CS;//���� ���ۿ����� ����ȭ�� ���� �Ӱ迵�� ��ü�̴�.
	sockaddr_in				remoteAddr;		//������ Ŭ���̾�Ʈ�� �ּҸ� ��Ÿ����.
	int				iKey;// ������ Ŭ���̾�Ʈ�� ������ �ִ� ���� �ε���


}CLIENTCONTEXT, *LPCLIENTCONTEXT;
//���� ���� ����ü�̴�. 
// ���� ���α׷� ��ü���� ����ϴ� ���� ������ ���� ���� ����ü�̴�.
typedef struct 
{
	SOCKET					sockListener;//������ ������ ��ٸ��� ���� �����̴�.
	HANDLE					hIocpWorkTcp;	//	IOCP �ڵ�μ� ������ �б� ���� �뵵
	HANDLE					hIocpAcpt;		// IOCP �ڵ�μ� ������ ������ �޾Ƶ帱 �� ����մϴ�.

	int						iMaxUserNum;// �ִ� ���� ��
	int						iCurUserNum;// ���� ���� ��

	CLIENTCONTEXT*			pClientBegin;// Ŭ���̾�Ʈ ����ü�鿡 ���� ���� ������ , �̰��� �̿��ؼ� ��ü ���� Ž�� ����

	//���� ����
	CRITICAL_SECTION		 CS;


}SERVERCONTEXT, *LPSERVERCONTEXT;



