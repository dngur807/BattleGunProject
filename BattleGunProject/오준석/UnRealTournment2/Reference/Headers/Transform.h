#pragma once
#include "Component.h"
#include "Engine_Include.h"
BEGIN(Engine)

class ENGINE_DLL CTransform :
	public CComponent
{
private:
	explicit CTransform(const XMFLOAT3& vLook);

public:
	virtual ~CTransform(void);

public:
	virtual int Update(void);
	virtual DWORD Release(void);

public:
	static CTransform* Create(const XMFLOAT3& vLook);

public:
	float				m_fAngle[ANGLE_END];
	XMFLOAT3			m_vScale;
	XMFLOAT3			m_vPos;
	XMFLOAT3			m_vDir;
	XMFLOAT4X4			m_matWorld;

	XMFLOAT3			m_vCenter;	// 충돌박스 중심
	XMFLOAT3			m_vExt;		// 충돌박스 길이
									//
	INFO m_tPlayer;
};

END	