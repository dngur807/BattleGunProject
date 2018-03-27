#pragma once

#include "Weapon1st.h"
#include "Include.h"

class CFlakCannon1st
	: public CWeapon1st
{
public:
	enum ANIID { ANI_IDLE = 4, ANI_FIRE, ANI_END };
private:
	CFlakCannon1st(Engine::MYGDI* pMyGDI, CCharacter* pMyCharacter);
public:
	virtual ~CFlakCannon1st(void);
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
	static CFlakCannon1st*		Create(Engine::MYGDI*		pMyGDI, CCharacter* pMyPayer);

};