#include "Input.h"


IMPLEMENT_SINGLETON(Engine::CInput)

Engine::CInput::CInput(void)
	: m_pInputSDK(NULL)
	, m_pKeyBoard(NULL)
	, m_pMouse(NULL)
{
	ZeroMemory(m_bMouseKeyDownState, sizeof(bool) * MOUSEKEYMAX);
	ZeroMemory(m_bKeyDownState, sizeof(bool) * KEYMAX);

	ZeroMemory(m_byKeyState, sizeof(BYTE) * KEYMAX);
	ZeroMemory(&m_MouseState, sizeof(DIMOUSESTATE));
}

Engine::CInput::~CInput(void)
{
	Release();
}

BYTE Engine::CInput::Get_DIKeyState(BYTE byKeyID)
{
	return m_byKeyState[byKeyID];
}

BYTE Engine::CInput::Get_DIMouseState(MOUSEKEYSTATE byMouseID)
{
	return m_MouseState.rgbButtons[byMouseID];
}

long Engine::CInput::Get_DIMouseMove(MOUSEMOVESTATE byMouseMoveState)
{
	return *(((long*)&m_MouseState) + byMouseMoveState);
}

void Engine::CInput::SetInputState(void)
{
	m_pKeyBoard->GetDeviceState(KEYMAX, m_byKeyState);
	m_pMouse->GetDeviceState(sizeof(DIMOUSESTATE), &m_MouseState);
}

HRESULT  Engine::CInput::Ready_InputDevice(HINSTANCE hInst, HWND hWnd)
{
	HRESULT		hr = NULL;

	hr = DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInputSDK, NULL);
	FAILED_CHECK_MSG(hr, L"다이렉트 인풋 생성 실패");

	hr = Init_KeyBoard(hWnd);
	FAILED_CHECK(hr);

	hr = Init_Mouse(hWnd);
	FAILED_CHECK(hr);
	return S_OK;
}

HRESULT  Engine::CInput::Init_KeyBoard(HWND hWnd)
{
	HRESULT		hr = NULL;
	hr = m_pInputSDK->CreateDevice(GUID_SysKeyboard, &m_pKeyBoard, NULL);
	FAILED_CHECK(hr);

	//m_pKeyBoard->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
	m_pKeyBoard->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
	m_pKeyBoard->SetDataFormat(&c_dfDIKeyboard);
	m_pKeyBoard->Acquire();
}

HRESULT  Engine::CInput::Init_Mouse(HWND hWnd)
{
	HRESULT		hr = NULL;
	hr = m_pInputSDK->CreateDevice(GUID_SysMouse, &m_pMouse, NULL);
	FAILED_CHECK(hr);

	//m_pMouse->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
	m_pMouse->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
	m_pMouse->SetDataFormat(&c_dfDIMouse);
	m_pMouse->Acquire();
	return S_OK;
}
//  한번만 눌리는지 체크
bool Engine::CInput::OnceMouseKeyDown(MOUSEKEYSTATE byMouseID)
{
	if (Get_DIMouseState(byMouseID) & 0x80)
	{
		//  눌리는 순간 true
		if (m_bMouseKeyDownState[byMouseID] == false)
		{
			m_bMouseKeyDownState[byMouseID] = true;
			return TRUE;
		}
	}
	else
	{
		//  떼는 순간 false
		m_bMouseKeyDownState[byMouseID] = false;
	}

	return FALSE;
}

bool Engine::CInput::OnceKeyDown(unsigned char byKeyID)
{
	const unsigned char    byKey = byKeyID;
	int iKey = 0;

	CopyMemory(&iKey, &byKey, sizeof(unsigned char));		// int 형으로 변환

	if (Get_DIKeyState(byKey) & 0x80)
	{
		//  눌리는 순간 true
		if (m_bKeyDownState[iKey] == false)
		{
			m_bKeyDownState[iKey] = true;
			return TRUE;
		}
	}
	else
	{
		//  떼는 순간 false
		m_bKeyDownState[iKey] = false;
	}

	return FALSE;
}

bool Engine::CInput::OnceKeyUp(unsigned char byKeyID)
{
	const unsigned char    byKey = byKeyID;
	int iKey = 0;

	CopyMemory(&iKey, &byKey, sizeof(unsigned char));		// int 형으로 변환

	if (Get_DIKeyState(byKey) & 0x80)
	{
		m_bKeyUpState[iKey] = true;
	}

	else
	{
		// 눌려진 순간 true가 되므로 true일때만 진입
		if (m_bKeyUpState[iKey] == true)
		{
			//  키상태를 false로 만들어 재진입 불가
			m_bKeyUpState[iKey] = false;

			return TRUE;
		}
	}

	return FALSE;
}


void  Engine::CInput::SetUp_InputState(void)
{
	m_pKeyBoard->GetDeviceState(KEYMAX, m_byKeyState);
	m_pMouse->GetDeviceState(sizeof(DIMOUSESTATE), &m_MouseState);
}

//BYTE Engine::CInput::GetDIKeyState(BYTE KeyFlag)
//{
//	return m_byKeyState[KeyFlag];
//}
//
//BYTE Engine::CInput::GetDIMouseState(MOUSECLICK KeyFlag)
//{
//	return m_MouseState.rgbButtons[KeyFlag];
//}
//
//long Engine::CInput::GetDIMouseMove(MOUSEMOVE KeyFlag)
//{
//	return *(((long*)&m_MouseState) + KeyFlag);
//}
//
//HRESULT Engine::CInput::InitInputDevice(HINSTANCE hInst, HWND hWnd)
//{
//	HRESULT		hr = NULL;
//	hr = DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInput, NULL);
//	FAILED_CHECK(hr);
//
//	hr = InitKeyBoard(hWnd);
//	FAILED_CHECK(hr);
//
//	hr = InitMouse(hWnd);
//	FAILED_CHECK(hr);
//
//	return S_OK;
//}
//
//HRESULT Engine::CInput::InitKeyBoard(HWND hWnd)
//{
//	HRESULT		hr = NULL;
//	hr = m_pInput->CreateDevice(GUID_SysKeyboard, &m_pKeyBoard, NULL);
//	FAILED_CHECK(hr);
//
//	m_pKeyBoard->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
//	m_pKeyBoard->SetDataFormat(&c_dfDIKeyboard);
//	m_pKeyBoard->Acquire();
//	return S_OK;
//}
//
//HRESULT Engine::CInput::InitMouse(HWND hWnd)
//{
//	HRESULT		hr = NULL;
//	hr = m_pInput->CreateDevice(GUID_SysMouse, &m_pMouse, NULL);
//	FAILED_CHECK(hr);
//
//	m_pMouse->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
//	m_pMouse->SetDataFormat(&c_dfDIMouse);
//	m_pMouse->Acquire();
//	return S_OK;
//}

void Engine::CInput::Release(void)
{
	Safe_Release(m_pKeyBoard);
	Safe_Release(m_pMouse);
	Safe_Release(m_pInputSDK);
}