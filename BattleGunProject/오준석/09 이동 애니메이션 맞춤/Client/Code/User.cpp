#include "stdafx.h"
#include "User.h"
#include "Character.h"
#include "Management.h"
#include "Scene.h"
#include "Engine_Function.h"

CUser::CUser()
	: m_pCharacter(NULL)
{

}

CUser::~CUser()
{

}

void CUser::SetCharacter(CCharacter* pCharacter)
{
	/*if (m_pCharacter != NULL)
	{
		Engine::Safe_Delete(m_pCharacter);
		m_pCharacter = NULL;
	}*/

	m_pCharacter = pCharacter;
	m_pCharacter->GetCharInfo().iUserIndex = m_tUserInfo.iIndex;
	m_pCharacter->GetCharInfo().iTeam = (int)m_tUserInfo.eTeam;

}



void CUser::NullCharacter()
{
	
	m_pCharacter = NULL;
}

void CUser::Update()
{
	m_pCharacter->ApplyInput(m_tCharInput);
}

CCharacter* CUser::GetCharacter()
{
	return m_pCharacter;
}

void CUser::SetDead()
{
	//m_pPlayer->SetDead();
}

void CUser::SetInput(UINT iInput)
{
	m_tCharInput.iKeyMask = iInput;
}

