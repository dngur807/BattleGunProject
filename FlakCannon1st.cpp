#include "stdafx.h"
#include "FlakCannon1st.h"
#include "DynamicMesh.h"
#include "Transform.h"
#include "GameObject.h"
#include "Engine_Struct.h"
#include "Effects.h"
#include "ResourceMgr.h"
#include "Export_Function.h"
#include "Weapon1st.h"
#include "StaticMesh.h"
#include "BulletMgr.h"
#include "MuzzleFlash.h"
#include "Ingame.h"
#include "Shield.h"
#include "MyCharacter.h"
#include "NetMgr.h"

CFlakCannon1st::CFlakCannon1st(Engine::MYGDI* pMyGDI, CMyCharacter* pMyCharacter)
	: CWeapon1st(pMyGDI, pMyCharacter)
{
	m_pShield = NULL;
	//셋팅
	m_tInfo.fPutDownTime = 0.4f;
	m_tInfo.fRepeatTimeLeft = 0.1f;
	m_tInfo.fRepeatTimeRight = 1.0f;
	m_tInfo.iPutDownIndex = ANI_PUTDOWN;
	m_tInfo.iIdleIndex = ANI_IDLE;
	m_tInfo.iPutUpIndex = ANI_PUTUP;


	m_tInfo.eWeaponType = WEAPON_FLAKCANNON;
}

CFlakCannon1st::~CFlakCannon1st(void)
{

}

HRESULT CFlakCannon1st::Initialize()
{
	FAILED_CHECK(AddComponent());

	m_pFlashPoint = m_pMesh->FindFrame("Bone_Flash");
	m_pInfo->m_vPos = XMFLOAT3(0.0f, 5.0f, 5);
	m_pInfo->m_vScale = XMFLOAT3(2 * g_ScaleRatio, 2 * g_ScaleRatio, 2 * g_ScaleRatio);


	m_dwAniIndex = ANI_IDLE;
	m_pMesh->SetAnimation(m_dwAniIndex);
	m_pMesh->SetAnimationTime(0.7f);

	m_fRepeatTimeLeft = m_tInfo.fRepeatTimeLeft;
	m_fRepeatTimeRight = m_tInfo.fRepeatTimeRight;



	return S_OK;
}

int CFlakCannon1st::Update()
{
	CWeapon1st::Update();

	if (m_pMesh->GetNumFrames() - 1 == m_pMesh->GetTrackPos())
	{
		m_dwPrevAniIndex = m_dwAniIndex = ANI_IDLE;
		m_pMesh->SetAnimation(m_dwAniIndex);
		m_pMesh->SetAnimationTime(0.7f);
		m_pMesh->SetElapsedTime(0);
	}

	m_pMesh->Update();
	CLandObject::Update();
	return 0;
}

void CFlakCannon1st::Render()
{
	//XMMATRIX matWorld;
	//matWorld = XMMatrixIdentity();

	//m_pMyGDI->pEffect->SetTexTransform(matWorld);
	//m_pMyGDI->pEffect->SetMaterial(m_tMtrl);
	//m_pMyGDI->pEffect->SetDirLights(&m_DirLight);
	//m_pMyGDI->pEffect->SetWorld(matWorld);


	XMMATRIX			matView;
	XMMATRIX			matRot;
	XMMATRIX			matScale;
	XMMATRIX			matTrans;

	matRot = XMMatrixRotationY(XMConvertToRadians(15));
	matTrans = XMMatrixTranslation(0.5f, -0.5f, 1.5f);
	matView = XMLoadFloat4x4(&m_pMyGDI->pEffect->GetView());
	matView = XMMatrixInverse(&XMVECTOR(), matView);
	matScale = XMMatrixScaling(0.05f, 0.05f, 0.05f);

	XMMATRIX ResultMat;
	ResultMat = matScale * matRot * matTrans * matView;
	XMStoreFloat4x4(&m_pInfo->m_matWorld, ResultMat);



	if (m_dwAniIndex == 3)
		m_pMesh->SetAnimationTime(0.3f);
	else if (m_dwAniIndex == 2)
		m_pMesh->SetAnimationTime(0.5f);
	else
		m_pMesh->SetAnimationTime(0.4f);


	XMMATRIX worldInvTranspose = MathHelper::InverseTranspose(XMLoadFloat4x4(&m_pInfo->m_matWorld));
	m_pMyGDI->pEffect->SetWorldInvTranspose(worldInvTranspose);
	m_pMesh->RenderMesh(XMLoadFloat4x4(&m_pInfo->m_matWorld));
}

