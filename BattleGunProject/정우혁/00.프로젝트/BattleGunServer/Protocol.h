#pragma once
enum eProtocolType
{
	REQUEST_LOGIN = 0,
	ANSWER_LOGIN, // ���
	NOTIFY_YOURINFO, // �ε��� �� ������ ���

	// �κ� ��������
	 REQUEST_LOBBYINFO,
	 NOTIFY_USERLIST , 

	 NOTIFY_ROOMMANAGER ,
	 NOTIFY_MAPCHANGE , 
};

enum
{
	// ANSWER_LOGIN
	SUCCESS_LOGIN
};