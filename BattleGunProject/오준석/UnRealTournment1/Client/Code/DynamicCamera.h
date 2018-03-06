/*!
* \file DynamicCamera.h
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

#ifndef DynamicCamera_h__
#define DynamicCamera_h__

#include "Camera.h"

namespace Engine
{
	class CTransform;
	class CVIBuffer;
}

class CDynamicCamera
	: public Engine::CCamera
{
private:
	explicit CDynamicCamera(Engine::MYGDI* pMyGDI);

public:
	virtual ~CDynamicCamera(void);

public:
	virtual int Update(void);

public:
	static CDynamicCamera* Create(Engine::MYGDI* pMyGDI
		, const XMFLOAT3* pEye, const XMFLOAT3* pAt, const Engine::CTransform* pGameObj);

private:
	HRESULT InitCamera(const XMFLOAT3* pEye, const XMFLOAT3* pAt);
	void KeyCheck(void);
	void FixMouse(void);
	void MouseMove(void);
	void PlayerMouseMove(void);
	void TargetRenewal();

private:
	float		m_fCamSpeed;
	bool		m_bMouseFix;
	bool		m_bClick;
	const Engine::CTransform*	m_pTargetInfo;
	float		m_fTargetDistance;
	float       m_fCameraHeight;

};

#endif // DynamicCamera_h__