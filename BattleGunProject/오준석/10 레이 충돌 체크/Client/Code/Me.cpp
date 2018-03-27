#include "stdafx.h"
#include "Me.h"
#include "Export_Function.h"
#include "Character.h"
#include "NetMgr.h"
#include "Transform.h"

CMe::CMe()
	: m_fTime(0.0f)
{

}

CMe::~CMe()
{

}

void CMe::Update()
{
	if (g_bNetwork == true)
	{
		Control_Character();
		m_pCharacter->ApplyInput(m_tCharInput);

		m_fTime += 30.0f * Engine::Get_TimeMgr()->DeltaTime();

		if (m_fTime > 1.0f)
		{
			m_fTime = 0.0f;
			CNetMgr::GetInstance()->RequestPosDir(m_tCharInput.iKeyMask , m_pCharacter->GetInfo()->m_vPos,
				m_pCharacter->GetInfo()->m_fRotateX, m_pCharacter->GetInfo()->m_fAngle[Engine::ANGLE_Y]);

		}
	}
	else// ½Ì±Û
	{
		Control_Character();
		m_pCharacter->ApplyInput(m_tCharInput);
	}
}

void CMe::Control_Character()
{
	m_tCharInput.iKeyMask = 0;

	if (Engine::Get_Input()->GetDIKeyState(DIK_W) & 0x80)
	{
		m_tCharInput.iKeyMask |= KEY_UP;
	}

	if (Engine::Get_Input()->GetDIKeyState(DIK_S) & 0x80)
	{
		m_tCharInput.iKeyMask |= KEY_DOWN;
	}

	if (Engine::Get_Input()->GetDIKeyState(DIK_A) & 0x80)
	{
		m_tCharInput.iKeyMask |= KEY_LEFT;
	}

	if (Engine::Get_Input()->GetDIKeyState(DIK_D) & 0x80)
	{
		m_tCharInput.iKeyMask |= KEY_RIGHT;
	}

	if (Engine::Get_Input()->GetDIKeyState(DIK_SPACE) & 0x80)
	{
		m_tCharInput.iKeyMask |= KEY_SPACE;
	}

	if (Engine::Get_Input()->GetDIMouseState(Engine::CInput::MOUSECLICK(0)) & 0x80)
	{
		m_tCharInput.iKeyMask |= MOUSE_LEFT;
	}
	if (Engine::Get_Input()->GetDIMouseState(Engine::CInput::MOUSECLICK(1)) & 0x80)
	{
		m_tCharInput.iKeyMask |= MOUSE_RIGHT;
	}

}



