#include "GameObject.h"
#include "Component.h"



Engine::CGameObject::CGameObject(MYGDI* pMyGDI)
	:m_pMyGDI(pMyGDI)
{
	m_mapComponent.clear();
}



Engine::CGameObject::~CGameObject(void)
{
	Release();
}

void Engine::CGameObject::Release(void)
{
	for_each(m_mapComponent.begin(), m_mapComponent.end(), CDeleteMap());
	m_mapComponent.clear();
}
const Engine::CComponent* Engine::CGameObject::GetComponent(const TCHAR* wstrComponentKey)
{
	MAPCOMPONENT::iterator	iter = m_mapComponent.find(wstrComponentKey);
	if (iter == m_mapComponent.end())
		return NULL;

	return iter->second;
}


int Engine::CGameObject::Update(void)
{
	MAPCOMPONENT::iterator	iter = m_mapComponent.begin();
	MAPCOMPONENT::iterator	iter_end = m_mapComponent.end();

	for (; iter != iter_end; ++iter)
	{
		iter->second->Update();
	}

	return 0;
}

