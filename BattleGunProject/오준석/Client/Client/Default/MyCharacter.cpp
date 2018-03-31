#include "stdafx.h"
#include "MyCharacter.h"
#include "AssultRifle1st.h"
#include "FlakCannon1st.h"
#include "CameraMgr.h"
#include "DynamicMesh.h"
#include "Transform.h"
#include "Define.h"
#include "Struct.h"
#include "NetMgr.h"
#include "Export_Function.h"

CMyCharacter::CMyCharacter(Engine::MYGDI* pMyGDI)
	: CCharacter(pMyGDI)
	, m_pSelectedWeapon(NULL)
{

}

CMyCharacter::~CMyCharacter()
{
	Release();
}

HRESULT CMyCharacter::Initialize()
{
	CCharacter::Initialize();
	FAILED_CHECK(AddComponent());

	m_vecWeapon1st.push_back(CAssultRifle1st::Create(m_pMyGDI, this));
	m_vecWeapon1st[0]->GetWeaponInfo()->iUserIndex = m_tCharInfo.iUserIndex;
	m_vecWeapon1st.push_back(CFlakCannon1st::Create(m_pMyGDI, this));
	m_vecWeapon1st[1]->GetWeaponInfo()->iUserIndex = m_tCharInfo.iUserIndex;

	m_pSelectedWeapon = m_vecWeapon1st[1];

	CCameraMgr::GetInstance()->StaticTrace(true);

	CCameraMgr::GetInstance()->SetFPSEye(m_pCharMesh->FindFrame("Bip01_Neck"));
	return S_OK;
}

int CMyCharacter::Update()
{
	CCharacter::Update();

	m_dwNaviIndex = Engine::Get_NaviMgr()->FindIndex(&m_pInfo->m_vPos);
	m_fNaviTop = Engine::Get_NaviMgr()->FindfTop(&m_pInfo->m_vPos);

	Jump();
	if (m_tCharInfo.eState != CHARSTATE_DEAD)
	{
		m_pSelectedWeapon->Update();
	}
	return 0;
}

void CMyCharacter::ApplyInput(CHARINPUT& pInput)
{
	if (m_tCharInfo.eState == CHARSTATE_DEAD)
		return;

	CCharacter::ApplyInput(pInput);

	// 총 발사
	if (CHK_KEY(pInput.iKeyMask, MOUSE_LEFT))
	{
		//총 교체 중 아니면
		m_pSelectedWeapon->LeftAction();
	}
}

void CMyCharacter::Render()
{
	/*if (m_tCharInfo.eState == CHARSTATE_NOTJOIN)
		return;*/
	if (g_iCamMode == CAMERA_STATIC)
	{
		CCharacter::Render();
	}
	else if (g_iCamMode == CAMERA_FPS)
	{
		//m_pCharMesh->RenderMesh(XMLoadFloat4x4(&m_pInfo->m_matWorld));
		m_pSelectedWeapon->Render();
	}
}

void CMyCharacter::GetDamage(int iAttackerIndex, int iDamage)
{
	if (m_tCharInfo.iHp <= 0)
		return;


	m_tCharInfo.iHp -= iDamage;
#ifdef _CLIENTDEBUGING_
	printf("내 현재 HP %d\n", m_tCharInfo.iHp);
#endif
	//Hit 유아이

	if (m_tCharInfo.iHp <= 0)
	{
		m_tCharInfo.iHp = 0;
		if (g_bNetwork)
		{
			//죽었다.. 킬로그 뛰우자
			CNetMgr::GetInstance()->RequestDead(g_iIndex, iAttackerIndex);
		}
		//hp 동기화
		CNetMgr::GetInstance()->RequestHpSync(g_iIndex, m_tCharInfo.iHp);
	}
	else
	{
		//피만 깍임
	}
}

void CMyCharacter::Shoot(void)
{
	CCharacter::Shoot();
}

void CMyCharacter::SetDead()
{
	if (m_tCharInfo.eState == CHARSTATE_DEAD)
		return;
	g_eMyState = CHARSTATE_DEAD;

	CCharacter::SetDead();
	m_pSelectedWeapon->NoAction();
	CCameraMgr::GetInstance()->SetCamera(CAMERA_DYNAMIC);
	CCameraMgr::GetInstance()->Update();
}

HRESULT CMyCharacter::AddComponent()
{
	Engine::CComponent*		pComponent = NULL;
	return S_OK;
}

void CMyCharacter::Release()
{
	for (auto iter : m_vecWeapon1st)
	{
		delete iter;
	}
	m_vecWeapon1st.clear();
}

CMyCharacter* CMyCharacter::Create(Engine::MYGDI* pMyGDI)
{
	CMyCharacter*	pChar = new CMyCharacter(pMyGDI);
	if (FAILED(pChar->Initialize()))
		Engine::Safe_Delete(pChar);
	return pChar;
}


void CMyCharacter::Jump()
{
	if (m_bJump == true && m_bDown == false)
	{
		m_fJumpAcc += 6.f*m_fTime;
		m_pInfo->m_vPos.y += -0.01f + m_fJumpAcc;
	}

	// 오준석//18-01-11// 올라가는 네비메쉬 자연스럽게 조건
	if (m_dwNaviIndex == 2 || m_dwNaviIndex == 3
		|| m_dwNaviIndex == 14 || m_dwNaviIndex == 17
		|| m_dwNaviIndex == 18 || m_dwNaviIndex == 19
		|| m_dwNaviIndex == 32 || m_dwNaviIndex == 33
		|| m_dwNaviIndex == 31 || m_dwNaviIndex == 28) //1~2층
	{
		if (m_pInfo->m_vPos.y > 266.f + m_fNaviTop)
			m_bDown = true;
	}
	else
	{
		if (m_pInfo->m_vPos.y > 10.f + m_fNaviTop)
			m_bDown = true;

	}

	if (m_bDown == true)
	{
		m_fJumpAcc -= 2.f*m_fTime;

		m_pInfo->m_vPos.y += -0.01f + m_fJumpAcc;

	}
	if (m_pInfo->m_vPos.y <= m_fNaviTop  && m_bDown == true)
	{

		m_bDown = false;
		m_bJump = false;
		m_fJumpAcc = 0.f;
		m_pInfo->m_vPos.y = m_fNaviTop;

	}

}

