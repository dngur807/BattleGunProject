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
	HRESULT Add_NaviMesh(void);

	void AnalysisInterMsg();
	void Release();
private:
	CLoading*					m_pLoading;// ��Ʈ��ũ ������ ���
	DirectionalLight m_DirLight;
	ID3DX11Effect*	m_pFX;


	//�׺�޽�
	vector<XMFLOAT3>     m_vecSaveMesh; // �����
	vector<int>				m_vecSaveOption; // �����
	int m_iOption;
	int m_iCount;
	void CheckClockWise(XMFLOAT3 vecNavMesh[3]);

};

