#pragma once
#include "GameObject.h"

namespace Engine
{
	class CResourceMgr;
	class CStaticMesh;
	class CTexture;
	class CNaviCell;
	class CCubeTex;
	class CTransform;
}

class CDynamicCamera;
class CStone :
	public  Engine::CGameObject
{
private:
	explicit CStone(Engine::MYGDI* pMyGDI);
public:
	virtual ~CStone();

public:
	virtual int Update(void);
	virtual void Render(void);
	void GetStoneY(float pFloat);

private:
	void KeyCheck(void);
	bool CheckLocation(Engine::CNaviCell* pCell);
	bool CheckNeighborCell(void);
	void Sliding(void);

private:
	HRESULT Initialize(void);
	HRESULT AddComponent(void);

public:
	static CStone* Create(Engine::MYGDI* pMyGDI, CDynamicCamera*	pDynamicCamera);

private:
	Engine::CResourceMgr*	m_pResourceMgr;
	Engine::CStaticMesh*	m_pMesh;
	Engine::CNaviCell*		m_pMyCell;

private:
	XMFLOAT3	m_vPrePos;
	Engine::CTransform*		m_pInfo;
	CDynamicCamera* m_pDynamicCamera;
};

