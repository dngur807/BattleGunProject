
#ifndef Stage_h__
#define Stage_h__

#include "Scene.h"
#include "Include.h"


namespace Engine
{
	//class CStaticMesh;
	class CGameObject;
	class CComponent;
};


class CObjectCol;
class CStage
	: public Engine::CScene
{
public:
	enum LAYERID { LAYER_ENVIRONMENT, LAYER_GAMELOGIC, LAYER_UI };

private:
	CStage(Engine::MYGDI* pMyGDI);

public:
	virtual ~CStage(void);

private:
	HRESULT InitScene(void);
	void	Release(void);

public:
	virtual int Update(void);
	virtual void Render(void);

private:
	HRESULT Add_Environment_Layer(void);
	HRESULT Add_GameLogic_Layer(void);
	HRESULT Add_UI_Layer(void);
	HRESULT	Add_Light(void);
	HRESULT Add_NaviMesh(void);
	HRESULT Add_ETC(void);

public:
	static CStage* Create(Engine::MYGDI* pMyGDI);

private:
	DirectionalLight m_DirLight;
	CObjectCol*		m_pObjectCol;
	//Engine::CComponent*
	//Engine::CGameObject*	m_pTargetPlayer;
	const Engine::CComponent*	m_pTargetPlayer;



	//네비메쉬
	vector<XMFLOAT3>     m_vecSaveMesh; // 저장용
	vector<int>				m_vecSaveOption; // 저장용
	int m_iOption;
	int m_iCount;
	void CheckClockWise(XMFLOAT3 vecNavMesh[3]);
};

#endif // Stage_h__