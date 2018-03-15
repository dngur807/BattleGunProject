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