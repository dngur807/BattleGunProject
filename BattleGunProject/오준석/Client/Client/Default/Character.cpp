#include "stdafx.h"
#include "Character.h"
#include "Export_Function.h"
#include "Weapon3rd.h"
#include "Transform.h"
#include "DynamicMesh.h"
#include "StaticMesh.h"
#include "SphereCol.h"
#include "CharCol.h"

CCharacter::CCharacter(Engine::MYGDI* pMyGDI)
	: CLandObject(pMyGDI)
	, m_pResourceMgr(Engine::Get_ResourceMgr())
	, m_fSpeed(50.0f * g_ScaleRatio)
	, m_pCharMesh(NULL)
	, m_pWeapon3rd(NULL)
	, m_dwCurAniIndex(0)
	, m_pSphereCol(NULL)
	, m_pCharCol(NULL)
	, m_dwNaviIndex(0)
	, m_pMyNaviCell(NULL)
	, m_fNaviTop(0)
{
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
	m_pInfo->m_fScaleFactor = 20.0f;
//	m_pInfo->m_fScaleFactor = 2.0f;
	m_pInfo->m_vPos = XMFLOAT3(float(rand() % 500), 0.0f, float(rand() % 500));
	m_pInfo->m_vScale = XMFLOAT3((1.0f * g_ScaleRatio) / m_pInfo->m_fScaleFactor, (1.0f * g_ScaleRatio) / m_pInfo->m_fScaleFactor, (1.0f * g_ScaleRatio) / m_pInfo->m_fScaleFactor);

	//m_pInfo->m_vScale *= 
	//m_pInfo->m_vScale *= g_ScaleRatio;

	m_dwCurAniIndex = ANI_IDLE;
	m_pCharMesh->SetAnimation(m_dwCurAniIndex);
	m_pCharMesh->SetAnimationTime(1);

	m_pInfo->m_fAngle[Engine::ANGLE_Y] = 90;
	return S_OK;
}

int CCharacter::Update()
{
	XMVECTOR vecDir =  XMVector3TransformNormal(XMLoadFloat3(&g_vLook), XMLoadFloat4x4(&m_pInfo->m_matWorld));
	XMVector3Normalize(vecDir);
	XMStoreFloat3(&m_pInfo->m_vDir, vecDir);
	//테스트
	/*if (GetAsyncKeyState(VK_SPACE) & 0x0001)
	{

		m_dwCurAniIndex++;
#ifdef _CLIENTDEBUGING_
		printf("애니메이션 %d \n", m_dwCurAniIndex);
#endif 
		m_pCharMesh->SetAnimation(m_dwCurAniIndex);
	}
*/
	m_dwNaviIndex = Engine::Get_NaviMgr()->FindIndex(&m_pInfo->m_vPos);
	m_fNaviTop = Engine::Get_NaviMgr()->FindfTop(&m_pInfo->m_vPos);

	if (m_pWeapon3rd)
		m_pWeapon3rd->Update();

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
	if (m_pWeapon3rd && m_tCharInfo.eState == CHARSTATE_ALIVE)
		m_pWeapon3rd->Render();
	if (g_bCollRender == true && m_pCharCol  && m_tCharInfo.eState == CHARSTATE_ALIVE)
	{
		m_pCharCol->Render();
	}
}

