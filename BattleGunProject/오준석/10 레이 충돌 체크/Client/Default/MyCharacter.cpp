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

	// �� �߻�
	if (CHK_KEY(pInput.iKeyMask, MOUSE_LEFT))
	{
		//�� ��ü �� �ƴϸ�
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
	printf("�� ���� HP %d\n", m_tCharInfo.iHp);
#endif
	//Hit ������

	if (m_tCharInfo.iHp <= 0)
	{
		m_tCharInfo.iHp = 0;
		if (g_bNetwork)
		{
			//�׾���.. ų�α� �ٿ���
			CNetMgr::GetInstance()->RequestDead(g_iIndex, iAttackerIndex);
		}
		//hp ����ȭ
		CNetMgr::GetInstance()->RequestHpSync(g_iIndex, m_tCharInfo.iHp);
	}
	else
	{
		//�Ǹ� ����
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
	CCameraMgr::GetInstance()->SetCamera(CAMERA_STATIC);
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

