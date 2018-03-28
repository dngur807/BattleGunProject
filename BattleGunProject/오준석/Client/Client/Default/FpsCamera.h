#pragma once

#include "Camera.h"

namespace Engine
{
	class CTransform;
}
class CFpsCamera
	: public Engine::CCamera
{
private:
	explicit CFpsCamera(Engine::MYGDI* pMyGID);
public:
	virtual ~CFpsCamera();
public:
	void SetTargetInfo(const Engine::CTransform* pTargetInfo);

public:
	HRESULT Initialize();
	virtual int Update();
	void MouseMove();
	void SetFPSEye( const XMFLOAT4X4* matEye);

public:
	static CFpsCamera* Create(Engine::MYGDI* pMyGID);

private:
	const Engine::CTransform*			m_pTargetInfo;
	const XMFLOAT4X4*					m_matEye;
	float								m_fAngle;

};