void CFlakCannon1st::LeftAction()
{
	if (m_pMyPlayer->GetMyCharInfo()->iAmmo[WEAPON_FLAKCANNON] <= 0)
		return;


	m_fRepeatTimeLeft += 1.0f * Engine::Get_TimeMgr()->DeltaTime();

	if (m_fRepeatTimeLeft >= m_tInfo.fRepeatTimeLeft)
	{
		// 총알 발싸
		m_pMyPlayer->Shoot();
		m_dwAniIndex = ANI_FIRE;
		m_pMesh->SetAnimation(m_dwAniIndex);
		m_pMesh->SetAnimationTime(0.5f);

		m_fRepeatTimeLeft = 0.0f;

		//반동
		XMMATRIX matScale = XMMatrixScaling(5.0f, 5.0f, 5.0f);

		XMVECTOR	vPos;
		XMMATRIX  matFlash = matScale * XMLoadFloat4x4(m_pFlashPoint) * XMLoadFloat4x4(&m_pInfo->m_matWorld);
		XMFLOAT4X4 mFlash;
		XMStoreFloat4x4(&mFlash, matFlash);


		memcpy(&vPos, &mFlash.m[3][0], sizeof(XMVECTOR));

		XMVECTOR vDir = XMLoadFloat3(&g_vFpsAt) - vPos;
		vDir = XMVector3Normalize(vDir);

		XMFLOAT3 vfPos, vfDir;
		XMStoreFloat3(&vfPos, vPos);
		XMStoreFloat3(&vfDir, vDir);
		CBulletMgr::GetInstance()->Fire_FlackCannon(&vfPos, &vfDir, g_iIndex);

		//=================================

		if (g_bNetwork == true)
		{
			//전달
			CNetMgr::GetInstance()->RequestFireWeapon(FIRE_FLAKCANNON_BULLET_1, vfPos, vfDir);
		}
		if (--m_pMyPlayer->GetMyCharInfo()->iAmmo[WEAPON_FLAKCANNON] <= 0)
		{
			m_pMyPlayer->GetMyCharInfo()->iAmmo[WEAPON_FLAKCANNON] = 50;
		}
		// vPos 해당 위치  레이 vRayDir로 발사
		// 이펙트 생성

		CGameObject* pGameObject = CMuzzleFlash::Create(m_pMyGDI, &m_pInfo->m_matWorld, m_pFlashPoint , m_tInfo.eWeaponType);
		Engine::Get_Management()->Instantiate(CIngame::LAYER_GAMELOGIC, L"MuzzleFlash", pGameObject);
		Engine::Get_Management()->AddRenderGroup(Engine::TYPE_EFFECT, pGameObject);


		


	}

}

void CFlakCannon1st::RightAction()
{

}

void CFlakCannon1st::Select()
{
	CWeapon1st::Select();
}

HRESULT CFlakCannon1st::AddComponent()
{
	Engine::CComponent*		pComponent = NULL;

	//Transform
	pComponent = m_pInfo = Engine::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pComponent));


	pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Mesh_FlakCannon1st");
	m_pMesh = dynamic_cast<Engine::CDynamicMesh*>(pComponent);
	NULL_CHECK_RETURN(m_pMesh, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"DynamicMesh", pComponent));


	return S_OK;
}

CFlakCannon1st* CFlakCannon1st::Create(Engine::MYGDI* pMyGDI, CMyCharacter* pMyPayer)
{
	CFlakCannon1st* pRifle = new CFlakCannon1st(pMyGDI, pMyPayer);
	if (FAILED(pRifle->Initialize()))
		Engine::Safe_Delete(pRifle);
	return pRifle;
}

