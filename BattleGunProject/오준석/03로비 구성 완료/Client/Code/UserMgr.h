#pragma once

#include "Engine_Include.h"

class CUser;
class CMe;

class CUserMgr
{
public :
	DECLARE_SINGLETON(CUserMgr)
public:
	CUserMgr();
	~CUserMgr();

public:
	CMe*			GetMe()
	{
		return m_pMe;
	}
	map<int, CUser*>*  GetUserList(void)
	{
		return &m_mapUserList;
	}

	map<int, CUser*>*  GetATeam(void)
	{
		return &m_mapATeam;
	}

	map<int, CUser*>*  GetBTeam(void)
	{
		return &m_mapBTeam;
	}

	map<int, CUser*>*  GetOtherTeam(void)
	{
		/*if (m_pMe->GetUserInfo().m_eTeam == TEAM_A)
			return GetBTeam();
		return GetATeam();*/
	}
public:
	void DivideTeam();
	void Reset();
private:
	void Release();
private:
	CMe*						m_pMe;
	map<int, CUser*>			m_mapUserList;
	map<int, CUser*>			m_mapATeam;
	map<int, CUser*>			m_mapBTeam;

	
};
