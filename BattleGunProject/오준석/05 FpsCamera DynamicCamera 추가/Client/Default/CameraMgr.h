#pragma once
#include "Engine_Include.h"
#include "Include.h"

namespace Engine
{
	class CCamera;
	class CTransform;
}

class CFpsCamera;
//class CStaticCamera;
class CDynamicCamera;

class CCameraMgr
{
public:
	DECLARE_SINGLETON(CCameraMgr)
private:
	CCameraMgr();
	~CCameraMgr();

public:
	void Initialize();
	void Update();
	void SetTargetInfo(const Engine::CTransform* pTargetInfo);
	void SetFPSEye(const XMFLOAT4X4* matEye);
	void SetCamera(CAMERAMODE eMode);
private:
	//ī�޶� ������
	CDynamicCamera*				m_pDynamicCamera;
	CFpsCamera*					m_pFpsCamera;
	Engine::CCamera*			m_pSelectedCamera;
};