/*!
* \file Stage.h
* \date 2015/04/03 0:13
*
* \author Administrator
* Contact: user@company.com
*
* \brief 스테이지
*
* TODO: long description
*
* \note
*/
#ifndef Stage_h__
#define Stage_h__

#include "Scene.h"
#include "Include.h"

namespace Engine
{
	class CGameObject;
	class CComponent;
};

class CLoading;
class CObjectCol;
class CPlayer;
class CStage
	: public Engine::CScene
{
public:
	enum LAYERID { LAYER_ENVIRONMENT, LAYER_GAMELOGIC, LAYER_UI, LAYER_PARTICLE};

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
	HRESULT Add_Particle(void);
	HRESULT Add_ETC(void);

public:
	static CStage* Create(Engine::MYGDI* pMyGDI);

private:
	DirectionalLight m_DirLight;
	CObjectCol*		m_pObjectCol;
	//네비메쉬
	vector<XMFLOAT3>     m_vecSaveMesh; // 저장용
	vector<int>				m_vecSaveOption; // 저장용
	int m_iOption;
	int m_iCount;
	void CheckClockWise(XMFLOAT3 vecNavMesh[3]);
	const Engine::CComponent*	m_pTarget;
	bool m_bWire;
	Engine::CGameObject*	m_pCamera = nullptr;
	CLoading*		m_pLoading;
	CPlayer*					m_pPlayer = nullptr;

};

#endif // Stage_h__