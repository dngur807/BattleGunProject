#pragma once

#include "Engine_Include.h"

BEGIN(Engine)

class ENGINE_DLL CLine2D
{
private:
	CLine2D();
public:
	~CLine2D();

public:
	HRESULT InitLine(const XMFLOAT3* pStartPoint, const XMFLOAT3* pEndPoint);

public:
	static CLine2D* Create(const XMFLOAT3* pStartPoint, const XMFLOAT3* pEndPoint);

public:
	const XMFLOAT2* GetNormal(void) { return &m_vNormal; }

private:
	XMFLOAT3	m_vStartPoint;
	XMFLOAT3	m_vEndPoint;
	XMFLOAT2	m_vNormal;
};

END

