#pragma once
#include "Component.h"

namespace Engine
{
	class CTransform;
	class CDynamicMesh;
	class CSphereTex;
}

class CCharCol
	: public Engine::CComponent
{
private:
	explicit CCharCol(Engine::MYGDI* pMyGDI, const XMFLOAT4X4* pMatPlayer);
public:
	virtual ~CCharCol();
public:
	static CCharCol* Create(Engine::MYGDI* pMyGDI, const XMFLOAT4X4* pMatPlayerInfo, Engine::CDynamicMesh* pCharMesh);
public:
	void Initialize(Engine::CDynamicMesh* pCharMesh);

	virtual int Update();
	void Render();
	virtual DWORD Release();
	XMFLOAT3*	 GetColPos(void) { return m_vSpherePos; };
private:
	Engine::MYGDI*			m_pMyGDI;

	const XMFLOAT4X4*		m_pmatPlayerInfo;
	XMFLOAT3				m_vStartPoint;
	XMFLOAT3				m_vSpherePos[16];
	const XMFLOAT4X4*	    m_pMatPartRef[16];
	Engine::CSphereTex*				m_pSphere[16];


};