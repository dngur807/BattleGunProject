#include "stdafx.h"
#include "UI_HUD.h"
#include "VIBuffer.h"
#include "Texture.h"
#include "Export_Function.h"
#include "CameraMgr.h"
#include "UserMgr.h"
#include "User.h"
#include "MyCharacter.h"
#include "Text.h"
#include "Me.h"
#include "GameMgr.h"
#include "Character.h"
#include "Transform.h"
CUI_Hud::CUI_Hud(Engine::MYGDI* pMyGDI)
	: CUI(pMyGDI)
	, m_pMyCharacter(NULL)

{
	m_bHit = false;
	m_fHitAlpha = 0.f;
}

CUI_Hud::~CUI_Hud()
{
	Engine::Safe_Delete(m_pIDText);
	Engine::Safe_Delete(m_pHPText);
}

HRESULT CUI_Hud::Initialize()

{
	FAILED_CHECK(AddComponent());

	// 직교 투영
	XMMATRIX matproj = XMMatrixOrthographicLH(WINCX, WINCY, 0.0f, 1.0f);
	XMStoreFloat4x4(&m_matProj, matproj);
	XMStoreFloat4x4(&m_matWorld, XMMatrixIdentity());


	return S_OK;

}

int CUI_Hud::Update()
{
	if (m_bHit)
	{
		m_fHitAlpha -= Engine::Get_TimeMgr()->DeltaTime() * 2.0f;
		if (m_fHitAlpha < 0.f)
		{
			m_bHit = false;
			m_fHitAlpha = 0.0f;
		}
	}
	

	return 0;
}

void CUI_Hud::Render()
{
	m_pMyGDI->pEffect->SetUseLighting(false);
//	Engine::Get_GraphicDev()->Belend_Begin();
	m_pMyGDI->pTech = m_pMyGDI->pEffect->GetLight4Tech();
	CCameraMgr::GetInstance()->GetTransFormView(&matOldView);
	CCameraMgr::GetInstance()->GetTransFormProj(&matOldProj);



	//여기부터 UI 출력하자
	CCameraMgr::GetInstance()->SetTransFormView(&m_matView);
	CCameraMgr::GetInstance()->SetTransFormProj(&m_matProj);

	MakeNameTag();

	RenderHUD();
	RenderNameTag();
	RenderCrossHair();
	RenderText();

	if (CGameMgr::GetInstance()->GetGameInfo().m_eServState == SERVSTATE_GAMEEND)
		RenderResult();
	m_pMyGDI->pEffect->SetUseLighting(true);
	//Engine::Get_GraphicDev()->Blend_End();

	CCameraMgr::GetInstance()->SetTransFormView(&matOldView);
	CCameraMgr::GetInstance()->SetTransFormProj(&matOldProj);

}

void CUI_Hud::SetMyCharacter(CMyCharacter* pCharacter)
{
	m_pMyCharacter = pCharacter;

	lstrcpy(m_szIDText, L"ID : ");
	lstrcat(m_szIDText, CUserMgr::GetInstance()->GetMe()->GetUserInfo().szID);
	m_pIDText->SetText(m_szIDText);

	m_pMyCharacter->SetUI_Hud(this);
}

void CUI_Hud::Hit()
{
	m_bHit = true;
	m_fHitAlpha = 1.0f;
}

void CUI_Hud::GameResult( bool IsVictory)
{
	m_IsVictory = IsVictory;
}

HRESULT CUI_Hud::AddComponent()
{
	Engine::CComponent*		pComponent = NULL;
	//CrossHair
	pComponent = m_pResourceMgr->CloneResource(RESOURCE_STATIC, L"Buffer_RcTex");
	m_pCrossHair = dynamic_cast<Engine::CVIBuffer*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"CrossHairBuffer", pComponent));


	pComponent = m_pResourceMgr->CloneResource(RESOURCE_STATIC, L"Buffer_RcTex");
	m_pBuffer = dynamic_cast<Engine::CVIBuffer*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"CrossHairBuffer", pComponent));


	pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Texture_CrossHair");
	m_pCrossHairTex = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"CrossHairTexture", pComponent));


	//HUD
	pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Texture_HealthBack");
	m_pHealthBackTex = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture_HealthBack", pComponent));
	
	pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Texture_HealthFront");
	m_pHealthFrontTex = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture_HealthFront", pComponent));


	pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Texture_Port");
	m_pPort = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture_Port", pComponent));


	pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Texture_HitEffect");
	m_pHitEffectTex = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture_HitEffect", pComponent));


	pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Texture_Victory");
	m_pVictoryTex = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture_Victory", pComponent));


	pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Texture_Defeat");
	m_pDefeatTex = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture_Defeat", pComponent));

	m_pIDText = CText::Create(m_pMyGDI, L"Font_koverwatch", 15);
	m_pIDText-> SetColor(D3DCOLOR_ARGB(255, 255, 255, 255));
	m_pIDText->SetSize(30);
	m_pIDText->SetPos(220, 600);

	m_pHPText = CText::Create(m_pMyGDI, L"Font_koverwatch", 15);
	m_pHPText->SetColor(D3DCOLOR_ARGB(255, 255, 255, 255));
	m_pHPText->SetSize(30);
	m_pHPText->SetPos(220, 650);

	for (int i = 0; i < 8; ++i)
	{
		m_tNameTag[i].m_pText = CText::Create(m_pMyGDI, L"Font_koverwatch", 15);
	}
	return S_OK;
}

