#pragma once
#include "LandObject.h"

namespace Engine
{
	class CResourceMgr;
	class CWavesTex;
	class CTexture;
	class CVIBuffer;
	//class CWaves;
}
class CWaves :
	public CLandObject
{
private:
	explicit CWaves(Engine::MYGDI* pMyGDI);
public:
	virtual ~CWaves();

public:
	virtual int Update(void);
	virtual void Render(void);

private:
	HRESULT Initialize(void);
	HRESULT AddComponent(void);

public:
	static CWaves* Create(Engine::MYGDI* pMyGDI);

private:
	Engine::CResourceMgr* m_pResourceMgr;
	Engine::CTexture* m_pTexture;
	//Engine::CWavesTex* m_pBuffer;
	Engine::CVIBuffer*		m_pBuffer;
	Material		m_tMtrl;
	Engine::CWavesTex* mWaves;
	float fTime;
	//
	ID3DX11EffectVariable* mfxMaterial;
	ID3DX11EffectVariable* mfxDirLight;
	ID3DX11Effect*	m_pFX;
	DirectionalLight mDirLight;

};

