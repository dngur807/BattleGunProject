#pragma once
#include "LandObject.h"

namespace Engine
{
	class CResourceMgr;
	class CFireTex;
	class CTexture;
	class CSphereTex;
	class CVIBuffer;
	class ParticleSystem;
}

class CFire :
	public CLandObject
{
private:
	explicit CFire(Engine::MYGDI* pMyGDI, XMFLOAT3& vPos, XMFLOAT3& vDir, const XMFLOAT4X4* matWeaponInfo, const XMFLOAT4X4* matWeaponRef);
public:
	virtual ~CFire();

public:
	virtual int Update(void);
	virtual void Render(void);

private:
	HRESULT Initialize(void);
	HRESULT AddComponent(void);

public:
	static CFire* Create(Engine::MYGDI* pMyGDI, XMFLOAT3& vPos, XMFLOAT3& vDir, const XMFLOAT4X4* matWeaponInfo, const XMFLOAT4X4* matWeaponRef);

private:
	Engine::CResourceMgr* m_pResourceMgr;
	Engine::CTexture* m_pTexture;
	Engine::CFireTex* m_pBuffer;
	Engine::CSphereTex* m_pTest;
	Engine::CVIBuffer* m_FireParticleBuffer;
	ID3D11RasterizerState*      m_pWire;
	float mAge = 0.f;
	ID3D11ShaderResourceView* mTexArraySRV;
	ID3D11ShaderResourceView* mRandomTexSRV;
	//
	const XMFLOAT4X4*		m_pmatWeaponInfo;
	const XMFLOAT4X4*		m_pmatWeaponRef;
};

