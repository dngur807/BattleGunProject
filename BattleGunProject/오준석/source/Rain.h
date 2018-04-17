#pragma once
#include "LandObject.h"

namespace Engine
{
	class CResourceMgr;
	class CRainTex;
	class CTexture;
	class CSphereTex;
	class CVIBuffer;
	//class ParticleSystem;
}

class CRain :
	public CLandObject
{
private:
	explicit CRain(Engine::MYGDI* pMyGDI);
public:
	virtual ~CRain();

public:
	virtual int Update(void);
	virtual void Render(void);

private:
	HRESULT Initialize(void);
	HRESULT AddComponent(void);

public:
	static CRain* Create(Engine::MYGDI* pMyGDI);

private:
	Engine::CResourceMgr* m_pResourceMgr;
	Engine::CTexture* m_pTexture;
	Engine::CRainTex* m_pBuffer;
	Engine::CSphereTex* m_pTest;
	Engine::CVIBuffer* m_RainParticleBuffer;
	ID3D11RasterizerState*      m_pWire;
	float mAge = 0.f;
	ID3D11ShaderResourceView* mTexArraySRV;
	ID3D11ShaderResourceView* mRandomTexSRV;
	//
};

