#pragma once
#include "Bullet.h"

namespace Engine
{
	class CResourceMgr;
	class CRocketLauncherBullet1Tex;
	class CTexture;
	class CSphereTex;
	class CVIBuffer;
	class ParticleSystem;
}

class CSphereCol;
class CRocketLauncherBullet1 :
	public CBullet
{
private:
	explicit CRocketLauncherBullet1(Engine::MYGDI* pMyGDI, XMFLOAT3 vPos, XMFLOAT3 vDir, int iIndex);
public:
	virtual ~CRocketLauncherBullet1();

public:
	virtual HRESULT Initialize(void);
	virtual int Update(void);
	virtual void Render(void);

private:
	HRESULT AddComponent(void);

public:
	static CRocketLauncherBullet1* Create(Engine::MYGDI* pMyGDI, XMFLOAT3 vPos, XMFLOAT3 vDir, int iIndex);

private:
	Engine::CResourceMgr* m_pResourceMgr;
	Engine::CTexture* m_pTexture;
	Engine::CRocketLauncherBullet1Tex* m_pBuffer;
	Engine::CSphereTex* m_pTest;
	Engine::CVIBuffer* m_FireParticleBuffer;
	CSphereCol*					m_pSphereCol;
	ID3D11RasterizerState*      m_pWire;
	Material		m_tMtrl;
	float mAge = 0.f;
	ID3D11ShaderResourceView* mTexArraySRV;
	ID3D11ShaderResourceView* mRandomTexSRV;
	//
	const XMFLOAT4X4*		m_pmatWeaponInfo;
	const XMFLOAT4X4*		m_pmatWeaponRef;

	float					m_fTime;
	XMFLOAT4X4				m_matWorld;
	XMFLOAT3				m_vPos;
	XMFLOAT3				m_vDir;
	XMFLOAT3				m_vScale;


};

