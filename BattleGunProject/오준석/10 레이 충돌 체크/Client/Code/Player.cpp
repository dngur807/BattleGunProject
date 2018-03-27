#include "stdafx.h"
#include "Player.h"

#include "Include.h"
#include "Transform.h"
#include "Export_Function.h"
#include "Include.h"
#include "Texture.h"
#include "DynamicMesh.h"
#include "StaticMesh.h"
#include "CubeColor.h"
#include "Bow.h"
#include "CameraMgr.h"
#include "AssultRifle1st.h"
#include "FlakCannon1st.h"
#include "Weapon3rd.h"
#include "KeyMgr.h"

CPlayer::CPlayer(Engine::MYGDI* pMyGDI)
	: CLandObject(pMyGDI)
	, m_pResourceMgr(Engine::Get_ResourceMgr())
	, m_iAniIndex(0)
	, m_fSpeed(0.f)
	, m_pSelectedWeapon(nullptr)
	, m_pBoxBuffer(nullptr)
	, m_pStaticMesh(nullptr)
	, m_pMesh(nullptr)
	, m_pWeapon3rd(NULL)
{

}

CPlayer::~CPlayer(void)
{
	Safe_Delete(m_pBoxBuffer);
	//Safe_Delete(m_pBow);
	for (auto iter = m_vecWeapon1st.begin(); iter != m_vecWeapon1st.end(); ++iter)
	{
		delete *iter;
	}
	m_vecWeapon1st.clear();
	Safe_Delete(m_pWeapon3rd);
}

HRESULT CPlayer::Initialize(void)
{
	FAILED_CHECK(AddComponent());
	FAILED_CHECK(AddWeapon());

	
	CCameraMgr::GetInstance()->SetFPSEye(m_pMesh->FindFrame("Bip01_Neck"));
	m_pWeapon3rd = CWeapon3rd::Create(m_pMyGDI, &m_pInfo->m_matWorld, m_pMesh->FindFrame("Bone_weapon"));

//	m_iAniIndex = 28;
	m_iAniIndex = ANI_IDLE;
	m_pMesh->SetAnimation(m_iAniIndex);
	m_pMesh->SetAnimationTime(0.7f);//높일 수록 빨라짐


	m_pInfo->m_fAngle[Engine::ANGLE_Y] += 110.f;
	m_pInfo ->m_vPos = XMFLOAT3(5, 5, 5);
	m_pInfo->m_vScale = XMFLOAT3(0.05f, 0.05f, 0.05f);
	m_pInfo->m_vDir = XMFLOAT3(1.f, 0.f, -1.f);;

	//g_vLook
	m_fSpeed = 50.f;

	//m_pMesh->SetAnimation(0);

	//Engine::Get_InfoSubject()->AddData(MSG_PLAYERINFO, m_pInfo);


	

	return S_OK;
}

int CPlayer::Update(void)
{
	if (GetAsyncKeyState(VK_SPACE) & 0x0001)
	{

		m_iAniIndex++;
#ifdef _CLIENTDEBUGING_
		printf("애니메이션 %d \n", m_iAniIndex);
#endif 
		m_pMesh->SetAnimation(m_iAniIndex);
	}
	if (CKeyMgr::GetInstance()->IsOnceKeyDown('c') ||
		CKeyMgr::GetInstance()->IsOnceKeyDown('C'))
	{
		++g_iCamMode;
		if (g_iCamMode > CAMERA_END)
		{
			g_iCamMode = 0;
		}
		CCameraMgr::GetInstance()->SetCamera((CAMERAMODE)g_iCamMode);
	}

	KeyCheck();

	m_pInfo->m_vPos.y -= 50.0f * Engine::Get_TimeMgr()->DeltaTime();

	if (m_pInfo->m_vPos.y < 0.f)
		m_pInfo->m_vPos.y = 0.f;

	// 건물과 충돌 체크


	//D3DXVec3TransformNormal(&m_pInfo->m_vDir, &g_vLook, &m_pInfo->m_matWorld);
	//D3DXVec3Normalize(&m_pInfo->m_vDir, &m_pInfo->m_vDir);

	//XMVECTOR vecLook;
	XMVECTOR  vDir = XMVector3TransformNormal(XMLoadFloat3(&g_vLook), XMLoadFloat4x4(&m_pInfo->m_matWorld));
	vDir = XMVector3Normalize(vDir);
	XMStoreFloat3(&m_pInfo->m_vDir, vDir);

	Engine::CGameObject::Update();
	m_pWeapon3rd->Update();

	
	//if (m_pMesh->GetNumFrames() - 1 == m_pMesh->GetTrackPos())
	//{
	//	//m_iAniIndex = 0;
	//	m_pMesh->SetAnimation(0);
	//	//m_pMesh->InitElapsedTime();
	//}
	m_pMesh->Update();


	m_pSelectedWeapon->Update();

	return 0;
}

