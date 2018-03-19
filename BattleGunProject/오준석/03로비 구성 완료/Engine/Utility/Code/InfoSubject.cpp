#include "InfoSubject.h"

IMPLEMENT_SINGLETON(Engine::CInfoSubject)

Engine::CInfoSubject::CInfoSubject()
{
}


Engine::CInfoSubject::~CInfoSubject()
{
	Release();
}

void Engine::CInfoSubject::Release(void)
{
	for (MAPDATALIST::iterator iter = m_mapDatalist.begin();
		iter != m_mapDatalist.end(); ++iter)
	{
		iter->second.clear();
	}
	m_mapDatalist.clear();
}

const list<void*> * Engine::CInfoSubject::GetDatalist(int iMsg)
{
	MAPDATALIST::iterator iter = m_mapDatalist.find(iMsg);

	if (m_mapDatalist.end() != iter)
		return &iter->second;

	return NULL;
}

void Engine::CInfoSubject::AddData(int iMsg, void * pData)
{
	if (pData)
	{
		m_mapDatalist.insert(MAPDATALIST::value_type(iMsg, DATALIST()));
		m_mapDatalist[iMsg].push_back(pData);
		Notify(iMsg);
	}
}

void Engine::CInfoSubject::RemoveData(int iMsg, void * pData)
{
	MAPDATALIST::iterator mapiter = m_mapDatalist.find(iMsg);

	if (m_mapDatalist.end() != mapiter)
	{
		DATALIST::iterator iterlist = mapiter->second.begin();
		DATALIST::iterator iterlist_end = mapiter->second.end();

		for (; iterlist != iterlist_end; ++iterlist)
		{
			if ((*iterlist) == pData)
			{
				mapiter->second.erase(iterlist);
				return;
			}
		}
	}
}
