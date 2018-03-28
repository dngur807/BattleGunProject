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

CFlakCannon1st::CFlakCannon1st(Engine::MYGDI* pMyGDI, CCharacter* pMyCharacter)
	: CWeapon1st(pMyGDI, pMyCharacter)
{
	//셋팅
	m_tInfo.fPutDownTime = 0.4f;
	m_tInfo.fRepeatTimeLeft = 0.1f;
	m_tInfo.fRepeatTimeRight = 1.0f;
	m_tInfo.iIdleIndex = ANI_IDLE;
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


	m_dwPrevAniIndex =	m_dwAniIndex = ANI_IDLE;
	m_pMesh->SetAnimation(m_dwAniIndex);
	m_pMesh->SetAnimationTime(0.7f);

	m_fRepeatTimeLeft = m_tInfo.fRepeatTimeLeft;
	m_fRepeatTimeRight = m_tInfo.fRepeatTimeRight;

	return S_OK;
}

int CFlakCannon1st::Update()
{
	/*if (GetAsyncKeyState(VK_SPACE) & 0x0001)
	{

		m_dwAniIndex++;
#ifdef _CLIENTDEBUGING_
		printf("애니메이션 %d \n", m_dwAniIndex);
#endif 
		m_pMesh->SetAnimation(m_dwAniIndex);
	}*/

	if (m_pMesh->GetElapsedTime() > 0.2f)
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
	m_fRepeatTimeLeft += 1.0f * Engine::Get_TimeMgr()->DeltaTime();

	if (m_fRepeatTimeLeft >= m_tInfo.fRepeatTimeLeft)
	{
		// 총알 발싸
		//m_pMyPlayer->Shoot();
		m_dwAniIndex = ANI_FIRE;
		m_pMesh->SetAnimation(m_dwAniIndex);
		m_pMesh->SetAnimationTime(0.5f);

		m_fRepeatTimeLeft = 0.0f;
		//반동

		XMVECTOR	vPos;
		XMVECTOR	vCross;
		XMVECTOR	vRayDir;
		XMMATRIX	matRot;

		float rand1 = XMConvertToRadians(float(rand() % 5));
		float rand2 = XMConvertToRadians(float(rand() % 360));


		XMMATRIX  matFlash = XMLoadFloat4x4(m_pFlashPoint) * XMLoadFloat4x4(&m_pInfo->m_matWorld);
		XMFLOAT4X4 mFlash;
		XMStoreFloat4x4(&mFlash, matFlash);


		memcpy(&vPos, &mFlash.m[3][0], sizeof(XMVECTOR));

		XMVECTOR vDir = XMLoadFloat3(&g_vFpsAt) - vPos;
		vDir = XMVector3Normalize(vDir);

		vCross = XMVector3Cross(XMLoadFloat3(&g_vLook), vDir);

		matRot = XMMatrixRotationAxis(vCross, rand1);
		vRayDir = XMVector3TransformNormal(vDir, matRot);
		matRot = XMMatrixRotationAxis(vDir, rand2);
		vRayDir = XMVector3TransformNormal(vRayDir, matRot);


		XMFLOAT3 vPosFloat3;
		XMFLOAT3 vRayFloat3;
		XMStoreFloat3(&vPosFloat3, vPos);
		XMStoreFloat3(&vRayFloat3, vRayDir);
		CBulletMgr::GetInstance()->Fire_AssultBullet(&vPosFloat3, &vRayFloat3, g_iIndex);


		// vPos 해당 위치  레이 vRayDir로 발사

		if (g_bNetwork == true)
		{
			//네트워크 전달
		}



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

CFlakCannon1st* CFlakCannon1st::Create(Engine::MYGDI* pMyGDI, CCharacter* pMyPayer)
{
	CFlakCannon1st* pRifle = new CFlakCannon1st(pMyGDI, pMyPayer);
	if (FAILED(pRifle->Initialize()))
		Engine::Safe_Delete(pRifle);
	return pRifle;
}

