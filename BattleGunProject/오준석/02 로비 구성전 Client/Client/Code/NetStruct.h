#pragma once

enum eTeamType
{
	TEAM_A,
	TEAM_B,
	TEAM_END,
};
enum eCharType
{
	CHAR_DEFAULT,
	CHAR_END,
};


//내부 메시지 구조체
typedef struct tagInterMsgData
{
	short sHeaderType;
	char tcInterMsg[256];
}INTERMSGDATA;


typedef struct tagUserInfo
{
	TCHAR		szID[256];
	eCharType	CharType;
	eTeamType	eTeam;
	int			iKill;
	int			iDeath;
	int			iGiveDamage;// 준 피해량
	int			iGetDamage;	// 받은 피해량
	int			iIndex;
	tagUserInfo()
	{
		CharType = CHAR_END;
		eTeam = TEAM_END;
		iKill = 0;
		iDeath = 0;
		iGiveDamage = 0;
		iGetDamage = 0;
	}
}USERINFO;
