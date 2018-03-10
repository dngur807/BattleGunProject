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

	Engine::CTransform* GetTransform() { return m_pInfo; }
protected:
	BoundingBox	m_BoundingBox;
protected:
	Engine::CTransform*		m_pInfo;
	CObjectObserver*		m_pObjectObserver;
	float					m_fSpeed;
	float					fTime;
	ID3D11RasterizerState*      m_pWire;

};

