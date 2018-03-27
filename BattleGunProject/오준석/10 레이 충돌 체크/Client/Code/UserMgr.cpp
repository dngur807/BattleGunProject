#include "stdafx.h"
#include "UserMgr.h"
#include "User.h"
#include "Me.h"

IMPLEMENT_SINGLETON(CUserMgr)

CUserMgr::CUserMgr()
{
	m_pMe = NULL;
	m_mapUserList.clear();
	m_mapATeam.clear();
	m_mapBTeam.clear();

	m_pMe = new CMe;
}

CUserMgr::~CUserMgr()
{
	if (m_pMe)
		delete m_pMe;
}

CUser* CUserMgr::GetUser(int iIndex)
{
	auto iter = m_mapUserList.find(iIndex);

	if (iter != m_mapUserList.end())
	{
		return iter->second;
	}

	return NULL;
}

void CUserMgr::Update()
{
	auto iter = m_mapUserList.begin();
	auto iter_end = m_mapUserList.end();

	for (iter; iter != iter_end; iter++)
	{
		(iter->second)->Update();
	}
}

void CUserMgr::DivideTeam()
{
	m_mapATeam.clear();
	m_mapBTeam.clear();
	map<int, CUser*>::iterator iter = m_mapUserList.begin();
	map<int, CUser*>::iterator iter_end = m_mapUserList.end();

	for (iter; iter != iter_end; iter++)
	{
		if (iter->second->GetUserInfo().eTeam == TEAM_A)
		{
			m_mapATeam.insert(make_pair(iter->first, iter->second));
		}
		else if (iter->second->GetUserInfo().eTeam == TEAM_B)
		{
			m_mapBTeam.insert(make_pair(iter->first, iter->second));
		}
	}
}

void CUserMgr::Reset()
{
	m_mapATeam.clear();
	m_mapBTeam.clear();

	map<int, CUser*>::iterator iter = m_mapUserList.begin();
	map<int, CUser*>::iterator iter_end = m_mapUserList.end();

	for (iter; iter != iter_end; iter++)
	{
		//나중에 적자
	}


}

void CUserMgr::Release()
{
	map<int, CUser*>::iterator iter = m_mapUserList.begin();
	map<int, CUser*>::iterator iter_end = m_mapUserList.end();

	for (iter; iter != iter_end; iter++)
	{
		Engine::Safe_Delete(iter->second);
	}

	m_mapUserList.clear();
}

