enum eProtocolType
{
	REQUEST_LOGIN = 0,
	ANSWER_LOGIN, // ���
	NOTIFY_YOURINFO, // �ε��� �� ������ ���

	// �κ� ��������
	REQUEST_LOBBYINFO,
	NOTIFY_USERLIST,

	NOTIFY_ROOMMANAGER,
	NOTIFY_MAPCHANGE,

	REQUEST_CHANGETEAM,//index , team
	NOTIFY_CHANGETEAM,

	REQUEST_ALLCHAT,
	// msgLen(1), msg(text) : �޽����� 256 �� �̳��� �ǰ���...
	NOTIFY_ALLCHAT
	// msgLen(1), msg(text) : �޽����� 256 �� �̳��� �ǰ���...

};

enum
{
	// ANSWER_LOGIN
	SUCCESS_LOGIN
};