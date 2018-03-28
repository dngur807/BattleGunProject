#pragma once

#include "Character.h"

class CWeapon1st;


class CMyCharacter
	: public CCharacter
{
private:
	CMyCharacter(Engine::MYGDI* pMyGDI);
public:
	virtual ~CMyCharacter();
public:
	virtual HRESULT Initialize();
	virtual int Update();
	virtual void ApplyInput(CHARINPUT& pInput);
	virtual void Render();
	virtual void GetDamage(int iAttackerIndex, int iDamage);
	virtual void Shoot(void);
	virtual void Jump();
	virtual void SetDead();
private:
	HRESULT AddComponent();
	void Release();
public:
	static CMyCharacter*		Create(Engine::MYGDI* pMyGDI);
private:
	CWeapon1st*					m_pSelectedWeapon;
	vector<CWeapon1st*>				m_vecWeapon1st;





};