void CPlayer::Render(void)
{
	if (g_iCamMode == CAMERA_FPS)
	{
		//총만 그리자
		m_pSelectedWeapon->Render();
	}
	else
	{
		XMMATRIX worldInvTranspose = MathHelper::InverseTranspose(XMLoadFloat4x4(&m_pInfo->m_matWorld));
		m_pMyGDI->pEffect->SetWorldInvTranspose(worldInvTranspose);
		m_pMesh->RenderMesh(XMLoadFloat4x4(&m_pInfo->m_matWorld));
		m_pWeapon3rd->Render();
	}

}




HRESULT CPlayer::AddComponent(void)
{
	Engine::CComponent*		pComponent = NULL;

	//Transform
	pComponent = m_pInfo = Engine::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pComponent));

	//Mesh
	pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"DynamicMesh_Sylva");
	m_pMesh = dynamic_cast<Engine::CDynamicMesh*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"DynamicMesh", pComponent));


	return S_OK;
}

HRESULT CPlayer::AddWeapon()
{
	//m_vecWeapon1st.push_back(CAssultRifle1st::Create(m_pMyGDI, this));
//	m_vecWeapon1st.push_back(CFlakCannon1st::Create(m_pMyGDI, this));
	//m_pSelectedWeapon = m_vecWeapon1st[1];
//
	return S_OK;
}

CPlayer* CPlayer::Create(Engine::MYGDI* pMyGDI)
{
	CPlayer*		pGameObject = new CPlayer(pMyGDI);
	if (FAILED(pGameObject->Initialize()))
		Engine::Safe_Delete(pGameObject);

	return pGameObject;
}

void CPlayer::KeyCheck(void)
{
	float fTime = Engine::Get_TimeMgr()->DeltaTime();

	//if (Engine::Get_Input()->GetDIKeyState(DIK_UPARROW))
	if (Engine::Get_Input()->GetDIKeyState(DIK_W))
	{
		XMVECTOR VP = XMLoadFloat3(&m_pInfo->m_vPos);
		XMVECTOR VD = XMLoadFloat3(&m_pInfo->m_vDir);
		VP += VD * m_fSpeed * fTime;
		XMStoreFloat3(&m_pInfo->m_vPos, VP);
	}
	//if (Engine::Get_Input()->GetDIKeyState(DIK_DOWNARROW))
	if (Engine::Get_Input()->GetDIKeyState(DIK_S))
	{
		XMVECTOR VP = XMLoadFloat3(&m_pInfo->m_vPos);
		XMVECTOR VD = XMLoadFloat3(&m_pInfo->m_vDir);
		VP -= VD * m_fSpeed * fTime;
		XMStoreFloat3(&m_pInfo->m_vPos, VP);
	}
	//if (Engine::Get_Input()->GetDIKeyState(DIK_LEFTARROW))
	if (Engine::Get_Input()->GetDIKeyState(DIK_D))
	{
		//m_pInfo->m_fAngle[Engine::ANGLE_Y] += XMConvertToRadians(-90) * fTime;
		XMVECTOR VP = XMLoadFloat3(&m_pInfo->m_vPos);
		XMVECTOR VD = XMLoadFloat3(&m_pInfo->m_vDir);
		XMVECTOR vUp = XMLoadFloat3(&XMFLOAT3(m_pInfo->m_matWorld._21, m_pInfo->m_matWorld._22, m_pInfo->m_matWorld._23));

		XMVECTOR vRight = XMVector3Cross(vUp , VD);
		vRight = XMVector3Normalize(vRight);
		VP += vRight * m_fSpeed * fTime;
		XMStoreFloat3(&m_pInfo->m_vPos, VP);
	}
	//if (Engine::Get_Input()->GetDIKeyState(DIK_RIGHTARROW))
	if (Engine::Get_Input()->GetDIKeyState(DIK_A))
	{
		XMVECTOR VP = XMLoadFloat3(&m_pInfo->m_vPos);
		XMVECTOR VD = XMLoadFloat3(&m_pInfo->m_vDir);
		XMVECTOR vUp = XMLoadFloat3(&XMFLOAT3(m_pInfo->m_matWorld._21, m_pInfo->m_matWorld._22, m_pInfo->m_matWorld._23));

		XMVECTOR vRight = XMVector3Cross(vUp, VD);
		vRight = XMVector3Normalize(vRight);
		VP -= vRight * m_fSpeed * fTime;
		XMStoreFloat3(&m_pInfo->m_vPos, VP);
	}

	if (Engine::Get_Input()->GetDIKeyState(DIK_SPACE))
	{
		m_pInfo->m_vPos.y += 0.5f;
	}
	else
		m_bPush = false;
}
