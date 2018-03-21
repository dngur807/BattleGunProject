#include "stdafx.h"
#include "User.h"
#include "Player.h"
#include "Management.h"
#include "Scene.h"
#include "Engine_Function.h"

CUser::CUser()
	: m_pPlayer(NULL)
{

}

CUser::~CUser()
{

}

void CUser::SetCharacter(eCharType eType)
{
	if (m_pPlayer != NULL)
	{
		Engine::Safe_Delete(m_pPlayer);
		m_pPlayer = NULL;
	}

	//Engine::MYGDI*	pGDI = Engine::CManagement::GetInstance()->GetGDI();
	switch (eType)
	{
	case CHAR_DEFAULT:
		// 기본 캐릭터 생성
		//m_pPlayer = CPlayer::Create(pGDI);
		break;
	case CHAR_END:
		break;
	}
	//m_pPlayer->SetUserInfo(&m_tUserInfo);
}



void CUser::NullCharacter()
{
	m_pPlayer = NULL;
}

CPlayer* CUser::GetCharacter()
{
	return m_pPlayer;
}

void CUser::SetDead()
{
	//m_pPlayer->SetDead();
}

