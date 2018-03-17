#pragma once

#include "Include.h"
#include "LightHelper.h"
#include "Engine_Struct.h"

namespace Engine
{
	class CGraphicDev;
	class CManagement;
	class CResourceMgr;
	class CTimeMgr;

	class CVIBuffer;
	class CBasicEffect;
}

class CMainApp
{
private:
	CMainApp();
public:
	~CMainApp();

public:
	HRESULT InitApp(void);

private:
	void Release(void);

public:
	int Update(float& fTime);
	void Render(void);

public:
	static CMainApp* Create(void);

public:

private:
	Engine::CGraphicDev*	m_pGraphicDev;
	Engine::CManagement*	m_pManagement;
	Engine::CResourceMgr*	m_pResourceMgr;
	Engine::CTimeMgr*		m_pTimeMgr;

private:
	Engine::MYGDI*			m_pMyGDI;
	ID3DX11Effect*	m_pFX;

private: // ±¤¿ø(LightHelper)
	/*DirectionalLight mDirLight;
	PointLight mPointLight;
	SpotLight mSpotLight;
	Material mLandMat; 
	XMFLOAT3 mEyePosW;*/
};

