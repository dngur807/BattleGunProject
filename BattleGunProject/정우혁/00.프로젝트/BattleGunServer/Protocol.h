enum eProtocolType
{
	REQUEST_LOGIN = 0,
	ANSWER_LOGIN, // ���
	NOTIFY_YOURINFO, // �ε��� �� ������ ���

					 // �κ� ��������
					 REQUEST_LOBBYINFO,
					 NOTIFY_USERLIST,

					 NOTIFY_ROOMMANAGER,

					 REQUEST_CHANGETEAM,//index , team
					 NOTIFY_CHANGETEAM,

					 REQUEST_MAPCHANGE,// maptype
					 NOTIFY_MAPCHANGE,

					 REQUEST_ALLCHAT,
					 // msgLen(1), msg(text) : �޽����� 256 �� �̳��� �ǰ���...
					 NOTIFY_ALLCHAT,
					 // msgLen(1), msg(text) : �޽����� 256 �� �̳��� �ǰ���...
					 REQUEST_GAMESTART,
					 NOTIFY_GAMELOADING,

					 REQUEST_NAVIMESH,
					 NOTIFY_NAVIMESH,
					 NOTIFY_NAVIMESH_SIZE,

					 // �ΰ��� ��������
					 REQUEST_LOADINGEND,
					 NOTIFY_LOADINGEND,

					 REQUEST_INPUT,
					 NOTIFY_INPUT,

					 REQUEST_POSDIR,
					 NOTIFY_POSDIR,


					 REQUEST_GIVEDAMAGE,
					 NOTIFY_GIVEDAMAGE,

					 REQUEST_DEAD,
					 NOTIFY_DEAD,

					 REQUEST_HPSYNC,
					 NOTIFY_HPSYNC,

					 REQUEST_REVIVE,
					 NOTIFY_REVIVE,

					 NOTIFY_GAMETIMER,
					 NOTIFY_GAMERESULT,

					 NOTIFY_GOLOBBY,

					 REQUEST_CHANGEWEAPON,
					 NOTIFY_CHANGEWEAPON

};

enum
{
	// ANSWER_LOGIN
	SUCCESS_LOGIN
};