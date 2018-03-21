#pragma once
#include "Scene.h"
#include "Include.h"
#include "ReactUI_2D.h"

namespace Engine
{
	class CResourceMgr;
	class CManagement;
}
class CLoading;
class CIngame;

class CScene_Loading :
	public Engine::CScene
{
public:
	enum LAYERID { LAYER_UI };
private:
	explicit CScene_Loading(Engine::MYGDI* pMyGDI);
public:
	virtual ~CScene_Loading();

public:
	virtual HRESULT InitScene();
	virtual int Update();
	virtual void Render();
	static CScene_Loading* Create(Engine::MYGDI* pMyGDI);

private:
	HRESULT LodingTexture();
	HRESULT	Add_UI_Layer(void);
	void AnalysisInterMsg();
	void CreateIngame();
	void Release();
private:

	Engine::CResourceMgr*		m_pResourceMgr;
	Engine::CManagement*		m_pManagement;
	CReactUI_2D*				m_pLoadingUI;
	CLoading*					m_pLoading;
	float				m_fLogoAngle;
	eMapType			m_eMapType;
	bool				m_bLoadingEnd;
	Engine::MYGDI*		m_pMyGDI;
	CIngame*			m_pIngame;
};