void CCharacter::ApplyInput(CHARINPUT& pInput)
{

	if (m_tCharInfo.eState == CHARSTATE_DEAD)
		return;

	float fTime = Engine::Get_TimeMgr()->DeltaTime();
	bool bMove = false;

	if (CHK_KEY(pInput.iKeyMask , KEY_UP))
	{
		XMVECTOR VP = XMLoadFloat3(&m_pInfo->m_vPos);
		XMVECTOR VD = XMLoadFloat3(&m_pInfo->m_vDir);
		//VP -= VD * m_fSpeed * fTime;
		//XMStoreFloat3(&m_pInfo->m_vPos, VP);

		//이동할 방향
		XMVECTOR vecDir = VD * m_fSpeed * fTime * -1.0f;
		XMFLOAT3 vDir;
		XMStoreFloat3(&vDir, vecDir);

		m_dwNaviIndex = Engine::Get_NaviMgr()->MoveOnNaviMesh(&m_pInfo->m_vPos
			, &vDir, m_dwNaviIndex, m_fNaviTop);

		m_dwNextAniIndex = ANI_UP;
		bMove = true;
	}
	if (CHK_KEY(pInput.iKeyMask, KEY_DOWN))
	{
		XMVECTOR VP = XMLoadFloat3(&m_pInfo->m_vPos);
		XMVECTOR VD = XMLoadFloat3(&m_pInfo->m_vDir);
		//VP += VD * m_fSpeed * fTime;
		//XMStoreFloat3(&m_pInfo->m_vPos, VP);


		XMVECTOR vecDir = VD * m_fSpeed * fTime;
		XMFLOAT3 vDir;
		XMStoreFloat3(&vDir, vecDir);

		m_dwNaviIndex = Engine::Get_NaviMgr()->MoveOnNaviMesh(&m_pInfo->m_vPos
			, &vDir, m_dwNaviIndex, m_fNaviTop);


		m_dwNextAniIndex = ANI_DOWN;
		bMove = true;
	}
	if (CHK_KEY(pInput.iKeyMask, KEY_RIGHT))
	{
		//m_pInfo->m_fAngle[Engine::ANGLE_Y] += XMConvertToRadians(-90) * fTime;
		XMVECTOR VP = XMLoadFloat3(&m_pInfo->m_vPos);
		XMVECTOR VD = XMLoadFloat3(&m_pInfo->m_vDir);
		XMVECTOR vUp = XMLoadFloat3(&XMFLOAT3(m_pInfo->m_matWorld._21, m_pInfo->m_matWorld._22, m_pInfo->m_matWorld._23));

		XMVECTOR vRight = XMVector3Cross(vUp, VD);
		vRight = XMVector3Normalize(vRight);
	/*	VP -= vRight * m_fSpeed * fTime;
		XMStoreFloat3(&m_pInfo->m_vPos, VP);*/


		XMVECTOR vecDir = vRight * m_fSpeed * fTime * -1.0f;
		XMFLOAT3 vDir;
		XMStoreFloat3(&vDir, vecDir);

		m_dwNaviIndex = Engine::Get_NaviMgr()->MoveOnNaviMesh(&m_pInfo->m_vPos
			, &vDir, m_dwNaviIndex, m_fNaviTop);


		m_dwNextAniIndex = ANI_RIGHT;
		bMove = true;
	}
	//if (Engine::Get_Input()->GetDIKeyState(DIK_RIGHTARROW))
	if (CHK_KEY(pInput.iKeyMask, KEY_LEFT))
	{
		XMVECTOR VP = XMLoadFloat3(&m_pInfo->m_vPos);
		XMVECTOR VD = XMLoadFloat3(&m_pInfo->m_vDir);
		XMVECTOR vUp = XMLoadFloat3(&XMFLOAT3(m_pInfo->m_matWorld._21, m_pInfo->m_matWorld._22, m_pInfo->m_matWorld._23));

		XMVECTOR vRight = XMVector3Cross(vUp, VD);
		vRight = XMVector3Normalize(vRight);
		//VP += vRight * m_fSpeed * fTime;
		//XMStoreFloat3(&m_pInfo->m_vPos, VP);

		XMVECTOR vecDir = vRight * m_fSpeed * fTime;
		XMFLOAT3 vDir;
		XMStoreFloat3(&vDir, vecDir);

		m_dwNaviIndex = Engine::Get_NaviMgr()->MoveOnNaviMesh(&m_pInfo->m_vPos
			, &vDir, m_dwNaviIndex, m_fNaviTop);

		bMove = true;

		m_dwNextAniIndex = ANI_LEFT;
	}
	if (CHK_KEY(pInput.iKeyMask, KEY_SPACE))
	{
		if (!m_tCharInfo.bJump)
		{ //Jump();
		}
	}
	
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

void CCharacter::Shoot(void)
{

}

void CCharacter::SetDead()
{
	if (m_tCharInfo.eState == CHARSTATE_DEAD)
		return;


	m_tCharInfo.iHp = 0;
	m_tCharInfo.eState = CHARSTATE_DEAD;

	m_dwCurAniIndex = ANI_DEAD;
	m_pCharMesh->SetAnimation(m_dwCurAniIndex);
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


	//SphereCol
	pComponent = m_pSphereCol = CSphereCol::Create(m_pMyGDI , 6.0f );
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_pSphereCol->SetTargetInfo(m_pInfo);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"SphereCol", pComponent));


	pComponent = m_pCharCol = CCharCol::Create(m_pMyGDI, &m_pInfo->m_matWorld , m_pCharMesh);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"CharCol", pComponent));


	return S_OK;
}

void CCharacter::Release()
{
	Engine::Safe_Delete(m_pWeapon3rd);
}

