#pragma once
#include "Enum.h"

typedef struct tagGameInfo//게임 매니저가 들고 있음
{
	eMapType		m_eMapType;
}GAMEINFO;


typedef struct tagWeapon1stInfo
{
	float fPutDownTime;
	float fRepeatTimeLeft;	//연사 속도
	float fRepeatTimeRight;


	//애니메이션 인덱스 정보
	int   iIdleIndex;
	int	  iSelectIndex;
	int   iPutDownIndex;

	int   iUserIndex;
}WEAPON1ST;

typedef struct tagCharInfo
{
	int		iTeam;
	int		iHp;
	int		iUserIndex;
	int		iSelectWeapon;
	bool	bJump;
	CHARSTATE	eState;
	tagCharInfo()
	{
		eState = CHARSTATE_ALIVE;
		iHp = 100;
		iSelectWeapon = 1;
		iUserIndex = 0;
		bJump = false;
	}
}CHARINFO;

typedef struct tagCharInput
{
	unsigned int iKeyMask;
	int			 iNextWeapon;

	tagCharInput(void)
	{
		iKeyMask = 0;
		iNextWeapon = 1;
	}
}CHARINPUT;

