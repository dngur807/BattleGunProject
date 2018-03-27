#pragma once
#include "Scene.h"
#include "..\..\Engine\Headers\LightHelper.h"
class CLoading;

namespace Engine
{
	class CComponent;
};

class CIngame :
	public Engine::CScene
{
public:
	enum LAYERID { LAYER_ENVIRONMENT, LAYER_GAMELOGIC, LAYER_UI };

public:
	explicit CIngame(Engine::MYGDI* pMyGDI);
	virtual ~CIngame();
public:
	virtual HRESULT InitScene();
	virtual int Update();
	virtual void Render();
	static CIngame* Create(Engine::MYGDI* pMyGDI);
private:
	HRESULT Add_Environment_Layer(void);
	HRESULT Add_GameLogic_Layer(void);
	HRESULT Add_UI_Layer(void);
	HRESULT Add_Light();
	void AnalysisInterMsg();
	void Release();
private:
	CLoading*					m_pLoading;// 네트워크 안켰을 경우
	DirectionalLight m_DirLight;
	ID3DX11Effect*	m_pFX;

};

