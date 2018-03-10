/*!
* \file StaticCamera.h
* \date 2017/05/31 15:37
*
* \author Administrator
* Contact: user@company.com
*
* \brief
*
* TODO: long description
*
* \note
*/

#ifndef StaticCamera_h__
#define StaticCamera_h__

#include "Camera.h"

namespace Engine
{
	class CTransform;
	class CVIBuffer;
	class CInput;
}

class CPlayer;

class CStaticCamera
	: public Engine::CCamera
{
private:
	explicit CStaticCamera(Engine::MYGDI* pMyGDI);

public:
	virtual ~CStaticCamera(void) = default;

public:
	virtual int Update(void);

public:
	/*static CStaticCamera* Create(Engine::MYGDI* pMyGDI
		, const XMFLOAT3* pEye, const XMFLOAT3* pAt, const Engine::CTransform* pGameObj);*/
	static CStaticCamera* Create(Engine::MYGDI* pMyGDI, const Engine::CTransform* pPlayer
		, const float& fFovY = XMConvertToRadians(60.0f), const float& fAspect = float(800) / float(600)
		, const float& fNear = 0.1f, const float& fFar = 3000.f);
private:
	HRESULT InitCamera(const Engine::CTransform* pPlayer, const float& fFovY, const float& fAspect, const float& fNear, const float& fFar);
	void KeyCheck(void);
	void FixMouse(void);
	
private:
	float		m_fTimeDelta=0.f;
	float		m_fWheelSpeed;
	float		m_fCamSpeed;
	bool		m_bMouseFix;
	bool		m_bClick;
	const Engine::CTransform*	m_pTargetInfo;
	float		m_fTargetDistance;
	float       m_fCameraHeight;
	const Engine::CTransform*		m_pPlayerTransform;

	CPlayer*						m_pPlayer;
	const XMFLOAT4X4*				m_pmatPlayerHeadInfo;
	const XMFLOAT4X4*				m_pmatCameraRef;

	//카메라 줌인 최대 최소 거리 제한
private:
	bool							m_bMaxHighDistanceState = false;
	bool							m_bMaxLowDistanceState = false;

	Engine::CInput*			m_pInputDev_Instance = nullptr;
	float							m_fAngle[Engine::ANGLE_END];

};

#endif // StaticCamera_h__