void CUI_Hud::RenderHUD()
{
	if (m_bHit)
	{
		m_matView._11 = 1280.0f;
		m_matView._22 = 720.0f;
		m_matView._41 = 0;
		m_matView._42 = 0;

		CCameraMgr::GetInstance()->SetTransFormView(&m_matView);
		m_pHitEffectTex->Render(0, 0);
		m_pBuffer->Render(m_matWorld);
	}
	float fScaleX = 395.0f;
	m_matView._11 = fScaleX;
	m_matView._22 = 16.f  * 3;
	m_matView._41 = -230;
	m_matView._42 = -350;

	CCameraMgr::GetInstance()->SetTransFormView(&m_matView);
	m_pHealthBackTex->Render(0, 0);
	m_pBuffer->Render(m_matWorld);
	

	m_matView._11 = ((float)m_pMyCharacter->GetCharInfo().iHp / m_pMyCharacter->GetCharInfo().iMaxHp) * fScaleX;
	m_matView._41 =  -230 - ( 1 - (float)m_pMyCharacter->GetCharInfo().iHp / m_pMyCharacter->GetCharInfo().iMaxHp ) *  195;
	CCameraMgr::GetInstance()->SetTransFormView(&m_matView);
	m_pHealthFrontTex->Render(0, 0);
	m_pBuffer->Render(m_matWorld);
	

	m_matView._11 = 512 / 2;
	m_matView._22 = 512 / 2;
	m_matView._41 = -510;
	m_matView._42 = -300;
	CCameraMgr::GetInstance()->SetTransFormView(&m_matView);
	m_pPort->Render(0, 0);
	m_pBuffer->Render(m_matWorld);
}

void CUI_Hud::RenderCrossHair()
{
	m_matView._11 = 50.f;
	m_matView._22 = 50.f;
	m_matView._41 = WINCX / 2.0f - (WINCX >> 1);
	m_matView._42 = -(WINCY / 2.0f) + (WINCY >> 1);
	CCameraMgr::GetInstance()->SetTransFormView(&m_matView);

	// 1이 기본
	m_pCrossHairTex->Render(0, 1);
	m_pCrossHair->Render(m_matWorld);

}

void CUI_Hud::RenderText()
{
	m_pIDText->Render();
	char buf[256];
	TCHAR Buf[256];
	itoa(m_pMyCharacter->GetCharInfo().iHp, buf, 10);
	//WideCharToMultiByte(CP_ACP, 0, m_pChatText->GetText(), -1, cTemp, MAX_STR, NULL, NULL);
	MultiByteToWideChar(CP_ACP, 0, buf, -1, Buf, sizeof(Buf));
	lstrcpy(m_szHPText, L"HP : ");
	lstrcat(m_szHPText, Buf);
	lstrcat(m_szHPText, L" / 100");
	m_pIDText->SetText(m_szIDText);
	m_pHPText->SetText(m_szHPText);
	m_pHPText->Render();
}

void CUI_Hud::RenderResult()
{
	if (m_IsVictory == true)
	{
		//승리
		m_matView._11 = 1280.0f / 2;
		m_matView._22 = 720.0f / 2;
		m_matView._41 = 0;
		m_matView._42 = 0;

		CCameraMgr::GetInstance()->SetTransFormView(&m_matView);
		m_pVictoryTex->Render(0, 0);
		m_pBuffer->Render(m_matWorld);

	}
	else
	{
		//패배
		m_matView._11 = 1280.0f;
		m_matView._22 = 720.0f;
		m_matView._41 = 0;
		m_matView._42 = 0;

		CCameraMgr::GetInstance()->SetTransFormView(&m_matView);
		m_pDefeatTex->Render(0, 0);
		m_pBuffer->Render(m_matWorld);
	}
}

