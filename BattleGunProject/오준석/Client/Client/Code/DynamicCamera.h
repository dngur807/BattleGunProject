#ifndef DynamicCamera_h__
#define DynamicCamera_h__

#include "Camera.h"

class CDynamicCamera
	: public Engine::CCamera
{
private:
	explicit CDynamicCamera(Engine::MYGDI* pMyGDI);
public:
	virtual ~CDynamicCamera();

public:
	HRESULT InitCamera(const XMFLOAT3* pEye, const XMFLOAT3* At);
	void KeyCheck();
	void FixMouse();
	void MouseMove();

public:
	virtual int Update();
public:
	static CDynamicCamera* Create(Engine::MYGDI* pMyGDI, const XMFLOAT3* pEye, const XMFLOAT3* pAt);
private:
	float			m_fSpeed;
	bool			m_bMouseFix;
	bool			m_bClick;
};
#endif // DynamicCamera_h__