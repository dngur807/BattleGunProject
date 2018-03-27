#pragma once

#include "Weapon1st.h"
#include "Include.h"


class CAssultRifle1st
	: public CWeapon1st
{
public:
	enum ANIID { ANI_SELECT, ANI_PUTDOWN, ANI_IDLE, ANI_FIRE, ANI_ALTFIRE, ANI_END };
private:
	CAssultRifle1st(Engine::MYGDI* pMyGDI, CCharacter* pMyCharacter);
public:
	virtual ~CAssultRifle1st(void);
public:
	virtual HRESULT Initialize();
	virtual int Update();
	virtual void Render();
	virtual void LeftAction();
	virtual void RightAction();
	virtual void Select();

private:
	HRESULT		AddComponent();
public:
	static CAssultRifle1st*		Create(Engine::MYGDI*		pMyGDI, CCharacter* pMyPayer);

};