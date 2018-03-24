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
#include "NaviCell.h"

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
	, m_dwIndex(0)
	, m_pMyCell(nullptr)
	, m_fTop(0)
	, m_bJump(false)
{
	m_bDown = false;
	m_fJumpAcc = 0.f;

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
	Safe_Delete(m_pMyCell);
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
	m_fSpeed = 500.f;

	//m_pMesh->SetAnimation(0);

	//Engine::Get_InfoSubject()->AddData(MSG_PLAYERINFO, m_pInfo);


	

	return S_OK;
}

int CPlayer::Update(void)
{
	m_fTime += Engine::Get_TimeMgr()->DeltaTime();

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

	XMVECTOR  vDir = XMVector3TransformNormal(XMLoadFloat3(&g_vLook), XMLoadFloat4x4(&m_pInfo->m_matWorld));
	vDir = XMVector3Normalize(vDir);
	XMStoreFloat3(&m_pInfo->m_vDir, vDir);

	Engine::CGameObject::Update();
	m_pWeapon3rd->Update();


	m_pMesh->Update();


	m_pSelectedWeapon->Update();

	m_dwIndex = Engine::Get_NaviMgr()->FindIndex(&m_pInfo->m_vPos);
	m_fTop = Engine::Get_NaviMgr()->FindfTop(&m_pInfo->m_vPos);
	Jump();
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

	if (Engine::Get_Input()->GetDIKeyState(DIK_LSHIFT))
	{
		m_dwState = STATE_RUN;
		m_fSpeed = 600;
	}
	else
	{
		m_fSpeed = 300;
	}


	if (Engine::Get_Input()->GetDIKeyState(DIK_W))
	{
		if (m_dwState != STATE_RUN)
		{
			if (m_dwState == STATE_SIT || m_dwState == STATE_SITATT || m_dwState == STATE_SITWALK)
				m_dwState = STATE_SITWALK;
			else
				m_dwState = STATE_WALK;
		}



		if (m_bJump || m_bDown || m_fTop>8.f) //1층을제외한건 걍떨어지게
		{
			XMVECTOR VP = XMLoadFloat3(&m_pInfo->m_vPos);
			XMVECTOR VD = XMLoadFloat3(&m_pInfo->m_vDir);
			VP -= VD * m_fSpeed * fTime;
			XMStoreFloat3(&m_pInfo->m_vPos, VP);
		}
		else
		{
			m_dwIndex = Engine::Get_NaviMgr()->MoveOnNaviMesh(&m_pInfo->m_vPos
				, &XMFLOAT3(-m_pInfo->m_vDir.x* m_fSpeed*fTime, -m_pInfo->m_vDir.y* m_fSpeed*fTime, -m_pInfo->m_vDir.z* m_fSpeed*fTime)
				, m_dwIndex, m_fTop);
		}

	}

	if (Engine::Get_Input()->GetDIKeyState(DIK_S))
	{

		if (m_dwState != STATE_RUN)
		{
			if (m_dwState == STATE_SIT || m_dwState == STATE_SITATT || m_dwState == STATE_SITWALK)
				m_dwState = STATE_SITWALK;
			else
				m_dwState = STATE_WALK;
		}

		if (m_bJump || m_bDown || m_fTop>8.f) //1층을제외한건 걍떨어지게   //계단높이!
		{
			XMVECTOR VP = XMLoadFloat3(&m_pInfo->m_vPos);
			XMVECTOR VD = XMLoadFloat3(&m_pInfo->m_vDir);
			VP += VD * m_fSpeed * fTime;
			XMStoreFloat3(&m_pInfo->m_vPos, VP);
		}
		else
		{
			m_dwIndex = Engine::Get_NaviMgr()->MoveOnNaviMesh(&m_pInfo->m_vPos
				, &XMFLOAT3(m_pInfo->m_vDir.x* m_fSpeed*fTime, m_pInfo->m_vDir.y* m_fSpeed*fTime, m_pInfo->m_vDir.z* m_fSpeed*fTime)
				, m_dwIndex, m_fTop);
		}
	}
	if (Engine::Get_Input()->GetDIKeyState(DIK_A))
	{
		m_pInfo->m_fAngle[Engine::ANGLE_Y] += XMConvertToRadians(-90) * fTime;
	
	}

	if (Engine::Get_Input()->GetDIKeyState(DIK_D))
	{

		m_pInfo->m_fAngle[Engine::ANGLE_Y] += XMConvertToRadians(90) * fTime;
	
	}

	if (Engine::Get_Input()->GetDIKeyState(DIK_SPACE))
	{
		if (m_bPush == true)
			return;
		m_bPush = true;
		m_bJump = true;
	}
	else
		m_bPush = false;
	if (Engine::Get_Input()->GetDIMouseState(Engine::CInput::DIM_LBUTTON))
	{
		if (m_dwState == STATE_SIT || m_dwState == STATE_SITATT)
		{
			m_dwState = STATE_SITATT;
		}
		else
			m_dwState = STATE_ATT;
	}


	if (Engine::Get_Input()->OnceKeyDown(DIK_LCONTROL))
	{
		if (m_dwState == STATE_STAND)
			m_dwState = STATE_SIT;
		else
			m_dwState = STATE_STAND;
	}

	if (Engine::Get_Input()->GetDIKeyState(DIK_T))
		m_dwState = STATE_RIDE;
	if (Engine::Get_Input()->GetDIKeyState(DIK_J))
		m_dwState = STATE_JOKE;
	if (Engine::Get_Input()->GetDIKeyState(DIK_P))
		m_dwState = STATE_PLANT;



	// 오준석//2018-01-10// 모든애니메이션 키땟을때 초기화!

	if (Engine::Get_Input()->OnceMouseKeyUp(Engine::CInput::DIM_LBUTTON))
	{
		if (m_dwState == STATE_SIT || m_dwState == STATE_SITATT || m_dwState == STATE_SITWALK)
			m_dwState = STATE_SIT;
		else
			m_dwState = STATE_STAND;
	}


	if ((Engine::Get_Input()->OnceKeyUp(DIK_W) || Engine::Get_Input()->OnceKeyUp(DIK_S))
		&& (m_dwState != STATE_SIT
			&& m_dwState != STATE_SITATT
			&& m_dwState != STATE_SITWALK))
		m_dwState = STATE_STAND;

}

