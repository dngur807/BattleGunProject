#pragma once
#include "GameObject.h"

namespace Engine
{
	class CTransform;
}

class CObjectObserver;

class CLandObject :
	public Engine::CGameObject
{
protected:
	explicit CLandObject(Engine::MYGDI* pMyGDI);
public:
	virtual ~CLandObject();

public:
	virtual int Update(void) PURE;
	virtual void Render(void) PURE;

protected:
	BoundingBox	m_BoundingBox;
protected:
	Engine::CTransform*		m_pInfo;
	CObjectObserver*		m_pObjectObserver;

};

