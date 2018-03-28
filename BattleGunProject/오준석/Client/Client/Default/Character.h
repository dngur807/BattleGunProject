#pragma once

#include "LandObject.h"
#include "Include.h"

namespace Engine
{
	class CResourceMgr;
	class CDynamicMesh;
	class CStaticMesh;
	class CNaviCell;

};
class CSphereCol;

class CWeapon3rd;
class CCharCol;

class CCharacter
	: public CLandObject
{
public:
	enum ANI_ID {
		ANI_IDLE = 38, ANI_DOWN = 8, ANI_UP = 7, ANI_LEFT = 6, ANI_RIGHT = 5 ,ANI_HIT = 41 , ANI_DEAD = 53};

protected:
	CCharacter(Engine::MYGDI*	pMyGDI);
public:
	virtual ~CCharacter();
public:
	virtual HRESULT Initialize();
	virtual int Update();
	virtual void Render();
	//virtual void Jump();

	virtual void ApplyInput(CHARINPUT& pInput);


	static CCharacter* Create(Engine::MYGDI* pMyGDI);

	void SetPosDir(XMFLOAT3& vPos, float fAngleX, float fAngleY);

	CHARINFO& GetCharInfo() { return m_tCharInfo; }
	CSphereCol* GetSphereCol() { return m_pSphereCol; }
	CCharCol* GetCharCol() {
		return m_pCharCol;
	}

	virtual void GetDamage(int iAttackerIndex, int iDamage) {};
	virtual void Shoot(void);

	virtual void SetDead();
protected:
	HRESULT AddComponent(void);
private:	
	void Release();
protected:
	Engine::CResourceMgr*		m_pResourceMgr;
	Engine::CDynamicMesh*		m_pCharMesh;//ĳ���� �޽�
	float						m_fSpeed;
	CWeapon3rd*					m_pWeapon3rd;
	DWORD						m_dwCurAniIndex;//���� �ִϸ��̼� �ε���
	DWORD						m_dwNextAniIndex;//���� �ִ�
	CHARINFO					m_tCharInfo;

	CSphereCol*					m_pSphereCol;
	CCharCol*					m_pCharCol;


	DWORD					m_dwNaviIndex;
	float					m_fNaviTop;
	Engine::CNaviCell*		m_pMyNaviCell;

	//����
	bool			m_bJump;
	bool			m_bDown;
	float			m_fJumpAcc;
	float           m_fSkillJumpAcc;
	float			m_fTop;
	DWORD			m_dwState;
	bool			m_bPush;
	float           m_fDownTime;

};