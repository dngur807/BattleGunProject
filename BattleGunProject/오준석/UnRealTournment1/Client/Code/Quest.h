/*!
* \file Quest.h
* \date 2015/04/03 0:04
*
* \author Administrator
* Contact: user@company.com
*
* \brief �ΰ�
*
* TODO: long description
*
* \note
*/
#ifndef Quest_h__
#define Quest_h__

#include "Scene.h"

namespace Engine
{
	class CResourceMgr;
	class CManagement;
}

class CLoading;

class CQuest
	: public Engine::CScene
{
public:
	//�� ���� �ʿ��� ���̾ �����Ѵ�.
	enum LAYERID { LAYER_ENVIRONMENT, LAYER_GAMELOGIC };

private:
	explicit CQuest(Engine::MYGDI* pMyGDi);

public:
	virtual ~CQuest(void);

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
	static CQuest* Create(Engine::MYGDI* pMyGDi);


private:
	Engine::CResourceMgr*	m_pResourceMgr;
	Engine::CManagement*	m_pManagement;

private:
	CLoading*		m_pLoading;
};

#endif // Quest_h__