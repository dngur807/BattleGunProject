#pragma once
#include "Camera.h"
namespace Engine
{
	class CTransform;
}

class CStaticCamera
	: public Engine::CCamera
{
private:
	explicit CStaticCamera(Engine::MYGDI* pMyGDI);
public:
	virtual ~CStaticCamera();

public:
	void SetTargetInfo(const Engine::CTransform* pTargetInfo);
public:
	HRESULT Initialize();
	virtual int Update();
	void MouseMove();

public:
	static CStaticCamera* Create(Engine::MYGDI* pMyGDI);
	void SetTrace(bool bTraceOn);

private:
	const Engine::CTransform* m_pTargetInfo;
private:
	float		m_fTargetDistance;
	float		m_fAngleX;
	float		m_fAngleY;
	float		m_fCamSpeed;
	XMFLOAT3	m_vPrevPos;
	bool		m_bTraceOn;
	//카메라 줌인 최대 최소 거리 제한
private:
	bool							m_bMaxHighDistanceState = false;
	bool							m_bMaxLowDistanceState = false;
	float		m_fTimeDelta = 0.f;

};