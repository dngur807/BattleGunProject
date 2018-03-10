#pragma once
#include "LandObject.h"

namespace Engine
{
	class CResourceMgr;
	class CWavesTex;
	class CTexture;
	class CVIBuffer;
	//class CSkyBox1;
}
class CSkyBox1 :
	public CLandObject
{
private:
	explicit CSkyBox1(Engine::MYGDI* pMyGDI);
public:
	virtual ~CSkyBox1();

public:
	virtual int Update(void);
	virtual void Render(void);

private:
	HRESULT Initialize(void);
	HRESULT AddComponent(void);

public:
	static CSkyBox1* Create(Engine::MYGDI* pMyGDI);

private:
	Engine::CResourceMgr* m_pResourceMgr;
	Engine::CTexture* m_pTexture;
	//Engine::CSkyBox1Tex* m_pBuffer;
	Engine::CVIBuffer*		m_pBuffer;
	Material		m_tMtrl;
	Engine::CWavesTex* mSkyBox1;
	float fTime;
	//
	ID3DX11EffectVariable* mfxMaterial;
	ID3DX11EffectVariable* mfxDirLight;
	ID3DX11Effect*	m_pFX;
	DirectionalLight mDirLight;

};

