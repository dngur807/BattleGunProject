#pragma once

#include "Include.h"
#include "NetStruct.h"
class CPlayer;

class CUser
{
public:
	CUser();
	~CUser();
public:
	USERINFO&  GetUserInfo()
	{
		return m_tUserInfo;
	}
	void		SetUserInfo(USERINFO tUserInfo)
	{
		m_tUserInfo = tUserInfo;
	}
public:
	void SetCharacter(eCharType eType = CHAR_DEFAULT);
	void SetDead();
	void NullCharacter();
	CPlayer*		GetCharacter();
protected:
	USERINFO			m_tUserInfo;
	CPlayer*			m_pPlayer;
};