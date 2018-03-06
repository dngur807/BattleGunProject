#pragma once

#include "Collision.h"

namespace Engine
{
	class CTransform;
}

class CObjectObserver;

class CObjectCol :
	public Engine::CCollision
{
private:
	CObjectCol(Engine::MYGDI* pDevice);
public:
	~CObjectCol(void);

private:
	HRESULT Initialize(void);
	DWORD Release(void);

public:
	virtual int Update(void);

public:
	bool CheckClsn(Engine::CTransform* pDstInfo, Engine::CTransform* pSrcInfo);

public:
	virtual Engine::CCollision* Clone(void);

public:
	static CObjectCol* Create(Engine::MYGDI* pDevice);

private:
	Engine::MYGDI*			m_pMyGDI;
private:
	Engine::CTransform*		m_pPlayerInfo;
	CObjectObserver*		m_pObjectObserver;
};
