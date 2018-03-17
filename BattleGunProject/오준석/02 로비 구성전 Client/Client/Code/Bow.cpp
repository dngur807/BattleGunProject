#include "stdafx.h"
#include "Bow.h"

#include "Include.h"
#include "Transform.h"
#include "Export_Function.h"
#include "Include.h"
#include "Texture.h"
#include "StaticMesh.h"
#include "Effects.h"

CBow::CBow(Engine::MYGDI* pMyGDI)
	: CGameObject(pMyGDI)
	, m_pResourceMgr(Engine::Get_ResourceMgr())
	, m_pMesh(NULL)
	, m_pInfo(NULL)
{
	XMMATRIX I = XMMatrixIdentity();

	XMStoreFloat4x4(&m_pmatPlayerInfo, I);
	XMStoreFloat4x4(&m_pmatWeaponRef, I);
}

CBow::~CBow(void)
{
}

HRESULT CBow::Initialize(void)
{
	FAILED_CHECK(AddComponent());

	m_pInfo->m_fAngle[Engine::ANGLE_Y] = XMConvertToRadians(90.f);
	return S_OK;
}

int CBow::Update(void)
{
	Engine::CGameObject::Update();

	return 0;
}

void CBow::Render(void)
{
	XMMATRIX WORLD = XMLoadFloat4x4(&m_pInfo->m_matWorld) *
		XMLoadFloat4x4(&m_pmatWeaponRef) * XMLoadFloat4x4(&m_pmatPlayerInfo);

	if (*m_pWeight == 0.f)
	{
		//! 씬 전환 상태가 아닐 경우
		XMStoreFloat4x4(&m_matPreWorld, WORLD);
	}
	else
	{
		//! 씬이 전환되는 상태일 경우 보간을 실시
		m_pMyGDI->pEffect->SetChangeScene(true);
		m_pMyGDI->pEffect->SetPreWorld(XMLoadFloat4x4(&m_matPreWorld));
		m_pMyGDI->pEffect->SetWeightFactor(*m_pWeight);
	}

	m_pMesh->RenderMesh(WORLD);

	m_pMyGDI->pEffect->SetChangeScene(false);
}

HRESULT CBow::AddComponent(void)
{
	Engine::CComponent*		pComponent = NULL;

	//Transform
	pComponent = m_pInfo = Engine::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pComponent));

	//Mesh
	pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"StaticMesh_Bow");
	m_pMesh = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"StaticMesh", pComponent));

	//Texture
	pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Texture_Bow");
	m_pTexture = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pComponent));

	return S_OK;
}

CBow* CBow::Create(Engine::MYGDI* pMyGDI)
{
	CBow* pGameObject = new CBow(pMyGDI);
	if (FAILED(pGameObject->Initialize()))
		Engine::Safe_Delete(pGameObject);

	return pGameObject;
}

void CBow::SetMatrixInfo(const XMMATRIX& pPlayerInfo, const XMMATRIX& pWeaponRef, bool bSceneChange)
{
	XMStoreFloat4x4(&m_pmatWeaponRef, pWeaponRef);
	XMStoreFloat4x4(&m_pmatPlayerInfo, pPlayerInfo);
	m_bChangeScene = bSceneChange;
}
