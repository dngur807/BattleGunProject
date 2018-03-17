#pragma once

#include "Engine_Include.h"

BEGIN(Engine)

const int   MOUSEKEYMAX = 4;
const int   KEYMAX = 256;

class ENGINE_DLL CInput
{
	DECLARE_SINGLETON(CInput)
public:
	enum MOUSEKEYSTATE { DIM_LB, DIM_RB, DIM_HB, DIM_END };
	enum MOUSEMOVESTATE { DIMS_X, DIMS_Y, DIMS_Z, DIMS_END };
private:
	CInput(void);
	~CInput(void);

public:
	BYTE Get_DIKeyState(BYTE byKeyID);
	BYTE Get_DIMouseState(MOUSEKEYSTATE byMouseID);
	long Get_DIMouseMove(MOUSEMOVESTATE byMouseMoveState);
	bool OnceMouseKeyDown(MOUSEKEYSTATE byMouseID);
	bool OnceKeyDown(unsigned char byKeyID);
	bool OnceKeyUp(unsigned char byKeyID);
public:
	void SetInputState(void);
public:
	HRESULT Ready_InputDevice(HINSTANCE hInst, HWND hWnd);
	HRESULT Init_KeyBoard(HWND hWnd);
	HRESULT Init_Mouse(HWND hWnd);
	void SetUp_InputState(void);
private:
	void Release(void);


private:
	// 내가 입력장치의 기본 설정을 셋팅하낟.
	LPDIRECTINPUT8				m_pInputSDK;
private:
	LPDIRECTINPUTDEVICE8		m_pKeyBoard;
	LPDIRECTINPUTDEVICE8		m_pMouse;
private:
	signed char						m_byKeyState[KEYMAX];
	DIMOUSESTATE				m_MouseState;;

	bool						m_bMouseKeyDownState[MOUSEKEYMAX];		// 마우스 키가 눌렸는지 체크할 배열
	bool						m_bKeyDownState[KEYMAX];				// 키보드 키가 눌렸는지 체크할 배열
	bool						m_bKeyUpState[KEYMAX];				// 키보드 키가 띄었는지 체크할 배열
};

END