void CPlayer::Jump()
{
	if (m_bJump == true && m_bDown == false)
	{
		m_fJumpAcc += 6.f*m_fTime;
		m_pInfo->m_vPos.y += -0.01f + m_fJumpAcc;
	}

	// 오준석//18-01-11// 올라가는 네비메쉬 자연스럽게 조건
	if (m_dwIndex == 2 || m_dwIndex == 3
		|| m_dwIndex == 14 || m_dwIndex == 17
		|| m_dwIndex == 18 || m_dwIndex == 19
		|| m_dwIndex == 32 || m_dwIndex == 33
		|| m_dwIndex == 31 || m_dwIndex == 28) //1~2층
	{
		if (m_pInfo->m_vPos.y > 256.f + m_fTop)
			m_bDown = true;
	}
	else
	{
		if (m_pInfo->m_vPos.y > 10.f + m_fTop)
			m_bDown = true;

	}

	if (m_bDown == true)
	{
		m_fJumpAcc -= 2.f*m_fTime;

		m_pInfo->m_vPos.y += -0.01f + m_fJumpAcc;

	}
	if (m_pInfo->m_vPos.y <= m_fTop  && m_bDown == true)
	{

		m_bDown = false;
		m_bJump = false;
		m_fJumpAcc = 0.f;
		m_pInfo->m_vPos.y = m_fTop;

	}

}
