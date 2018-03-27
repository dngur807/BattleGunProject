#pragma once
#include "Component.h"

namespace Engine
{
	class CTransform;
	class CSphereTex;
	class CTexture;
	class CResourceMgr;


}

class CSphereCol
	: public Engine::CComponent
{
private:
	explicit CSphereCol(Engine::MYGDI* pMyGDI);

public:
	virtual ~CSphereCol();
public:
	void SetTargetInfo(const Engine::CTransform* pTargetInfo);
public:
	static  CSphereCol* Create(Engine::MYGDI* pMyGDI , float fRad);

public:
	void Initialize(float fRad);
	virtual int Update();
	void Render();
	virtual DWORD Release();
	float GetRadian() { return m_fRad; }
	XMFLOAT3* GetPos() { return &m_vPos; }
private:
	Engine::MYGDI*					m_pMyGDI;
	const Engine::CTransform*		m_pTargetInfo;
	XMFLOAT3						m_vPos;

	Engine::CResourceMgr* m_pResourceMgr;
	Engine::CTexture* m_pTexture;
	Engine::CSphereTex* m_pBuffer;
	Material				m_tMtrl;
	float					m_fRad;

};