enum eProtocolType
{
	REQUEST_LOGIN = 0,
	ANSWER_LOGIN, // ���
	NOTIFY_YOURINFO, // �ε��� �� ������ ���

	 // �κ� ��������
	 REQUEST_LOBBYINFO,
	 NOTIFY_USERLIST,

	 NOTIFY_ROOMMANAGER,

	 REQUEST_CHANGETEAM ,//index , team
	 NOTIFY_CHANGETEAM , 

	 REQUEST_MAPCHANGE,// maptype
	 NOTIFY_MAPCHANGE,

	 REQUEST_ALLCHAT,
	 // msgLen(1), msg(text) : �޽����� 256 �� �̳��� �ǰ���...
	 NOTIFY_ALLCHAT,
	 // msgLen(1), msg(text) : �޽����� 256 �� �̳��� �ǰ���...
	 REQUEST_GAMESTART,
	 NOTIFY_GAMELOADING,


	 // �ΰ��� ��������
};

enum
{
	// ANSWER_LOGIN
	SUCCESS_LOGIN
};