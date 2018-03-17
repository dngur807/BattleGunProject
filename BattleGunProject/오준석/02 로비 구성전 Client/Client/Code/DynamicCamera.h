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
	class CComponent;
	class CTransform;
};
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
		, const XMFLOAT3* pEye, const XMFLOAT3* pAt, const  Engine::CTransform* pPlayer);

private:
	HRESULT InitCamera(const XMFLOAT3* pEye, const XMFLOAT3* pAt);
	void KeyCheck(void);
	void FixMouse(void);
	void MouseMove(void);

private:
	float		m_fCamSpeed;
	bool		m_bMouseFix;
	bool		m_bClick;
	const  Engine::CTransform* m_pPlayer;
	Engine::CTransform* m_pRealPlayer;

	void TargetRenewal();
	float	m_fDynmaicTargetDistance;
	float   m_fDynamicHeight;
};

#endif // DynamicCamera_h__