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
		, const XMFLOAT3* pEye, const XMFLOAT3* pAt);
	XMFLOAT3 GetEye() { return m_vEye; }
	HRESULT InitCamera(const XMFLOAT3* pEye, const XMFLOAT3* pAt);

private:
	void KeyCheck(void);
	void FixMouse(void);
	void MouseMove(void);

private:
	float		m_fCamSpeed;
	bool		m_bMouseFix;
	bool		m_bClick;
	XMFLOAT3    m_pEye;
};

#endif // DynamicCamera_h__