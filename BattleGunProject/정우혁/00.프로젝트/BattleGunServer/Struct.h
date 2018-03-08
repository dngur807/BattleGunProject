#pragma once
#include "Default.h"
#include "Define.h"
#include "Enum.h"

class CServerProcess;

typedef struct
{
	OVERLAPPED				ovl; // hEvent����� �Ű澲�� �ȴ�.
	int						mode; // IOCP�� IO �ϷḸ �˷��� �� �Է����� ������� �˷������ʴ´�.
}EOVERLAPPED, *LPEOVERLAPPED;		//Ȯ��� ������ ����ü


typedef struct tagUserInfo
{
	TCHAR		szID[256];
	eTeamType	eTeam;
	int			iKill;
	int			iDeath;
	int			iGiveDamage;// �� ���ط�
	int			iGetDamage;	// ���� ���ط�
	int			iIndex;
	tagUserInfo()
	{
		eTeam = TEAM_END;
		iKill = 0;
		iDeath = 0;
		iGiveDamage = 0;
		iGetDamage = 0;
	}
}USERINFO;

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



	// ���� ���μ���
	char		szID[IDLEN];
	int			idLen;//���̵� ����

	int			iProcess;		//���� ������ ��ġ�ϰ� �ִ� ���� �ε���

	USERINFO	tUserInfo;
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
	CServerProcess*			ps;//���μ��� ������

	int						iMaxProcess;
	//���� ����
	CRITICAL_SECTION		 CS;
	map<int, LPCLIENTCONTEXT>  m_mapAllUserList;

}SERVERCONTEXT, *LPSERVERCONTEXT;


typedef struct
{
	int(*proc)(LPCLIENTCONTEXT lpSockContext, char *cpPacket);
}ONTRANSFUNC;

