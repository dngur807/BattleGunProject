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
	virtual HRESULT Initialize();
	virtual int Update(void);
	virtual void Render(void);

	void SetPos(XMFLOAT3 vPos);
	void SetScale(XMFLOAT3 vScale);
protected:
	BoundingBox	m_BoundingBox;
protected:
	Engine::CTransform*		m_pInfo;
	CObjectObserver*		m_pObjectObserver;

};

