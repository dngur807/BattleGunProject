#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CSphereTex 
	: public CVIBuffer
{
private:
	explicit CSphereTex(MYGDI* pMyGDI);
public:
	virtual ~CSphereTex();

public:
	virtual HRESULT CreateBuffer(float radius = 0.5f, UINT numSubdivisions = 1);

public:
	virtual CResources* CloneResource(void);

public:
	static CSphereTex* Create(MYGDI* pMyGDI , float radius = 0.5f, UINT numSubdivisions = 1);
	void Subdivide(MESHINFO& meshData);
	float AngleFromXY(float x, float y);
private:
	MESHINFO				m_MeshInfo;
};

END
