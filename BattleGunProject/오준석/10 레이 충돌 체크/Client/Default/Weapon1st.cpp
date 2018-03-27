#include "stdafx.h"
#include "Weapon1st.h"
#include "Export_Function.h"
#include "DynamicMesh.h"

CWeapon1st::CWeapon1st(Engine::MYGDI* pMyGDI, CCharacter* pMyPlayer)
	: CLandObject(pMyGDI)
	, m_pMyPlayer(pMyPlayer)
	, m_pMesh(NULL)
	, m_dwIndex(0)
	, m_dwAniIndex(0)
	, m_fRepeatTimeLeft(0)
	, m_fRepeatTimeRight(0)
	, m_pResourceMgr(Engine::Get_ResourceMgr())
{

}

CWeapon1st::~CWeapon1st()
{

}

HRESULT CWeapon1st::Initialize()
{
	return S_OK;
}

int CWeapon1st::Update()
{
	return 0;
}

void CWeapon1st::Render()
{

}

float CWeapon1st::PutDown()
{
	m_dwAniIndex = m_tInfo.iPutDownIndex;
	m_fRepeatTimeLeft = m_tInfo.fRepeatTimeLeft;
	m_fRepeatTimeRight = m_tInfo.fRepeatTimeRight;
	m_pMesh->SetAnimation(m_dwAniIndex);
	//m_pMesh->SetTrackPos(0.0);
	return S_OK;
}

void CWeapon1st::Select()
{
	m_dwAniIndex = m_tInfo.iSelectIndex;
	m_pMesh->SetAnimation(m_dwIndex);
	//m_pMesh->SetTrackPos(0.0);
}

void CWeapon1st::NoAction()
{
	if (m_fRepeatTimeLeft < m_tInfo.fRepeatTimeLeft)
		m_fRepeatTimeLeft += 1.f * Engine::Get_TimeMgr()->DeltaTime();

	if (m_fRepeatTimeRight < m_tInfo.fRepeatTimeRight)
		m_fRepeatTimeRight += 1.f * Engine::Get_TimeMgr()->DeltaTime();

}

