#pragma once
#include "LandObject.h"

namespace Engine
{
	class CStaticMesh;
	class CResourceMgr;
};

class CGestalt
	: public CLandObject
{
private:
	CGestalt(Engine::MYGDI* pMyGDI);
public:
	virtual ~CGestalt();
public:
	virtual HRESULT Initialize();
	virtual int Update(void);
	virtual void Render(void);

	
	static CGestalt* Create(Engine::MYGDI* pMyGDI);
private:
	HRESULT AddComponent(void);
private:
	Engine::CResourceMgr*	m_pResourceMgr;
private:
	Engine::CStaticMesh*	m_pP[9];
	Engine::CStaticMesh*	m_pE[2];
	Engine::CStaticMesh*	m_pMassL;
	Engine::CStaticMesh*	m_pSmallL;
	Engine::CStaticMesh*	m_pBlackBox;
private:
	//Material		m_tMtrl;
	//DirectionalLight			m_DirLight;

};