void CUI_Hud::RenderNameTag()
{
	DWORD dColor;
	XMMATRIX matTrans;
	XMMATRIX matScale;
	XMMATRIX matWorld;
	XMMATRIX matRot;
	XMFLOAT4X4 mat4x4Wolrd;
	for (int i = 0; i < CUserMgr::GetInstance()->GetUserList()->size(); i++)
	{
		//if (m_tNameTag[i].fDist > 500.0f)
			//continue;

		if (m_tNameTag[i].m_bActive == false || m_tNameTag[i].m_bScreen == false)
			continue;
		/*if ( m_tNameTag[i].m_bScreen == false)
			continue;*/
		CUser* pUser = (*CUserMgr::GetInstance()->GetUserList())[i];

		if (pUser == NULL )
			continue;

		if (g_iIndex == pUser->GetUserInfo().iIndex)
			continue;

		CCharacter* pCharacter = pUser->GetCharacter();

		if (m_tNameTag[i].m_bTeam)
		{
			dColor = D3DCOLOR_ARGB(255 , 255, 0, 0);
		}
		else
		{
			dColor = D3DCOLOR_ARGB(255 , 0, 0, 255);
		}
		lstrcpy(m_szText, pUser->GetUserInfo().szID);
	
	
		//폰트 랜더
		if (m_tNameTag[i].m_pText)
		{
			m_tNameTag[i].m_pText->SetColor(dColor);
			m_tNameTag[i].m_pText->SetSize(20);
			m_tNameTag[i].m_pText->SetPos(m_tNameTag[i].vPos.x - 10, m_tNameTag[i].vPos.y + 20 );
			m_tNameTag[i].m_pText->SetText(m_szText);
			m_tNameTag[i].m_pText->Render();
		}

		float fScaleX = 395.0f;

	
		m_matView._11 = fScaleX * 0.3f;
		m_matView._22 = 16.f * 1.0f;
		m_matView._41 = -WINCX / 2 + m_tNameTag[i].vPos.x;
		m_matView._42 = +WINCY / 2 - m_tNameTag[i].vPos.y  - 50 ;

		CCameraMgr::GetInstance()->SetTransFormView(&m_matView);
		m_pHealthBackTex->Render(0, 0);
		m_pBuffer->Render(m_matWorld);


	
		m_matView._11 = ((float)pCharacter->GetCharInfo().iHp / pCharacter->GetCharInfo().iMaxHp) * fScaleX * 0.3f;
		m_matView._41 = -WINCX / 2 + m_tNameTag[i].vPos.x - (1 - (float)pCharacter->GetCharInfo().iHp / pCharacter->GetCharInfo().iMaxHp) * (205) / 3.5f;
		CCameraMgr::GetInstance()->SetTransFormView(&m_matView);
		m_pHealthFrontTex->Render(0, 0);
		m_pBuffer->Render(m_matWorld);
	}
}

void CUI_Hud::MakeNameTag()
{
	auto iter = CUserMgr::GetInstance()->GetUserList()->begin();
	auto iter_end = CUserMgr::GetInstance()->GetUserList()->end();

	int	iTeam = m_pMyCharacter->GetCharInfo().iTeam;
	CCharacter* pCharacter = NULL;
	int iIndex = -1;

	for (iter; iter != iter_end; ++iter)
	{
		pCharacter = iter->second->GetCharacter();
		iIndex = pCharacter->GetCharInfo().iUserIndex;

		if (iIndex == g_iIndex)
			continue;

		if (pCharacter->GetCharInfo().iTeam == iTeam)
		{
			m_tNameTag[iIndex].m_bTeam = true;
		}
		else
			m_tNameTag[iIndex].m_bTeam = false;
		
		XMVECTOR vDir = XMLoadFloat3(&pCharacter->GetInfo()->m_vPos) - XMLoadFloat3(&m_pMyCharacter->GetInfo()->m_vPos);
		XMFLOAT3 vecDist;
		XMStoreFloat3(&vecDist, XMVector3Length(vDir));
		m_tNameTag[iIndex].fDist = vecDist.x;


		XMFLOAT3 vPos = pCharacter->GetInfo()->m_vPos;
		vPos.y += 110 + m_tNameTag[iIndex].fDist / 20;
	
		m_tNameTag[iIndex].vPos = GetScreenPos(vPos
			, matOldView
			, matOldProj);

		m_tNameTag[iIndex].vPos.z = 0.0f;
		if (pCharacter->GetCharInfo().iHp <=  0)
		{
			m_tNameTag[iIndex].m_bScreen = false;
		}
		if (m_tNameTag[iIndex].vPos.x > 0.f
			&& m_tNameTag[iIndex].vPos.x < 1280.f
			&& m_tNameTag[iIndex].vPos.y > 0.f
			&& m_tNameTag[iIndex].vPos.y < 720.f)
		{
			m_tNameTag[iIndex].m_bScreen = true;
		}
		else
		{
			m_tNameTag[iIndex].m_bScreen = false;
			continue;
		}

		

		vDir = XMVector3Normalize(vDir);

		XMVECTOR vDot = XMVector3Dot(XMLoadFloat3(&m_pMyCharacter->GetInfo()->m_vDir), vDir);

		XMFLOAT3 vecDot;
		XMStoreFloat3(&vecDot, vDot);

		if (acosf(vecDot.x) <= 3.141592 / 2.f)
		{

			m_tNameTag[iIndex].m_bScreen = false;
			continue;
		}

		m_tNameTag[iIndex].m_bShow = true;

	}
}

CUI_Hud* CUI_Hud::Create(Engine::MYGDI* pMyGDI)
{
	CUI_Hud* pHD = new CUI_Hud(pMyGDI);
	pHD->Initialize();
	return pHD;
}
