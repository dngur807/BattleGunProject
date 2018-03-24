#include "stdafx.h"
#include "MyCharacter.h"
#include "AssultRifle1st.h"
#include "FlakCannon1st.h"
#include "CameraMgr.h"
#include "DynamicMesh.h"
#include "Transform.h"
#include "Define.h"
#include "Struct.h"


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

	// รั น฿ป็
	if (CHK_KEY(pInput.iKeyMask, MOUSE_LEFT))
	{
		
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


