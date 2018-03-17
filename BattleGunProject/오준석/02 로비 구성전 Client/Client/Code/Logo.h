#ifndef Logo_h__
#define Logo_h__

#include "Scene.h"

namespace Engine
{
	class CResourceMgr;
	class CManagement;
}

class CLoading;

class CLogo
	: public Engine::CScene
{
public:
	//각 씬에 필요한 레이어를 정의한다.
	enum LAYERID { LAYER_ENVIRONMENT, LAYER_GAMELOGIC };

private:
	explicit CLogo(Engine::MYGDI* pMyGDi);

public:
	virtual ~CLogo(void);

private:
	HRESULT InitScene(void);
	void Release(void);

public:
	virtual int Update(void);
	virtual void Render(void);

private:
	HRESULT Add_Environment_Layer(void);
	HRESULT Add_GameLogic_Layer(void);
	HRESULT Add_Light(void);

public:
	static CLogo* Create(Engine::MYGDI* pMyGDi);


private:
	Engine::CResourceMgr*	m_pResourceMgr;
	Engine::CManagement*	m_pManagement;

private:
	CLoading*		m_pLoading;
};

#endif // Logo_h__