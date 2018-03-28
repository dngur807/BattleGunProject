#include "Input.h"

IMPLEMENT_SINGLETON(Engine::CInput)

Engine::CInput::CInput(void)
	: m_pInput(NULL)
	, m_pKeyBoard(NULL)
	, m_pMouse(NULL)
{
	ZeroMemory(m_byKeyState, sizeof(BYTE) * 256);
	ZeroMemory(&m_MouseState, sizeof(DIMOUSESTATE));
}

Engine::CInput::~CInput(void)
{
	Release();
}

void Engine::CInput::SetInputState(void)
{
	HRESULT hr;
	hr = m_pKeyBoard->GetDeviceState(256, m_byKeyState);

	if ((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED))
		m_pKeyBoard->Acquire();

	hr = m_pMouse->GetDeviceState(sizeof(DIMOUSESTATE), &m_MouseState);

	if ((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED))
		m_pMouse->Acquire();
}

BYTE Engine::CInput::GetDIKeyState(BYTE KeyFlag)
{
	return m_byKeyState[KeyFlag];
}

BYTE Engine::CInput::GetDIMouseState(MOUSECLICK KeyFlag)
{
	return m_MouseState.rgbButtons[KeyFlag];
}

long Engine::CInput::GetDIMouseMove(MOUSEMOVE KeyFlag)
{
	return *(((long*)&m_MouseState) + KeyFlag);
}

HRESULT Engine::CInput::InitInputDevice(HINSTANCE hInst, HWND hWnd)
{
	HRESULT		hr = NULL;
	hr = DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInput, NULL);
	FAILED_CHECK(hr);

	hr = InitKeyBoard(hWnd);
	FAILED_CHECK(hr);

	hr = InitMouse(hWnd);
	FAILED_CHECK(hr);

	return S_OK;
}

HRESULT Engine::CInput::InitKeyBoard(HWND hWnd)
{
	HRESULT		hr = NULL;
	hr = m_pInput->CreateDevice(GUID_SysKeyboard, &m_pKeyBoard, NULL);
	FAILED_CHECK(hr);

	m_pKeyBoard->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	m_pKeyBoard->SetDataFormat(&c_dfDIKeyboard);
	m_pKeyBoard->Acquire();
	return S_OK;
}

HRESULT Engine::CInput::InitMouse(HWND hWnd)
{
	HRESULT		hr = NULL;
	hr = m_pInput->CreateDevice(GUID_SysMouse, &m_pMouse, NULL);
	FAILED_CHECK(hr);

	m_pMouse->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	m_pMouse->SetDataFormat(&c_dfDIMouse);
	m_pMouse->Acquire();
	return S_OK;
}

void Engine::CInput::Release(void)
{
	Safe_Release(m_pKeyBoard);
	Safe_Release(m_pMouse);
}



//  한번만 눌리는지 체크
bool    Engine::CInput::OnceMouseKeyDown(MOUSECLICK byMouseID)
{
	if (GetDIMouseState(byMouseID) & 0x80)
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

bool    Engine::CInput::OnceMouseKeyUp(MOUSECLICK byMouseID)
{

	const unsigned char    byKey = byMouseID;
	int iKey = 0;

	CopyMemory(&iKey, &byKey, sizeof(unsigned char));		// int 형으로 변환

	if (GetDIMouseState(byMouseID) & 0x80)
	{
		m_bMouseKeyDownState[byMouseID] = true;
	}

	else
	{
		// 눌려진 순간 true가 되므로 true일때만 진입
		if (m_bMouseKeyDownState[byMouseID] == true)
		{
			//  키상태를 false로 만들어 재진입 불가
			m_bMouseKeyDownState[byMouseID] = false;

			return TRUE;
		}
	}

	return FALSE;
	////

	//const unsigned char    byKey = byMouseID;
	//int iKey = 0;

	//CopyMemory(&iKey, &byKey, sizeof(unsigned char));		// int 형으로 변환

	//if (GetDIMouseState(byKey) & 0x80)
	//{
	//	m_bMouseKeyDownState[iKey] = true;
	//}

	//else
	//{
	//	// 눌려진 순간 true가 되므로 true일때만 진입
	//	if (m_bMouseKeyDownState[iKey] == true)
	//	{
	//		//  키상태를 false로 만들어 재진입 불가
	//		m_bMouseKeyDownState[iKey] = false;

	//		return TRUE;
	//	}
	//}

	//return FALSE;


}

//  한번만 눌리는지 체크
bool     Engine::CInput::OnceKeyDown(unsigned char byKeyID)
{
	const unsigned char    byKey = byKeyID;
	int iKey = 0;

	CopyMemory(&iKey, &byKey, sizeof(unsigned char));		// int 형으로 변환

	if (GetDIKeyState(byKey) & 0x80)
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

//  한번 눌렸다가 띄어지는지 체크
bool      Engine::CInput::OnceKeyUp(unsigned char byKeyID)
{
	const unsigned char    byKey = byKeyID;
	int iKey = 0;

	CopyMemory(&iKey, &byKey, sizeof(unsigned char));		// int 형으로 변환

	if (GetDIKeyState(byKey) & 0x80)
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