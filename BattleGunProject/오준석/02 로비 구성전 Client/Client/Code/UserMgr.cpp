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

}

void CUserMgr::DivideTeam()
{

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

