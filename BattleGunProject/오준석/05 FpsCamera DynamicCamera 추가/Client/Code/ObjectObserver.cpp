#include "StdAfx.h"
#include "ObjectObserver.h"

#include "Include.h"
#include "Export_Function.h"

CObjectObserver::CObjectObserver(void)
	:m_pInfolist(NULL),
	m_pPlayerInfo(NULL)
{
}

CObjectObserver::~CObjectObserver(void)
{
}

int CObjectObserver::Update(int iMsg)
{
	const list<void*>* pDatalist = Engine::Get_InfoSubject()->GetDatalist(iMsg);
	if (pDatalist == NULL)
		return -1;

	switch (iMsg)
	{
	case MSG_INFOLIST:
		m_pInfolist = (const list<Engine::CTransform*>*)pDatalist;
		break;
	case MSG_PLAYERINFO:
		m_pPlayerInfo = S_CAST(const Engine::CTransform*, pDatalist->front());
		break;
	}

	return 0;
}

CObjectObserver* CObjectObserver::Create(void)
{
	return new CObjectObserver;
}
