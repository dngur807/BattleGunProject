#include "stdafx.h"
#include "AssultRifle1st.h"
#include "DynamicMesh.h"
#include "Transform.h"
#include "GameObject.h"
#include "Engine_Struct.h"
#include "Effects.h"
#include "ResourceMgr.h"
#include "Export_Function.h"
#include "Weapon1st.h"
#include "StaticMesh.h"

CAssultRifle1st::CAssultRifle1st(Engine::MYGDI* pMyGDI, CCharacter* pMyCharacter)
	: CWeapon1st(pMyGDI , pMyCharacter)
{
	//셋팅
	m_tInfo.fPutDownTime = 0.4f;
	m_tInfo.fRepeatTimeLeft = 0.1f;
	m_tInfo.fRepeatTimeRight = 1.0f;
	m_tInfo.iPutDownIndex = ANI_PUTDOWN;
	m_tInfo.iSelectIndex = ANI_SELECT;
	m_tInfo.iIdleIndex = ANI_IDLE;
}

CAssultRifle1st::~CAssultRifle1st(void)
{

}

HRESULT CAssultRifle1st::Initialize()
{
	FAILED_CHECK(AddComponent());

	m_pFlashPoint = m_pMesh->FindFrame("Bone_Flash_a");
	m_pInfo->m_vPos = XMFLOAT3(0.f, 5.0f, 0.f);
	m_pInfo->m_vScale = XMFLOAT3(0.1f, 0.1f, 0.1f);


	m_dwAniIndex = ANI_IDLE;
	m_pMesh->SetAnimation(m_dwAniIndex);

	m_fRepeatTimeLeft = m_tInfo.fRepeatTimeLeft;
	m_fRepeatTimeRight = m_tInfo.fRepeatTimeRight;

	return S_OK;
}

int CAssultRifle1st::Update()
{
	
	if (m_pMesh->GetNumFrames() - 0.01f == m_pMesh->GetTrackPos())
	{
		m_dwAniIndex = ANI_IDLE;
		m_pMesh->SetAnimation(m_dwAniIndex);
		m_pMesh->SetTrackPos(0.0f);
	}
	CLandObject::Update();
	return 0;
}

void CAssultRifle1st::Render()
{
	/*XMMATRIX matWorld;
	matWorld = XMMatrixIdentity();

	m_pMyGDI->pEffect->SetTexTransform(matWorld);
	m_pMyGDI->pEffect->SetMaterial(m_tMtrl);
	m_pMyGDI->pEffect->SetDirLights(&m_DirLight);
	m_pMyGDI->pEffect->SetWorld(matWorld);*/


	XMMATRIX			matView;
	XMMATRIX			matRot;
	XMMATRIX			matScale;
	XMMATRIX			matTrans;

	matRot = XMMatrixRotationY(XMConvertToRadians(-90.0f));
	matTrans = XMMatrixTranslation(0.5f, -0.5f, 4);
	matView = XMLoadFloat4x4(&m_pMyGDI->pEffect->GetView());
	matView = XMMatrixInverse(&XMVECTOR(), matView);
	matScale = XMMatrixScaling(0.05f, 0.05f, 0.05f);

	XMMATRIX ResultMat;
	ResultMat = matScale * matRot * matTrans * matView;
	XMStoreFloat4x4(&m_pInfo->m_matWorld, ResultMat);

	XMMATRIX worldInvTranspose = MathHelper::InverseTranspose(XMLoadFloat4x4(&m_pInfo->m_matWorld));
	m_pMyGDI->pEffect->SetWorldInvTranspose(worldInvTranspose);
	m_pMesh->RenderMesh(XMLoadFloat4x4(&m_pInfo->m_matWorld));
}

void CAssultRifle1st::LeftAction()
{
	m_fRepeatTimeLeft += 1.0f * Engine::Get_TimeMgr()->DeltaTime();

	if (m_fRepeatTimeLeft >= m_tInfo.fRepeatTimeLeft)
	{
		// 총알 발싸
		//m_pMyPlayer->Shoot();
		m_dwAniIndex = ANI_FIRE;
		m_pMesh->SetAnimation(m_dwAniIndex);

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

		matRot = XMMatrixRotationAxis( vCross, rand1);
		vRayDir = XMVector3TransformNormal(vDir, matRot);
		matRot = XMMatrixRotationAxis(vDir, rand2);
		vRayDir = XMVector3TransformNormal(vRayDir, matRot);



		// vPos 해당 위치  레이 vRayDir로 발사

		if (g_bNetwork == true)
		{
			//네트워크 전달
		}




	}

}

void CAssultRifle1st::RightAction()
{

}

void CAssultRifle1st::Select()
{
	CWeapon1st::Select();
}

HRESULT CAssultRifle1st::AddComponent()
{
	Engine::CComponent*		pComponent = NULL;

	//Transform
	pComponent = m_pInfo = Engine::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pComponent));



	pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Mesh_AssultRifle1st");
	m_pMesh = dynamic_cast<Engine::CDynamicMesh*>(pComponent);
	NULL_CHECK_RETURN(m_pMesh, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"DynamicMesh", pComponent));


	return S_OK;
}

CAssultRifle1st* CAssultRifle1st::Create(Engine::MYGDI* pMyGDI, CCharacter* pMyPayer)
{
	CAssultRifle1st* pRifle = new CAssultRifle1st(pMyGDI, pMyPayer);
	if (FAILED(pRifle->Initialize()))
		Engine::Safe_Delete(pRifle);
	return pRifle;
}

