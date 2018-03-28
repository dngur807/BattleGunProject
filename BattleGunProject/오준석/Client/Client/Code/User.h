#pragma once

#include "Include.h"
#include "NetStruct.h"

class CCharacter;

class CUser
{
public:
	CUser();
	~CUser();
public:
	virtual void Update();

	USERINFO&  GetUserInfo()
	{
		return m_tUserInfo;
	}
	void		SetUserInfo(USERINFO tUserInfo)
	{
		m_tUserInfo = tUserInfo;
	}
public:
	void SetCharacter(CCharacter* pCharacter);
	void SetDead();
	void SetInput(UINT iInput);
	void NullCharacter();

	CCharacter*		GetCharacter();
protected:
	USERINFO			m_tUserInfo;
	CCharacter*			m_pCharacter;
	CHARINPUT			m_tCharInput;
};