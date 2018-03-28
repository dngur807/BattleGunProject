#pragma once
#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CCubeColor :
	public CVIBuffer
{
private:
	explicit CCubeColor(MYGDI* pMyGDI);
public:
	virtual ~CCubeColor();

public:
	virtual HRESULT CreateBuffer(const XMVECTOR& MIN, 
		const XMVECTOR& MAX, const XMFLOAT4& dwColor);

public:
	virtual CResources* CloneResource(void);

public:
	static CCubeColor* Create(MYGDI* pMyGDI, XMVECTOR& MIN, XMVECTOR& MAX,
		XMFLOAT4& vColor);
};

END