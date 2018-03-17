#pragma once
#include "LandObject.h"

namespace Engine
{
	class CResourceMgr;
	class CWavesTex;
	class CTexture;
	class CVIBuffer;
	//class CRain;
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
	//Engine::CRainTex* m_pBuffer;
	Engine::CVIBuffer*		m_pBuffer;
	Material		m_tMtrl;
	Engine::CWavesTex* mRain;
	float fTime;
	//
	ID3DX11EffectVariable* mfxMaterial;
	ID3DX11EffectVariable* mfxDirLight;
	ID3DX11Effect*	m_pFX;
	DirectionalLight mDirLight;

};

