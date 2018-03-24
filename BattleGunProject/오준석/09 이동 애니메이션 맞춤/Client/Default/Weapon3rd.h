#pragma once
#include "LandObject.h"
#include "Include.h"

namespace Engine
{
	class CResourceMgr;
	class CStaticMesh;
	class CTransform;
};

class CWeapon3rd
	:public CLandObject
{
private:
	CWeapon3rd(Engine::MYGDI* pMyGDI, const XMFLOAT4X4* pMatPlayerInfo, const XMFLOAT4X4* pMatWeaponRef);
public:
	virtual ~CWeapon3rd(void);

public:
	virtual HRESULT Initialize();
	virtual int Update();
	virtual void Render();
	void SetWeapon(int iType);

public:
	static CWeapon3rd* Create(Engine::MYGDI* pMyGDI, const XMFLOAT4X4* pMatPlayerInfo, const XMFLOAT4X4* pMatWeaponRef);
private:
	HRESULT AddComponent();
private:
	Engine::CResourceMgr*		m_pResourceMgr;
	Engine::CTransform*			m_pInfo;
	vector<Engine::CStaticMesh*>	m_vecWeapon3rdMesh;
	Engine::CStaticMesh*			m_pSelectedWeaponMesh;

private:
	const XMFLOAT4X4*		m_pMatPlayerInfo;
	const XMFLOAT4X4*		m_pMatWeaponRef;

};