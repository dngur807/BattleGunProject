#include "stdafx.h"
#include "Weapon3rd.h"
#include "Export_Function.h"
#include "StaticMesh.h"
#include "Transform.h"

CWeapon3rd::CWeapon3rd(Engine::MYGDI* pMyGDI, const XMFLOAT4X4* pMatPlayerInfo, const XMFLOAT4X4* pMatWeaponRef)
	: CLandObject(pMyGDI)
	, m_pSelectedWeaponMesh(NULL)
	, m_pResourceMgr(Engine::Get_ResourceMgr())
	, m_pMatPlayerInfo(pMatPlayerInfo)
	, m_pMatWeaponRef(pMatWeaponRef)
{
}

CWeapon3rd::~CWeapon3rd(void)
{
}

HRESULT CWeapon3rd::Initialize()
{
	FAILED_CHECK(AddComponent());
	m_pSelectedWeaponMesh = m_vecWeapon3rdMesh[1];

	SetWeapon(WEAPON_FLAKCANNON);
	return S_OK;
}

int CWeapon3rd::Update()
{
	XMVECTOR vDir = XMVector3TransformNormal(XMLoadFloat3(&g_vLook), XMLoadFloat4x4(&m_pInfo->m_matWorld));
	XMStoreFloat3(&m_pInfo->m_vDir, vDir);

	
	return 0;
}

void CWeapon3rd::Render()
{
	
	CLandObject::Update();

	XMMATRIX matWorld;
	matWorld = XMMatrixIdentity();

	m_pMyGDI->pEffect->SetTexTransform(matWorld);
	m_pMyGDI->pEffect->SetMaterial(m_tMtrl);
	m_pMyGDI->pEffect->SetDirLights(&m_DirLight);
	m_pMyGDI->pEffect->SetWorld(matWorld);



	matWorld = XMLoadFloat4x4(&m_pInfo->m_matWorld) * XMLoadFloat4x4(m_pMatWeaponRef) * XMLoadFloat4x4(m_pMatPlayerInfo);
	
	XMStoreFloat4x4(&m_pInfo->m_matWorld, matWorld);
	
	m_pSelectedWeaponMesh->RenderMesh(matWorld);
}

void CWeapon3rd::SetWeapon(int iType)
{
	ZeroMemory(m_pInfo->m_fAngle, sizeof(float) * 3);

	switch (iType)
	{
	case WEAPON_ASSULTRIFLE:
		m_pInfo->m_fAngle[Engine::ANGLE_X] = XMConvertToRadians(-90.f);
		m_pInfo->m_fAngle[Engine::ANGLE_Y] = 0.f;
		m_pInfo->m_fAngle[Engine::ANGLE_Z] = XMConvertToRadians(-90.f);
		m_pInfo->m_vPos.z = -35;//위아래 조절
		break;
	case WEAPON_FLAKCANNON:
		m_pInfo->m_fAngle[Engine::ANGLE_X] = XMConvertToRadians(-90.f);
		m_pInfo->m_fAngle[Engine::ANGLE_Y] = 0.f;
		m_pInfo->m_fAngle[Engine::ANGLE_Z] = XMConvertToRadians(-90.f);
		m_pInfo->m_vPos.z = -35;//위아래 조절
	

		break;
	default:
		break;
	}
	m_pSelectedWeaponMesh = m_vecWeapon3rdMesh[iType];
}

CWeapon3rd* CWeapon3rd::Create(Engine::MYGDI* pMyGDI, const XMFLOAT4X4* pMatPlayerInfo, const XMFLOAT4X4* pMatWeaponRef)
{
	CWeapon3rd* pWea = new CWeapon3rd(pMyGDI, pMatPlayerInfo, pMatWeaponRef);
	if (FAILED(pWea->Initialize()))
		Engine::Safe_Delete(pWea);
	return pWea;
}

HRESULT CWeapon3rd::AddComponent()
{
	Engine::CComponent*		pComponent = NULL;
	Engine::CStaticMesh* Weapon = NULL;
	//Transform
	pComponent = m_pInfo = Engine::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pComponent));
	//StaticMesh 무기들!!
	pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Mesh_AssultRifle3rd");
	Weapon = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	NULL_CHECK_RETURN(Weapon, E_FAIL);
	m_vecWeapon3rdMesh.push_back(Weapon);


	pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Mesh_FlakCannon3rd");
	Weapon = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	NULL_CHECK_RETURN(Weapon, E_FAIL);
	m_vecWeapon3rdMesh.push_back(Weapon);

	return S_OK;
}

