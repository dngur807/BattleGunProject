#include "stdafx.h"
#include "Character.h"
#include "Export_Function.h"
#include "Weapon3rd.h"
#include "Transform.h"
#include "DynamicMesh.h"
#include "StaticMesh.h"
#include "NaviCell.h"

CCharacter::CCharacter(Engine::MYGDI* pMyGDI)
	: CLandObject(pMyGDI)
	, m_pResourceMgr(Engine::Get_ResourceMgr())
	, m_fSpeed(0.0f)
	, m_pCharMesh(NULL)
	, m_pWeapon3rd(NULL)
	, m_dwCurAniIndex(0)
	, m_dwIndex(0)
	, m_fTop(0)
	, m_bJump(false)
	, m_pMyCell(nullptr)
{
	m_bDown = false;
	m_fJumpAcc = 0.f;

	if (g_bNetwork == false)
		m_tCharInfo.eState = CHARSTATE_ALIVE;
}

CCharacter::~CCharacter()
{
	Release();
}

HRESULT CCharacter::Initialize()
{
	FAILED_CHECK(AddComponent());
	m_pWeapon3rd = CWeapon3rd::Create(m_pMyGDI , &m_pInfo->m_matWorld , m_pCharMesh->FindFrame("Bone_weapon"));
	m_pInfo->m_fScaleFactor = 2.0f;

	m_pInfo->m_vPos = XMFLOAT3(float(rand() % 50), 0.5f, float(rand() % 50));

	m_pInfo->m_vScale = XMFLOAT3(1.0f / m_pInfo->m_fScaleFactor, 1.0f / m_pInfo->m_fScaleFactor, 1.0f / m_pInfo->m_fScaleFactor);

	m_dwCurAniIndex = ANI_IDLE;
	m_pCharMesh->SetAnimation(m_dwCurAniIndex);
	m_pCharMesh->SetAnimationTime(1);

	m_pInfo->m_fAngle[Engine::ANGLE_Y] = 90;

	m_fSpeed = 300.f;
	return S_OK;
}

int CCharacter::Update()
{
	XMVECTOR vecDir =  XMVector3TransformNormal(XMLoadFloat3(&g_vLook), XMLoadFloat4x4(&m_pInfo->m_matWorld));
	XMVector3Normalize(vecDir);
	XMStoreFloat3(&m_pInfo->m_vDir, vecDir);
	

	/*//테스트
	if (GetAsyncKeyState(VK_SPACE) & 0x0001)
	{

		m_dwCurAniIndex++;
#ifdef _CLIENTDEBUGING_
		printf("애니메이션 %d \n", m_dwCurAniIndex);
#endif 
		m_pCharMesh->SetAnimation(m_dwCurAniIndex);
	}*/


	if (m_pWeapon3rd)
		m_pWeapon3rd->Update();

	//cout << m_dwIndex << endl;
	Jump();
	m_fTime = Engine::Get_TimeMgr()->DeltaTime();

	m_dwIndex = Engine::Get_NaviMgr()->FindIndex(&m_pInfo->m_vPos);
	m_fTop = Engine::Get_NaviMgr()->FindfTop(&m_pInfo->m_vPos);

	Engine::CGameObject::Update();//컴포넌트들 업데이트
	return 0;
}

void CCharacter::Render()
{
	//if (m_tCharInfo.eState == CHARSTATE_NOTJOIN)//들어오지 않았으면
	//	return;

	XMMATRIX worldInvTranspose = MathHelper::InverseTranspose(XMLoadFloat4x4(&m_pInfo->m_matWorld));
	m_pMyGDI->pEffect->SetWorldInvTranspose(worldInvTranspose);
	if (m_pCharMesh)
		m_pCharMesh->RenderMesh(XMLoadFloat4x4(&m_pInfo->m_matWorld));
	if (m_pWeapon3rd)
		m_pWeapon3rd->Render();
}

