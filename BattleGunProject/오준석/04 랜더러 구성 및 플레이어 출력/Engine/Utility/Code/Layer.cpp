#include "Layer.h"
#include "GameObject.h"

Engine::CLayer::CLayer(MYGDI* pMyGDI)
	: m_pMyGDI(pMyGDI)
{
	m_mapObjlist.clear();
}

Engine::CLayer::~CLayer(void)
{
	Release();
}

HRESULT Engine::CLayer::AddObject(const TCHAR* pObjectKey, CGameObject* pGameObject)
{
	if (pGameObject)
	{
		MAPOBJLIST::iterator	iter = find_if(m_mapObjlist.begin(), m_mapObjlist.end(), CTagFinder(pObjectKey));
		if (iter == m_mapObjlist.end())
		{
			m_mapObjlist[pObjectKey] = OBJECTLIST();
		}
		m_mapObjlist[pObjectKey].push_back(pGameObject);
	}

	return S_OK;
}

int Engine::CLayer::Update(void)
{
	MAPOBJLIST::iterator	iter = m_mapObjlist.begin();
	MAPOBJLIST::iterator	iter_end = m_mapObjlist.end();

	for (; iter != iter_end; ++iter)
	{
		OBJECTLIST::iterator	iterList = iter->second.begin();
		OBJECTLIST::iterator	iterList_end = iter->second.end();

		for (; iterList != iterList_end; ++iterList)
		{
			(*iterList)->Update();
		}
	}

	return 0;
}

void Engine::CLayer::Render(void)
{
	MAPOBJLIST::iterator	iter = m_mapObjlist.begin();
	MAPOBJLIST::iterator	iter_end = m_mapObjlist.end();

	for (; iter != iter_end; ++iter)
	{
		OBJECTLIST::iterator	iterList = iter->second.begin();
		OBJECTLIST::iterator	iterList_end = iter->second.end();

		for (; iterList != iterList_end; ++iterList)
		{
			(*iterList)->Render();
		}
	}
}

Engine::CLayer* Engine::CLayer::Create(MYGDI* pMyGDI)
{
	return new CLayer(pMyGDI);
}

void Engine::CLayer::Release(void)
{
	MAPOBJLIST::iterator	iter = m_mapObjlist.begin();
	MAPOBJLIST::iterator	iter_end = m_mapObjlist.end();

	for (; iter != iter_end; ++iter)
	{
		OBJECTLIST::iterator	iterList = iter->second.begin();
		OBJECTLIST::iterator	iterList_end = iter->second.end();

		for (; iterList != iterList_end; ++iterList)
			Engine::Safe_Delete((*iterList));

		iter->second.clear();
	}
	m_mapObjlist.clear();
}

list<Engine::CGameObject*>* Engine::CLayer::GetObjectList(const TCHAR* pObjKey)
{
	MAPOBJLIST::iterator iter = m_mapObjlist.find(pObjKey);

	if (iter == m_mapObjlist.end())
		return NULL;

	return &iter->second;
}

const Engine::CComponent* Engine::CLayer::GetComponent(const TCHAR* pObjKey, const TCHAR* pComponentKey)
{
	MAPOBJLIST::iterator	mapiter = find_if(m_mapObjlist.begin(), m_mapObjlist.end(), CTagFinder(pObjKey));
	if (mapiter == m_mapObjlist.end())
		return NULL;

	OBJECTLIST::iterator	iterlist = mapiter->second.begin();
	OBJECTLIST::iterator	iterlist_end = mapiter->second.end();

	for (; iterlist != iterlist_end; ++iterlist)
	{
		const CComponent*		pComponent = (*iterlist)->GetComponent(pComponentKey);
		if (pComponent != NULL)
			return pComponent;
	}
	return NULL;
}
