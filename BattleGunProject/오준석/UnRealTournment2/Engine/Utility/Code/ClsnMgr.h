#pragma once

#include "Engine_Include.h"

BEGIN(Engine)

class CCollision;
class CComponent;

class ENGINE_DLL CClsnMgr
{
	DECLARE_SINGLETON(CClsnMgr)

private:
	CClsnMgr();
	~CClsnMgr();

private:
	void Release(void);

public:
	HRESULT AddClsnObject(COLLISIONID eCollisionID, CComponent* pCollision);
	CCollision* CloneClsnObject(COLLISIONID eCollisionID);

private:
	typedef map<COLLISIONID, Engine::CComponent*> MAPCOLLISION;
	MAPCOLLISION	m_mapCollision;
};

END