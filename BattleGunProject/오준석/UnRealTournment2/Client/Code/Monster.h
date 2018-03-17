#pragma once
#include "LandObject.h"

namespace Engine
{
	class CResourceMgr;
	class CStaticMesh;
	class CTexture;
	class CNaviCell;
	class CCubeTex;
}

class CMonster :
	public CLandObject
{
private:
	explicit CMonster(Engine::MYGDI* pMyGDI);
public:
	virtual ~CMonster();

public:
	virtual int Update(void);
	virtual void Render(void);

private:
	void KeyCheck(void);
	bool CheckLocation(Engine::CNaviCell* pCell);
	bool CheckNeighborCell(void);
	void Sliding(void);

private:
	HRESULT Initialize(void);
	HRESULT AddComponent(void);

public:
	static CMonster* Create(Engine::MYGDI* pMyGDI);

private:
	Engine::CResourceMgr*	m_pResourceMgr;
	Engine::CStaticMesh*	m_pMesh;
	Engine::CNaviCell*		m_pMyCell;

private:
	XMFLOAT3	m_vPrePos;
};