void CCharacter::ApplyInput(CHARINPUT& pInput)
{

	if (m_tCharInfo.eState == CHARSTATE_DEAD)
		return;

	float fTime = Engine::Get_TimeMgr()->DeltaTime();
	bool bMove = false;

	if (Engine::Get_Input()->GetDIKeyState(DIK_LSHIFT))
	{
		m_dwState = STATE_RUN;
		m_fSpeed = 600;
	}
	else
	{
		m_fSpeed = 300;
	}

	if (CHK_KEY(pInput.iKeyMask , KEY_UP))
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

		m_dwNextAniIndex = ANI_UP;
		bMove = true;
	}
	if (CHK_KEY(pInput.iKeyMask, KEY_DOWN))
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
		m_dwNextAniIndex = ANI_DOWN;
		bMove = true;

		//XMVECTOR VP = XMLoadFloat3(&m_pInfo->m_vPos);
		//XMVECTOR VD = XMLoadFloat3(&m_pInfo->m_vDir);
		//VP -= VD * m_fSpeed * fTime;
		//XMStoreFloat3(&m_pInfo->m_vPos, VP);
	}
	if (CHK_KEY(pInput.iKeyMask, KEY_RIGHT))
	{
		m_pInfo->m_fAngle[Engine::ANGLE_Y] += XMConvertToRadians(90) * fTime;

		//XMVECTOR VP = XMLoadFloat3(&m_pInfo->m_vPos);
		//XMVECTOR VD = XMLoadFloat3(&m_pInfo->m_vDir);
		//XMVECTOR vUp = XMLoadFloat3(&XMFLOAT3(m_pInfo->m_matWorld._21, m_pInfo->m_matWorld._22, m_pInfo->m_matWorld._23));

		//XMVECTOR vRight = XMVector3Cross(vUp, VD);
		//vRight = XMVector3Normalize(vRight);
		//

		//if (m_dwState != STATE_RUN)
		//{
		//	if (m_dwState == STATE_SIT || m_dwState == STATE_SITATT || m_dwState == STATE_SITWALK)
		//		m_dwState = STATE_SITWALK;
		//	else
		//		m_dwState = STATE_WALK;
		//}

		//if (m_bJump || m_bDown || m_fTop > 8.f) //1층을제외한건 걍떨어지게   //계단높이!
		//{
		//	VP -= vRight * m_fSpeed * fTime;
		//	XMStoreFloat3(&m_pInfo->m_vPos, VP);
		//}
		//else
		//{
		//	m_dwIndex = Engine::Get_NaviMgr()->MoveOnNaviMesh(&m_pInfo->m_vPos
		//		, &XMFLOAT3(m_pInfo->m_vDir.x*XMVectorGetX(VP)*fTime, m_pInfo->m_vDir.y* XMVectorGetY(VP)*fTime, m_pInfo->m_vDir.z*XMVectorGetZ(VP)*fTime)
		//		, m_dwIndex, m_fTop);
		//}
		//m_dwNextAniIndex = ANI_RIGHT;
		bMove = true;
	}
	//if (Engine::Get_Input()->GetDIKeyState(DIK_RIGHTARROW))
	if (CHK_KEY(pInput.iKeyMask, KEY_LEFT))
	{
		m_pInfo->m_fAngle[Engine::ANGLE_Y] += XMConvertToRadians(-90) * fTime;

		//XMVECTOR VP = XMLoadFloat3(&m_pInfo->m_vPos);
		//XMVECTOR VD = XMLoadFloat3(&m_pInfo->m_vDir);
		//XMVECTOR vUp = XMLoadFloat3(&XMFLOAT3(m_pInfo->m_matWorld._21, m_pInfo->m_matWorld._22, m_pInfo->m_matWorld._23));

		//XMVECTOR vRight = XMVector3Cross(vUp, VD);
		//vRight = XMVector3Normalize(vRight);
		//VP += vRight * m_fSpeed * fTime;
		//XMStoreFloat3(&m_pInfo->m_vPos, VP);
		//m_dwNextAniIndex = ANI_LEFT;


		//XMVECTOR VP = XMLoadFloat3(&m_pInfo->m_vPos);
		//XMVECTOR VD = XMLoadFloat3(&m_pInfo->m_vDir);
		//XMVECTOR vUp = XMLoadFloat3(&XMFLOAT3(m_pInfo->m_matWorld._21, m_pInfo->m_matWorld._22, m_pInfo->m_matWorld._23));

		//XMVECTOR vRight = XMVector3Cross(vUp, VD);
		//vRight = XMVector3Normalize(vRight);


		//if (m_dwState != STATE_RUN)
		//{
		//	if (m_dwState == STATE_SIT || m_dwState == STATE_SITATT || m_dwState == STATE_SITWALK)
		//		m_dwState = STATE_SITWALK;
		//	else
		//		m_dwState = STATE_WALK;
		//}

		//if (m_bJump || m_bDown || m_fTop > 8.f) //1층을제외한건 걍떨어지게   //계단높이!
		//{
		//	VP += vRight * m_fSpeed * fTime;
		//	XMStoreFloat3(&m_pInfo->m_vPos, VP);
		//}
		//else
		//{
		//	m_dwIndex = Engine::Get_NaviMgr()->MoveOnNaviMesh(&m_pInfo->m_vPos
		//		, &XMFLOAT3(-XMVectorGetX(VP)*fTime, -XMVectorGetY(VP)*fTime, -XMVectorGetZ(VP)*fTime)
		//		, m_dwIndex, m_fTop);
		//}
		//m_dwNextAniIndex = ANI_LEFT;
		bMove = true;
	}
	if (CHK_KEY(pInput.iKeyMask, KEY_SPACE))
	{
		if (!m_tCharInfo.bJump)
		{ //Jump();
		}
		if (m_bPush == true)
			return;
		m_bPush = true;
		m_bJump = true;
	}
	else
		m_bPush = false;

	if (bMove == false)
	{
		m_dwNextAniIndex = ANI_IDLE;
	}

	if (m_dwCurAniIndex != m_dwNextAniIndex)
	{
		m_dwCurAniIndex = m_dwNextAniIndex;
		m_pCharMesh->SetAnimation(m_dwCurAniIndex);

	}


}

CCharacter* CCharacter::Create(Engine::MYGDI* pMyGDI)
{
	CCharacter* pChar = new CCharacter(pMyGDI);

	if (FAILED(pChar->Initialize()))
		Engine::Safe_Delete(pChar);
	return pChar;
}

void CCharacter::SetPosDir(XMFLOAT3& vPos, float fAngleX, float fAngleY)
{
	m_pInfo->m_vPos = vPos;
	m_pInfo->m_fRotateX = fAngleX;
	m_pInfo->m_fAngle[Engine::ANGLE_Y] = fAngleY;
}

HRESULT CCharacter::AddComponent(void)
{
	Engine::CComponent*		pComponent = NULL;
	//Transform
	pComponent = m_pInfo = Engine::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pComponent));

	//Mesh
	pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"DynamicMesh_Sylva");
	m_pCharMesh = dynamic_cast<Engine::CDynamicMesh*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"DynamicMesh", pComponent));



	return S_OK;
}

void CCharacter::Release()
{
	Engine::Safe_Delete(m_pWeapon3rd);
	Safe_Delete(m_pMyCell);
}



void CCharacter::Jump()
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
