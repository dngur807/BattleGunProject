/*!
* \file Camera.h
* \date 2017/05/31 15:30
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

#ifndef Camera_h__
#define Camera_h__

#include "GameObject.h"

BEGIN(Engine)

class CBasicEffect;

class ENGINE_DLL CCamera
	: public CGameObject
{
protected:
	explicit CCamera(MYGDI* mMyGDI);

public:
	virtual ~CCamera(void);

public:
	virtual int Update(void);

public:
	void Invalidate_ViewMatrix(void);
	void Invalidate_ProjMatrix(void);
	XMFLOAT4X4 GetView() { return m_matView; }
	XMFLOAT4X4 GetProj() { return m_matProj; }
	XMFLOAT3 GetEye() { return m_vEye; }

protected:
	XMFLOAT4X4		m_matView;
	XMFLOAT3		m_vEye, m_vAt, m_vUp;

protected:
	XMFLOAT4X4		m_matProj;
	float			m_fFovY, m_fAspect, m_fNear, m_fFar;

protected:
	CBasicEffect*	m_pEffect;
};

END

#endif // Camera_h__