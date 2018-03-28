#pragma once
#include "Enum.h"

typedef struct tagGameInfo//���� �Ŵ����� ��� ����
{
	eMapType		m_eMapType;
}GAMEINFO;


typedef struct tagWeapon1stInfo
{
	float fPutDownTime;
	float fRepeatTimeLeft;	//���� �ӵ�
	float fRepeatTimeRight;


	//�ִϸ��̼� �ε��� ����
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

