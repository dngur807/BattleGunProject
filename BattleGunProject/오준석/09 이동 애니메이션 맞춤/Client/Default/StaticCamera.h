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

};