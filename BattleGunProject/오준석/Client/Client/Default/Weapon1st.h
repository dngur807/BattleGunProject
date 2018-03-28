#pragma once
#include "LandObject.h"
#include "Include.h"


namespace Engine
{
	class CResourceMgr;
	class CDynamicMesh;
	class CStaticMesh;
};
class CCharacter;
class CWeapon1st
	: public CLandObject
{
protected:
	CWeapon1st(Engine::MYGDI* pMyGDI, CCharacter* pMyPlayer);
public:
	virtual ~CWeapon1st();

public:
	virtual HRESULT Initialize();
	virtual int Update();
	virtual void Render();

	virtual float PutDown();
	virtual void Select();
	virtual void LeftAction()PURE;
	virtual void RightAction()PURE;
	virtual void NoAction();

	WEAPON1ST* GetWeaponInfo() { return &m_tInfo; }

protected:
	Engine::CDynamicMesh*		m_pMesh;
	Engine::CResourceMgr*		m_pResourceMgr;

	DWORD						m_dwIndex;
	DWORD						m_dwAniIndex;
	DWORD						m_dwPrevAniIndex;
	float*						m_fPeriodTime;

	float						m_fRepeatTimeLeft;
	float						m_fRepeatTimeRight;

	float						m_fSpeed;
	WEAPON1ST					m_tInfo;

	const XMFLOAT4X4*			m_pFlashPoint;
	bool						m_bPush;

	CCharacter*					m_pMyPlayer;
};