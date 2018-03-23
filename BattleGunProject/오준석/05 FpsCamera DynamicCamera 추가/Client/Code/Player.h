

#ifndef Player_h__
#define Player_h__

#include "LandObject.h"

namespace Engine
{
	class CResourceMgr;
	class CVIBuffer;
	class CTransform;
	class CDynamicMesh;
	class CStaticMesh;
};

class CPlayer
	: public CLandObject
{
private:
	CPlayer(Engine::MYGDI* pMyGDI);

public:
	virtual ~CPlayer(void);

public:
	virtual int Update(void);
	virtual void Render(void);

private:
	HRESULT Initialize(void);
	HRESULT AddComponent(void);
	void KeyCheck(void);

public:
	static CPlayer* Create(Engine::MYGDI* pMyGDI);

private:
	Engine::CResourceMgr*	m_pResourceMgr;
	Engine::CVIBuffer*		m_pBoxBuffer;

private:
	Engine::CStaticMesh*	m_pStaticMesh;
	Engine::CDynamicMesh*	m_pMesh;

private:
	bool		m_bPush;
	UINT		m_iAniIndex;
	float		m_fSpeed;

private:
	CGameObject*	m_pBow;
};


#endif // Player_h__