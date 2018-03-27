

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
class CWeapon1st;
class CWeapon3rd;

class CPlayer
	: public CLandObject
{
public:
	enum ANI_ID { ANI_IDLE = 38 };
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
	HRESULT AddWeapon();
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
	CWeapon1st*			m_pSelectedWeapon;//들고 있는 무기
	vector<CWeapon1st*>	m_vecWeapon1st;//무기 리스트
	CWeapon3rd*			m_pWeapon3rd;
};


#endif // Player_h__