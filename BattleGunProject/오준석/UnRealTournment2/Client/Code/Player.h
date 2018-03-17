/*!
* \file Player.h
* \date 2017/06/09 15:30
*
* \author Administrator
* Contact: user@company.com
*
* \brief
*
* TODO: long description
*
* \note
*/

#ifndef Player_h__
#define Player_h__

#include "LandObject.h"

namespace Engine
{
	class CResourceMgr;
	class CVIBuffer;
	class CTransform;
	class CDynamicMesh;
	class CNaviCell;
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
	void Jump();
public:
	//มกวม
	bool			m_bJump;
	bool			m_bDown;
	float			m_fJumpAcc;
	float           m_fSkillJumpAcc;
	float			m_fTop;

public:
	static CPlayer* Create(Engine::MYGDI* pMyGDI);
	void MyFrame();

private:
	Engine::CResourceMgr*	m_pResourceMgr;
	Engine::CVIBuffer*		m_pBoxBuffer;

private:
	Engine::CDynamicMesh*	m_pMesh;
	Engine::CDynamicMesh*	m_pMesh1;

private:
	bool		m_bPush;
	UINT		m_iAniIndex;
	float		m_fSpeed;
	float       m_fTime = 0.f;
private:
	CGameObject*	m_pBow;
	DWORD		m_dwIndex;
	Engine::CNaviCell*		m_pMyCell;

	POINT		m_pMouse, m_pMovept;
	DirectionalLight mDirLight;
	DWORD	m_dwState;
	DWORD	m_dwPrevState;


};


#endif // Player_h__