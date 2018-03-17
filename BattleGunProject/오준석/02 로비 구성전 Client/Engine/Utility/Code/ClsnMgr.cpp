#include "ClsnMgr.h"
#include "Collision.h"

IMPLEMENT_SINGLETON(Engine::CClsnMgr)

Engine::CClsnMgr::CClsnMgr()
{
}


Engine::CClsnMgr::~CClsnMgr()
{
	Release();
}

void Engine::CClsnMgr::Release(void)
{
	for_each(m_mapCollision.begin(), m_mapCollision.end(), CDeleteMap());
	m_mapCollision.clear();
}

HRESULT Engine::CClsnMgr::AddClsnObject(COLLISIONID eCollisionID,
	CComponent* pCollision)
{
	if (m_mapCollision.insert(MAPCOLLISION::value_type(eCollisionID, pCollision)).second)
		return S_OK;
	else
		return E_FAIL;
}

Engine::CCollision * Engine::CClsnMgr::CloneClsnObject(COLLISIONID eCollisionID)
{
	MAPCOLLISION::iterator iter = m_mapCollision.find(eCollisionID);
	if (m_mapCollision.end() == iter)
		return NULL;

	return dynamic_cast<CCollision*>(iter->second)->Clone();
}
