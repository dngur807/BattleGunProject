#pragma once

#include "Engine_Include.h"

BEGIN(Engine)

const int   MOUSEKEYMAX = 4;
const int   KEYMAX = 256;

class ENGINE_DLL CInput
{
public:
	enum MOUSECLICK { DIM_LBUTTON, DIM_RBUTTON, DIM_MBUTTON, };
	enum MOUSEMOVE { DIM_X, DIM_Y, DIM_Z, };

public:
	DECLARE_SINGLETON(CInput)

private:
	CInput(void);
	~CInput(void);

public:
	BYTE GetDIKeyState(BYTE KeyFlag);
	BYTE GetDIMouseState(MOUSECLICK KeyFlag);
	long GetDIMouseMove(MOUSEMOVE KeyFlag);
	bool OnceMouseKeyDown(MOUSECLICK byMouseID);
	bool OnceMouseKeyUp(MOUSECLICK byMouseID);
	bool OnceKeyDown(unsigned char byKeyID);
	bool OnceKeyUp(unsigned char byKeyID);

public:
	void SetInputState(void);

public:
	HRESULT InitInputDevice(HINSTANCE hInst, HWND hWnd);

private:
	HRESULT InitKeyBoard(HWND hWnd);
	HRESULT InitMouse(HWND hWnd);
	void Release(void);

private:
	LPDIRECTINPUT8			m_pInput;
	LPDIRECTINPUTDEVICE8	m_pKeyBoard;
	LPDIRECTINPUTDEVICE8	m_pMouse;

private:
	BYTE			m_byKeyState[256];
	DIMOUSESTATE	m_MouseState;

	bool						m_bMouseKeyDownState[MOUSEKEYMAX];		// 마우스 키가 눌렸는지 체크할 배열
	bool						m_bKeyDownState[KEYMAX];				// 키보드 키가 눌렸는지 체크할 배열
	bool						m_bKeyUpState[KEYMAX];				// 키보드 키가 띄었는지 체크할 배열
};

END